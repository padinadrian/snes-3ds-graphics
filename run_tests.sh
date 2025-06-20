#!/usr/bin/env bash

BUILD_DIR="build.local"
TEST_FILENAME="$BUILD_DIR/bin/run_tests"

rm $TEST_FILENAME
make run_tests -C $BUILD_DIR
$TEST_FILENAME
