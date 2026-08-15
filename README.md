# Text Editor
---
This text editor was built from scratch using SFML.

Warning: This program was built as a learning project and has not been properly tested on all
platforms. To be safe, avoid opening any important files with this text editor.

## Getting Started

Warning: the text editor only supports ASCII characters. Opening and saving a file that contains
non-ASCII characters will remove the non-ASCII characters from that file.

### Windows

You can open text-editor.exe directly without attaching it to a file. You can also open an existing
text file by dragging it onto text-editor.exe or via "Open with".

### macOS

The binary is unsigned, so macOS will block it. Clearing the quarantine flag with
`xattr -d com.apple.quarantine text-editor` in Terminal is the workaround.

Warning: The build has not been tested on macOS. It may fail to open.

### Linux

1) Extract file with `tar -xzf text-editor-Linux.tar.gz`
2) Run `./text-editor [FILEPATH]` to open the text editor window.
- `./text-editor` opens an empty editor. `./text-editor FILEPATH` opens an existing file.

## Using the Text Editor

The only way to produce a new file is to create an empty one outside the editor and pass it in.

You can type, delete, and select characters.

Use the arrow keys to move the caret position.
- `shift + arrow key` will select the text your caret moves through.

Double clicking will select the word. Triple clicking will select the line.

You can scroll and zoom in and out.
Zoom with the commands below, or by scrolling the mouse wheel while holding ctrl.

### Commands

Undo is not implemented.

`ctrl + s`: Save the text file. Does nothing if the text editor was not open with an existing text
file.

`ctrl + c`: Copy

`ctrl + v`: Paste

`ctrl + a`: Select all

`ctrl + -`: Zoom out

`ctrl + =`: Zoom in

## License

Code is MIT licensed — see [LICENSE](LICENSE).

The embedded JetBrains Mono font is licensed under the SIL Open Font License;
see [LICENSES/OFL.txt](LICENSES/OFL.txt). SFML is licensed under the zlib
license; see [LICENSES/SFML.txt](LICENSES/SFML.txt).