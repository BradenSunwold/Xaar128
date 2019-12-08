#ifndef XAAR128_H_
#define XAAR128_H_

#include "Arduino.h"

// MEGA SPI: SCK: 52, MISO: 50, MOSI: 51
// UNO SPI: SCK: 13, MISO: 12, MOSI: 11

// Supply / Supply Ctrl pins
// #define nSS1          10 // Chip select for XARR 128 1st IC
// #define nSS2           7 // Chip select for XARR 128 2nd IC
// #define relayVHCH     49
// #define relayVHCL     48
//
// #define nCLK          11
// #define xVDD          23
//
// // Output pins
// //int PHO = 5;
// #define nRESET           8
// #define nFIRE            5
//
// // Input pins
// #define READY            2 // This will be an interrupt pin.
void powerUp(int xVDD_1, int nRESET_1, int xVDD_2, int nRESET_2, int xVDD_3, int nRESET_3, int xVDD_4, int nRESET_4, int relayVHCH, int relayVHCL);
void powerDown(int xVDD_1, int nRESET_1, int xVDD_2, int nRESET_2, int xVDD_3, int nRESET_3, int xVDD_4, int nRESET_4, int relayVHCH, int relayVHCL);
class Xaar128 {
  //relayVHCH, relayVHCL, nCLK,
  int nSS1, nSS2, xVDD, nRESET, nFIRE, READY;
public:
  int readyState = 0;

  Xaar128();//int nSS1, int nSS2, int relayVHCH, int relayVHCL, int nCLK, int xVDD, int nRESET, int nFIRE, int READY);
  //int relayVHCH_x, int relayVHCL_x, int nCLK_x,
  void set_pins(int nSS1_x, int nSS2_x, int xVDD_x, int nRESET_x, int nFIRE_x, int READY_x);
  void init();
  //void powerUp();
  //void powerDown();
  void loadBuffer64(byte *val);
  void loadData(int pending);
  void go(int pending);
  bool fire();

// private:
  // int nSS1;
  // int nSS2;
  // int relayVHCH;
  // int relayVHCL;
  // int nCLK;
  // int xVDD;
  // int nRESET;
  // int nFIRE;
  // int READY;
};

#endif
