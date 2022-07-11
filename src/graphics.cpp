
#include <functional>

#include <emscripten.h>
#include <GLES2/gl2.h>
#include <SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "graphics.h"
#include "map.h"

#include <map>

namespace graphics {

	int s_window_width = 1024;
	int s_window_height = 1024;

	struct entity_data {
		entity::entity *ent;
		float vertex_data[24];
		GLuint array_buffer_id;
		GLuint vertex_array_id;
	};

	struct texture_data {
		GLuint texture_id;
		int width;
		int height;
	};

	std::map<std::string, struct texture_data> s_textures;
	std::map<int, entity_data> s_entities;
	std::map<int, animation::animation *> s_animations;
	std::vector<entity::entity *> s_mouse_layer;
	std::vector<entity::entity *> s_window_layer;
	int s_last_entity_id = 1;
	int s_last_animation_id = 1;

	GLint s_position_attribute;
	GLint s_uv_attribute;
	GLuint s_shader_program;

	SDL_Renderer *s_renderer;
	SDL_Window *s_window;

	std::string s_background_texture;
	entity_data s_background_entity = {
		.ent = nullptr,
		.vertex_data = {
				-1.0, -1.0, 0.0, 1.0 ,  0.0f, 0.0f,
				-1.0, 1.0, 0.0, 1.0 ,  0.0f, 8.0f,
				1.0, 1.0, 0.0, 1.0 ,  2.0f, 8.0f,
				1.0, -1.0, 0.0, 1.0 ,  2.0f, 0.0f,
		}
	};

