# DOTplatform PCB Design

## Board Specifications

| Spec | Value |
|------|-------|
| Dimensions | 2.5" x 1.5" (63.5mm x 38.1mm) |
| Layers | 2 (top + bottom) |
| Thickness | 1.6mm |
| Copper | 1oz |
| Component style | Through-hole with DIP sockets |
| Finish | HASL (lead-free) |
| Solder mask | Matte black (or color of choice) |
| Silkscreen | White |

---

## Component Layout

### Front Side (Top)

```
┌─────────────────────────────────────────────────────────────────┐
│                        2.5" (63.5mm)                            │
│  ┌─────────────────────────────────────┐                        │
│  │                                     │                        │
│  │         8x8 LED MATRIX              │   ○ ○ ○ ○ ○   5-pin   │  1.5"
│  │           (1" x 1")                 │   expansion   header  │ (38.1mm)
│  │          25.4mm x 25.4mm            │                        │
│  │                                     │                        │
│  └─────────────────────────────────────┘                        │
│                                                                 │
│  ┌────────────────┐     ┌──────────────────────────┐           │
│  │   ATtiny85     │     │       MAX7219            │           │
│  │   DIP-8        │     │       DIP-24             │           │
│  │   SOCKET       │     │       SOCKET             │           │
│  └────────────────┘     └──────────────────────────┘           │
│                                                                 │
│     [LEFT]        [RIGHT]               [FIRE optional]        │
│                                                                 │
│  [PWR SW]                                      [PROG HEADER]   │
└─────────────────────────────────────────────────────────────────┘
```

### Back Side (Bottom)

```
┌─────────────────────────────────────────────────────────────────┐
│                                                                 │
│     ┌───────────────────────────────────────────────┐          │
│     │                                               │          │
│     │           3xAAA BATTERY HOLDER                │          │
│     │              1" x 2" (25.4mm x 50.8mm)        │          │
│     │                                               │          │
│     │      [AAA]     [AAA]     [AAA]                │          │
│     │                                               │          │
│     └───────────────────────────────────────────────┘          │
│                                                                 │
│     ○ ○ ○   SMD passives (decoupling caps, resistors)          │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## Components List (BOM)

### Through-Hole Components (Front)

| Qty | Component | Package | Value/Part | Notes |
|-----|-----------|---------|------------|-------|
| 1 | ATtiny85 | DIP-8 | ATtiny85-20PU | Microcontroller |
| 1 | DIP-8 Socket | 8-pin | Machined pin | For ATtiny85 |
| 1 | MAX7219 | DIP-24 | MAX7219CNG | LED driver |
| 1 | DIP-24 Socket | 24-pin | Machined pin | For MAX7219 |
| 1 | 8x8 LED Matrix | 20mm | Common cathode | 1088AS or similar |
| 2-3 | Tactile Switch | 6x6mm THT | Momentary | LEFT, RIGHT, FIRE (optional) |
| 1 | Slide Switch | SPDT | - | Power on/off |
| 1 | 5-pin Header | 2.54mm | Male | Expansion port |
| 1 | 6-pin Header | 2.54mm | Male | ISP programming |

### SMD Components (Back) - Optional

| Qty | Component | Package | Value | Notes |
|-----|-----------|---------|-------|-------|
| 3 | Ceramic Cap | 0805 | 100nF | Decoupling |
| 1 | Ceramic Cap | 0805 | 10µF | Bulk cap |
| 1 | Resistor | 0805 | 10kΩ | ISET for MAX7219 |
| 2 | Resistor | 0805 | 10kΩ | Pullups (optional) |

### Battery & Power

| Qty | Component | Size | Notes |
|-----|-----------|------|-------|
| 1 | 3xAAA Battery Holder | 1" x 2" (25.4 x 50.8mm) | Flat, rear-mount |

---

## Power System

### 3xAAA Configuration

```
3xAAA (4.5V nominal) ──[POWER SW]──┬── VCC (ATtiny85)
                                   └── VCC (MAX7219)

