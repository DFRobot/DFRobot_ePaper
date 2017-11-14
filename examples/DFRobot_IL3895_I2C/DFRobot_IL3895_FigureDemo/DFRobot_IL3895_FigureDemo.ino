/*!
 *file DFRobot_IL3895_FigureDemo.ino
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
#include "DFRobot_IL3895_I2C.h"
DFRobot_IL3895_I2C eink(A0);

#define BUSY     D5
void setup(void)
{
    Serial.begin(115200);
    //Select the corresponding pins
    eink.begin(BUSY);

    //Clear the screen and display white
    eink.fillScreen(WHITE);
    //Refresh screen display
    eink.flush(FULL);
    
    //Let me draw a red dot
    for(uint8_t x=12,y=12; y<110; y+=3)
    {
        eink.drawPixel(x,y,BLACK);
    }
    //Refresh screen display
    eink.flush(PART);
    //Draw two lines
    eink.drawLine(24,12,36,110,BLACK);
    eink.drawLine(36,12,24,110,BLACK);
    //Refresh screen display
    eink.flush(PART);
    //Draw a red rectangle
    eink.drawRect(48,12,60,98,BLACK);
    //Refresh screen display
    eink.flush(PART);
    //Fill a rectangle with black
    eink.fillRect(55,19,46,84,BLACK);
    //Refresh screen display
    eink.flush(PART);
    //Draw a hollow circle
    eink.drawCircle(160,61,40,BLACK);
    //Refresh screen display
    eink.flush(PART);
    //Draw a solid circle
    eink.fillCircle(160,61,30,BLACK);
    //Refresh screen display
    eink.flush(PART);
}

void loop(void)
{
    delay(8000);
}