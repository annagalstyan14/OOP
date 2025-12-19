#!/bin/bash

# Build and run unit tests
# Dependencies:
#   Ubuntu: sudo apt install nlohmann-json3-dev
#   macOS:  brew install nlohmann-json

# Detect OS and set include path
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS with Homebrew
    JSON_INCLUDE="-I$(brew --prefix nlohmann-json)/include"
else
    # Linux
    JSON_INCLUDE="-I/usr/include"
fi

echo "Building unit tests..."

g++ -std=c++17 -Wall -Wextra -g \
    $JSON_INCLUDE \
    -I. \
    Tests/TestRunner.cpp \
    Parser/Tokenizer.cpp \
    Parser/Parser.cpp \
    Command/CommandFactory.cpp \
    Command/Argument.cpp \
    Document/Presentation.cpp \
    Document/Slide.cpp \
    Document/BaseSlideObject.cpp \
    Document/Rectangle.cpp \
    Document/Circle.cpp \
    Document/Line.cpp \
    Document/TextObject.cpp \
    Rendering/SVGPainter.cpp \
    -o run_tests

if [ $? -eq 0 ]; then
    echo "Build complete. Running tests..."
    echo ""
    ./run_tests
    exit $?
else
    echo "Build failed!"
    exit 1
fi
