#!/bin/bash

# Build the PPT CLI
# Dependencies: nlohmann-json3-dev (Ubuntu: sudo apt install nlohmann-json3-dev)

g++ -std=c++17 -Wall -Wextra \
    -I/usr/include \
    main.cpp \
    CLI/Controller/Controller.cpp \
    CLI/Parser/Parser.cpp \
    CLI/Parser/Tokenizer.cpp \
    CLI/Repository/CommandRepository.cpp \
    CLI/Command/AddCommand.cpp \
    CLI/Command/RemoveCommand.cpp \
    CLI/Command/ListCommand.cpp \
    CLI/Command/SaveCommand.cpp \
    CLI/Command/LoadCommand.cpp \
    Document/Presentation.cpp \
    Document/Slide.cpp \
    Document/Elements/Shape.cpp \
    Document/Elements/BoundingBox.cpp \
    Serialization/JsonSerializer.cpp \
    Serialization/JsonDeserializer.cpp \
    Visualization/SvgShapeRenderer.cpp \
    Visualization/SlideRenderer.cpp \
    Action/Editor.cpp \
    Action/AddSlideAction.cpp \
    Action/RemoveSlideAction.cpp \
    Action/AddShapeAction.cpp \
    Action/RemoveShapeAction.cpp \
    -o ppt-cli

if [ $? -eq 0 ]; then
    echo "Build complete. Run: ./ppt-cli"
else
    echo "Build failed!"
fi
