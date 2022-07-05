#!/bin/bash

cd `dirname $0`

# Make sure npm is installed.

em_version=latest

# use the following line instead if running on raspberry PI for example. But it takes many hours to build on raspberry...
#em_version=sdk-upstream-master-32bit

if [ ! -d "build/emsdk" ]; then

	mkdir build
	cd build
	git clone https://github.com/emscripten-core/emsdk.git
	cd emsdk
	./emsdk install $em_version
	./emsdk activate $em_version
	source ./emsdk_env.sh 2> /dev/null
	cd ..

else

	cd build
	source emsdk/emsdk_env.sh > /dev/null 2> /dev/null

fi

mkdir -p textures
cp ../textures/* ./textures/

emcc ../src/main.cpp ../src/graphics.cpp ../src/entity.cpp -std=c++17 -s WASM=1 -s USE_SDL=2  -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s ALLOW_MEMORY_GROWTH=1  -s USE_WEBGL2=1 --use-preload-plugins --preload-file textures/ -g -o index.html
