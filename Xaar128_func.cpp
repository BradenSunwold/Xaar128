#include <SPI.h>
#include "Xaar128.h"
#include "image.h"
#include <Arduino.h>
#include <stdint.h>

// Function to drive correct power up timing
void powerUp(int xVDD_1, int nRESET_1, int xVDD_2, int nRESET_2, int xVDD_3, int nRESET_3, int xVDD_4, int nRESET_4, int relayVHCH, int relayVHCL) {
  // Enable VDD HIGH.
  digitalWrite(xVDD_1, HIGH);
  digitalWrite(xVDD_2, HIGH);
  digitalWrite(xVDD_3, HIGH);
  digitalWrite(xVDD_4, HIGH);
  delay(50);

  // nRESET pulse width = 500ns
  digitalWrite(nRESET_1, LOW);
  digitalWrite(nRESET_2, LOW);
  digitalWrite(nRESET_3, LOW);
  digitalWrite(nRESET_4, LOW);
  delayMicroseconds(1);

  digitalWrite(relayVHCH, LOW);
  delay(50);
  digitalWrite(relayVHCL, LOW);
  delay(50);

  // Reset HIGH.
  digitalWrite(nRESET_1, HIGH);
  digitalWrite(nRESET_2, HIGH);
  digitalWrite(nRESET_3, HIGH);
  digitalWrite(nRESET_4, HIGH);
  delay(10);
}

// Function to drive correct power down timing
void powerDown(int xVDD_1, int nRESET_1, int xVDD_2, int nRESET_2, int xVDD_3, int nRESET_3, int xVDD_4, int nRESET_4, int relayVHCH, int relayVHCL) {
  // nRESET pulse width = 500ns
  digitalWrite(nRESET_1, HIGH);
  digitalWrite(nRESET_2, HIGH);
  digitalWrite(nRESET_3, HIGH);
  digitalWrite(nRESET_4, HIGH);
  delayMicroseconds(1);

  // Reset LOW?
  digitalWrite(nRESET_1, LOW);
  digitalWrite(nRESET_2, LOW);
  digitalWrite(nRESET_3, LOW);
  digitalWrite(nRESET_4, LOW);
  delay(50);

  digitalWrite(relayVHCH, HIGH);
  delay(50);
  digitalWrite(relayVHCL, HIGH);

  // reset RESET to inactive
  digitalWrite(nRESET_1, HIGH);
  digitalWrite(nRESET_2, HIGH);
  digitalWrite(nRESET_3, HIGH);
  digitalWrite(nRESET_4, HIGH);

  // Set VDD LOW.
  digitalWrite(xVDD_1, LOW);
  digitalWrite(xVDD_2, LOW);
  digitalWrite(xVDD_3, LOW);
  digitalWrite(xVDD_4, LOW);
}
