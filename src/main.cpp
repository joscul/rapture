
#include "graphics.h"
#include "entity.h"
#include <vector>

int main() {

	graphics::init();

	std::vector<entity> entities;
	for (int i = 0; i < 1000; i++) {
		entities.emplace_back(entity("test.png"));
	}
	entity ent2("test.png", 10, 10);
	entity ent22("test.png", 10, 10);
	entity ent3("test.png", 30, 30, 20, 20);
	entity ent4("test.png", 50, 60, 5, 5);
	entity ent5("alma.png", -10, 60, 30, 30);
	
	graphics::main_loop();

	return 0;
}

