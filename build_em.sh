#!/bin/bash

cd `dirname $0`

if [ ! -d "build/emsdk" ]; then

	mkdir build
	cd build
	git clone https://github.com/emscripten-core/emsdk.git
	cd emsdk
	./emsdk install latest
	./emsdk activate latest
	source ./emsdk_env.sh 2> /dev/null
	cd ..

else

	cd build
	source emsdk/emsdk_env.sh > /dev/null 2> /dev/null

fi

mkdir -p textures
cp ../textures/* ./textures/

emcc ../src/main.cpp ../src/graphics.cpp -std=c++14 -s WASM=1 -s USE_SDL=2  -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s ALLOW_MEMORY_GROWTH=1  -s USE_WEBGL2=1 --use-preload-plugins --preload-file textures/ -g -o index.html
