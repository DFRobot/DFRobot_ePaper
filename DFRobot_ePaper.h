#include <Arduino.h>
#include <Wire.h>
#include <SPI.h> 
#include <String.h>
#include <math.h>
#include "DFRobot_Display.h"
#include "DFRobot_GT30L.h"

#ifndef __DFRobot_ePaper_H
#define __DFRobot_ePaper_H

//Pin definition
#ifdef __AVR__
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#endif
//scan_direction
#define EINK_SCAN_DIR1        ((uint8_t)2<<2)//From left to right, top to bottom
#define EINK_SCAN_DIR2        ((uint8_t)0<<2)//From left to right, down to the top
#define EINK_SCAN_DIR3        ((uint8_t)3<<2)//From right to left, top to bottom
#define EINK_SCAN_DIR4        ((uint8_t)1<<2)//From right to left, down to top  
//color
#define BLACK    0
#define WHITE    1
#define LUCENCY  2
#define RED      3
//Instruction set
#define  DFR_IL3895_FLUSH                    1
#define  DFR_IL3895_CLEAR                    2
#define  DFR_IL3895_DRAWPICTURE              3
#define  DFR_IL3895_DISSTRING                4
#define  DFR_IL3895_DRAWPOINT                5
#define  DFR_IL3895_DRAWLINE                 6
#define  DFR_IL3895_DEAWRECTANGLE            7
#define  DFR_IL3895_DRAWCIRCLE               8
#define  DFR_IL3895_DRAWFILLRECT             9
#define  DFR_IL3895_STANDBY                 10
#define  DFR_IL3895_DISSTRING_START         11
#define  DFR_IL3895_DISSTRING_END           12
#define  DFR_IL3895_DRAWVLINE               13
#define  DFR_IL3895_DRAWHLINE               14
//ePaper id
#define IL0376F_SPI  0
#define IL3895_I2C   1
#define IL0376F_I2C  2
#define IL3895_SPI   3
//refresh mode
#define FULL     0
#define PART     1


//eINK important parameters 
typedef struct{
    uint16_t highly;
    uint16_t width;
    uint8_t scandir;
}eInk_dev;

typedef bool InkScreen_Error;

class DFRobot_ePaper : public DFRobot_Display
{
public:
    DFRobot_ePaper(uint16_t width, uint16_t height, uint8_t scandir, uint8_t ADDRESS, uint8_t id);
    ~DFRobot_ePaper();

    void             flush();

    void             flush(uint8_t mode);

    void             drawPicture(const unsigned char *pic_bw, const unsigned char *pic_red);

    void             drawPicture(const unsigned char *pic_bw_3895);

    void             fillScreen(uint16_t color);

    void             drawPixel(int16_t x, int16_t y, uint16_t color);

    void             disString(uint8_t x, uint8_t y, uint8_t size, char *ch, uint8_t color);


private:
    void             setWindow(uint16_t x, uint16_t y);
    void             powerOn(void);
    void             powerOff(void);
    void             clear(uint8_t color);

    virtual void     stateScan(void) {}
    virtual void     spiRead(uint32_t addr, uint8_t len, char *ch) {}
    virtual void     spiWriteByte(unsigned char value) {}
    virtual void     spiWirteCmd(unsigned char command) {}
    virtual void     spiWirteData(unsigned char data) {}
    virtual void     spiDelay(unsigned char xsp) {}
    virtual void     disStringIL0376FSPI(uint8_t x, uint8_t y, uint8_t size, char *ch, uint8_t color) {}

    virtual void     i2cWrite(unsigned char addr, unsigned char Reg, unsigned char *buf, unsigned char Num) {}
    virtual void     i2cWriteByte(unsigned char addr, unsigned char Cmd) {}
    virtual void     i2cWriteBuffer(unsigned char addr,  unsigned char *buf, unsigned char Num) {}


    uint8_t           Address;
    uint8_t           epaPerID;
};

#endif
