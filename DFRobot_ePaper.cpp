#include "DFRobot_ePaper.h"


unsigned char  *DF_Display_bw = new unsigned char[2756];
unsigned char  *DF_Display_red = new unsigned char[2756];
eInk_dev epaPerdev;

DFRobot_ePaper::DFRobot_ePaper(uint16_t width, uint16_t height, uint8_t scandir, uint8_t ADDRESS, uint8_t id) : DFRobot_Display(212, 104)
{
    epaPerdev.width = width;
    epaPerdev.highly = height;
    epaPerdev.scandir = scandir;
    Address = ADDRESS;
    epaPerID = id;
}

DFRobot_ePaper::~DFRobot_ePaper()
{}

void DFRobot_ePaper::powerOn(void)
{
    spiWirteCmd(0x06);//Set the boost
    spiWirteData(0x17);
    spiWirteData(0x17);
    spiWirteData(0x17);
    spiWirteCmd(0x04);//Access to electricity
    stateScan();
    spiWirteCmd(0X00);//Set sweeping direction
    spiWirteData(0xc3|epaPerdev.scandir);
    spiWirteCmd(0X50);
    spiWirteData(0x37);
    spiWirteCmd(0x30);//Set the PLL
    spiWirteData(0x39);
    setWindow(212,104);//Pixel setting 212*104
    spiWirteCmd(0x82);//Vcom setting
    spiWirteData (0x0a);
}

void DFRobot_ePaper::powerOff(void)
{
    stateScan();
    spiWirteCmd(0x12);
    delay(12000);
    spiWirteCmd(0x82);//to solve Vcom drop
    spiWirteData(0x00);
    spiWirteCmd(0x01);//power setting
    spiWirteData(0x02);
    spiWirteData(0x00);
    spiWirteData(0x00);
    spiWirteData(0x00);
    spiWirteCmd(0X02);//Power off
    delay(3000);
}


void DFRobot_ePaper::setWindow(uint16_t x, uint16_t y){
    uint8_t hres,vres_h,vres_l;
    hres = y/8;
    hres <<= 3; 
    vres_h = x>>8;
    vres_l = x&0xff;
    spiWirteCmd(0x61);
    spiWirteData(hres);
    spiWirteData(vres_h);
    spiWirteData(vres_l);
}

void DFRobot_ePaper::drawPicture(const unsigned char *pic_bw = NULL, const unsigned char *pic_red = NULL)
{
	//The image data to be displayed is stored in the cache
	if(pic_bw == NULL){
		for(int i = 0; i < 2756; i++)
			DF_Display_bw[i] = 0;
	}else{
		for(int i = 0; i < 2756; i++)
			DF_Display_bw[i] = pic_bw[i];
	}
	if(pic_red == NULL){
		for(int i = 0; i < 2756; i++)
			DF_Display_red[i] = 0;
	}else{
		for(int i = 0; i < 2756; i++)
			DF_Display_red[i] = pic_red[i];
	}
}

void DFRobot_ePaper::drawPicture(const unsigned char *pic_bw_3895 = NULL)
{
	//uint16_t pj = 0;
	#ifdef __AVR__
	stateScan();int t1=micros();
	for(uint16_t i=0; i<160; i++){
		Wire.beginTransmission(Address); // transmit to device #8
		if(i == 0) Wire.write(DFR_IL3895_DRAWPICTURE);
		for(unsigned char j = 0; j  <25; j++)
		{
			Wire.write(pgm_read_byte_near(~pic_bw_3895+25*i+j));
		}
		Wire.endTransmission();    // stop transmitting
	}int t2=micros();//Serial.print(t2-t1);Serial.println("(us)");
	#else
	stateScan();int t1=micros();
	for(uint16_t i=0; i<160; i++){
		Wire.beginTransmission(Address); // transmit to device #8
		if(i == 0) Wire.write(DFR_IL3895_DRAWPICTURE);
		for(unsigned char j = 0; j  <25; j++)
		{
			Wire.write(~pic_bw_3895[25*i+j]);
		}
		Wire.endTransmission();    // stop transmitting
	}int t2=micros();//Serial.print(t2-t1);Serial.println("(us)");
	#endif
}

void DFRobot_ePaper::flush()
{
    if(epaPerdev.scandir == EINK_SCAN_DIR3){
        powerOn();
        spiWirteCmd(0x10);
        for(int i = 0; i < 2756; i++) spiWirteData(~DF_Display_bw[i]);
        spiWirteCmd(0x11);
        spiWirteCmd(0x13);
        for(int i = 0; i < 2756; i++) spiWirteData(~DF_Display_red[i]);
        spiWirteCmd(0x11);
        powerOff();
    }else if(epaPerdev.scandir == EINK_SCAN_DIR1){
        int16_t y=2743;
        powerOn();
        spiWirteCmd(0x10);
        while(y>=0){
            for(int i = 0; i < 13; i++) spiWirteData(~DF_Display_bw[y+i]);
            y-=13;
        }
        y=2743;
        spiWirteCmd(0x11);
        spiWirteCmd(0x13);
        while(y>=0){
            for(int i = 0; i < 13; i++) spiWirteData(~DF_Display_red[y+i]);
            y-=13;
        }
        spiWirteCmd(0x11);
        powerOff();
    }else if(epaPerdev.scandir == EINK_SCAN_DIR2){
        int16_t y=2755;
        powerOn();
        spiWirteCmd(0x10);
        while(y>=0){
            for(int i = 0; i < 13; i++) spiWirteData(~DF_Display_bw[y-i]);
            y-=13;
        }
        y=2755;
        spiWirteCmd(0x11);
        spiWirteCmd(0x13);
        while(y>=0){
            for(int i = 0; i < 13; i++) spiWirteData(~DF_Display_red[y-i]);
            y-=13;
        }
        spiWirteCmd(0x11);
        powerOff();
    }else if(epaPerdev.scandir == EINK_SCAN_DIR4){
        int16_t y=12;
        powerOn();
        spiWirteCmd(0x10);
        while(y<=2755){
            for(int i = 0; i < 13; i++) spiWirteData(~DF_Display_bw[y-i]);
            y+=13;
        }
        y=2755;
        spiWirteCmd(0x11);
        spiWirteCmd(0x13);
        while(y<=2755){
            for(int i = 0; i < 13; i++) spiWirteData(~DF_Display_red[y-i]);
            y+=13;
        }
        spiWirteCmd(0x11);
        powerOff();
    }
}

