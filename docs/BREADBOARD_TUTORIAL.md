# DOTplatform Breadboard Build Tutorial

A step-by-step guide to building the DOTplatform prototype on a breadboard.

## Bill of Materials (BOM)

### Full 3-Button Setup (Recommended)

| Qty | Component | Example Part | Est. Price | Notes |
|-----|-----------|--------------|------------|-------|
| 1 | ATtiny85 | ATtiny85-20PU (DIP-8) | $1.50 | Microcontroller |
| 1 | MAX7219 LED Module | 8x8 LED matrix module | $2.00 | Pre-assembled module with matrix |
| 3 | Tactile Switch | 6x6mm momentary | $0.15 | Left, Right, Fire buttons |
| 2 | Signal Diode | 1N4148 | $0.04 | Fire button isolation |
| 2 | Ceramic Capacitor | 100nF (0.1µF) | $0.10 | Decoupling caps |
| 1 | Ceramic Capacitor | 10µF | $0.10 | Bulk cap for MAX7219 |
| 1 | Battery Holder | 3xAAA with leads | $0.75 | 4.5V power source |
| 1 | Breadboard | Half-size (400 tie points) | $2.50 | |
| 1 | Jumper Wire Set | Male-to-male | $2.00 | |
| 3 | AAA Batteries | Alkaline or NiMH | $1.50 | |

**Estimated Total: ~$11**

### Alternate: 2-Button Setup (Minimal)

If you only need left/right controls (fire = chord both buttons):

| Qty | Component | Est. Price | Notes |
|-----|-----------|------------|-------|
| 2 | Tactile Switch | $0.10 | Left, Right only |
| 0 | Signal Diode | $0.00 | Not needed without dedicated fire |

**Saves: ~$0.10** (but less convenient gameplay)

### Alternate: Breadboard Power Supply

Instead of battery holder, use a breadboard power module:

| Qty | Component | Est. Price | Notes |
|-----|-----------|------------|-------|
| 1 | Breadboard PSU Module | $1.50 | MB102 or similar, 5V output |
| 1 | USB Cable or 9V Adapter | $0.00 | Use existing |
| 0 | Decoupling Capacitors | $0.00 | PSU module has built-in filtering |

**Trade-off:** Tethered to power, but cleaner power and no capacitors needed.

---

## Component Details

### ATtiny85

The ATtiny85 is a small 8-pin AVR microcontroller:
- 8KB flash, 512B RAM, 512B EEPROM
- Runs at 8MHz (internal oscillator)
- Operating voltage: 2.7V - 5.5V

**Get the DIP-8 package** (ATtiny85-20PU), not the surface-mount version.

### MAX7219 LED Module

Buy a **pre-assembled module** that includes:
- MAX7219 driver chip
- 8x8 LED matrix (common cathode)
- Current-limiting resistor
- Header pins

These cost $1.50-3.00 on AliExpress/eBay. Much easier than wiring a bare MAX7219.

### 1N4148 Diodes

Small signal diodes for isolating the fire button. The stripe on the diode body marks the cathode (the side that points toward GND in our circuit).

---

## Step-by-Step Assembly

### Step 1: Place Components on Breadboard

```
Breadboard Layout (top view):

     1  5  10  15  20  25  30
   ┌──────────────────────────────┐
 + │ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○  │ ← Power rail (+4.5V)
 - │ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○  │ ← Ground rail
   │                              │
 a │     █ █ █ █                  │ ← ATtiny85 pins 1-4
 b │     █ █ █ █                  │
 c │     █ █ █ █                  │
 d │     █ █ █ █                  │
 e │     █ █ █ █                  │ ← ATtiny85 pins 5-8
   │ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─  │ ← Center gap
 f │                 ○ ○ ○ ○ ○    │ ← MAX7219 module area
 g │                 ○ ○ ○ ○ ○    │
 ...
```

1. Insert the **ATtiny85** straddling the center gap
   - Pin 1 (with dot/notch) toward the left
   - Pins 1-4 on one side, pins 5-8 on the other

2. Connect the **MAX7219 module** to the right side
   - VCC, GND, DIN, CS, CLK pins

### Step 2: Wire Power

Connect power rails to the ATtiny85 and MAX7219 module:

```
Battery + (red)  → Breadboard + rail
Battery - (black) → Breadboard - rail

ATtiny85 pin 8 (VCC) → + rail
ATtiny85 pin 4 (GND) → - rail

MAX7219 module VCC → + rail
MAX7219 module GND → - rail
```

### Step 3: Add Decoupling Capacitors

Place capacitors close to the ICs:

