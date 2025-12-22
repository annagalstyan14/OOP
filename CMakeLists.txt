cmake_minimum_required(VERSION 3.16)
project(ppt-cli VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find nlohmann_json
find_package(nlohmann_json 3.11 QUIET)
if(NOT nlohmann_json_FOUND)
    message(STATUS "nlohmann_json not found, please install: apt install nlohmann-json3-dev")
endif()

# Document sources
set(DOCUMENT_SOURCES
    Document/BaseSlideObject.cpp
    Document/Rectangle.cpp
    Document/Circle.cpp
    Document/Line.cpp
    Document/TextObject.cpp
)

# CLI sources
set(CLI_SOURCES
    CLI/Argument.cpp
    CLI/Tokenizer.cpp
    CLI/Parser.cpp
    CLI/CommandFactory.cpp
    CLI/Controller.cpp
)

# Rendering sources
set(RENDERING_SOURCES
    Rendering/SVGPainter.cpp
    Rendering/RenderCommand.cpp
)

# Main executable
add_executable(ppt-cli
    main.cpp
    ${DOCUMENT_SOURCES}
    ${CLI_SOURCES}
    ${RENDERING_SOURCES}
)

target_include_directories(ppt-cli PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ppt-cli PRIVATE nlohmann_json::nlohmann_json)

# Compiler warnings
if(MSVC)
    target_compile_options(ppt-cli PRIVATE /W4)
else()
    target_compile_options(ppt-cli PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation
install(TARGETS ppt-cli DESTINATION bin)
