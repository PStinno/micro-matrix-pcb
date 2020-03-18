#ifndef _UMATRIX_H
#define _UMATRIX_H

/* ---Defines--- */
// I2C Address
#define UMATRIX_DEFAULT_ADDR 0x60

/* ---Typedefs--- */

/* ---Class Definition--- */
class uMatrix {
  public:
    /* ---Functions--- */
    // Constructor
    uMatrix();
    // Initialisation function
    bool init(uint8_t addr, int sd_pin);
    /* Matrix Functions */
    void clearMatrix();
    void displayChar(char c);
    void flashString(char string[], int stringLength);
    void flashString(char string[]);
    void scrollString(char string[], int stringLength);
    void scrollString(char string[]);
  private:
    /* ---Variables--- */
    uint8_t i2c_addr;
    int shutdown_pin;
    /* ---Functions--- */
    // Write to uMatrix register
    void writeReg(uint8_t addr, uint8_t reg, uint8_t data);
    // Read from uMatrix register
    uint8_t readReg(uint8_t addr, uint8_t reg);
};

#endif
