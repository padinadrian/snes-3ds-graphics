#!/usr/bin/env bash

BUILD_DIR="build"
TEST_FILENAME="$BUILD_DIR/bin/unittests"

rm $TEST_FILENAME
make unittests -C $BUILD_DIR
$TEST_FILENAME
