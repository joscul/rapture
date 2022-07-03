
#pragma once

#include "entity.h"
#include <iostream>

namespace graphics {

	void init();
	void main_loop();
	int load_texture(const std::string &texture_file);
	int register_entity(entity *e);
	void re_register_entity(entity *e);

}
