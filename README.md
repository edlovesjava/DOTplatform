# DOTplatform

An open-source gaming platform built on ATtiny85 + MAX7219 8x8 LED matrix.

## Current Hardware (Breadboard Prototype)

The current working setup is a simple breadboard prototype:

| Component | Description |
|-----------|-------------|
| ATtiny85 | Microcontroller (DIP-8) |
| MAX7219 module | 8x8 LED matrix with driver (pre-assembled module) |
| 2x Push buttons | Left and right input (chord = fire) |
| Breadboard power supply | 5V regulated power |
| Breadboard + wires | For prototyping |

**This is what DOTinvaders and DOTris currently run on.**

## Future Plans (Handheld Device)

The full DOTplatform handheld is planned to include:
- USB in-place programming (Micronucleus bootloader)
- Battery power with boost converter
- Expansion cartridge port
- 3D printed enclosure
- ~$5 BOM target

See [hardware/SPEC.md](hardware/SPEC.md) for the full future specification.

## Repository Structure

```
DOTplatform/
├── hardware/           # Hardware specifications and designs
│   ├── SPEC.md         # Full hardware specification
│   ├── schematic/      # KiCad schematic files
│   ├── pcb/            # PCB layout files
│   └── enclosure/      # 3D printable STL files
├── firmware/
│   └── DotPlatform/    # Arduino library
│       ├── src/        # Library source code
│       └── examples/   # Example games and templates
├── flasher/            # Game flasher desktop app
└── docs/               # Documentation
```

## Games

Official games built on DOTplatform:
- [DOTinvaders](https://github.com/edlovesjava/dotinvaders) - Space Invaders clone
- [DOTris](https://github.com/edlovesjava/DOTris) - Tetris clone

## Quick Start (Current Breadboard Setup)

### Hardware Required

- ATtiny85 (DIP-8)
- MAX7219 8x8 LED matrix module
- 2x tactile push buttons
- Breadboard + jumper wires
- 5V power supply (breadboard power module or USB)
- USBtinyISP or similar AVR programmer

### Wiring

| ATtiny85 | Function | Connect To |
|----------|----------|------------|
| PB0 (pin 5) | DIN | MAX7219 DIN |
| PB1 (pin 6) | CLK | MAX7219 CLK |
| PB2 (pin 7) | CS | MAX7219 CS |
| PB3 (pin 2) | RIGHT | Button → GND |
| PB4 (pin 3) | LEFT | Button → GND |
| VCC (pin 8) | Power | +5V |
| GND (pin 4) | Ground | GND |

### Programming

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Install ATTinyCore: Add `https://drazzy.com/package_drazzy.com_index.json` to Board URLs
3. Select: Board "ATtiny85", Clock "8 MHz (internal)"
4. Connect programmer and burn bootloader (once)
5. Upload game sketch

```cpp
#include <DotPlatform.h>

void setup() {
  Dot.begin();
}

void loop() {
  if (Dot.input.pressed(BTN_LEFT)) {
    // Handle left button
  }
  if (Dot.input.chord()) {
    // Handle both buttons (fire!)
  }

  Dot.display.clear();
  Dot.display.setPixel(x, y, ON);
  Dot.display.show();
}
```

## Hardware

See [hardware/SPEC.md](hardware/SPEC.md) for the full hardware specification including:
- Component list and BOM
- Schematic and PCB design
- Power system (boost converter)
- Expansion port for cartridges
- 3D printable enclosure

## Contributing

Contributions welcome! See [CONTRIBUTING.md](docs/CONTRIBUTING.md) for guidelines.

## License

- Hardware: [CERN-OHL-P v2](https://ohwr.org/cern_ohl_p_v2.txt) (Permissive)
- Firmware/Software: [MIT License](LICENSE)

## Links

- [Hardware Specification](hardware/SPEC.md)
- [Game Development Guide](docs/GAME_DEV.md)
- [Community Games](https://github.com/edlovesjava/DOTplatform/wiki/Community-Games)
