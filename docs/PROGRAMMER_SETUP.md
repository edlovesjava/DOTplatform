# ATtiny85 Programmer Setup Guide

The ATtiny85 doesn't have USB, so you need a programmer to flash code. This guide covers three options, from cheapest to most convenient.

---

## Option 1: Arduino Nano as ISP (Recommended for Beginners)

Use a $3 Arduino Nano clone as a programmer. Cheapest and most flexible option.

### Parts Needed

| Component | Est. Price |
|-----------|------------|
| Arduino Nano (or clone) | $3.00 |
| 10µF capacitor | $0.05 |
| Jumper wires | (included with breadboard kit) |

### Step 1: Upload ArduinoISP to the Nano

1. Connect the Arduino Nano to your computer via USB
2. Open Arduino IDE
3. Select **Tools → Board → Arduino Nano**
4. Select **Tools → Processor → ATmega328P** (or "Old Bootloader" for clones)
5. Select your COM port under **Tools → Port**
6. Open **File → Examples → 11.ArduinoISP → ArduinoISP**
7. Click **Upload**

The Nano is now an ISP programmer.

### Step 2: Wire Nano to ATtiny85

```
Arduino Nano          ATtiny85
───────────────────────────────
D10 (SS)      →      Pin 1 (RESET)
D11 (MOSI)    →      Pin 5 (PB0/MOSI)
D12 (MISO)    →      Pin 6 (PB1/MISO)
D13 (SCK)     →      Pin 7 (PB2/SCK)
5V            →      Pin 8 (VCC)
GND           →      Pin 4 (GND)

IMPORTANT: 10µF capacitor between Nano RESET and GND
(prevents Nano from resetting during programming)
```

### Wiring Diagram

```
                    ATtiny85
                    ┌──────┐
  Nano D10 ────────┤1 ○  8├──────── Nano 5V
                   │      │
          (PB3) ───┤2    7├──────── Nano D13 (SCK)
                   │      │
          (PB4) ───┤3    6├──────── Nano D12 (MISO)
                   │      │
  Nano GND ────────┤4    5├──────── Nano D11 (MOSI)
                   └──────┘

  Nano RESET ──┤├── Nano GND
              10µF
         (+ toward RESET)
```

### Step 3: Configure Arduino IDE for ATtiny85

1. **Add ATTinyCore board support:**
   - Go to **File → Preferences**
   - In "Additional Board Manager URLs", add:
     ```
     http://drazzy.com/package_drazzy.com_index.json
     ```
   - Click OK

2. **Install ATTinyCore:**
   - Go to **Tools → Board → Boards Manager**
   - Search for "ATTinyCore"
   - Install **ATTinyCore by Spence Konde**

3. **Select board settings:**
   - **Board:** ATtiny25/45/85 (No bootloader)
   - **Chip:** ATtiny85
   - **Clock:** 8 MHz (internal)
   - **Programmer:** Arduino as ISP

### Step 4: Burn Bootloader (First Time Only)

This sets the fuses for 8MHz internal clock:

1. **Tools → Burn Bootloader**
2. Wait for "Done burning bootloader" message

You only need to do this once per new ATtiny85.

### Step 5: Upload Your Sketch

1. Open your sketch (e.g., DOTinvaders)
2. Instead of clicking Upload, use:
   - **Sketch → Upload Using Programmer**
   - Or press **Ctrl+Shift+U**

---

## Option 2: USB-to-TTL Adapter with Arduino Bootloader

If you have a USB-to-TTL (FTDI) adapter, you can use it similarly.

### Parts Needed

| Component | Est. Price |
|-----------|------------|
| USB-to-TTL adapter (CP2102, CH340, FTDI) | $1.50 |
| Arduino Uno/Nano (for initial bootloader) | $3.00 |

### Wiring

```
USB-TTL Adapter       ATtiny85
───────────────────────────────
DTR (via 100nF cap) → Pin 1 (RESET)
TX            →      Pin 5 (PB0)
RX            →      Pin 6 (PB1)
5V            →      Pin 8 (VCC)
GND           →      Pin 4 (GND)
```

