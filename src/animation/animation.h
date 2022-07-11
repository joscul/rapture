
#pragma once

#include <iostream>

/*
 * An animation defines a positionless and sizeless square that renders a sprite animation.
 * */

namespace animation {

	class animation {

		public:
			animation();
			animation(const std::string &texture_file, int texture_cols = 1, int texture_rows = 1, int texture_gutter = 0, int animation_begin = 0,
					int animation_end = 0);
			~animation();
			animation(const animation &other) = delete;
			animation &operator=(const animation &other) = delete;
			animation(animation &&other);
			animation &operator=(animation &&other);

			void update() {
				m_current_frame = (m_current_frame + 1) % (m_animation_end - m_animation_begin + 1);
			}

			int texture_id() const { return m_texture_id; }
			int texture_cols() const { return m_texture_cols; }
			int texture_rows() const { return m_texture_rows; }
			int texture_gutter() const { return m_texture_gutter; }
			int animation_begin() const { return m_animation_begin; }
			int animation_end() const { return m_animation_end; }
			int current_frame() const { return m_current_frame; }
			int id() const { return m_id; }

		private:

			int m_id;

			int m_texture_rows;
			int m_texture_cols;
			int m_texture_gutter;
			int m_animation_begin;
			int m_animation_end;
			int m_current_frame = 0;

			int m_texture_id;

	};

	enum class type {
		transport_west_east,
		transport_east_west,
		transport_south_north,
		transport_north_south,
		transport_east_north,
		transport_north_east,
		transport_west_north,
		transport_north_west,
		transport_south_east,
		transport_east_south,
		transport_south_west,
		transport_west_south,
		transport_south_start,
		transport_south_end,
		transport_west_start,
		transport_west_end,
		transport_north_start,
		transport_north_end,
		transport_east_start,
		transport_east_end,

		window_256_x_128,
	};

	void init();
	animation *find_animation(type animation_type);


}
