
#pragma once

#include "entity/entity.h"
#include "direction.h"
#include <vector>
#include <memory>

namespace map {

	const int map_width = 32;
	const int map_height = 32;

	extern std::vector<entity::entity *> s_entities;

	int map_pos(int xpos, int ypos);
	bool is_empty(int xpos, int ypos);

	template<typename entity_type>
	void add_entity(int xpos, int ypos, entity_type &ent) {
		if (is_empty(xpos, ypos)) {
			const int mpos = map_pos(xpos, ypos);
			if (mpos < 0) return;
			std::cout << "xpos: " << xpos << " ypos: " << ypos << std::endl;
			std::cout << "adding entity to pos " << mpos << std::endl;
			s_entities[mpos] = static_cast<entity::entity *>(new entity_type(ent));
		}
	}

	void remove_entity(int xpos, int ypos);
	int screen_coordinate_to_map_x(int xpos);
	int screen_coordinate_to_map_y(int ypos);

	/*
	 * Find the entity at position (xpos, ypos)
	 * Returns nullptr if the square is empty or if the square is outside the map.
	 * Returns pointer to entity otherwise.
	 * */
	entity::entity *entity(int xpos, int ypos);

	/*
	 * Find the entity at map position neighbour in dir direction.
	 * Returns nullptr if the square is empty or if the square is outside the map.
	 * Returns pointer to entity otherwise.
	 * */
	entity::entity *entity_neighbour(int xpos, int ypos, direction::type dir);

	/*
	 * Helper functions to fund entities in different directions
	 * Returns nullptr if the square is empty or if the square is outside the map.
	 * Returns pointer to entity otherwise.
	 */
	entity::entity *entity_west_of(int xpos, int ypos);
	entity::entity *entity_north_of(int xpos, int ypos);
	entity::entity *entity_east_of(int xpos, int ypos);
	entity::entity *entity_south_of(int xpos, int ypos);

}
