
#pragma once

#include "animation/animation.h"
#include <iostream>

/*
 * An entity is an abstraction of a square with a possibly animated texture.
 * */
namespace entity {

	enum class type {
		unknown,
		belt,
		inventory,
	};

	class entity {

		public:
			entity(int xpos = 0, int ypos = 0, int width = 32, int height = 32);
			entity(const animation::animation *animation, int xpos = 0, int ypos = 0, int width = 32, int height = 32);
			entity(const entity &);
			entity(entity &&);
			~entity();

			void update() {
			}

			int xpos() const { return m_xpos; }
			int ypos() const { return m_ypos; }
			int width() const { return m_width; }
			int height() const { return m_height; }
			bool visible() const { return m_visible; }
			int id() const { return m_id; }
			float alpha() const { return m_alpha; }
			float red() const { return m_red; }
			float green() const { return m_green; }
			float blue() const { return m_blue; }
			virtual type entity_type() const = 0;
			const animation::animation *animation() const { return m_animation; }

			void xpos(int pos) { m_xpos = pos; }
			void ypos(int pos) { m_ypos = pos; }
			void pos(int xpos, int ypos) { m_xpos = xpos; m_ypos = ypos; }

			void width(int width) { m_width = width; }
			void height(int height) { m_height = height; }
			void visible(bool visible) { m_visible = visible; }
			void alpha_overlay(float alpha) { m_alpha = alpha; }
			void color_overlay(float red, float green, float blue) { m_red = red; m_green = green; m_blue = blue; }
			void animation(animation::animation *new_anim) { m_animation = new_anim; }
		
		protected:
			int m_id;
			int m_xpos = 0;
			int m_ypos = 0;

			int m_width = 32;
			int m_height = 32;

			bool m_visible = true;

			float m_alpha = 1.0;
			float m_red = 1.0;
			float m_green = 1.0;
			float m_blue = 1.0;

			const animation::animation *m_animation = nullptr;

	};

}
