cmake_minimum_required(VERSION 3.16)
project(ppt-cli VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find nlohmann_json (system-installed or FetchContent)
find_package(nlohmann_json 3.11 QUIET)
if(NOT nlohmann_json_FOUND)
    message(STATUS "nlohmann_json not found, please install it via: apt install nlohmann-json3-dev")
endif()

# Source files
set(DOCUMENT_SOURCES
    Document/BaseSlideObject.cpp
    Document/Rectangle.cpp
    Document/Circle.cpp
    Document/Line.cpp
    Document/TextObject.cpp
    Document/Slide.cpp
    Document/Presentation.cpp
)

set(CLI_SOURCES
    CLI/Command/AddCommand.cpp
    CLI/Command/RemoveCommand.cpp
    CLI/Command/ListCommand.cpp
    CLI/Command/SaveCommand.cpp
    CLI/Command/LoadCommand.cpp
    CLI/Parser/Tokenizer.cpp
    CLI/Parser/Parser.cpp
    CLI/Repository/CommandRepository.cpp
    CLI/Controller/Controller.cpp
)

set(SERIALIZATION_SOURCES
    Serialization/JsonSerializer.cpp
    Serialization/JsonDeserializer.cpp
)

set(VISUALIZATION_SOURCES
    Visualization/SvgShapeRenderer.cpp
    Visualization/SlideRenderer.cpp
)

# Create executable
add_executable(ppt-cli
    Main.cpp
    ${DOCUMENT_SOURCES}
    ${CLI_SOURCES}
    ${SERIALIZATION_SOURCES}
    ${VISUALIZATION_SOURCES}
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
