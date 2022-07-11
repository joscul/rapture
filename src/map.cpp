
#include "map.h"

namespace map {

	std::vector<entity::entity *> s_entities(map_width * map_height, nullptr);

	int map_x(int xpos) {
		return xpos / 32;
	}

	int map_y(int ypos) {
		return ypos / 32;
	}
	
	int map_pos(int xpos, int ypos) {

		// Go from global coordinates to map coordinates.
		xpos = map_x(xpos);
		ypos = map_y(ypos);

		const int dw = map_width / 2;
		const int dh = map_height / 2;
		if (xpos < -dw || xpos >= dw) return -1;
		if (ypos < -dh || ypos >= dh) return -1;
		return (xpos + dh) + ((map_height - 1) - (ypos + dh)) * map_width;
	}

	bool is_empty(int xpos, int ypos) {
		const int mpos = map_pos(xpos, ypos);
		if (mpos < 0) return true;
		return s_entities[mpos] == nullptr;
	}

	void remove_entity(int xpos, int ypos) {
		if (!is_empty(xpos, ypos)) {
			delete s_entities[map_pos(xpos, ypos)];
			s_entities[map_pos(xpos, ypos)] = nullptr;
		}
	}

	int screen_coordinate_to_map_x(int xpos) {
		return std::round((xpos - 16)/32.0f) * 32;
	}

	int screen_coordinate_to_map_y(int ypos) {
		return std::round((ypos - 16)/32.0f) * 32;
	}

	entity::entity *entity(int xpos, int ypos) {
		const int mpos = map_pos(xpos, ypos);

		if (mpos < 0) return nullptr;
		return s_entities[mpos]; 
	}

	entity::entity *entity_neighbour(int xpos, int ypos, direction::type dir) {
		int mpos;
		if (dir == direction::west) mpos = map_pos(xpos - 32, ypos);
		if (dir == direction::north) mpos = map_pos(xpos, ypos + 32);
		if (dir == direction::east) mpos = map_pos(xpos + 32, ypos);
		if (dir == direction::south) mpos = map_pos(xpos, ypos - 32);

		if (mpos < 0) return nullptr;
		return s_entities[mpos]; 
	}

	entity::entity *entity_west_of(int xpos, int ypos) {
		return entity_neighbour(xpos, ypos, direction::west);
	}

	entity::entity *entity_north_of(int xpos, int ypos) {
		return entity_neighbour(xpos, ypos, direction::north);
	}

	entity::entity *entity_east_of(int xpos, int ypos)  {
		return entity_neighbour(xpos, ypos, direction::east);
	}

	entity::entity *entity_south_of(int xpos, int ypos) {
		return entity_neighbour(xpos, ypos, direction::south);
	}
}
