
#pragma once

#include "animation/animation.h"
#include "entity.h"
#include "direction.h"
#include <iostream>
#include <map>

/*
 * An entity is an abstraction of a square with a possibly animated texture.
 * */
namespace entity {

	class belt : public entity {

		public:
			belt(direction::type dir_from, direction::type dir_to, int xpos = 0, int ypos = 0);
			~belt();

			void update();
			void rotate();
			void bend();

			type entity_type() const { return ::entity::type::belt; }

			direction::type dir_from() const { return m_dir_from; }
			direction::type dir_to() const { return m_dir_to; }

			void dir_from(direction::type dir_from) { m_dir_from = dir_from; update_animation(); }
			void dir_to(direction::type dir_to) { m_dir_to = dir_to; update_animation(); }

		private:

			direction::type m_dir_from;
			direction::type m_dir_to;

			void update_animation();

	};

}
