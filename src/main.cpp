
#include "entity/belt.h"
#include "entity/inventory.h"
#include "graphics.h"
#include "controls.h"
#include "map.h"
#include <vector>

int main() {

	graphics::init();
	controls::init();
	animation::init();

	graphics::set_background("dirt-bg.png");

	entity::belt template_belt(direction::west, direction::east, 0, 0);
	template_belt.alpha_overlay(0.5);
	template_belt.color_overlay(0.5, 1.0, 0.5);
	graphics::add_entity_to_mouse_layer(template_belt);

	entity::inventory main_inventory(-40, -80);
	graphics::add_entity_to_window_layer(main_inventory);

	entity::belt *belt_edit = nullptr;
	controls::on_mousemove([&template_belt, &belt_edit] (int xpos, int ypos, auto event) {
		xpos = map::screen_coordinate_to_map_x(xpos);
		ypos = map::screen_coordinate_to_map_y(ypos);

		auto ent = map::entity(xpos, ypos);
		if (belt_edit != nullptr && ent != belt_edit) {
			belt_edit->visible(true);
			belt_edit = nullptr;
		}

		template_belt.pos(xpos, ypos);
		template_belt.bend();

		if (belt_edit != nullptr && ent == belt_edit && !event.ctrl_key) return;

		if (belt_edit == nullptr && ent != nullptr && ent->entity_type() == template_belt.entity_type()) {
			belt_edit = static_cast<entity::belt *>(ent);
			belt_edit->visible(false);
		}

		if (controls::is_dragging()) {
			if (event.ctrl_key) {
				map::remove_entity(xpos, ypos);
			} else {
				map::remove_entity(xpos, ypos);
				map::add_entity(xpos, ypos, template_belt);
			}
		}
	});

	controls::on_mousedown([&template_belt] (int xpos, int ypos, auto event) {
		xpos = map::screen_coordinate_to_map_x(xpos);
		ypos = map::screen_coordinate_to_map_y(ypos);

		if (controls::is_dragging()) {
			if (event.ctrl_key) {
				map::remove_entity(xpos, ypos);
			} else {
				map::remove_entity(xpos, ypos);
				map::add_entity(xpos, ypos, template_belt);
			}
		}
	});

	controls::on_keydown([&template_belt, &belt_edit] (std::string key) {
		if (key == "r") {
			auto ent = map::entity(template_belt.xpos(), template_belt.ypos());
			template_belt.rotate();
			template_belt.bend();
			if (belt_edit == ent) {
				belt_edit->dir_from(template_belt.dir_from());
				belt_edit->dir_to(template_belt.dir_to());
			}
		}
	});

	/*animation::animation anim1("factory_noshadow.png", 9, 1, 128, 0, 8);
	entity::entity belt_beg(&anim1, 20, 10, 30, 30);

	entity::belt belt1(direction::west, direction::east, 0, 0);
	entity::belt belt2(direction::west, direction::east, 10, 0);
	entity::belt belt3(direction::west, direction::east, 20, 0);
	entity::belt belt4(direction::west, direction::north, 30, 0);


	entity belt_beg("hr-transport-belt.png", 16, 20, 32, 224, 239);
	belt_beg.xpos(-10);

	entity belt3("hr-transport-belt.png", 16, 20, 32, 96, 96+15);
	belt3.xpos(20);

	entity belt4("hr-transport-belt.png", 16, 20, 32, 32, 32+15);
	belt4.xpos(20);
	belt4.ypos(10);

	entity belt6("hr-transport-belt.png", 16, 20, 32, 160, 160+15);
	belt6.xpos(20);
	belt6.ypos(30);

	entity belt5("hr-transport-belt.png", 16, 20, 32, 32, 32+15);
	belt5.xpos(20);
	belt5.ypos(20);*/

	/*entity belt7("hr-transport-belt.png", 16, 20, 32, 32, 32+15);
	belt7.xpos(20);
	belt7.ypos(10);

	
	animation::animation anim3("hr-steam-engine-H-shadow.png", 8, 4, 16, 0, 31);
	entity::entity steam_engine_shadow(&anim3, 50, -30, 50, 30);

	animation::animation anim2("hr-steam-engine-H.png", 8, 4, 16, 0, 31);
	entity::entity steam_engine(&anim2, 50, -30, 50, 30);

	animation::animation anim4("hr-assembling-machine-1.png", 8, 4, 11, 0, 31);
	entity::entity steam_engine3(&anim4, 50, 50, 30, 30);*/

	graphics::main_loop();

	return 0;
}

