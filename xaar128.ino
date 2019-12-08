#include <stdint.h>
#include <avr/pgmspace.h>
#include <image.h>
#include <Arduino.h>
#include <Xaar128.h>
#include "Xaar128.h"

int PENDING = 200;
int CYCLES = 10;

// Define pins for print heads

int nCLK = 11;      // Define global clk signal for all print heads

int relayVHCH = 47;  // Define global relay control signals for all print heads
int relayVHCL = 46;

// Print head C
int nSS1_C = 10;  
int nSS2_C = 7;
int xVDD_C = 23;    
int nRESET_C = 9;   
int nFIRE_C = 5;    
int READY_C = 2;    

// Print head M
int nSS1_M = 12;
int nSS2_M = 6;
int xVDD_M = 25;
int nRESET_M = 24;
int nFIRE_M = 4;
int READY_M = 3;

// Print head Y
int nSS1_Y = 26;
int nSS2_Y = 27;
int xVDD_Y = 29;
int nRESET_Y = 28;
int nFIRE_Y = 31;
int READY_Y = 30;

//Print head K
int nSS1_K = 33;
int nSS2_K = 32;
int xVDD_K = 34;
int nRESET_K = 35;
int nFIRE_K = 36; 
int READY_K = 37;

// Instantiate each print head object
Xaar128 xaar128_C;
Xaar128 xaar128_M;
Xaar128 xaar128_Y;
Xaar128 xaar128_K;

void setup (void) {
  Serial.begin(115200);     // Open serial port at 115200 baud
  
  // Initialize relays in off state (active LOW)
  pinMode(relayVHCH, OUTPUT);
  digitalWrite(relayVHCH, HIGH);
  pinMode(relayVHCL, OUTPUT);
  digitalWrite(relayVHCL, HIGH);

  pinMode(nCLK, OUTPUT);    // Initialize global clk as output 

  // Define pins for each print head
  xaar128_C.set_pins(nSS1_C, nSS2_C, xVDD_C, nRESET_C, nFIRE_C, READY_C);
  xaar128_M.set_pins(nSS1_M, nSS2_M, xVDD_M, nRESET_M, nFIRE_M, READY_M);
  xaar128_Y.set_pins(nSS1_Y, nSS2_Y, xVDD_Y, nRESET_Y, nFIRE_Y, READY_Y);
  xaar128_K.set_pins(nSS1_K, nSS2_K, xVDD_K, nRESET_K, nFIRE_K, READY_K);

  // Initialize all print heads
  xaar128_C.init();
  xaar128_M.init();
  xaar128_Y.init();
  xaar128_K.init();

  // Set up a 1mHZ square wave for Xaar state machine CLK on pin 11.
  // http://forum.arduino.cc/index.php?topic=103370.0
  TCCR1A = _BV(COM1A0);              // toggle OC1A on compare match
  OCR1A = 7;                         // top value for counter
  // UNO //TCCR1B = _BV(WGM12) | _BV(CS10);   // CTC mode, prescaler clock/1
  TCCR1B = _BV(WGM12) | _BV(CS10);   // CTC mode, prescaler clock/1

  // Write to serial port "Begin:"
  Serial.print("Begin: ");
  Serial.println(PENDING);

  float steps = 0.0;
  float fails = 0.0;

  // Power up all print heads
  powerUp(xVDD_C, nRESET_C, xVDD_M, nRESET_M, xVDD_Y, nRESET_Y, xVDD_K, nRESET_K, relayVHCH, relayVHCL);
  delay(1000);

  // Fire print head C
  for (int c=0; c<CYCLES; c++) {
    for (int p=0; p < PENDING; p++) {
      xaar128_C.loadData(p);
      delayMicroseconds(1);
      if (!xaar128_C.fire()) fails++;
      steps++;
    }
  }

  delay(50);
  // Fire print head M
  for (int c=0; c<CYCLES; c++) {
    for (int p=0; p < PENDING; p++) {
      xaar128_M.loadData(p);
      delayMicroseconds(1);
      if (!xaar128_M.fire()) fails++;
      steps++;
    }
  }

  delay(50);
  // Fire print head Y
  for (int c=0; c<CYCLES; c++) {
    for (int p=0; p < PENDING; p++) {
      xaar128_Y.loadData(p);
      delayMicroseconds(1);
      if (!xaar128_Y.fire()) fails++;
      steps++;
    }
  }

  delay(50);
  // Fire print head K
  for (int c=0; c<CYCLES; c++) {
    for (int p=0; p < PENDING; p++) {
      xaar128_K.loadData(p);
      delayMicroseconds(1);
      if (!xaar128_K.fire()) fails++; 
      steps++;
    }
  }

  delay(100);
  //xaar128_C.powerDown();
  powerDown(xVDD_C, nRESET_C, xVDD_M, nRESET_M, xVDD_Y, nRESET_Y, xVDD_K, nRESET_K, relayVHCH, relayVHCL); 
  printSummary(fails, steps);
}

// Function to output success of print - be careful trusting this
void printSummary(float fails, float steps) {
  Serial.println("Done.");
  float f = fails / steps;
  Serial.print("Steps: ");
  Serial.print(steps);
  Serial.print("    Fails: ");
  Serial.print(fails);
  Serial.print("    Failure Rate: ");
  Serial.println(f, 4);
}

void loop (void) {} // end of loop
