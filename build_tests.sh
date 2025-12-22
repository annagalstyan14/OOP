#!/bin/bash

#!/bin/bash

# Build and run unit tests via CMake

set -e

echo "Configuring tests with CMake..."

mkdir -p build
cd build

cmake .. >/dev/null

echo "Building unit tests (run_tests target)..."
cmake --build . --target run_tests >/dev/null

echo "Running tests..."
./run_tests

