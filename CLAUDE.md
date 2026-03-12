# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ESP-IDF embedded project for automatic desk lighting control using WS2812 LED strips on an ESP32 microcontroller. The project controls 60 LEDs connected to GPIO 5 using the RMT peripheral.

## Build Commands

This project requires ESP-IDF to be sourced before running commands. The IDF is installed at `/Users/ciprianstefan/.espressif/v5.5.3/esp-idf`.

**Setup environment:**
```bash
source /Users/ciprianstefan/.espressif/v5.5.3/esp-idf/export.sh
```

**Build the project:**
```bash
idf.py build
```

**Flash to device:**
```bash
idf.py -p /dev/tty.usbserial-0001 flash
```

**Monitor serial output:**
```bash
idf.py -p /dev/tty.usbserial-0001 monitor
```

**Flash and monitor in one command:**
```bash
idf.py -p /dev/tty.usbserial-0001 flash monitor
```

**Configure project (menuconfig):**
```bash
idf.py menuconfig
```

**Clean build:**
```bash
idf.py fullclean
```

## Architecture

### Main Application (`main/`)
- `main.c` - Entry point that initializes LEDs and creates the LED task
- `main/leds/leds.c` - LED control implementation using RMT driver
- `main/leds/leds.h` - LED control interface
- `main/CMakeLists.txt` - Component build configuration registering all source files

### Custom Components (`components/`)
- `components/led_animation/` - In-development custom component for LED animations (currently minimal implementation)

### Dependencies
- `espressif__led_strip` - Managed component from ESP Component Registry providing WS2812 LED strip control via RMT peripheral
- Declared in `main/idf_component.yml`

### Hardware Configuration
- **Target chip:** ESP32 (Xtensa architecture)
- **LED strip:** 60x WS2812 LEDs (GRB color format)
- **GPIO pin:** GPIO 5
- **Communication:** RMT peripheral at 10 MHz resolution
- **Serial port:** /dev/tty.usbserial-0001

### Key Files
- `sdkconfig` - ESP-IDF configuration (gitignored but present on disk)
- `CMakeLists.txt` - Root build configuration
- `dependencies.lock` - Locked versions of managed components

## Development Setup

The project includes a devcontainer configuration (`.devcontainer/`) with ESP-IDF Docker image. VSCode settings are configured for:
- ESP-IDF extension
- clangd language server (using ESP clang from `/Users/ciprianstefan/.espressif/tools/esp-clang/`)
- Build path: `build/`
- Flash type: UART

## Code Patterns

### Adding Source Files
When adding new .c files to the main component, update `main/CMakeLists.txt`:
```cmake
idf_component_register(SRCS "main.c" "leds/leds.c" "newfile.c"
                    INCLUDE_DIRS "."
                    REQUIRES led_strip
                    )
```

### LED Control
The LED strip is initialized once with `leds_init()` and controlled in a FreeRTOS task (`leds_task`). The handle `strip` is a module-level variable. Always check for NULL before using the strip handle.

### FreeRTOS Tasks
Tasks are created with sufficient stack size (4096 bytes for LED task). Use `vTaskDelay(pdMS_TO_TICKS(ms))` for delays instead of busy-waiting.

### User preferences
The user prefers to use you as a guide, rather than the one making the code. Often, you will be asked opinions about different ideas or approaches. Respond accordingly.