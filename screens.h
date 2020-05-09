#include <U8g2lib.h>  //u8g2 Library
#include "splash.h"
U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, 16, 15, 4);  //Oled Display

#define font1 u8g2_font_inb38_mn // 38px numbers of speedometer
#define font2 u8g2_font_trixel_square_tf //4px total trip1 trip2
#define font3 u8g2_font_profont10_tf //6px fuel low 
#define font4 u8g2_font_helvB12_tf //12px km/h temps
#define font5 u8g2_font_tenfatguys_tf //10px odometer numbers
#define font6 u8g2_font_profont22_tf //14px oil
#define splash_width 128
#define splash_height 64
extern int maxOT;
char buf1[128];
uint32_t tempoF;

//------------------------------------SPLASH SCREEN
  void draw_splash()
{
oled.clearBuffer();
oled.drawXBMP(0,0,splash_width,splash_height,splash_bits);  
oled.sendBuffer();
}


//------------------------------------MAIN SCREEN
bool drawMain(int vel, int odo, int odoc, int trip1, int trip2, int type, int fqty)
{ 
  fqty = map(fqty,0,100,0,29);
  oled.clearBuffer();
  oled.drawFrame(0,0,oled.getDisplayWidth(),oled.getDisplayHeight());
  oled.setFont(font4);
  oled.drawStr(126-oled.getStrWidth("km/h"),47,"km/h");
  oled.setFont(font3);
  oled.drawStr(103,12,"FUEL");
  oled.drawFrame(96,13,30,10);
  oled.drawBox(97,14,fqty,9);
   if(fqty<=3){
  oled.drawStr(104,21,"LOW");
   }
  oled.setFont(font1);
  oled.setFontMode(1);
  sprintf(buf1,"%3d",vel);
  oled.drawStr(1,47,buf1);
  
     switch(type)
  {
    case 0: oled.setFont(font2);
            oled.drawStr(105,56,"TOTAL"); 
            oled.setFont(font5);
            sprintf(buf1,"%05d.%d km",odo,odoc);
            oled.drawStr(3,62,buf1)
            ;break;
    case 1: oled.setFont(font2);
            oled.drawStr(105,62,"TRIP 1"); 
            oled.setFont(font5);
            sprintf(buf1,"%05d km",trip1);
            oled.drawStr(3,62,buf1);break;
    case 2: oled.setFont(font2);
            oled.drawStr(105,62,"TRIP 2"); 
            oled.setFont(font5);
            sprintf(buf1,"%05d km",trip2);
            oled.drawStr(3,62,buf1);break;
  }
  oled.sendBuffer();
  return 1; 
}


//------------------------------------STATUS SCREEN
void drawStats(float ot, float batStat)
{
 unsigned int Ot, otD;
 Ot = (int)ot;
 ot = ot - Ot;
 ot = ot*10;
 otD = (int)ot;

 unsigned int Bt, BtD;
 Bt = (int)batStat;
 batStat = batStat - Bt;
 batStat = batStat*10;
 BtD = (int)batStat;
 
  oled.clearBuffer();
  oled.drawFrame(0,0,oled.getDisplayWidth(),oled.getDisplayHeight());
  oled.setFont(font6);
  oled.setFontMode(1);
  oled.drawStr((128-oled.getStrWidth("STATS"))/2,18,"STATS");
  sprintf(buf1,"OIL: %3d.%d",Ot,otD);
  if(Ot >= maxOT)
  {
    oled.setDrawColor(1);
    oled.drawBox(1,24,126,20);
    oled.setDrawColor(2);
    oled.drawStr((128-oled.getStrWidth("XXX: XXX.X"))/2,41,buf1);
  }
  else{
      oled.setDrawColor(1);
      oled.drawStr((128-oled.getStrWidth("XXX: XXX.X"))/2,41,buf1);
  }
 
  //BATTERY
  sprintf(buf1,"BAT: %3d.%d",Bt,BtD);
  if(Bt <= minBatVolt)
  {
    oled.setDrawColor(1);
    oled.drawBox(1,44,126,20);
    oled.setDrawColor(2);
    oled.drawStr((128-oled.getStrWidth("XXX: XXX.X"))/2,61,buf1);
  }
  else{
      oled.setDrawColor(1);
      oled.drawStr((128-oled.getStrWidth("XXX: XXX.X"))/2,61,buf1);
  }
  oled.sendBuffer();  
}
