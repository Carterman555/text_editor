# Text Editor
---
This text editor was built from scratch using SFML.

Warning: This program was built as a learning project and has not been properly tested on all
platforms. To be safe, avoid opening any important documents with this text editor.

## Getting Started
---

Download the release file for your platform.

### Windows

You can open text-editor.exe directly without attaching it to a file. You can also open an existing
text file by dragging it onto text-editor.exe or via "Open with".

### MacOS

Warning: the build has not been tested on 

### Linux



## Functionality

Run `./text_editor` to open the text editor window.

`./text_editor [FILEPATH]` can be used to open an existing text file.
- Warning: the text editor only supports ASCII characters. Opening and saving a file that contains
non-ASCII characters will remove the non-ASCII characters from that file.

You can type, delete, and select characters.

Use the arrow keys to move the caret position.
- `shift + arrow key` will select the text your caret moves through.

Double clicking will select the word.
Triple clicking will select the line.

### Commands

