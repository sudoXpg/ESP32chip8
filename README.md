# CHIP-8 Emulator on ESP-32 using Arduino IDE

## Overview

This project is an implementation of the CHIP-8 emulator written in C, using the Arduino IDE. CHIP-8 is a simple 8-bit virtual machine that was originally designed for early home computers and video game consoles. This emulator allows you to run CHIP-8 programs (ROM files) and simulate the original CHIP-8 behavior on modern hardware, such as Arduino-based platforms.

The project emulates the CHIP-8 architecture and provides basic graphical output to a screen, along with keyboard or button input handling for user interaction.

## Features

- Emulates the CHIP-8 architecture using C in the Arduino IDE
- Supports executing CHIP-8 programs (ROM files)
- Provides pixel rendering on a simple display (e.g., LED matrix, TFT screen)
- Input handling using hardware buttons or an Arduino-compatible keyboard
- Customizable key mappings and screen size

## Getting Started

### Prerequisites

- **Arduino IDE**: You can download it from [here](https://www.arduino.cc/en/software).
- **ESP32 board**
- **Display module**: Such as an LED matrix or TFT display (e.g., 128x64 OLED)
- **Input device**: Buttons or a compatible keyboard (e.g., keypad matrix)

### Installing

1. Clone this repository:
   ```bash
   git clone https://github.com/sudoXpg/chip-8-emulator.git
   cd chip-8-emulator

2. Open the project in Arduino IDE:

### Controls
4x4 Keyboard matrix used as a Controller.

Display

The emulator currently supports a basic graphical output to a display module (e.g., an LED matrix or TFT display).

You can modify the code to use different screen types if needed.



# Future Enhancements and Roadmap

Below is a list of planned enhancements and features for the CHIP-8 Emulator project:

- [ ] **Support for Multiple Display Types**  
  Extend the display support to various modules (e.g., 128x64 OLED, TFT, etc.), providing more options for visual output.

- [ ] **Input Handling**  
  Improve input handling with support for more keypads, joystick controllers, and other input devices for a wider range of user interactions.


- [ ] **Custom ROM Support**  
  Implement a system for loading and running custom CHIP-8 ROMs. This will include features for loading files from SD cards or external memory, making the emulator more versatile.

- [ ] **Better Key Mapping**  
  Allow users to customize key mappings for different controllers, offering flexibility for different hardware setups.

- [ ] **Memory Viewer**  
  Add a simple memory viewer or debugger to track the state of the emulated system, providing useful insights for debugging CHIP-8 programs.

- [ ] **Optimizations**  
  Improve the emulator's performance, especially on higher-level Arduino boards, to enhance speed and responsiveness.

These features will be prioritized and integrated over time, enhancing the emulator’s functionality and usability.


### License

This project is licensed under the [MIT License](./LICENSE.md) - see the LICENSE.md file for details.
