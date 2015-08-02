#!/bin/bash

set -e

cmake .
make -j4
cd web
for dir in */
do
	cd $dir
	mkdir -p bin
	if [ -f gulpfile.coffee ]
	then
		gulp
	fi
	if [[ $dir != "rcms/" && $dir != "node_modules/" ]]
	then
		mkdir -p ../../plugins/$dir
		mv bin ../../plugins/$dir/webgui
	fi
	cd ..
done
cd ..
mv web/rcms/bin webgui
