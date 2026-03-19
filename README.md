# Shortcut Buttons

This project sends predefined keyboard actions to a PC when buttons are pressed.

Download or clone this repository and open it in PlatformIO.



## Notes

- This project is designed for Windows PCs using a Japanese keyboard layout and was tested on Windows 11.
- Please check and modify the key mappings as needed to match your environment before use.



## Components

- ESP32-S3 Mini
- 3 push buttons



## Wiring

- D0 -> Push button -> GND
- D1 -> Push button -> GND
- D2 -> Push button -> GND



## Button Functions

- D0: Create a text file
- D1: Open Command Prompt
- D2: Lock the PC



## Project Structure

```text
src/
└── main.cpp
```

The `platformio.ini` file is also included.