
#pragma once

namespace direction {

	typedef int type;

	const type west = 0;
	const type north = 1;
	const type east = 2;
	const type south = 3;

	type rotate_clockwise(type dir);
	type rotate_180(type dir);
	
};


