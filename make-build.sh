#!/bin/bash

set -e

cmake .
make -j4
mkdir -p webgui
cd web
for dir in */
do
	cd $dir
	mkdir -p bin
	if [ -f gulpfile.coffee ]
	then
		gulp
	fi
	mv bin ../../webgui/$dir
	cd ..
done
