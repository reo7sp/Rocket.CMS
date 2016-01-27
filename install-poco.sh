#!/bin/bash

set -e

POCO_VERSION=1.6.0

mkdir -p _poco
cd _poco

wget http://pocoproject.org/releases/poco-$POCO_VERSION/poco-$POCO_VERSION.tar.gz
tar xzf poco-$POCO_VERSION.tar.gz
cd poco-$POCO_VERSION

./configure --prefix=/usr/local --no-samples --no-tests
make -j4
make install

cd ../..
rm -rf _poco