	const char *vert_shader_2 = R"(#version 100
		attribute vec4 position;
		attribute vec2 a_uv;
		varying vec2 v_uv;
		void main (void)
		{
			v_uv = a_uv;
			gl_Position = position;
		})";

	const char *frag_shader_2 = R"(#version 100
		precision mediump float;

		varying vec2 v_uv;
		uniform sampler2D tex;
		uniform vec4 overlay;

		void main(void)
		{
			gl_FragColor = texture2D(tex, v_uv);
			gl_FragColor *= overlay;
		})";

	// an example of something we will control from the javascript side
	bool background_is_black = true;

	// the function called by the javascript code
	extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color() { background_is_black = !background_is_black; }

	std::function<void()> loop;
	void tmp_main_loop() { loop(); }

	int window_width() {
		return s_window_width;
	}

	int window_height() {
		return s_window_height;
	}

	int flip_image_upside_down(int width, int height, int bytes_per_pixel, char *image_pixels) {

		const int width_bytes = width * bytes_per_pixel;

		std::unique_ptr<char[]> temp_row = std::make_unique<char[]>(width_bytes);
		if (temp_row.get() == nullptr) {
			SDL_SetError("Not enough memory for image inversion");
			return -1;
		}

		// if height is odd, don't need to swap middle row
		int height_div_2 = height / 2;
		for (int index = 0; index < height_div_2; index++) {
			// copy row to tem_row
			memcpy(temp_row.get(), &image_pixels[width_bytes * index], width_bytes);
			memcpy(&image_pixels[width_bytes * index], &image_pixels[width_bytes * (height - index - 1)], width_bytes);
			memcpy(&image_pixels[width_bytes * (height - index - 1)], temp_row.get(), width_bytes);
		}
		return 0;
	}

	struct texture_data load_gl_texture(const std::string &filename, int texture_wrap = GL_CLAMP_TO_EDGE) {

		int status = false;
		unsigned int texture;

		/* Create storage space for the texture */
		SDL_Surface *texture_image[1];

		std::ifstream input(filename.c_str(), std::ios::binary);

		// copies all data into buffer
		std::vector<char> buffer((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
		std::cout << filename << " size "<< buffer.size() << std::endl;

		int width = 0;
		int height = 0;

		/* Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit */
		if ((texture_image[0] = IMG_Load(filename.c_str()))) {

			/* Set the status to true */
			status = true;

			/* Create The Texture */
			glGenTextures(1, &texture);

			/* Typical Texture Generation Using Data From The Bitmap */
			glBindTexture(GL_TEXTURE_2D, texture);

			std::cout << "Loaded "<< texture_image[0]->w << " " << texture_image[0]->h << std::endl;

			// Enforce RGB/RGBA
			int format;
			SDL_Surface* formatted_surface;
			if (texture_image[0]->format->BytesPerPixel == 3) {
				std::cout << "bytes per pixel = 3" << std::endl;
				formatted_surface = SDL_ConvertSurfaceFormat(texture_image[0], SDL_PIXELFORMAT_RGB24, 0);
				format = GL_RGB;
			} else {
				std::cout << "bytes per pixel != 3" << std::endl;
				formatted_surface = SDL_ConvertSurfaceFormat(texture_image[0], SDL_PIXELFORMAT_RGBA32, 0);
				format = GL_RGBA;
			}

			width = formatted_surface->w;
			height = formatted_surface->h;
			char *data = static_cast<char *>(formatted_surface->pixels);

			flip_image_upside_down(width, height, formatted_surface->format->BytesPerPixel, data);

			/* Generate The Texture */
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			//glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			/* Linear Filtering */
			SDL_FreeSurface(formatted_surface);
			SDL_FreeSurface(texture_image[0]);
		} else {
			std::cout << "Cannot load " << filename << std::endl;
		}

		return texture_data{
			.texture_id = texture,
			.width = width,
			.height = height
		};
	}

	GLuint init_shader(const char *v_shader, const char *f_shader) {

		struct Shader {
			GLenum type;
			const char *source;
		};

		struct Shader shaders[2] = {
				{ GL_VERTEX_SHADER, v_shader },
				{ GL_FRAGMENT_SHADER, f_shader }
		};

		GLuint program = glCreateProgram();

		for ( int i = 0; i < 2; ++i ) {
			Shader& s = shaders[i];
			GLuint shader = glCreateShader(s.type);
			glShaderSource(shader, 1, (const GLchar**) &s.source, NULL);
			glCompileShader(shader);

			GLint  compiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) {
				std::cerr << " failed to compile:" << std::endl;
				GLint  log_size;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
				std::unique_ptr<char[]> log_msg = std::make_unique<char[]>(log_size);

				glGetShaderInfoLog(shader, log_size, NULL, log_msg.get());

				std::cerr << log_msg.get() << std::endl;

				exit( EXIT_FAILURE );
			}

			glAttachShader(program, shader);
		}

		/* link  and error check */
		glLinkProgram(program);

		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked) {

			std::cerr << "Shader program failed to link" << std::endl;

			GLint  log_size;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
			std::unique_ptr<char[]> log_msg = std::make_unique<char[]>(log_size);
			glGetProgramInfoLog(program, log_size, NULL, log_msg.get());
			std::cerr << log_msg.get() << std::endl;

			exit( EXIT_FAILURE );
		}

		/* use program object */
		glUseProgram(program);

		return program;
	}

	void gen_array_buffer(GLuint &array_buffer_id, GLuint &vertex_array_id, float *vertex_data) {

		glGenVertexArraysOES(1, &vertex_array_id);
		glBindVertexArrayOES(vertex_array_id);
		glGenBuffers(1, &array_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, array_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertex_data, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(s_position_attribute);
		glEnableVertexAttribArray(s_uv_attribute);
		int vertex_size = sizeof(float) * 6;
		glVertexAttribPointer(s_position_attribute, 4, GL_FLOAT, GL_FALSE, vertex_size, (const GLvoid *)0);
		glVertexAttribPointer(s_uv_attribute, 2, GL_FLOAT, GL_FALSE, vertex_size, (const GLvoid *)(sizeof(float) * 4));

	}

	float to_gl_coord_x(int xpos) {
		return (xpos / 512.0f);
	}

	float to_gl_coord_y(int ypos) {
		return (ypos / 512.0f);
	}

	void draw_entity(entity_data &data, GLuint shader_program) {

		const entity::entity *ent = data.ent;
		if (!ent->visible()) return;

		const animation::animation *anim = ent->animation();

		glBindVertexArrayOES(data.vertex_array_id);

		const float gutter_x = (float)anim->texture_gutter() / (float)s_window_width;
		const float gutter_y = (float)anim->texture_gutter() / (float)s_window_height;

		const int xpos = ent->xpos();
		const int ypos = ent->ypos();

		const int width = ent->width();
		const int height = ent->height();

		data.vertex_data[0] = to_gl_coord_x(xpos) - gutter_x;
		data.vertex_data[1] = to_gl_coord_y(ypos) - gutter_y;

		data.vertex_data[6] = to_gl_coord_x(xpos) - gutter_x;
		data.vertex_data[7] = to_gl_coord_y(ypos + height) + gutter_y;

		data.vertex_data[12] = to_gl_coord_x(xpos + width) + gutter_x;
		data.vertex_data[13] = to_gl_coord_y(ypos + height) + gutter_y;

		data.vertex_data[18] = to_gl_coord_x(xpos + width) + gutter_x;
		data.vertex_data[19] = to_gl_coord_y(ypos) - gutter_y;

		const int texture_cols = anim->texture_cols();
		const int texture_rows = anim->texture_rows();

		const int frame_in_texture = anim->current_frame() + anim->animation_begin();

		const int current_col = frame_in_texture % texture_cols;
		const int current_row = frame_in_texture / texture_cols;

		const float sprite_width = (1.0f/texture_cols);
		const float sprite_height = (1.0f/texture_rows);

		data.vertex_data[4] = sprite_width * current_col;
		data.vertex_data[5] = 1.0 - (sprite_height * (current_row + 1));

		data.vertex_data[10] = sprite_width * current_col;
		data.vertex_data[11] = 1.0f - (sprite_height * current_row);

		data.vertex_data[16] = sprite_width * (current_col + 1);
		data.vertex_data[17] = 1.0f - (sprite_height * current_row);

		data.vertex_data[22] = sprite_width * (current_col + 1);
		data.vertex_data[23] = 1.0f - (sprite_height * (current_row + 1));

		glBindBuffer(GL_ARRAY_BUFFER, data.array_buffer_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(float), data.vertex_data);
		glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);
		glBindTexture(GL_TEXTURE_2D, anim->texture_id());
		glUniform4f(glGetUniformLocation(shader_program, "overlay"), ent->red(), ent->green(), ent->blue(), ent->alpha());

		// Draw square
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void draw_background(const struct texture_data &background, GLuint shader_program) {

		glBindVertexArrayOES(s_background_entity.vertex_array_id);

		glBindBuffer(GL_ARRAY_BUFFER, s_background_entity.array_buffer_id);
		glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);
		glBindTexture(GL_TEXTURE_2D, background.texture_id);
		glUniform4f(glGetUniformLocation(shader_program, "overlay"), 1.0, 1.0, 1.0, 1.0);

		// Draw square
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void init() {
		SDL_CreateWindowAndRenderer(s_window_width, s_window_height, SDL_WINDOW_OPENGL, &s_window, &s_renderer);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		s_shader_program = init_shader(vert_shader_2, frag_shader_2);

		s_uv_attribute = glGetAttribLocation(s_shader_program, "a_uv");
		if (s_uv_attribute < 0) {
			std::cerr << "Shader did not contain the 'color' attribute." << std::endl;
		}
		s_position_attribute = glGetAttribLocation(s_shader_program, "position");
		if (s_position_attribute < 0) {
			std::cerr << "Shader did not contain the 'position' attribute." << std::endl;
		}
	}

	void main_loop() {

		/*

		GLuint vertex_buffer, vertex_array_object;
		float vertexData[24] = {
				0.0, 0.0, 0.0, 1.0 ,  0.0, 0.0,
				0.0, 1.0, 0.0, 1.0 ,  0.0, 1.0,
				1.5, 1.5, 0.0, 1.0 ,  1.0, 1.0,
				1.0, 0.0, 0.0, 1.0 ,  1.0, 0.0,
		};

		glGenVertexArraysOES(1, &vertex_array_object);
		glBindVertexArrayOES(vertex_array_object);
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertexData, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(s_position_attribute);
		glEnableVertexAttribArray(s_uv_attribute);
		int vertex_size = sizeof(float) * 6;
		glVertexAttribPointer(s_position_attribute, 4, GL_FLOAT, GL_FALSE, vertex_size, (const GLvoid *)0);
		glVertexAttribPointer(s_uv_attribute, 2, GL_FLOAT, GL_FALSE, vertex_size, (const GLvoid *)(sizeof(float) * 4));

		textures[0] = load_gl_texture("../textures/test.png");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glEnable(GL_DEPTH_TEST);
		*/

		glEnable(GL_BLEND);
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glActiveTexture(GL_TEXTURE0);

		loop = [&]
		{

			for (auto &iter : s_animations) {
				iter.second->update();
			}

			// Clear the screen
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(s_shader_program);

			if (s_textures.count(s_background_texture)) {
				draw_background(s_textures[s_background_texture], s_shader_program);
			}

			for (auto &ent_data : s_entities) {
				ent_data.second.ent->update();
			}

			// Draw all entities on map.
			for (auto &map_tile : map::s_entities) {
				if (map_tile != nullptr) {
					draw_entity(s_entities[map_tile->id()], s_shader_program);
				}
			}
			// Draw mouse layer.
			for (auto &ent : s_mouse_layer) {
				draw_entity(s_entities[ent->id()], s_shader_program);
			}

			// Draw window layer.
			for (auto &ent : s_window_layer) {
				draw_entity(s_entities[ent->id()], s_shader_program);
			}

			SDL_GL_SwapWindow(s_window);
		};

		emscripten_set_main_loop(tmp_main_loop, 0, true);
	}

	int load_texture(const std::string &texture_file) {
		if (s_textures.count(texture_file)) {
			return s_textures[texture_file].texture_id;
		}

		s_textures[texture_file] = load_gl_texture("../textures/" + texture_file);

		return s_textures[texture_file].texture_id;
	}

	void set_background(const std::string &texture_file) {
		s_background_texture = texture_file;
		if (s_textures.count(texture_file) == 0) {
			s_textures[texture_file] = load_gl_texture("../textures/" + texture_file, GL_REPEAT);
		}

		gen_array_buffer(s_background_entity.array_buffer_id, s_background_entity.vertex_array_id, s_background_entity.vertex_data);
	}

	int register_entity(entity::entity *e) {
		int my_id = s_last_entity_id++;

		entity_data ent = {
			.ent = e,
			.vertex_data = {
					0.0, -0.5, 0.0, 1.0 ,  0.0f, 0.0f,
					-0.5,  1.5, 0.0, 1.0 ,  0.0f, 1.0f,
					1.5,  1.5, 0.0, 1.0 ,  1.0f, 1.0f,
					1.5, -0.5, 0.0, 1.0 ,  1.0f, 0.0f,
			}
		};

		gen_array_buffer(ent.array_buffer_id, ent.vertex_array_id, ent.vertex_data);

		s_entities.emplace(my_id, std::move(ent));

		return my_id;
	}

	void re_register_entity(entity::entity *e) {
		s_entities[e->id()].ent = e;
	}

	void unregister_entity(int entity_id) {
		s_entities.erase(entity_id);
	}

	int register_animation(animation::animation *a) {
		int my_id = s_last_animation_id++;
		s_animations[my_id] = a;

		return my_id;
	}

	void re_register_animation(animation::animation *a) {
		s_animations[a->id()] = a;
	}

	void unregister_animation(int animation_id) {
		s_animations.erase(animation_id);
	}

	void add_entity_to_mouse_layer(entity::entity &ent) {
		s_mouse_layer.push_back(&ent);
	}

	void add_entity_to_window_layer(entity::entity &ent) {
		s_window_layer.push_back(&ent);
	}

}
