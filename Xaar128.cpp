#include <SPI.h>
#include "Xaar128.h"
#include "image.h"
#include <Arduino.h>
#include <stdint.h>

Xaar128::Xaar128() {}

// Function to define pins of print head
void Xaar128::set_pins(int nSS1_x, int nSS2_x, int xVDD_x, int nRESET_x, int nFIRE_x, int READY_x) {
  nSS1 = nSS1_x;
  nSS2 = nSS2_x;
  xVDD = xVDD_x;
  nRESET = nRESET_x;
  nFIRE = nFIRE_x;
  READY = READY_x;
}

// Function to initialze print head pins as inputs or outputs
void Xaar128::init() {
  pinMode(nSS1, OUTPUT);
  pinMode(nSS2, OUTPUT);

  pinMode(SCK, OUTPUT);
  //pinMode(PHO, OUTPUT);
  pinMode(nRESET, OUTPUT);
  pinMode(xVDD, OUTPUT);
  pinMode(nFIRE, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);

  pinMode(READY, INPUT);
  pinMode(MISO, INPUT);

  // https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus#Clock_polarity_and_phase
  // Xaar: Data is clocked into the shift register on the leading edge of SCK and clocked
  // out on the falling edge of the SCK. (CPHA=0)
  // Idle clock is high. (pg. 33) (CPOL=1)
  SPI.setDataMode (SPI_MODE2);

  // XAAR SPI clock speed must be 1mhz. SCK is sample clock, not same as CLK.
  SPI.setClockDivider (SPI_CLOCK_DIV16); //16
  SPI.begin ();

  // Set initial state for phase order to NORMAL
  //digitalWrite(PHO, LOW);

  // Set initial state for nFIRE
  digitalWrite(nFIRE, HIGH);

  // Chip select disable (=HIGH)
  digitalWrite(nSS2, HIGH);
  digitalWrite(nSS1, HIGH);

  // Reset is active low, set inactive.
  digitalWrite(nRESET, HIGH);

  // xVDD LOW
  digitalWrite(xVDD, LOW);
  readyState = LOW;
  delay(10);
}

// Function to create data buffer for loading into print head from memory
void Xaar128::loadBuffer64(byte *val) {
  // SPI.transfer is a destructive operation. Make a copy of the array.
  byte B[64];
  memcpy(B, val, 64);
  for (int i = 0; i < 8; i++) {
    SPI.transfer(B[i]);
  }
}

// Functin to load data into print head
void Xaar128::loadData(int pending) {
  int pos = (pending % (COLS/2)) * 2;

  // Load image from Flash
  byte c1[8], c2[8];
  for (int i=0; i<8; i++) {
    c1[i] = pgm_read_byte(&IMAGE[pos][i]);
    c2[i] = pgm_read_byte(&IMAGE[pos+1][i]);
  }

  // SCK is used to clock data into the shift register on the rising edge. The nSS(x) signal
  // should be pulled low to enable the target chip to load the new print data.
  digitalWrite(nSS2, LOW);
  digitalWrite(nSS1, HIGH);
  this->loadBuffer64(c2);

  digitalWrite(nSS2, HIGH);
  digitalWrite(nSS1, LOW);
  this->loadBuffer64(c1);

  // Exit with both chips disabled (HIGH).
  digitalWrite(nSS1, HIGH);
}

// Function to fire print head nozzles
bool Xaar128::fire() {
  // Wait for start of READY active cycle
  while (digitalRead(READY) == LOW) {}
  digitalWrite(nFIRE, LOW);
  // Check state. If READY does not go low, we failed to fire.
  delayMicroseconds(5); // T3 (max: 3.35micros)
  bool okay = (digitalRead(READY) == LOW);
  delayMicroseconds(100);
  digitalWrite(nFIRE, HIGH);
  delayMicroseconds(1);
  return okay;
}