Voltage range: 3.6V (depleted) to 4.8V (fresh alkaline)
```

**Advantages of 3xAAA:**
- 4.5V is within operating range of both ATtiny85 (2.7-5.5V) and MAX7219 (4.0-5.5V)
- No boost converter needed (simpler circuit, higher efficiency)
- Fresh alkaline batteries provide ~4.5V, NiMH provide ~3.6V (still works)
- Common battery type, easy to replace

**Power Budget:**

| Component | Current (mA) |
|-----------|--------------|
| ATtiny85 @ 8MHz | ~5 |
| MAX7219 (intensity 8) | ~50 |
| **Total** | ~55mA |

With 3xAAA (~2000mAh): **~35 hours** playtime

---

## Pin Assignments

### ATtiny85 Pinout

```
          ATtiny85 DIP-8
              ┌──────┐
   RESET PB5 ─┤1    8├─ VCC (+4.5V)
    BTN3 PB3 ─┤2    7├─ PB2 (CS)  ──► MAX7219 CS
    BTN4 PB4 ─┤3    6├─ PB1 (CLK) ──► MAX7219 CLK
         GND ─┤4    5├─ PB0 (DIN) ──► MAX7219 DIN
              └──────┘
```

### Button Configuration (2 Pins, 2-3 Buttons)

Two required buttons plus one optional dedicated FIRE button, all using only 2 GPIO pins:

| Pin | Physical Button | Logical Input |
|-----|-----------------|---------------|
| PB3 | LEFT (required) | Move left |
| PB4 | RIGHT (required) | Move right |
| PB3 + PB4 | FIRE (optional) | Fire (pulls both pins low) |

**Chord detection:** Pressing LEFT + RIGHT together triggers FIRE action. The optional third FIRE button provides a dedicated key that electrically does the same thing (shorts both pins to GND).

```
                    (internal pullups enabled)
                              │
        ┌─────────────────────┴─────────────────────┐
        │                                           │
       PB3                                         PB4
        │                                           │
        ├────[BTN LEFT]────┐                       ├────[BTN RIGHT]───┐
        │                  │                       │                  │
        └────[BTN FIRE]────┼───────────────────────┘   (optional)     │
             (optional)    │                                          │
                          GND ────────────────────────────────────────┘
```

**FIRE button wiring (optional):** Two traces from the button - one to PB3, one to PB4. When pressed, both pins go LOW, identical to pressing LEFT + RIGHT simultaneously.

**No external resistors needed** - ATtiny85 internal pullups (~20-50kΩ)

### Input Truth Table

| LEFT | RIGHT | FIRE | PB3 | PB4 | Action |
|------|-------|------|-----|-----|--------|
| - | - | - | HIGH | HIGH | Idle |
| X | - | - | LOW | HIGH | Move left |
| - | X | - | HIGH | LOW | Move right |
| - | - | X | LOW | LOW | Fire |
| X | - | X | LOW | LOW | Fire (+ left) |
| - | X | X | LOW | LOW | Fire (+ right) |

**Additional inputs via software:**
- **Long hold LEFT** → Menu/alternate function
- **Long hold RIGHT** → Menu/alternate function

---

## Schematic

```
                                      VCC (+4.5V from 3xAAA)
                                            │
        ┌───────────────────────────────────┼───────────────────────┐
        │                                   │                       │
        │                                  ┌┴┐                      │
        │                                  │ │ 10kΩ (ISET)          │
        │                                  └┬┘                      │
        │                                   │                       │
        │              ┌────────────────────┼────────────────────┐  │
        │              │                    │                    │  │
        │              │18               19 │                    │  │
        │              │ ISET           VCC │                    │  │
        │              │    ┌──────────┐    │                    │  │
        │              └────┤          ├────┘                    │  │
        │                   │  MAX7219 │                         │  │
        │           DIN ────┤1      24 ├──── DOUT (expansion)    │  │
        │           DIG0────┤2      23 ├──── SEG DP              │  │
        │           DIG4────┤3      22 ├──── SEG A               │  │
