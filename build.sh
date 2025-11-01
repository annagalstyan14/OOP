#!/bin/bash

# Build the CLI with nlohmann/json from Homebrew
g++ -std=c++17 -Wall -Wextra \
    -I$(brew --prefix nlohmann-json)/include \
    main.cpp \
    Parser/Parser.cpp Tokenizer/Tokenizer.cpp Semantic/Semantic.cpp \
    Parser/CommandFactory.cpp \
    Command/AddCommand.cpp Command/RemoveCommand.cpp Command/ListCommand.cpp \
    Document/Presentation.cpp Document/Slide.cpp \
    Document/Models/Shape.cpp Document/Models/BoundingBox.cpp \
    -o main

if [ $? -eq 0 ]; then
    echo "Build complete. Run: ./main"
else
    echo "Build failed!"
fi
