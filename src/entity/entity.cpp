
#include "entity.h"
#include "graphics.h"

namespace entity {

	entity::entity(int xpos, int ypos, int width, int height)
	: m_xpos(xpos), m_ypos(ypos), m_width(width), m_height(height) {
		m_id = graphics::register_entity(this);
	}

	entity::entity(const animation::animation *animation, int xpos, int ypos, int width, int height)
	: m_animation(animation), m_xpos(xpos), m_ypos(ypos), m_width(width), m_height(height)
	{
		m_id = graphics::register_entity(this);
	}

	entity::entity(const entity &other) {
		m_xpos = other.m_xpos;
		m_ypos = other.m_ypos;
		m_width = other.m_width;
		m_height = other.m_height;
		m_animation = other.m_animation;

		m_id = graphics::register_entity(this);
		std::cout << "registered " << m_id << std::endl;
	}

	entity::entity(entity &&other) {
		m_id = other.m_id;
		m_xpos = other.m_xpos;
		m_ypos = other.m_ypos;
		m_width = other.m_width;
		m_height = other.m_height;
		m_animation = other.m_animation;

		graphics::re_register_entity(this);
	}

	entity::~entity() {
		std::cout << "unregistering " << m_id << std::endl;
		graphics::unregister_entity(m_id);
	}
}
