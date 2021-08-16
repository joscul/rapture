
var if_obstacle_at_position = function(obstacles, x_pos, y_pos) {
	for(var i in obstacles) {
		if	(obstacles[i].overlaps_with_position(x_pos, y_pos)) {
			return true;
		}
	}
	return false;
}

var available_slot = function(inventory_slots) {
	for(var i in inventory_slots) {
		if	(inventory_slots[i].state.available_slot) {
			return i;
		}
	}
	return null;
}

var if_obstacle_at_coordinates = function(obstacles, coordinates) {
	for(var n in coordinates) {
		var coordinate = coordinates[n];
		for(var i in obstacles) {
			if	(obstacles[i].overlaps_with_position(coordinate[0], coordinate[1])) {
				return true;
			}
		}
	}
	return false;
	
}

var if_items_at_position = function(items, x_pos, y_pos) {
	for(var i in items) {
		if	(items[i].overlaps_with_position(x_pos, y_pos)) {
			return true;
		}
	}
	return false;
}

class Obstacle {

	constructor(initial_width, initial_height, initial_xpos, initial_ypos) {
		this.state = {
			color: "green",
			width: initial_width,
			height: initial_height,
			x_pos: initial_xpos, 
			y_pos: initial_ypos,
		}
	}

	draw(ctx) {
		const {color, width, height, x_pos, y_pos} = this.state;
		ctx.fillStyle = color;
		ctx.fillRect(x_pos, y_pos, width, height);
	}

	update(global_state, dt) {
		
	}

	overlaps_with_position(x, y) {
		const {width, height, x_pos, y_pos} = this.state;
		if (x > x_pos+width){
			return false;
		}
		if (x < x_pos){
			return false;
		}
		if (y > y_pos+height){
			return false;
		}
		if (y < y_pos){
			return false;
		}
		return true;
	}

}

class InventorySlot {

	constructor(initial_xpos, initial_ypos) {
		this.state = {
			color: "black",
			width: 40,
			height: 40,
			x_pos: initial_xpos, 
			y_pos: initial_ypos,
			available_slot: true
		}
	}

	draw(ctx) {
		const {color, width, height, x_pos, y_pos} = this.state;
		ctx.fillStyle = color;
		ctx.fillRect(x_pos, y_pos, width, height);
	}

	update(global_state, dt) {
		
	}

	
	place_item() {
		this.state.available_slot = false;
	}

	use_item() {
		this.state.available_slot = true;
	}
}

class Item {

	constructor(initial_width, initial_height, initial_xpos, initial_ypos, initial_img_name) {
		var img = new Image(); // Image constructor
		img.src = initial_img_name;
		this.state = {
			image: img,
			width: initial_width,
			height: initial_height,
			
			x_pos: initial_xpos, 
			y_pos: initial_ypos,
			is_in_inventory: false,
			inventory_slot: null,
			used_up_item: false
		}
	}

	draw(ctx) {
		const {image, width, height, x_pos, y_pos, is_in_inventory} = this.state;
		ctx.drawImage(image, x_pos, y_pos);

	}

	update(global_state, dt) {
		if (this.overlaps_with_position (global_state.player.state.x_pos, global_state.player.state.y_pos)){
			this.state.is_in_inventory = true;
			this.state.inventory_slot = available_slot(global_state.inventory_slots);
			this.state.x_pos = global_state.inventory_slots[this.state.inventory_slot].state.x_pos;
			this.state.y_pos = global_state.inventory_slots[this.state.inventory_slot].state.y_pos;
			global_state.inventory_slots[this.state.inventory_slot].place_item();
		}

	}

	overlaps_with_position(x, y) {
		const {width, height, x_pos, y_pos} = this.state;
		if (x > x_pos+width){
			return false;
		}
		if (x < x_pos){
			return false;
		}
		if (y > y_pos+height){
			return false;
		}
		if (y < y_pos){
			return false;
		}
		return true;
	}

}
class Player {

