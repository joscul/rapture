
#include "controls.h"
#include "graphics.h"
#include <iostream>
#include <vector>

namespace controls {

	std::vector<std::function<void(int, int, const event &)>> s_mousemove_callbacks;
	std::vector<std::function<void(int, int, const event &)>> s_mousedown_callbacks;
	std::vector<std::function<void(int, int, const event &)>> s_mouseup_callbacks;
	std::vector<std::function<void(int, int, const event &)>> s_click_callbacks;
	std::vector<std::function<void(std::string)>> s_keydown_callbacks;
	bool s_is_dragging = false;

	std::string emscripten_result_to_string(EMSCRIPTEN_RESULT result) {
		if (result == EMSCRIPTEN_RESULT_SUCCESS) return "EMSCRIPTEN_RESULT_SUCCESS";
		if (result == EMSCRIPTEN_RESULT_DEFERRED) return "EMSCRIPTEN_RESULT_DEFERRED";
		if (result == EMSCRIPTEN_RESULT_NOT_SUPPORTED) return "EMSCRIPTEN_RESULT_NOT_SUPPORTED";
		if (result == EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED) return "EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED";
		if (result == EMSCRIPTEN_RESULT_INVALID_TARGET) return "EMSCRIPTEN_RESULT_INVALID_TARGET";
		if (result == EMSCRIPTEN_RESULT_UNKNOWN_TARGET) return "EMSCRIPTEN_RESULT_UNKNOWN_TARGET";
		if (result == EMSCRIPTEN_RESULT_INVALID_PARAM) return "EMSCRIPTEN_RESULT_INVALID_PARAM";
		if (result == EMSCRIPTEN_RESULT_FAILED) return "EMSCRIPTEN_RESULT_FAILED";
		if (result == EMSCRIPTEN_RESULT_NO_DATA) return "EMSCRIPTEN_RESULT_NO_DATA";
		return "Unknown EMSCRIPTEN_RESULT!";
	}

	std::string emscripten_event_type_to_string(int type) {
		const char *events[] = { "(invalid)", "(none)", "keypress", "keydown", "keyup", "click", "mousedown", "mouseup", "dblclick", "mousemove", "wheel", "resize",
			"scroll", "blur", "focus", "focusin", "focusout", "deviceorientation", "devicemotion", "orientationchange", "fullscreenchange", "pointerlockchange",
			"visibilitychange", "touchstart", "touchend", "touchmove", "touchcancel", "gamepadconnected", "gamepaddisconnected", "beforeunload",
			"batterychargingchange", "batterylevelchange", "webglcontextlost", "webglcontextrestored", "(invalid)" };
		++type;
		if (type < 0) type = 0;
		if (type >= sizeof(events)/sizeof(events[0])) type = sizeof(events)/sizeof(events[0])-1;
		return events[type];
	}

	int mouse_xpos_to_pos(int mouse_pos) {
		float p = (float)mouse_pos - graphics::window_width() / 2.0f;
		return std::floor(p);
	}

	int mouse_ypos_to_pos(int mouse_pos) {
		float p = (float)mouse_pos - graphics::window_height() / 2.0f;
		return -std::floor(p);
	}

	EM_BOOL mouse_callback(int type, const EmscriptenMouseEvent *e, void *user_data) {
		/*printf("%s, screen: (%ld,%ld), client: (%ld,%ld),%s%s%s%s button: %hu, buttons: %hu, movement: (%ld,%ld), target: (%ld, %ld)\n",
			emscripten_event_type_to_string(type).c_str(), e->screenX, e->screenY, e->clientX, e->clientY,
			e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "", 
			e->button, e->buttons, e->movementX, e->movementY, e->targetX, e->targetY);

		if (type == EMSCRIPTEN_EVENT_CLICK && e->screenX == -500000) {
			printf("ERROR! Received an event to a callback that should have been unregistered!\n");
		}*/

		event evt{.ctrl_key = static_cast<bool>(e->ctrlKey)};

		if (type == EMSCRIPTEN_EVENT_MOUSEMOVE) {
			for (auto &callback : s_mousemove_callbacks) {
				callback(mouse_xpos_to_pos(e->targetX), mouse_ypos_to_pos(e->targetY), evt);
			}
		}
		if (type == EMSCRIPTEN_EVENT_MOUSEDOWN) {
			s_is_dragging = true;
			for (auto &callback : s_mousedown_callbacks) {
				callback(mouse_xpos_to_pos(e->targetX), mouse_ypos_to_pos(e->targetY), evt);
			}
		}
		if (type == EMSCRIPTEN_EVENT_MOUSEUP) {
			s_is_dragging = false;
			for (auto &callback : s_mouseup_callbacks) {
				callback(mouse_xpos_to_pos(e->targetX), mouse_ypos_to_pos(e->targetY), evt);
			}
		}
		if (type == EMSCRIPTEN_EVENT_CLICK) {
			for (auto &callback : s_click_callbacks) {
				callback(mouse_xpos_to_pos(e->targetX), mouse_ypos_to_pos(e->targetY), evt);
			}
		}

		return 0;
	}

	EM_BOOL keyboard_callback(int type, const EmscriptenKeyboardEvent *e, void *user_data) {

		std::cout << "got keyboard_callback" << std::endl;

		if (type == EMSCRIPTEN_EVENT_KEYPRESS) {
		}
		if (type == EMSCRIPTEN_EVENT_KEYDOWN) {
			for (auto &callback : s_keydown_callbacks) {
				callback(e->key);
			}
		}
		if (type == EMSCRIPTEN_EVENT_KEYUP) {
		}

		return 0;
	}

	void init() {

		const char *target = "#canvas";

		EMSCRIPTEN_RESULT ret = emscripten_set_click_callback(target, 0, 1, mouse_callback);
		ret = emscripten_set_mousedown_callback(target, 0, 1, mouse_callback);
		ret = emscripten_set_mouseup_callback(target, 0, 1, mouse_callback);
		ret = emscripten_set_dblclick_callback(target, 0, 1, mouse_callback);
		ret = emscripten_set_mousemove_callback(target, 0, 1, mouse_callback);

		ret = emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, 0, true, keyboard_callback);
		ret = emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, 0, true, keyboard_callback);
		ret = emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, 0, true, keyboard_callback);
	}

	void on_mousemove(std::function<void(int, int, const event &)> callback) {
		s_mousemove_callbacks.push_back(callback);
	}

	void on_mousedown(std::function<void(int, int, const event &)> callback) {
		s_mousedown_callbacks.push_back(callback);
	}

	void on_mouseup(std::function<void(int, int, const event &)> callback) {
		s_mouseup_callbacks.push_back(callback);
	}

	void on_click(std::function<void(int, int, const event &)> callback) {
		s_click_callbacks.push_back(callback);
	}

	void on_keydown(std::function<void(std::string)> callback) {
		s_keydown_callbacks.push_back(callback);
	}

	bool is_dragging() {
		return s_is_dragging;
	}
}
