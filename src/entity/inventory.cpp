
#include "inventory.h"

namespace entity {
	inventory::inventory(int xpos, int ypos) {
		this->xpos(xpos);
		this->ypos(ypos);
		this->width(256);
		this->height(128);
		this->m_animation = animation::find_animation(animation::type::window_256_x_128);
	}

	inventory::~inventory() {
	}

	void inventory::update() {
	}

	void inventory::rotate() {
	}

}
