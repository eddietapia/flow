/*
 * Author: Eddie Tapia, etapiahe@andrew.cmu.edu
 * File Name: timer.ino
 * Date: September 3rd 2019
 * 
 * Purpose: Configures a timer so co-workers do not disturb you when you 
 * are trying to get work done.
 */

// Import the modules we will need to control the led matrix
#include "LedControl.h"

const int DIN = 11;
const int CLK = 13;
const int CS = 10;
const int DISPLAY_NUM = 4;
const int DISPLAY_1 = 0;
const int DISPLAY_2 = 1;
const int DISPLAY_3 = 2;
const int DISPLAY_4 = 3;

unsigned long delayTime = 200;               // Delay between frames
LedControl display = LedControl(DIN, CLK, CS, DISPLAY_NUM);  // Pins: DIN,CLK,CS, # of Display connected

// Initialize our array of numbers 0-9
const uint64_t NUMBERS[] = {
  0x1c2222222222221c,
  0x1c08080808080c08,
  0x3e0408102020221c,
  0x1c2220201820221c,
  0x20203e2224283020,
  0x1c2220201e02023e,
  0x1c2222221e02221c,
  0x040404081020203e,
  0x1c2222221c22221c,
  0x1c22203c2222221c
};

const uint64_t NUMBERS_WITH_COLON[] = {
  0x1c22a22222a2221c,
  0x1c08880808880c08,
  0x3e04881020a0221c,
  0x1c22a02018a0221c,
  0x2020be2224a83020,
  0x1c22a0201e82023e,
  0x1c22a2221e82221c,
  0x0404840810a0203e,
  0x1c22a2221ca2221c,
  0x1c22a03c22a2221c
};

const int NUMBERS_LEN = sizeof(NUMBERS)/8;

// Initialize our array of letters A-Z and a-z
const uint64_t LETTERS[] = {
  0x6666667e66663c00,  // A
  0x3e66663e66663e00,  // ..
  0x3c66060606663c00,
  0x3e66666666663e00,
  0x7e06063e06067e00,
  0x0606063e06067e00,
  0x3c66760606663c00,
  0x6666667e66666600,
  0x3c18181818183c00,
  0x1c36363030307800,
  0x66361e0e1e366600,
  0x7e06060606060600,
  0xc6c6c6d6feeec600,
  0xc6c6e6f6decec600,
  0x3c66666666663c00,
  0x06063e6666663e00,
  0x603c766666663c00,
  0x66361e3e66663e00,
  0x3c66603c06663c00,
  0x18181818185a7e00,
  0x7c66666666666600,
  0x183c666666666600,
  0xc6eefed6c6c6c600,
  0xc6c66c386cc6c600,
  0x1818183c66666600,
  0x7e060c1830607e00,  // Z
  0x7c667c603c000000,  // a
  0x3e66663e06060600,  // ..
  0x3c6606663c000000,
  0x7c66667c60606000,
  0x3c067e663c000000,
  0x0c0c3e0c0c6c3800,
  0x3c607c66667c0000,
  0x6666663e06060600,
  0x3c18181800180000,
  0x1c36363030003000,
  0x66361e3666060600,
  0x1818181818181800,
  0xd6d6feeec6000000,
  0x6666667e3e000000,
  0x3c6666663c000000,
  0x06063e66663e0000,
  0xf0b03c36363c0000,
  0x060666663e000000,
  0x3e403c027c000000,
  0x1818187e18180000,
  0x7c66666666000000,
  0x183c666600000000,
  0x7cd6d6d6c6000000,
  0x663c183c66000000,
  0x3c607c6666000000,
  0x3c0c18303c000000  // z
};

const int LETTERS_LEN = sizeof(LETTERS)/8;


void setup() {
  // Wake up the four displays
  display.shutdown(0, false);
  display.shutdown(1, false);
  display.shutdown(2, false);
  display.shutdown(3, false);

  // Set the intensity levels
  display.setIntensity(0, 5);
  display.setIntensity(1, 5);
  display.setIntensity(2, 5);
  display.setIntensity(3, 5);

  // Clear the displays
  display.clearDisplay(0);
  display.clearDisplay(1);
  display.clearDisplay(2);
  display.clearDisplay(3);
}

void displayImage(uint64_t image, int displayNum) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed(displayNum, i, j, bitRead(row, j));
    }
  }

}

// Initialize our veriables to start the timer
int i = 0;
int secondsTwosPlace = 0;
int minutesOnesPlace = 0;
int minutesTwosPlace = 0;

void loop() {
  displayImage(NUMBERS[secondsTwosPlace], DISPLAY_2);
  displayImage(NUMBERS_WITH_COLON[minutesOnesPlace], DISPLAY_3);
  displayImage(NUMBERS[minutesTwosPlace], DISPLAY_4);
  
  displayImage(NUMBERS[i], DISPLAY_1);
  if (++i >= NUMBERS_LEN ) {
    i = 0;
    secondsTwosPlace++;
  }
  if(secondsTwosPlace >= 6){
    secondsTwosPlace = 0;
    minutesOnesPlace++;
  }
  if(minutesOnesPlace >= NUMBERS_LEN){
    minutesOnesPlace = 0;
    minutesTwosPlace++;
  }
  if(minutesTwosPlace >= 6){
    // Reset the clock since we only have four displays
    secondsTwosPlace = 0;
    minutesOnesPlace = 0;
    minutesTwosPlace = 0;
  }
  delay(1000);
}
