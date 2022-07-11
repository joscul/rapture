#include "animation.h"
#include "graphics.h"
#include <map>

namespace animation {

	animation::animation()
	: m_texture_cols(0), m_texture_rows(0), m_texture_gutter(0), m_animation_begin(0), m_animation_end(0), m_texture_id(0), m_id(0)
	{
	}

	animation::animation(const std::string &texture_file, int texture_cols, int texture_rows, int texture_gutter, int animation_begin,
			int animation_end)
	: m_texture_cols(texture_cols), m_texture_rows(texture_rows), m_texture_gutter(texture_gutter), m_animation_begin(animation_begin),
		m_animation_end(animation_end)
	{
		m_texture_id = graphics::load_texture(texture_file);
		m_id = graphics::register_animation(this);
	}

	animation::~animation() {
	}

	animation::animation(animation &&other) {
		*this = std::move(other);
	}

	animation &animation::operator=(animation &&other) {
		m_id = other.m_id;
		m_texture_rows = other.m_texture_rows;
		m_texture_cols = other.m_texture_cols;
		m_texture_gutter = other.m_texture_gutter;
		m_animation_begin = other.m_animation_begin;
		m_animation_end = other.m_animation_end;
		m_current_frame = other.m_current_frame;

		m_texture_id = other.m_texture_id;

		graphics::re_register_animation(this);

		return *this;
	}

	std::map<type, animation> s_animations;

	void init() {
		int i = 0;
		s_animations[type::transport_west_east] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_east_west] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_south_north] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_north_south] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_east_north] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_north_east] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_west_north] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_north_west] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_south_east] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_east_south] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_south_west] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_west_south] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_south_start] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_south_end] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_west_start] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_west_end] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_north_start] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_north_end] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_east_start] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;
		s_animations[type::transport_east_end] = animation("hr-transport-belt.png", 16, 20, 32, i, i + 15); i += 16;

		// Window textures.
		s_animations[type::window_256_x_128] = animation("window_256_x_128.png", 1, 1, 0, 0, 0);
	}

	animation *find_animation(type animation_type) {
		if (s_animations.count(animation_type)) {
			return &s_animations[animation_type];
		}
		return nullptr;
	}
}
