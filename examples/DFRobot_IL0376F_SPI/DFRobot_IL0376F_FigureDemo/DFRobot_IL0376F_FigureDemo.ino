/*!
 *file DFRobot_IL0376F_FigureDemo.ino
 *
 *@n 下载这个程序后，ePaper将会显示出一些图形，包括虚线、斜线、矩形边框、填充矩形区域、
 *   圆形边框、填充圆形区域，它展示了ePaper作图的基础功能。
 *
 * Copyright    [DFRobot](http://www.dfrobot.com), 2016
 * Copyright    GNU Lesser General Public License
 *
 * version  V0.3
 * date  2017-10-9
 */
 
#include "Arduino.h"
#include "DFRobot_IL0376F_SPI.h"

DFRobot_IL0376F_SPI eink;

#define EINK_CS  D3
#define Font_CS  D6
#define EINK_DC  D8
#define BUSY     D7

void setup(void)
{
    Serial.begin(115200);
    //Select the corresponding pins
    eink.begin(EINK_CS, Font_CS, EINK_DC, BUSY);

    //Clear the screen and display white
    eink.fillScreen(WHITE);
    //Let me draw a red dot
    for(uint8_t x=12,y=12; y<92; y+=2)
    {
        eink.drawPixel(x,y,RED);
    }
    //Draw two lines
    eink.drawLine(24,12,36,92,RED);
    eink.drawLine(36,12,24,92,RED);
    //Draw a red rectangle
    eink.drawRect(48,12,60,80,RED);
    //Fill a rectangle with black
    eink.fillRect(55,19,46,66,BLACK);
    //Draw a hollow circle
    eink.drawCircle(160,51,40,RED);
    //Draw a solid circle
    eink.fillCircle(160,51,30,BLACK);
    //Refresh screen display
    eink.flush();
}

void loop(void)
{
    delay(8000);
}

