# PPT-CLI: PowerPoint Command Line Interface

A C++ command-line application for creating and managing presentations with undo/redo support.

## Project Structure

```
ppt-cli/
├── Action/
│   ├── IAction.h           # Action interface
│   ├── Editor.h            # Undo/redo stack manager
│   ├── ShapeActions.h      # Add/remove shape actions
│   └── SlideActions.h      # Add/remove slide actions
├── CLI/
│   ├── ICommand.h          # CLI command interface
│   ├── Argument.h/cpp      # Command argument wrapper
│   ├── Tokenizer.h/cpp     # Lexical tokenizer
│   ├── Parser.h/cpp        # Command parser
│   ├── CommandFactory.h/cpp # Factory for creating commands
│   └── Controller.h/cpp    # Main CLI controller
├── Document/
│   ├── SlideObject.h       # Abstract shape interface
│   ├── BaseSlideObject.h/cpp # Base implementation
│   ├── Rectangle.h/cpp     # Rectangle shape
│   ├── Circle.h/cpp        # Circle shape
│   ├── Line.h/cpp          # Line shape
│   ├── TextObject.h/cpp    # Text object
│   ├── Slide.h             # Slide container (header-only)
│   ├── Presentation.h      # Presentation container (header-only)
│   └── JSONSerializer.h    # Serializer & Deserializer classes
├── Geometry/
│   └── Geometry.h          # Point and Geometry structs
├── Rendering/
│   ├── IBrush.h            # Brush interface
│   ├── IPen.h              # Pen interface
│   ├── ICanvas.h           # Canvas interface
│   ├── IPainter.h          # Painter interface
│   ├── PaintProperties.h   # Paint properties struct
│   ├── SVGCanvas.h         # SVG canvas implementation
│   ├── SVGPainter.h/cpp    # SVG painter implementation
│   └── RenderCommand.h/cpp # Render command
├── main.cpp
├── CMakeLists.txt
└── build.sh
```

## Design Patterns

1. **Command Pattern**: CLI commands (`AddCommand`, `RemoveCommand`, etc.)
2. **Factory Pattern**: `CommandFactory` creates command instances
3. **Action/Memento Pattern**: Undoable actions with state preservation
4. **Strategy Pattern**: Rendering interfaces (`IPainter`, `ICanvas`, `IBrush`, `IPen`)

## Serialization

Serialization and deserialization are separated into distinct classes:

- `Serializer`: Converts `Presentation` → JSON, saves to file
- `Deserializer`: Loads JSON → `Presentation`

```cpp
// Serialize
ppt::Serializer::saveToFile(presentation, "output.json");
std::string json = ppt::Serializer::toString(presentation);

// Deserialize
ppt::Deserializer::loadFromFile(presentation, "input.json");
ppt::Deserializer::fromString(presentation, jsonString);
```

## Building

### Prerequisites

```bash
sudo apt install cmake nlohmann-json3-dev
```

### Build with CMake

```bash
mkdir build && cd build
cmake ..
make
```

### Build with Shell Script

```bash
chmod +x build.sh
./build.sh
```

## Usage

```bash
./ppt-cli
```

### Commands

| Command | Description |
|---------|-------------|
| `new` | Create new presentation |
| `open <file>` | Load presentation from JSON |
| `save [file]` | Save presentation |
| `add slide [title]` | Add a new slide |
| `add shape <n> [type] [color]` | Add a shape |
| `add text <area> "content"` | Add text |
| `remove slide <id>` | Remove slide |
| `remove shape <id>` | Remove shape |
| `list` | List all contents |
| `undo` | Undo last action |
| `redo` | Redo last undone action |
| `render [basename]` | Export to SVG |
| `exit` | Exit application |

## License

MIT License
