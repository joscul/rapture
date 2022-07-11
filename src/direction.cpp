
#include "direction.h"

namespace direction {
	type rotate_clockwise(type dir) {
		return (dir + 1) % 4;
	}

	type rotate_counterclockwise(type dir) {
		return (dir + 3) % 4;
	}

	type rotate_180(type dir) {
		return (dir + 2) % 4;
	}
}
