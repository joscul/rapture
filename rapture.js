
class Map {

	constructor() {
	}

}

var state = {
	x_pos: 0,
	y_pos: 0,
	width: 100,
	height: 100,
	color: "green",
	speed: 300,
	right_pressed: false,
	left_pressed: false,
	up_pressed: false,
	down_pressed: false
}

var draw = function (ctx, canvas_width, canvas_height, dt) {
	ctx.fillStyle = state.color;
	ctx.fillRect(state.x_pos, state.y_pos, state.width, state.height);
	ctx.fillStyle = "red";
	ctx.beginPath();
	ctx.arc(100, 75, 50, 0, Math.PI);
	ctx.fill();
	if (state.right_pressed && (state.x_pos+state.width) < canvas_width) {
		state.x_pos += state.speed * dt; //Speed in pixels per second multiplied by seconds since last update
	}
	if (state.left_pressed && state.x_pos > 0) {
		state.x_pos -= state.speed * dt; //Speed in pixels per second multiplied by seconds since last update
	}
	if (state.down_pressed && (state.y_pos+state.height) < canvas_height) {
		state.y_pos += state.speed * dt; //Speed in pixels per second multiplied by seconds since last update
	}
	if (state.up_pressed && state.y_pos > 0) {
		state.y_pos -= state.speed * dt; //Speed in pixels per second multiplied by seconds since last update
	}

};

var clear_canvas = function (ctx, canvas_width, canvas_height) {
	ctx.clearRect(0, 0, canvas_width, canvas_height);
};

var on_keydown = function (event) {
	if (event.key == "ArrowRight") {
		state.right_pressed = true;
	}
	if (event.key == "ArrowLeft") {
		state.left_pressed = true;
	}
	if (event.key == "ArrowDown") {
		state.down_pressed = true;
	}
	if (event.key == "ArrowUp") {
		state.up_pressed = true;
	}
};

var on_keyup = function (event) {
	if (event.key == "ArrowRight") {
		state.right_pressed = false;
	}
	if (event.key == "ArrowLeft") {
		state.left_pressed = false;
	}
	if (event.key == "ArrowDown") {
		state.down_pressed = false;
	}
	if (event.key == "ArrowUp") {
		state.up_pressed = false;
	}
};

var old_timestamp = 0;
var fps = 0;
var main_loop = function (ctx, canvas_width, canvas_height, timestamp) {

	var dt = (timestamp - old_timestamp) / 1000;
	old_timestamp = timestamp;

	fps = Math.round(1.0 / dt);

	clear_canvas(ctx, canvas_width, canvas_height);
	draw(ctx, canvas_width, canvas_height, dt);

	requestAnimationFrame(function (timestamp) {
		main_loop(ctx, canvas_width, canvas_height, timestamp);
	});
}

var start_main_loop = function (canvas) {
	var ctx = canvas.getContext("2d");

	requestAnimationFrame(function (timestamp) {
		old_timestamp = timestamp;
		main_loop(ctx, canvas.width, canvas.height, timestamp);
	});

};

var main = function () {
	document.addEventListener("keydown", on_keydown);
	document.addEventListener("keyup", on_keyup);

	var canvas = document.createElement("canvas");

	canvas.id = "Main";
	canvas.width = window.innerWidth;
	canvas.height = window.innerHeight;
	canvas.style.zIndex = 8;
	canvas.style.position = "absolute";

	var body = document.getElementsByTagName("body")[0];
	body.appendChild(canvas);
	body.style["margin"] = "0px";
	body.style["padding"] = "0px";

	start_main_loop(canvas);
};

main();


