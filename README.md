# OOP

# ppt-cli: Command Line Interface for Creating PowerPoint Presentations

`ppt-cli` is a lightweight, command-line tool for creating, editing, and managing PowerPoint presentations without a graphical user interface. It supports creating slides, adding text, images, charts, and more, with export options for PPTX, PDF, and image formats.

## Features

- **File Management**: Create, open, save, and export presentations.
- **Slide Management**: Add, delete, reorder, and list slides.
- **Content Creation**: Add text, images, shapes, tables, and charts to slides.
- **Formatting**: Apply themes, fonts, backgrounds, and alignments.
- **Transitions & Animations**: Set slide transitions and element animations.
- **Batch Processing**: Support for scripting and automation.
- **Cross-Platform**: Works on Windows, macOS, and Linux.
- **Extensible**: Supports custom templates and plugins.

- ## Installation

- pip install ppt-cli

- ## Interactive Mode
- **Launch the CLI shell**: ppt-cli
- Once inside, use the following commands:

- ## File Management

- 1. create presentation.pptx
  2. open presentation.pptx
  3. save presentation.pptx
  4. export presentation.pptx --format pdf
  5. export presentation.pptx -- format png --slide index
 
  ## Slide Management

  1. slide add --title
  2. slide delete --index
  3. slide list
  4. slide reorder --from index --to index

  ## Content creation

  1. add text --slide index --context
  2. add image --slide index --path --position
  3. add chart --slide --type --data
  4. add table --slide --data
 
  ## Formatting

  1. theme apply --name
  2. font set --slide --font --size
  3. backgrund set --slide --color
  4. align text --slide --position
 
  ## Example Session
  
ppt-cli
 create demo.pptx
 slide add --title "Welcome"
 add text --slide 1 --content "Hello, world!"
 add image --slide 1 --path ./logo.png
 save demo.pptx
 export demo.pptx --format pdf
