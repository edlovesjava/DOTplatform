# DOTplatform

An open-source gaming platform built on ATtiny85 + MAX7219 8x8 LED matrix.

## Current Hardware (Breadboard Prototype)

The current working setup is a breadboard prototype:

| Component | Description |
|-----------|-------------|
| ATtiny85 | Microcontroller (DIP-8) |
| MAX7219 module | 8x8 LED matrix with driver (pre-assembled module) |
| 3x Push buttons | Left, right, and fire input |
| 2x 1N4148 diodes | Isolation for fire button (prevents sneak path) |
| 100nF capacitor | Decoupling cap for ATtiny85 |
| 100nF + 10µF capacitors | Decoupling caps for MAX7219 |
| 3xAAA battery holder | 4.5V power (leads soldered to jumper pins) |
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
- 3x tactile push buttons (left, right, fire)
- 2x 1N4148 signal diodes
- 1x 100nF ceramic capacitor (ATtiny85 decoupling)
- 1x 100nF + 1x 10µF ceramic capacitors (MAX7219 decoupling)
- 3xAAA battery holder (4.5V nominal)
- Breadboard + jumper wires
- USBtinyISP or similar AVR programmer

### Wiring

| ATtiny85 | Function | Connect To |
|----------|----------|------------|
| PB0 (pin 5) | DIN | MAX7219 DIN |
| PB1 (pin 6) | CLK | MAX7219 CLK |
| PB2 (pin 7) | CS | MAX7219 CS |
| PB3 (pin 2) | LEFT | Button → GND |
| PB4 (pin 3) | RIGHT | Button → GND |
| VCC (pin 8) | Power | +4.5V from 3xAAA (with 100nF cap to GND) |
| GND (pin 4) | Ground | GND |

**Button wiring with fire button:**

```
PB3 ──┬──[LEFT BTN]───┬─── GND
      │               │
      └────►|─────────┤  (D1: 1N4148, cathode toward GND)
                      │
                 [FIRE BTN]
                      │
      ┌────►|─────────┤  (D2: 1N4148, cathode toward GND)
      │               │
PB4 ──┴──[RIGHT BTN]──┴─── GND
```

The diodes prevent "sneak path" current flow - without them, pressing LEFT would also trigger RIGHT through the FIRE button wiring.

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
  if (Dot.input.pressed(BTN_RIGHT)) {
    // Handle right button
  }
  if (Dot.input.fire()) {
    // Handle fire (dedicated button or left+right chord)
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