```
100nF cap: ATtiny85 pin 8 (+) to pin 4 (GND)
100nF cap: MAX7219 VCC to GND (on module)
10µF cap:  MAX7219 VCC to GND (on module)
```

**Skip this step if using a breadboard power supply module.**

### Step 4: Wire ATtiny85 to MAX7219

Connect the SPI signals:

| ATtiny85 | Pin | Wire to | MAX7219 Module |
|----------|-----|---------|----------------|
| PB0 | 5 | → | DIN |
| PB1 | 6 | → | CLK |
| PB2 | 7 | → | CS |

### Step 5: Wire Buttons (3-Button with Diodes)

**Left Button:**
```
PB3 (pin 2) ──── Button ──── GND rail
```

**Right Button:**
```
PB4 (pin 3) ──── Button ──── GND rail
```

**Fire Button (with isolation diodes):**
```
PB3 (pin 2) ────►|──┐
                    ├── Button ── GND rail
PB4 (pin 3) ────►|──┘

►| = 1N4148 diode (stripe toward the button/GND)
```

Wire the diode anodes to PB3 and PB4, cathodes (stripe) connect together at one leg of the fire button, other leg to GND.

### Step 5 (Alternate): Wire Buttons (2-Button, No Diodes)

If skipping the dedicated fire button:

```
PB3 (pin 2) ──── Left Button ──── GND rail
PB4 (pin 3) ──── Right Button ──── GND rail
```

Fire is triggered by pressing both buttons simultaneously (chord).

### Step 6: Verify Connections

Before powering on, double-check:

- [ ] ATtiny85 pin 8 → +4.5V
- [ ] ATtiny85 pin 4 → GND
- [ ] MAX7219 VCC → +4.5V
- [ ] MAX7219 GND → GND
- [ ] PB0 (pin 5) → MAX7219 DIN
- [ ] PB1 (pin 6) → MAX7219 CLK
- [ ] PB2 (pin 7) → MAX7219 CS
- [ ] Buttons wired to correct pins and GND
- [ ] Diode polarity correct (stripe toward GND)
- [ ] Capacitors installed (if using battery power)

---

## Programming the ATtiny85

You'll need a programmer to flash code to the ATtiny85. See:

**[Programmer Setup Guide](PROGRAMMER_SETUP.md)** - How to set up:
- Arduino Nano as ISP programmer (cheapest option)
- USB-to-TTL adapter with Arduino ISP
- Dedicated USBtinyISP programmer

---

## Testing

### First Power-On

1. Insert batteries (or connect power supply)
2. The LED matrix may show random pixels or nothing - this is normal before programming

### Upload Test Sketch

After setting up your programmer, upload a test sketch:

```cpp
#include <DotPlatform.h>

void setup() {
  Dot.begin();
}

void loop() {
  // Show a pixel that moves with button presses
  static int x = 3;

  if (Dot.input.pressed(BTN_LEFT) && x > 0) x--;
  if (Dot.input.pressed(BTN_RIGHT) && x < 7) x++;

  Dot.display.clear();
  Dot.display.setPixel(x, 3, ON);
  Dot.display.show();

  delay(100);
}
```

If the pixel moves when you press buttons, your build is working.

---

## Troubleshooting

### LED matrix doesn't light up
- Check VCC/GND connections to MAX7219 module
- Verify DIN, CLK, CS wiring to correct ATtiny85 pins
- Try adjusting display brightness in code

### Buttons don't respond
- Verify buttons connect the pin to GND when pressed (not VCC)
- Check internal pullups are enabled (DotPlatform library does this automatically)
- For fire button: check diode polarity (stripe toward GND)

### Fire button triggers left or right
- Diode isolation issue - verify both diodes are installed
- Check diode direction (anode to pin, cathode to button junction)

### Erratic behavior
- Add/check decoupling capacitors
- Try fresh batteries
- Keep wires short to reduce noise

---

## Next Steps

Once your breadboard prototype is working:

1. **Play some games** - Flash DOTinvaders or DOTris
2. **Build the PCB version** - See [hardware/PCB_DESIGN.md](../hardware/PCB_DESIGN.md)
3. **Make your own game** - See the firmware examples

## Where to Buy Components

| Source | Shipping | Notes |
|--------|----------|-------|
| AliExpress | 2-4 weeks | Cheapest, buy in bulk |
| eBay | 1-3 weeks | Good for modules |
| Amazon | 1-2 days | Faster, higher price |
| DigiKey/Mouser | 1-3 days | Quality parts, exact specs |
| Adafruit/SparkFun | 1-5 days | Beginner-friendly, tutorials |