This method requires burning a bootloader first using Arduino as ISP (Option 1), then you can use the USB-TTL for subsequent uploads. More complex, not recommended unless you already have the adapter.

---

## Option 3: Dedicated USBtinyISP Programmer

A purpose-built programmer. More convenient but costs more.

### Parts Needed

| Component | Est. Price |
|-----------|------------|
| USBtinyISP programmer | $8-15 |
| 6-pin ISP cable | (usually included) |

### Wiring

USBtinyISP uses a standard 6-pin ISP header:

```
ISP Header (looking at cable end)
┌───────────┐
│ 1 MISO  2 VCC │
│ 3 SCK   4 MOSI│
│ 5 RST   6 GND │
└───────────┘

Connect to ATtiny85:
  MISO → Pin 6 (PB1)
  SCK  → Pin 7 (PB2)
  MOSI → Pin 5 (PB0)
  RST  → Pin 1 (RESET)
  VCC  → Pin 8
  GND  → Pin 4
```

### Arduino IDE Settings

1. **Tools → Programmer → USBtinyISP**
2. **Sketch → Upload Using Programmer** (Ctrl+Shift+U)

### Driver Installation (Windows)

USBtinyISP may need drivers on Windows:
1. Download Zadig: https://zadig.akeo.ie/
2. Connect USBtinyISP
3. In Zadig, select USBtinyISP and install libusb-win32 driver

---

## Building a Dedicated Programming Jig

For repeated programming, build a permanent setup:

### Simple Jig with ZIF Socket

```
┌─────────────────────────────────────┐
│                                     │
│   ┌─────────┐      ┌───────────┐    │
│   │ Arduino │      │ 8-pin ZIF │    │
│   │  Nano   │──────│  Socket   │    │
│   │         │      │           │    │
│   └─────────┘      └───────────┘    │
│                                     │
│   [10µF cap]       Lift lever,      │
│                    insert ATtiny,   │
│                    close lever,     │
│                    program!         │
└─────────────────────────────────────┘
```

### Parts for Jig

| Component | Est. Price |
|-----------|------------|
| Arduino Nano | $3.00 |
| 8-pin ZIF socket | $2.00 |
| Perfboard or PCB | $1.00 |
| 10µF capacitor | $0.05 |
| Pin headers | $0.50 |

**Total: ~$7**

A ZIF (Zero Insertion Force) socket lets you quickly insert and remove ATtiny85 chips without wear on the pins.

---

## Troubleshooting

### "avrdude: initialization failed, rc=-1"

- Check all wiring connections
- Verify 10µF capacitor is on Nano RESET (if using Arduino as ISP)
- Try different USB port
- Check ATtiny85 orientation (pin 1 dot in correct position)

### "avrdude: Device signature = 0x000000"

- ATtiny85 not powered or not connected
- Wrong pin connections
- Damaged ATtiny85

### "avrdude: stk500_recv(): programmer is not responding"

- Arduino Nano issue - re-upload ArduinoISP sketch
- Try selecting "Old Bootloader" for Nano clones
- Check USB cable (some are charge-only)

### Upload works but ATtiny85 doesn't run

- Burn bootloader first to set fuses
- Check clock setting matches (8 MHz internal)
- Verify code compiles for ATtiny85, not Arduino

---

## Quick Reference

### Arduino IDE Settings for ATtiny85

| Setting | Value |
|---------|-------|
| Board | ATtiny25/45/85 (No bootloader) |
| Chip | ATtiny85 |
| Clock | 8 MHz (internal) |
| Programmer | Arduino as ISP |

### Upload Commands

| Action | Method |
|--------|--------|
| Burn fuses (once) | Tools → Burn Bootloader |
| Upload sketch | Sketch → Upload Using Programmer (Ctrl+Shift+U) |

### Nano → ATtiny85 Wiring

| Nano Pin | ATtiny85 Pin | Function |
|----------|--------------|----------|
| D10 | 1 | RESET |
| D11 | 5 | MOSI |
| D12 | 6 | MISO |
| D13 | 7 | SCK |
| 5V | 8 | VCC |
| GND | 4 | GND |

Don't forget: **10µF capacitor between Nano RESET and GND**
