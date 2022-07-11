
#pragma once

#include "animation/animation.h"
#include "entity.h"
#include "direction.h"
#include <iostream>
#include <map>

/*
 * An inventory is a square with a grid. Each square in the grid can contain a number of minerals/entities.
 * since an inventory is a square with textures it inherits the entity class.
 * */
namespace entity {

	class inventory : public entity {

		public:
			inventory(int xpos = 0, int ypos = 0);
			~inventory();

			void update();
			void rotate();
			type entity_type() const { return ::entity::type::inventory; }

		private:

	};

}
