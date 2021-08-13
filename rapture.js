
class Player {

	constructor() {
		this.state = {
			x_pos: 50,
			y_pos: 50,
			shape: "circle",
			radius: 20,
			width: 100,
			height: 100,
			color: "yellow",
			speed: 300,
		}
	}

	draw(ctx) {
		const {color, x_pos, y_pos, shape, radius, width, height} = this.state;
		if (shape == "circle") {
			ctx.fillStyle = color;
			ctx.beginPath();
			ctx.arc(x_pos, y_pos, radius, 0, 2*Math.PI);
			ctx.fill();
			//Draw mouth
			ctx.strokeStyle = "red";
			ctx.beginPath();
			ctx.lineWidth = 3;
			ctx.arc(x_pos, y_pos, radius*0.75, 0.25*Math.PI, 0.75*Math.PI);
			ctx.stroke();
			//Draw left eye
			ctx.fillStyle = "white";
			ctx.beginPath();
			ctx.arc(x_pos-radius*0.35, y_pos-radius*0.25, radius*0.25, 0, 2*Math.PI);
			ctx.fill();
			ctx.strokeStyle = "black";
			ctx.beginPath();
			ctx.lineWidth = 1;
			ctx.arc(x_pos-radius*0.35, y_pos-radius*0.25, radius*0.25, 0, 2*Math.PI);
			ctx.stroke();
			ctx.fillStyle = "black";
			ctx.beginPath();
			ctx.arc(x_pos-radius*0.35, y_pos-radius*0.25, radius*0.1, 0, 2*Math.PI);
			ctx.fill();
			//Draw right eye
			ctx.fillStyle = "white";
			ctx.beginPath();
			ctx.arc(x_pos+radius*0.35, y_pos-radius*0.25, radius*0.25, 0, 2*Math.PI);
			ctx.fill();
			ctx.strokeStyle = "black";
			ctx.beginPath();
			ctx.lineWidth = 1;
			ctx.arc(x_pos+radius*0.35, y_pos-radius*0.25, radius*0.25, 0, 2*Math.PI);
			ctx.stroke();
			ctx.fillStyle = "black";
			ctx.beginPath();
			ctx.arc(x_pos+radius*0.35, y_pos-radius*0.25, radius*0.1, 0, 2*Math.PI);
			ctx.fill();
		} else if (shape == "rect"){
			ctx.fillRect(x_pos-(width*0.5), y_pos-(height*0.5), width, height);
		}

	}

	update(global_state, dt) {
		const {color, x_pos, y_pos, shape, radius, width, height, speed} = this.state;
		const distance = speed * dt //Speed in pixels per second multiplied by seconds since last update
		var left_edge = x_pos - (width*0.5)
		var right_edge = x_pos + (width*0.5)
		var top_edge = y_pos - (height*0.5)
		var bottom_edge = y_pos + (height*0.5)
		if (shape == "circle"){
			var left_edge = x_pos - radius
			var right_edge = x_pos + radius
			var top_edge = y_pos - radius
			var bottom_edge = y_pos + radius
		}
		if (global_state.right_pressed && right_edge < global_state.canvas_width) {
			this.state.x_pos += distance;
		}
		if (global_state.left_pressed && left_edge > 0) {
			this.state.x_pos -= distance;
		}
		if (global_state.down_pressed && bottom_edge < global_state.canvas_height) {
			this.state.y_pos += distance;
		}
		if (global_state.up_pressed && top_edge > 0) {
			this.state.y_pos -= distance;
		}
	}

}

class Background {

	constructor(initial_width, initial_height) {
		this.state = {
			color: "black",
			width: initial_width,
			height: initial_height
		}
	}

	draw(ctx) {
		const {color, width, height} = this.state;
		ctx.fillStyle = color;
		ctx.fillRect(0, 0, width, height);
	}

	update(global_state, dt) {
		this.state.width = global_state.canvas_width;
		this.state.height = global_state.canvas_height;
		
	}

}

var initial_state = function (canvas) {
	return {
		right_pressed: false,
		left_pressed: false,
		up_pressed: false,
		down_pressed: false,
		canvas_width: canvas.width,
		canvas_height: canvas.height,
		player: new Player(),
		background: new Background(canvas.width, canvas.height)
	};
}


var draw = function (ctx, state, dt) {
	state.background.draw(ctx)
	state.player.draw(ctx)

	state.player.update(state, dt);
	state.background.update(state, dt);

};

var clear_canvas = function (ctx, state) {
	ctx.clearRect(0, 0, state.canvas_width, state.canvas_height);
};

var on_keydown = function (event, state) {
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

var on_keyup = function (event, state) {
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
var main_loop = function (ctx, state, timestamp) {

	var dt = (timestamp - old_timestamp) / 1000;
	old_timestamp = timestamp;

	fps = Math.round(1.0 / dt);

	clear_canvas(ctx, state);
	draw(ctx, state, dt);

	requestAnimationFrame(function (timestamp) {
		main_loop(ctx, state, timestamp);
	});
}

var start_main_loop = function (canvas) {
	var ctx = canvas.getContext("2d");
	var state = initial_state(canvas);

	document.addEventListener("keydown", (event) => on_keydown(event, state));
	document.addEventListener("keyup", (event) => on_keyup(event, state));

	requestAnimationFrame(function (timestamp) {
		old_timestamp = timestamp;
		main_loop(ctx, state, timestamp);
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


