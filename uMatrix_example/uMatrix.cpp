#include "Arduino.h"
#include <Wire.h>
#include <string.h>

#include "uMatrix.h"
#include "uMatrix_font.h"


uMatrix::uMatrix(void) {}

bool uMatrix::init(uint8_t addr, int sd_pin) {
  // Assign instance address and shutdown pin
  i2c_addr = addr;
  shutdown_pin = sd_pin;
  // Configure shutdown pin and enable display
  pinMode(shutdown_pin, OUTPUT);
  digitalWrite(shutdown_pin, HIGH);
  // Start i2c
  Wire.begin();
  // Configure display
  writeReg(i2c_addr, 0x00, 0x00); //Default Config
  writeReg(i2c_addr, 0x0D, 0x09); //10ma Drive Current
  writeReg(i2c_addr, 0x19, 0x80); //Full Brightness
}

void uMatrix::clearMatrix(void) {
  for(int i = 0; i < 7; i++) {
    writeReg(i2c_addr, i+1, 0x00);
  }
  writeReg(i2c_addr, 0x0C, 0xFF);
}

void uMatrix::displayChar(char c) {
  for(int i = 0; i < 7; i++) {
    writeReg(i2c_addr, i+1, font[c - 'A'][i]);
  }
  writeReg(i2c_addr, 0x0C, 0xFF);
}

void uMatrix::flashString(char string[], int stringLength) {
  for(int i = 0; i < stringLength; i++) {
    if(string[i] == ' ') {
      clearMatrix();
    } else {
      displayChar(string[i]);
    }

    delay(400);
    clearMatrix();
    delay(150);
  }

  clearMatrix();
  delay(400);
}

void uMatrix::flashString(char string[]) {
  flashString(string, strlen(string));
}

void uMatrix::scrollString(char string[], int stringLength) {
  int current;
  int next;
  
  for(int i = 0; i < (stringLength - 1); i++) {
    if(string[i] == ' ') {
      current = 26;
    } else {
      current = string[i] - 'A';
    }

    if(string[i + 1] == ' ') {
      next = 26;
    } else {
      next = string[i + 1] - 'A';
    }
    
    for(int j = 0; j < 6; j++) {
      for (int k = 1; k < 8; k++) {
        writeReg(i2c_addr, k, ((font[current][k - 1] << j) | (font[next][k - 1] >> (6 - j))) & 0x1F);
      }
      writeReg(i2c_addr, 0x0C, 0xFF);
      delay(200);
    }
  }

  for (int i = 0; i < 7; i++) {
    writeReg(i2c_addr, i+1, font[next][i]);
  }
  writeReg(i2c_addr, 0x0C, 0xFF);
}

void uMatrix::scrollString(char string[]) {
  scrollString(string, strlen(string));
}

/* ---Private Functions--- */
void uMatrix::writeReg(uint8_t addr, uint8_t reg, uint8_t data) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t uMatrix::readReg(uint8_t addr, uint8_t reg) {
  uint8_t retVal = 0xFF;
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(addr, (uint8_t) 1);
  if (Wire.available() >= 1) {
   retVal = Wire.read();
  }
  return retVal;
}