void DFRobot_ePaper::clear(uint8_t color)
{
    uint8_t bw,red;
    if(epaPerID == IL0376F_SPI){
        if(color == WHITE){
            bw = 0x00; red = 0x00;
        }else if(color == RED){
            bw = 0x00; red = 0xff;
        }else if(color == BLACK){
            bw = 0xff; red = 0x00;
        }else{
            Serial.println("no color!");
            // return false;
        }
        for(int i = 0; i < 2756; i++){
            DF_Display_bw[i]=bw;
            DF_Display_red[i]=red;
        }
    }else if(epaPerID == IL3895_I2C){
        uint8_t buf[1]={color};
        stateScan();
        i2cWrite(Address, DFR_IL3895_CLEAR, buf, 1);
    }else if(epaPerID == IL3895_SPI){

    }else if(epaPerID == IL0376F_I2C){

    }
}

void DFRobot_ePaper::fillScreen(uint16_t color)
{
    clear(color);
}

void DFRobot_ePaper::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if(epaPerID == IL0376F_SPI){
        uint8_t sx,sy;
        uint16_t sby;
        if(x>211 || y>103){
            Serial.println("Out of display!");
            return;
        }
        sx = 211-x;      sby = sx*13;
        sy = (y+1)/8;    sby += sy;
        sy = (y+1)%8;
        if(color == RED){//Red dot
            if(sy != 0){
                DF_Display_bw[sby] &= (~(int)pow(2,8-sy)); 
                DF_Display_red[sby] |= ((int)pow(2,8-sy)); 
            }else{
                DF_Display_bw[sby-1] &= 0xFE;
                DF_Display_red[sby-1] |= 0x01;
            }
            return;
        }else if(color == BLACK){//Black spots
            if(sy != 0){
                DF_Display_red[sby] &= (~(int)pow(2,8-sy)); 
                DF_Display_bw[sby] |= ((int)pow(2,8-sy)); 
            }else{
                DF_Display_red[sby-1] &= 0xFE;
                DF_Display_bw[sby-1] |= 0x01;
            }
            return;
        }else if(color == WHITE){//White dots
            if(sy != 0){
                DF_Display_bw[sby] &= (~(int)pow(2,8-sy)); 
                DF_Display_red[sby] &= (~(int)pow(2,8-sy)); 
            }else{
                DF_Display_bw[sby-1] &= 0xFE;
                DF_Display_red[sby-1] &= 0xFE;
            }
            return;
        }else if(color == LUCENCY){
            return;
        }else return;
    }else if(epaPerID == IL3895_I2C){
        uint8_t buf[3]={x, y, color};
        stateScan();
        i2cWrite(Address, DFR_IL3895_DRAWPOINT, buf, 3);
    }else if(epaPerID == IL0376F_I2C){

    }else if(epaPerID == IL3895_SPI){

    }
}

void DFRobot_ePaper::flush(uint8_t mode)
{
    if(epaPerID == IL3895_I2C){
        uint8_t buf[1]={mode};
        stateScan();
        i2cWrite(Address, DFR_IL3895_FLUSH, buf, 1);
    }else if(epaPerID == IL0376F_SPI){
        flush();
    }else if(epaPerID == IL0376F_I2C){
    }else if(epaPerID == IL3895_SPI){
    }
}


void DFRobot_ePaper::disString(uint8_t x, uint8_t y, uint8_t size, char *ch, uint8_t color)
{
    if(epaPerID == IL3895_I2C){
        //Serial.println(strlen(ch));Serial.println(ch[0],HEX);Serial.println(ch[1],HEX);
        uint16_t len=strlen(ch);
        uint8_t buf[len+3];
        buf[0] = x;
        buf[1] = y;
        buf[len+2] = color;
        for(uint16_t i=0; i<len; i++)
        {
            buf[2+i] = (uint8_t)ch[i];
        }
        if(len+3<30){
            for(uint16_t i=0; i<len+3; i++)
            stateScan();
            i2cWrite(Address, DFR_IL3895_DISSTRING, buf, len+3);
        }else{
            uint16_t quotient=(len+3)/30, remainder=(len+3)%30;
            uint8_t *P = buf;
            stateScan();
            i2cWriteByte(Address, DFR_IL3895_DISSTRING_START);
            for(uint16_t i=0; i<quotient; i++)
            {
                i2cWriteBuffer(Address, P, 30);
                P += 30;
            }
            if(remainder != 0){
                i2cWriteBuffer(Address, P, remainder);
            }
            i2cWriteByte(Address, DFR_IL3895_DISSTRING_END);
        }
    }else if(epaPerID == IL0376F_SPI){
        disStringIL0376FSPI(x, y, size, ch, color);
    }else if(epaPerID == IL0376F_I2C){
    }else if(epaPerID == IL3895_SPI){
    }
}
