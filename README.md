# Automatic Desk Lighting

A HomeKit-enabled smart desk lighting system built with ESP32, featuring RGB LED strip control and environmental monitoring.

## Overview

This project transforms a standard LED strip into a smart lighting system controllable via Apple HomeKit. It includes temperature and humidity monitoring for complete desk environment awareness. All devices are accessible through the Apple Home app, Siri voice commands, and automation routines.

## Features

- **HomeKit Integration**: Full Apple HomeKit support via HomeSpan library
- **RGB LED Control**: WS2812B addressable LED strip with smooth color transitions
- **Climate Monitoring**: Real-time temperature and humidity tracking with DHT11 sensor
- **Smooth Animations**: Custom interpolation engine for fluid color transitions
- **Voice Control**: Siri integration for hands-free operation
- **Native Apple Home**: No additional apps or hubs required

## Hardware Requirements

- **Microcontroller**: ESP32 development board
- **LED Strip**: WS2812B addressable RGB LEDs (configured for 60 LEDs)
- **Climate Sensor**: DHT11 temperature and humidity sensor
- **Power Supply**: 5V power supply (appropriate wattage for LED count)

### Pin Configuration

| Component | GPIO Pin |
|-----------|----------|
| LED Strip Data | GPIO 5 |
| DHT11 Sensor | GPIO 18 |
| Status LED | GPIO 2 |

## Software Dependencies

Built with PlatformIO and the Arduino framework for ESP32:

- [FastLED](https://github.com/FastLED/FastLED) (v3.10.3) - LED control library
- [HomeSpan](https://github.com/HomeSpan/HomeSpan) (v2.1.7) - Apple HomeKit integration
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library) (v1.4.6) - Temperature/humidity sensor
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor) (v1.1.15) - Sensor abstraction layer

## Project Structure

```
.
├── include/
│   ├── led_config.h              # LED hardware and state configuration
│   ├── led_core.h                # LED transition function declarations
│   ├── dev_ledstrip_smooth.h     # HomeKit LED accessory with smooth transitions
│   ├── dev_temp.h                # HomeKit temperature sensor accessory
│   └── dev_humid.h               # HomeKit humidity sensor accessory
├── src/
│   ├── main.cpp                  # Main program and HomeKit setup
│   └── led_core.cpp              # LED color transition implementation
└── platformio.ini                # PlatformIO configuration
```

## Installation

### Prerequisites

- [PlatformIO](https://platformio.org/) installed (via VS Code extension or CLI)
- USB cable for ESP32 programming

### Building and Flashing

1. Clone this repository:
   ```bash
   git clone <repository-url>
   cd "Automatic Desk Lightning"
   ```

2. Open the project in PlatformIO (VS Code or CLI)

3. Build the project:
   ```bash
   pio run
   ```

4. Upload to ESP32:
   ```bash
   pio run --target upload
   ```

5. Monitor serial output (optional):
   ```bash
   pio device monitor
   ```

## HomeKit Setup

1. After flashing, the ESP32 will create a HomeKit accessory called "Desk Bridge"
2. Open the Apple Home app on your iOS device
3. Tap "+" to add a new accessory
4. Scan the setup code displayed in the serial monitor, or enter it manually
5. The following accessories will be available:
   - **LED Strip**: Full RGB color control with brightness adjustment (5-100%)
   - **Temperature Sensor**: Current temperature reading
   - **Humidity Sensor**: Current humidity reading

## Configuration

### LED Strip Customization

Edit `include/led_config.h` to modify LED configuration:

```cpp
#define LED_PIN 5              // Data pin for LED strip
#define NUM_LEDS 60            // Number of LEDs in your strip
#define LED_UPDATE_INTERVAL 10 // Update interval in milliseconds
#define LED_TRANSITION_SPEED 0.05 // Transition smoothness (0.01-1.0)
```

### Climate Sensor

Edit `src/main.cpp` to change the DHT sensor pin or type:

```cpp
#define DHT_PIN 18
DHT climateSensor(DHT_PIN, DHT11); // Change to DHT22 if needed
```

## Technical Details

### Smooth Color Transitions

The system implements custom HSV interpolation for smooth color changes:
- Circular interpolation for hue values (handles 360° wrap-around)
- Linear interpolation for saturation and brightness
- Frame-rate limited updates (configurable interval)
- Eliminates jarring color jumps when changing via HomeKit

### HomeKit Architecture

Exposed as a HomeKit Bridge with three accessories:
1. **Bridge Accessory**: Main HomeSpan bridge
2. **Climate Accessory**: Combined temperature and humidity sensor
3. **Light Accessory**: RGB LED strip controller

### Performance Optimization

- Volatile state management for thread-safe LED updates
- Early return pattern to minimize unnecessary operations
- FreeRTOS-friendly delays to prevent watchdog timeouts

## Troubleshooting

**LED strip not responding:**
- Verify wiring and power supply
- Check `LED_PIN` matches your hardware
- Ensure adequate power for LED count

**HomeKit pairing fails:**
- Check WiFi credentials in HomeSpan setup
- Verify ESP32 is on same network as iOS device
- Try resetting HomeSpan storage (hold status button)

**Temperature sensor shows inactive:**
- Verify DHT11 wiring and pull-up resistor
- Check serial output for sensor errors
- Try increasing read interval in `dev_temp.h`

## Future Enhancements

Potential additions for this project:
- Motion sensor integration for automatic lighting
- Adaptive brightness based on time of day
- Multiple animation modes (rainbow, fire, etc.)
- MQTT support for integration with other smart home systems
- Web interface for configuration

## License

This is a personal project. Feel free to use and modify for your own purposes.

## Acknowledgments

- [HomeSpan](https://github.com/HomeSpan/HomeSpan) for excellent HomeKit library
- [FastLED](https://github.com/FastLED/FastLED) for powerful LED control
- ESP32 community for extensive documentation and examples
