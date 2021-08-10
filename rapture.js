
class Map {

	constructor() {
	}

}

var x_pos = 0;
var draw = function (ctx, canvas_width, canvas_height, dt) {
	ctx.fillStyle = 'green';
	ctx.fillRect(x_pos, 10, 40, 40);
	if (x_pos < 500) {
		x_pos += 100 * dt;
	}

};

var clear_canvas = function (ctx, canvas_width, canvas_height) {
	ctx.clearRect(0, 0, canvas_width, canvas_height);
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
		main_loop(ctx, canvas.width, canvas.height, timestamp);
	});

};

var main = function () {
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

