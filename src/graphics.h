
#pragma once

#include "entity/entity.h"
#include "animation/animation.h"
#include <iostream>

namespace graphics {

	int window_width();
	int window_height();

	void init();
	void main_loop();
	int load_texture(const std::string &texture_file);
	void set_background(const std::string &texture_file);

	int register_entity(entity::entity *e);
	void re_register_entity(entity::entity *e);
	void unregister_entity(int entity_id);

	int register_animation(animation::animation *a);
	void re_register_animation(animation::animation *a);
	void unregister_animation(int animation_id);

	void add_entity_to_mouse_layer(entity::entity &ent);
	void add_entity_to_window_layer(entity::entity &ent);

}
