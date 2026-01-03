/*
 * BasicGame - DOTplatform example
 *
 * Simple game demonstrating the DotPlatform framework:
 * - Move a pixel with LEFT/RIGHT buttons
 * - Chord (both buttons) to toggle pixel on/off
 *
 * https://github.com/edlovesjava/DOTplatform
 */

#include <DotPlatform.h>

uint8_t playerX = 3;
uint8_t playerY = 7;
bool playerVisible = true;

void setup() {
  Dot.begin();
}

void loop() {
  // Update input state
  Dot.update();

  // Handle input
  if (Dot.input.released(BTN_LEFT)) {
    if (playerX < 7) playerX++;
  }
  if (Dot.input.released(BTN_RIGHT)) {
    if (playerX > 0) playerX--;
  }
  if (Dot.input.pressed(BTN_CHORD)) {
    playerVisible = !playerVisible;
  }

  // Draw
  Dot.display.clear();
  if (playerVisible) {
    Dot.display.setPixel(playerX, playerY, ON);
  }
  Dot.display.show();

  // Maintain 60 FPS
  Dot.waitFrame(60);
}