	constructor() {
		this.state = {
			x_pos: 55,
			y_pos: 100,
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
			ctx.arc(x_pos-radius*0.35, y_pos-radius*0.14, radius*0.25, 0, 2*Math.PI);
			ctx.fill();
			ctx.strokeStyle = "black";
			ctx.beginPath();
			ctx.lineWidth = 1;
			ctx.arc(x_pos-radius*0.35, y_pos-radius*0.14, radius*0.25, 0, 2*Math.PI);
			ctx.stroke();
			ctx.fillStyle = "black";
			ctx.beginPath();
			ctx.arc(x_pos-radius*0.35, y_pos-radius*0.14, radius*0.1, 0, 2*Math.PI);
			ctx.fill();
			//Draw right eye
			ctx.fillStyle = "white";
			ctx.beginPath();
			ctx.arc(x_pos+radius*0.35, y_pos-radius*0.14, radius*0.25, 0, 2*Math.PI);
			ctx.fill();
			ctx.strokeStyle = "black";
			ctx.beginPath();
			ctx.lineWidth = 1;
			ctx.arc(x_pos+radius*0.35, y_pos-radius*0.14, radius*0.25, 0, 2*Math.PI);
			ctx.stroke();
			ctx.fillStyle = "black";
			ctx.beginPath();
			ctx.arc(x_pos+radius*0.35, y_pos-radius*0.14, radius*0.1, 0, 2*Math.PI);
			ctx.fill();
			//Draw hat
			ctx.fillStyle = "brown"
			ctx.beginPath();
			ctx.arc(x_pos, y_pos, radius, 1.14*Math.PI, 1.86*Math.PI);
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

		var leftCoordinates = [
			[x_pos - distance - radius * 0.5, y_pos - radius],
			[x_pos - distance - radius, y_pos - radius * 0.5],
			[x_pos - distance - radius, y_pos],
			[x_pos - distance - radius, y_pos + radius * 0.5],
			[x_pos - distance - radius * 0.5, y_pos + radius]
		];
		var rightCoordinates = [
			[distance + x_pos + radius * 0.5, y_pos - radius],
			[distance + x_pos + radius, y_pos - radius * 0.5],
			[distance + x_pos + radius, y_pos],
			[distance + x_pos + radius, y_pos + radius * 0.5],
			[distance + x_pos + radius * 0.5, y_pos + radius]
		];
		var topCoordinates = [
			[x_pos - radius, y_pos - distance - radius * 0.5],
			[x_pos - radius * 0.5, y_pos - distance - radius],
			[x_pos, y_pos - distance - radius],
			[x_pos + radius * 0.5, y_pos - distance - radius],
			[x_pos + radius, y_pos - distance - radius * 0.5]
		];
		var bottomCoordinates = [
			[x_pos - radius, y_pos + distance + radius * 0.5],
			[x_pos - radius * 0.5, y_pos + distance + radius],
			[x_pos, y_pos + distance + radius],
			[x_pos + radius * 0.5, y_pos + distance + radius],
			[x_pos + radius, y_pos + distance + radius * 0.5]
		];

		if (global_state.right_pressed && !if_obstacle_at_coordinates(global_state.obstacles, rightCoordinates)) {
			this.state.x_pos += distance;
		}
		if (global_state.left_pressed && !if_obstacle_at_coordinates(global_state.obstacles, leftCoordinates)) {
			this.state.x_pos -= distance;
		}
		if (global_state.down_pressed && !if_obstacle_at_coordinates(global_state.obstacles, bottomCoordinates)) {
			this.state.y_pos += distance;
		}
		if (global_state.up_pressed && !if_obstacle_at_coordinates(global_state.obstacles, topCoordinates) ) {
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
	var inventory_slots_create = [];
	for (var i = 0; i < 10; i++) {
		inventory_slots_create.push(new InventorySlot(10 + i * 50, 10))
	}
	return {
		right_pressed: false,
		left_pressed: false,
		up_pressed: false,
		down_pressed: false,
		canvas_width: canvas.width,
		canvas_height: canvas.height,
		player: new Player(),
		background: new Background(canvas.width, canvas.height),
		obstacles: [
			new Obstacle(30,canvas.height ,0 ,0),//left edge
			new Obstacle(canvas.width,70,0,0),//top edge
			new Obstacle(30, canvas.height, canvas.width - 30, 0),//right edge
			new Obstacle(canvas.width, 30, 0, canvas.height - 30),//bottom edge
			new Obstacle(200, 100, 10, 250),
			new Obstacle(200, 100, 100, 30),
			new Obstacle(200, 100, 100, 700),
			new Obstacle(100, 200, 270, 150),
			new Obstacle(30, canvas.height/1.2, canvas.width/2, 0),
		],
		items: [
			new Item(40, 40, canvas.width-100, 100, "key.png"),
			new Item(40, 40, canvas.width-100, 500, "key.png"),
			
		],
		inventory_slots: inventory_slots_create
	};

}


var draw = function (ctx, state, dt) {
	state.background.draw(ctx);
	for(var i in state.obstacles){
		state.obstacles[i].draw(ctx);
	}
	for(var i in state.inventory_slots){
		state.inventory_slots[i].draw(ctx);
	}
	for(var i in state.items){
		state.items[i].draw(ctx);
	}
	state.player.draw(ctx);	

	state.player.update(state, dt);	
	state.background.update(state, dt);
	for(var i in state.obstacles){
		state.obstacles[i].update(state, dt);
	}
	for(var i in state.items){
		state.items[i].update(state, dt);
	}
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


