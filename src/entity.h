
#pragma once

#include <iostream>

class entity {

	public:
		entity(const std::string &texture_file, int xpos = 0, int ypos = 0, int width = 10, int height = 10, int xdirection = (rand() % 3) - 1,
			int ydirection = (rand() % 3) - 1);
		entity(entity &&);
		~entity();

		void update() {

			m_xpos += (rand() % 3) - 1 + m_xdirection;
			m_ypos += (rand() % 3) - 1 + m_ydirection;

			if (m_xpos > 100 || m_xpos < -100) {
				m_xdirection = -m_xdirection;
			}

			if (m_ypos > 100 || m_ypos < -100) {
				m_ydirection = -m_ydirection;
			}
		}

		int xpos() const { return m_xpos; }
		int ypos() const { return m_ypos; }
		int width() const { return m_width; }
		int height() const { return m_height; }
		int texture_id() const { return m_texture_id; }
		int xdirection() const { return m_xdirection; }
		int ydirection() const { return m_ydirection; }
		int id() const { return m_id; }
	
	private:
		int m_id;
		int m_xpos;
		int m_ypos;

		int m_width;
		int m_height;

		int m_texture_id;

		int m_xdirection;
		int m_ydirection;

};
