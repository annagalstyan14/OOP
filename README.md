# PPT-CLI: PowerPoint Command Line Interface

A C++ command-line application for creating and managing presentations with undo/redo support.

## Features

- Create and manage presentations with slides and shapes
- Add shapes: rectangles, circles, lines, text
- Full undo/redo support
- Save/load presentations as JSON
- Export slides to SVG format
- Design patterns: Command, Action, Factory, Observer

## Architecture

### Namespaces

- **ppt**: Core document model (Presentation, Slide, SlideObject hierarchy)
- **ppt_cli**: CLI interface layer and alternative serialization

### Design Patterns Used

1. **Command Pattern**: CLI commands (AddCommand, RemoveCommand, ListCommand, etc.)
2. **Action Pattern**: Undoable actions (AddSlideAction, RemoveShapeAction, etc.)
3. **Factory Pattern**: CommandCreators for command instantiation
4. **Strategy Pattern**: IShapeRenderer for different rendering approaches

## Building

### Prerequisites

- CMake 3.16+
- C++17 compiler (GCC 9+, Clang 10+, MSVC 2019+)
- nlohmann-json library

### Ubuntu/Debian

```bash
sudo apt install cmake nlohmann-json3-dev
```

### Build Steps

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
./ppt-cli
```

### Commands

| Command | Description |
|---------|-------------|
| `new` | Create a new presentation |
| `open <file>` | Load a presentation from JSON |
| `save [file]` | Save the current presentation |
| `add slide [title]` | Add a new slide |
| `add shape <name> [type] [color]` | Add a shape (rectangle, circle, line) |
| `add text <area> "content"` | Add text to a slide |
| `remove slide <id>` | Remove a slide by ID |
| `remove shape <id>` | Remove a shape by ID |
| `list` | List all slides and objects |
| `undo` | Undo the last action |
| `redo` | Redo the last undone action |
| `export [basename]` | Export slides to SVG |
| `exit` | Exit the application |

### Example Session

```
ppt-cli> new
Created new presentation.

ppt-cli> add slide "Introduction"
Added slide 1: Introduction

ppt-cli> add shape rect1 rectangle blue
Added shape rect1 to slide 1

ppt-cli> add shape circle1 circle red
Added shape circle1 to slide 1

ppt-cli> list
Presentation (1 slides):
  Slide 1: Introduction (2 objects)
    - rect1 (rectangle) - blue
    - circle1 (circle) - red

ppt-cli> save myPresentation.json
Saved to myPresentation.json

ppt-cli> export mySlides
Exported 1 slides to SVG.

ppt-cli> exit
Goodbye!
```

## Project Structure

```
ppt-cli/
├── Action/              # Undoable actions (Add/Remove Slide/Shape)
├── CLI/
│   ├── Command/         # CLI commands
│   ├── Controller/      # Main CLI controller
│   ├── Parser/          # Command parser and tokenizer
│   └── Repository/      # Command registry
├── Document/
│   ├── Elements/        # ppt_cli namespace elements
│   ├── Presentation.*   # Main presentation class
│   ├── Slide.*          # Slide container
│   └── *Object.*        # Shape classes
├── Geometry/            # Point and Geometry structs
├── Serialization/       # JSON serialization
├── Visualization/       # SVG rendering
├── Main.cpp             # Entry point
└── CMakeLists.txt       # Build configuration
```

## License

MIT License
