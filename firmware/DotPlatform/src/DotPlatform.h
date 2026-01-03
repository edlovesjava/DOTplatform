/*
 * DotPlatform.h - Game development framework for DOTplatform handheld
 *
 * Hardware: ATtiny85 + MAX7219 + 8x8 LED Matrix
 * Input: 2 buttons (PB3, PB4) with chord detection for 3rd input
 *
 * https://github.com/edlovesjava/DOTplatform
 */

#ifndef DOT_PLATFORM_H
#define DOT_PLATFORM_H

#include <Arduino.h>

// Pin definitions
#define DOT_PIN_DIN   0   // MAX7219 Data In
#define DOT_PIN_CLK   1   // MAX7219 Clock
#define DOT_PIN_CS    2   // MAX7219 Chip Select
#define DOT_PIN_RIGHT 3   // Right button (active low)
#define DOT_PIN_LEFT  4   // Left button (active low)

// Button constants
#define BTN_LEFT   0
#define BTN_RIGHT  1
#define BTN_CHORD  2  // Both buttons pressed

// Pixel states
#define OFF 0
#define ON  1

// MAX7219 Registers
#define MAX7219_REG_NOOP        0x00
#define MAX7219_REG_DIGIT0      0x01
#define MAX7219_REG_DECODE      0x09
#define MAX7219_REG_INTENSITY   0x0A
#define MAX7219_REG_SCANLIMIT   0x0B
#define MAX7219_REG_SHUTDOWN    0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F

// Forward declarations
class DotDisplay;
class DotInput;

/**
 * DotDisplay - 8x8 LED matrix display control
 */
class DotDisplay {
public:
  DotDisplay();

  void begin();
  void clear();
  void show();

  void setPixel(uint8_t x, uint8_t y, uint8_t state);
  uint8_t getPixel(uint8_t x, uint8_t y);

  void drawSprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t width, uint8_t height);
  void scrollText(const char* text, uint16_t delayMs = 100);

  void setBrightness(uint8_t level);  // 0-15
  void fill(uint8_t state);

private:
  uint8_t buffer[8];
  void sendByte(uint8_t b);
  void sendCmd(uint8_t addr, uint8_t data);
};

/**
 * DotInput - Button input handling with chord detection
 *
 * Uses release detection for responsive single-button input
 * while supporting chord (both buttons) for third action.
 */
class DotInput {
public:
  DotInput();

  void begin();
  void update();  // Call once per frame

  // Edge detection (true only on the frame it happens)
  bool pressed(uint8_t button);   // Just pressed this frame
  bool released(uint8_t button);  // Just released this frame

  // State detection
  bool held(uint8_t button);      // Currently held down
  bool chord();                   // Both buttons pressed

  // Timing
  uint16_t holdTime(uint8_t button);  // How long held (ms)

private:
  bool lastLeft, lastRight;
  bool currLeft, currRight;
  bool chordedThisPress;
  uint16_t leftHoldTime, rightHoldTime;
  uint32_t lastUpdateTime;
};

/**
 * DotPlatformClass - Main platform singleton
 */
class DotPlatformClass {
public:
  DotDisplay display;
  DotInput input;

  void begin();
  void update();  // Call once per frame (updates input)

  // Frame timing
  void waitFrame(uint8_t fps = 60);  // Wait to maintain target FPS
  uint32_t frameCount();

private:
  uint32_t lastFrameTime;
  uint32_t frames;
};

// Global instance
extern DotPlatformClass Dot;

#endif // DOT_PLATFORM_H
