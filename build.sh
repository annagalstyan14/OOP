#!/bin/bash

# Build PPT CLI
# Dependencies: sudo apt install nlohmann-json3-dev

if [[ "$OSTYPE" == "darwin"* ]]; then
    JSON_INCLUDE="-I$(brew --prefix nlohmann-json)/include"
else
    JSON_INCLUDE="-I/usr/include"
fi

echo "Building PPT CLI..."

g++ -std=c++17 -Wall -Wextra -O2 \
    $JSON_INCLUDE \
    -I. \
    main.cpp \
    Document/Presentation.cpp \
    Document/Slide.cpp \
    CLI/Argument.cpp \
    CLI/Parser/Tokenizer.cpp \
    CLI/Parser/Parser.cpp \
    CLI/Command/AddCommand.cpp \
    CLI/Command/RemoveCommand.cpp \
    CLI/Command/ListCommand.cpp \
    CLI/Command/SaveCommand.cpp \
    CLI/Command/LoadCommand.cpp \
    CLI/Repository/CommandRepository.cpp \
    CLI/Controller/Controller.cpp \
    Document/BaseSlideObject.cpp \
    Document/Rectangle.cpp \
    Document/Circle.cpp \
    Document/Line.cpp \
    Document/TextObject.cpp \
    Rendering/SVGPainter.cpp \
    Rendering/RenderCommand.cpp \
    -o ppt-cli

if [ $? -eq 0 ]; then
    echo "Build complete. Run: ./ppt-cli"
else
    echo "Build failed!"
    exit 1
fi
