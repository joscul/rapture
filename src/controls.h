
#pragma once

#include <emscripten/html5.h>
#include <emscripten.h>
#include <functional>

namespace controls {

	struct event {
		bool ctrl_key;
		bool shift_key;
		bool alt_key;
	};

	void init();
	void on_mousemove(std::function<void(int, int, const event &)> callback);
	void on_mousedown(std::function<void(int, int, const event &)> callback);
	void on_mouseup(std::function<void(int, int, const event &)> callback);
	void on_click(std::function<void(int, int, const event &)> callback);
	void on_keydown(std::function<void(std::string)> callback);
	bool is_dragging();
}
