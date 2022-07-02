#!/bin/bash

cd `dirname $0`
cd build

python3 -m http.server
