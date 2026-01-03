/*
 * DotPlatform.cpp - Game development framework for DOTplatform handheld
 * https://github.com/edlovesjava/DOTplatform
 */

#include "DotPlatform.h"

// Global instance
DotPlatformClass Dot;

// ============================================================================
// DotDisplay Implementation
// ============================================================================

DotDisplay::DotDisplay() {
  clear();
}

void DotDisplay::begin() {
  pinMode(DOT_PIN_DIN, OUTPUT);
  pinMode(DOT_PIN_CLK, OUTPUT);
  pinMode(DOT_PIN_CS, OUTPUT);

  // Initialize MAX7219
  sendCmd(MAX7219_REG_DISPLAYTEST, 0x00);
  sendCmd(MAX7219_REG_DECODE, 0x00);
  sendCmd(MAX7219_REG_SCANLIMIT, 0x07);
  sendCmd(MAX7219_REG_INTENSITY, 0x08);
  sendCmd(MAX7219_REG_SHUTDOWN, 0x01);

  clear();
  show();
}

void DotDisplay::clear() {
  for (uint8_t i = 0; i < 8; i++) {
    buffer[i] = 0;
  }
}

void DotDisplay::fill(uint8_t state) {
  uint8_t val = state ? 0xFF : 0x00;
  for (uint8_t i = 0; i < 8; i++) {
    buffer[i] = val;
  }
}

void DotDisplay::show() {
  for (uint8_t row = 0; row < 8; row++) {
    sendCmd(row + 1, buffer[row]);
  }
}

void DotDisplay::setPixel(uint8_t x, uint8_t y, uint8_t state) {
  if (x >= 8 || y >= 8) return;
  if (state) {
    buffer[y] |= (1 << x);
  } else {
    buffer[y] &= ~(1 << x);
  }
}

uint8_t DotDisplay::getPixel(uint8_t x, uint8_t y) {
  if (x >= 8 || y >= 8) return 0;
  return (buffer[y] >> x) & 1;
}

void DotDisplay::drawSprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t width, uint8_t height) {
  for (uint8_t sy = 0; sy < height; sy++) {
    for (uint8_t sx = 0; sx < width; sx++) {
      uint8_t byte_idx = sy;
      uint8_t bit_idx = sx;
      if (sprite[byte_idx] & (1 << bit_idx)) {
        setPixel(x + sx, y + sy, ON);
      }
    }
  }
}

void DotDisplay::setBrightness(uint8_t level) {
  if (level > 15) level = 15;
  sendCmd(MAX7219_REG_INTENSITY, level);
}

void DotDisplay::sendByte(uint8_t b) {
  for (int i = 7; i >= 0; i--) {
    digitalWrite(DOT_PIN_CLK, LOW);
    digitalWrite(DOT_PIN_DIN, (b >> i) & 1);
    digitalWrite(DOT_PIN_CLK, HIGH);
  }
}

void DotDisplay::sendCmd(uint8_t addr, uint8_t data) {
  digitalWrite(DOT_PIN_CS, LOW);
  sendByte(addr);
  sendByte(data);
  digitalWrite(DOT_PIN_CS, HIGH);
}

// Placeholder for scrollText - can be implemented later
void DotDisplay::scrollText(const char* text, uint16_t delayMs) {
  // TODO: Implement text scrolling with font
  (void)text;
  (void)delayMs;
}

// ============================================================================
// DotInput Implementation
// ============================================================================

DotInput::DotInput() {
  lastLeft = lastRight = false;
  currLeft = currRight = false;
  chordedThisPress = false;
  leftHoldTime = rightHoldTime = 0;
  lastUpdateTime = 0;
}

void DotInput::begin() {
  pinMode(DOT_PIN_LEFT, INPUT_PULLUP);
  pinMode(DOT_PIN_RIGHT, INPUT_PULLUP);
}

void DotInput::update() {
  // Save previous state
  lastLeft = currLeft;
  lastRight = currRight;

  // Read current state (active low)
  currLeft = (digitalRead(DOT_PIN_LEFT) == LOW);
  currRight = (digitalRead(DOT_PIN_RIGHT) == LOW);

  // Calculate delta time
  uint32_t now = millis();
  uint16_t dt = now - lastUpdateTime;
  lastUpdateTime = now;

  // Update hold times
  if (currLeft) {
    leftHoldTime += dt;
  } else {
    leftHoldTime = 0;
  }

  if (currRight) {
    rightHoldTime += dt;
  } else {
    rightHoldTime = 0;
  }

  // Track chord state
  if (currLeft && currRight) {
    chordedThisPress = true;
  }
  if (!currLeft && !currRight) {
    chordedThisPress = false;
  }
}

bool DotInput::pressed(uint8_t button) {
  switch (button) {
    case BTN_LEFT:
      return currLeft && !lastLeft;
    case BTN_RIGHT:
      return currRight && !lastRight;
    case BTN_CHORD:
      return (currLeft && currRight) && !(lastLeft && lastRight);
    default:
      return false;
  }
}

bool DotInput::released(uint8_t button) {
  switch (button) {
    case BTN_LEFT:
      return !currLeft && lastLeft && !chordedThisPress;
    case BTN_RIGHT:
      return !currRight && lastRight && !chordedThisPress;
    case BTN_CHORD:
      return !(currLeft && currRight) && (lastLeft && lastRight);
    default:
      return false;
  }
}

bool DotInput::held(uint8_t button) {
  switch (button) {
    case BTN_LEFT:
      return currLeft && !currRight;
    case BTN_RIGHT:
      return currRight && !currLeft;
    case BTN_CHORD:
      return currLeft && currRight;
    default:
      return false;
  }
}

bool DotInput::chord() {
  return currLeft && currRight;
}

uint16_t DotInput::holdTime(uint8_t button) {
  switch (button) {
    case BTN_LEFT:
      return leftHoldTime;
    case BTN_RIGHT:
      return rightHoldTime;
    default:
      return 0;
  }
}

// ============================================================================
// DotPlatformClass Implementation
// ============================================================================

void DotPlatformClass::begin() {
  display.begin();
  input.begin();
  lastFrameTime = millis();
  frames = 0;
}

void DotPlatformClass::update() {
  input.update();
  frames++;
}

void DotPlatformClass::waitFrame(uint8_t fps) {
  uint16_t frameTime = 1000 / fps;
  uint32_t elapsed = millis() - lastFrameTime;
  if (elapsed < frameTime) {
    delay(frameTime - elapsed);
  }
  lastFrameTime = millis();
}

uint32_t DotPlatformClass::frameCount() {
  return frames;
}