┌────────┐          GND ────┤4      21 ├──── SEG B               │  │
│        │          DIG6────┤5      20 ├──── SEG C         ┌────────┐
│ 8x8    │          DIG2────┤6      19 ├──── VCC           │        │
│ LED    │          DIG3────┤7      18 ├──── ISET          │3xAAA   │
│ MATRIX │          DIG7────┤8      17 ├──── SEG G         │BATTERY │
│        │          GND ────┤9      16 ├──── SEG F         │HOLDER  │
│(front) │          DIG5────┤10     15 ├──── SEG E         │        │
│        │          DIG1────┤11     14 ├──── SEG D         │(back)  │
│        │          LOAD────┤12     13 ├──── CLK           │        │
└────────┘                  └──────────┘                   └────────┘
                                │  │
                               CS  CLK
                                │   │
        ┌────────┐              │   │
        │        │1      8      │   │
        │  PB5/RST├──VCC───────┼───┼────VCC
        │        │              │   │
        │    PB3 ├──[BUTTONS]──│───┼───┐  (4-button ladder)
        │        │              │   │   │
        │    PB4 ├──(optional)─│───┼───│─  (2nd button group)
        │        │              │   │   │
        │    GND ├─────GND────┼───┼───┴── GND
        │        │              │   │
        │    PB0 ├──────DIN────┘   │
        │        │                  │
        │    PB1 ├──────CLK────────┘
        │        │
        │    PB2 ├──────CS (directly to MAX7219 pin 12)
        │        │
        └────────┘
         ATtiny85
```

---

## PCB Trace Routing Guidelines

### Critical Traces

| Signal | Width | Notes |
|--------|-------|-------|
| VCC/GND | 0.5mm+ | Power rails, wide traces |
| DIN/CLK/CS | 0.25mm | Signal traces |
| LED matrix | 0.4mm | Higher current paths |

### Routing Priority

1. Power rails (VCC, GND) - use ground plane on bottom layer
2. MAX7219 to LED matrix connections (keep short)
3. ATtiny85 to MAX7219 (DIN, CLK, CS)
4. Button connections

### Placement Rules

- Keep ATtiny85 close to MAX7219 for short signal paths
- Decoupling caps (100nF) close to IC VCC pins
- Ground plane on bottom layer under ICs
- Battery holder centered on back for balance

---

## Expansion Header

5-pin header for future expansion/cartridges:

| Pin | Signal | Description |
|-----|--------|-------------|
| 1 | VCC | +4.5V power |
| 2 | GND | Ground |
| 3 | DOUT | MAX7219 data out (chain displays) |
| 4 | CLK | Clock signal |
| 5 | CS | Directly from ATtiny (directly directly from ATtiny PB2) |

---

## ISP Programming Header

6-pin header for in-circuit programming:

```
┌───────────────┐
│ 1 MISO   VCC 2│
│ 3 SCK  MOSI 4 │
│ 5 RST   GND 6 │
└───────────────┘

Pin mapping:
  MISO = PB1
  MOSI = PB0
  SCK  = PB2
  RST  = PB5
```

---

## Mechanical

### Mounting Holes

4x M2 mounting holes at corners:
- Position: 3mm inset from edges
- Hole diameter: 2.2mm
- Pad diameter: 4.5mm (plated, connected to GND)

### Board Outline

```
       ┌────────────────────────────────────────┐
       │ ○                                    ○ │
       │      63.5mm (2.5")                     │
       │                                        │
       │                                        │ 38.1mm
       │                                        │ (1.5")
       │                                        │
       │ ○                                    ○ │
       └────────────────────────────────────────┘
```

---

## Manufacturing Files Needed

- [ ] Schematic (KiCad .kicad_sch)
- [ ] PCB Layout (KiCad .kicad_pcb)
- [ ] Gerber files (for fabrication)
- [ ] Drill files
- [ ] BOM (CSV/Excel)
- [ ] Pick-and-place file (if using SMD assembly service)
- [ ] 3D model (STEP file)

---

## Design Software

Recommended: **KiCad 7+** (free, open source)

Footprints needed:
- DIP-8 socket
- DIP-24 socket
- 8x8 LED matrix (20mm pitch, 2.54mm pins)
- 6x6mm tactile switch
- Slide switch (SPDT)
- 2.54mm pin headers (5-pin, 6-pin)
- 3xAAA battery holder (check specific holder dimensions)

---

## Next Steps

1. Choose specific battery holder model (get exact footprint)
2. Create schematic in KiCad
3. Create/find footprints for all components
4. Layout PCB
5. Generate Gerbers
6. Order prototype from JLCPCB/PCBWay (~$5 for 5 boards)
