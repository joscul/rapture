
#include "belt.h"
#include "map.h"
#include <array>

namespace entity {
	belt::belt(direction::type dir_from, direction::type dir_to, int xpos, int ypos)
	: m_dir_from(dir_from), m_dir_to(dir_to) {

		update_animation();

		this->xpos(xpos);
		this->ypos(ypos);
	}

	belt::~belt() {
	}

	void belt::update() {
	}

	/*
	 * Invoked when user presse r. 
	 * */
	void belt::rotate() {

		m_dir_to = direction::rotate_clockwise(m_dir_to);
		m_dir_from = direction::rotate_clockwise(m_dir_from);
		update_animation();
	}

	void belt::bend() {

		std::array<direction::type, 4> directions = {
			direction::west,
			direction::east,
			direction::north,
			direction::south,
		};

		bool found_one = false;
		for (auto dir : directions) {
			if (dir == m_dir_to) continue;
			auto neighbour = (belt *)map::entity_neighbour(m_xpos, m_ypos, dir);

			// Is the neighbour interesting?
			if (neighbour != nullptr && neighbour->entity_type() == ::entity::type::belt) {
				// Points towards me?
				if (neighbour->m_dir_to == direction::rotate_180(dir)) {
					m_dir_from = direction::rotate_180(neighbour->m_dir_to);
					found_one = true;
					break;
				}
			}
		}

		if (!found_one) {
			m_dir_from = direction::rotate_180(m_dir_to);
		}

		update_animation();
	}


	void belt::update_animation() {
		// from west
		if (m_dir_from == direction::west) {
			if (m_dir_to == direction::north) {
				this->m_animation = animation::find_animation(animation::type::transport_west_north);
			}
			if (m_dir_to == direction::east) {
				this->m_animation = animation::find_animation(animation::type::transport_west_east);
			}
			if (m_dir_to == direction::south) {
				this->m_animation = animation::find_animation(animation::type::transport_west_south);
			}
		}

		// from north
		if (m_dir_from == direction::north) {
			if (m_dir_to == direction::east) {
				this->m_animation = animation::find_animation(animation::type::transport_north_east);
			}
			if (m_dir_to == direction::south) {
				this->m_animation = animation::find_animation(animation::type::transport_north_south);
			}
			if (m_dir_to == direction::west) {
				this->m_animation = animation::find_animation(animation::type::transport_north_west);
			}
		}

		// from east
		if (m_dir_from == direction::east) {
			if (m_dir_to == direction::south) {
				this->m_animation = animation::find_animation(animation::type::transport_east_south);
			}
			if (m_dir_to == direction::west) {
				this->m_animation = animation::find_animation(animation::type::transport_east_west);
			}
			if (m_dir_to == direction::north) {
				this->m_animation = animation::find_animation(animation::type::transport_east_north);
			}
		}

		// from south
		if (m_dir_from == direction::south) {
			if (m_dir_to == direction::west) {
				this->m_animation = animation::find_animation(animation::type::transport_south_west);
			}
			if (m_dir_to == direction::north) {
				this->m_animation = animation::find_animation(animation::type::transport_south_north);
			}
			if (m_dir_to == direction::east) {
				this->m_animation = animation::find_animation(animation::type::transport_south_east);
			}
		}
	}
}
