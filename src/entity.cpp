
#include "entity.h"
#include "graphics.h"

entity::entity(const std::string &texture_file, int xpos, int ypos, int width, int height, int xdirection, int ydirection)
: m_xpos(xpos), m_ypos(ypos), m_width(width), m_height(height), m_xdirection(xdirection), m_ydirection(ydirection)
{
	m_texture_id = graphics::load_texture(texture_file);
	m_id = graphics::register_entity(this);
}

entity::entity(entity &&other) {
	m_id = other.m_id;
	m_xpos = other.m_xpos;
	m_ypos = other.m_ypos;
	m_width = other.m_width;
	m_height = other.m_height;
	m_xdirection = other.m_xdirection;
	m_ydirection = other.m_ydirection;
	m_texture_id = other.m_texture_id;

	graphics::re_register_entity(this);
}

entity::~entity() {
}
