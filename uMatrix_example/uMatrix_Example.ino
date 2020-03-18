#include <Wire.h>
#include "uMatrix.h"

#define ADDR 0x60
#define SD_PIN 9

uMatrix um;

void setup() {
  um.init(ADDR, SD_PIN);
  um.clearMatrix();
}

void loop() {
  um.scrollString(" UMATRIX ");
}
