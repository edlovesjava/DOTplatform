# DOTplatform

An open-source handheld gaming platform built on ATtiny85 + MAX7219 8x8 LED matrix.

## Overview

DOTplatform is a hackable, ultra-low-cost gaming handheld designed for:
- **Players**: Swap games via USB with the flasher app
- **Hackers**: Build custom games with the Arduino framework
- **Makers**: Build your own hardware from open designs

## Features

- **Hardware**: ATtiny85 MCU, MAX7219 LED driver, 8x8 LED matrix
- **Input**: 2 buttons providing 3 inputs via chord detection
- **Power**: USB or battery (2xAAA / LiPo)
- **Programming**: USB in-place via Micronucleus bootloader
- **Expansion**: Cartridge port for display chaining and smart modules
- **Cost**: ~$5 BOM target

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

## Quick Start

### For Players

1. Download the [DOTplatform Flasher](flasher/) app
2. Connect your DOT device via USB
3. Select a game and click Install

### For Developers

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Add board URL: `https://raw.githubusercontent.com/edlovesjava/DOTplatform/main/package_dotplatform_index.json`
3. Install "DOTplatform" from Boards Manager
4. Install "DotPlatform" library from Library Manager
5. Open an example and upload!

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
