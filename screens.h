//V0.1
//
// MAIN SCREEN
//  
//
//
//
#include <U8g2lib.h>  //u8g2 Library
#include "fusca.h"
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 16, 15, 4);  //Oled Display

#define font1 u8g2_font_inb38_mn // 38px numbers of speedometer
#define font2 u8g2_font_trixel_square_tf //4px total trip1 trip2
#define font3 u8g2_font_profont10_tf //6px fuel low 
#define font4 u8g2_font_helvB12_tf //12px km/h temps
#define font5 u8g2_font_tenfatguys_tf //10px odometer numbers
#define font6 u8g2_font_profont22_tf //14px oil
#define fusca_width 128
#define fusca_height 64

char buf1[128];
uint32_t tempoF;

//------------------------------------SPLASH SCREEN
  void draw_splash()
{
u8g2.clearBuffer();
u8g2.drawXBMP(0,0,fusca_width,fusca_height,fusca_bits);  
u8g2.sendBuffer();
}


//------------------------------------MAIN SCREEN
bool drawMain(int vel, int odo, int odoc, int trip1, int trip2, int type, int fqty)
{ 
  fqty = map(fqty,0,100,0,29);
  u8g2.clearBuffer();
  u8g2.drawFrame(0,0,u8g2.getDisplayWidth(),u8g2.getDisplayHeight());
  u8g2.setFont(font4);
  u8g2.drawStr(126-u8g2.getStrWidth("km/h"),47,"km/h");
  u8g2.setFont(font3);
  u8g2.drawStr(103,12,"FUEL");
  u8g2.drawFrame(96,13,30,10);
  u8g2.drawBox(97,14,fqty,9);
   if(fqty<=3){
  u8g2.drawStr(104,21,"LOW");
   }
  u8g2.setFont(font1);
  u8g2.setFontMode(1);
  sprintf(buf1,"%3d",vel);
  u8g2.drawStr(1,47,buf1);
  
     switch(type)
  {
    case 0: u8g2.setFont(font2);
            u8g2.drawStr(105,56,"TOTAL"); 
            u8g2.setFont(font5);
            sprintf(buf1,"%05d.%d km",odo,odoc);
            u8g2.drawStr(3,62,buf1)
            ;break;
    case 1: u8g2.setFont(font2);
            u8g2.drawStr(105,62,"TRIP 1"); 
            u8g2.setFont(font5);
            sprintf(buf1,"%05d km",trip1);
            u8g2.drawStr(3,62,buf1);break;
    case 2: u8g2.setFont(font2);
            u8g2.drawStr(105,62,"TRIP 2"); 
            u8g2.setFont(font5);
            sprintf(buf1,"%05d km",trip2);
            u8g2.drawStr(3,62,buf1);break;
  }
  u8g2.sendBuffer();
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
 
  u8g2.clearBuffer();
  u8g2.drawFrame(0,0,u8g2.getDisplayWidth(),u8g2.getDisplayHeight());
  u8g2.setFont(font6);
  u8g2.setFontMode(1);
  u8g2.drawStr((128-u8g2.getStrWidth("STATS"))/2,18,"STATS");
  sprintf(buf1,"OIL: %3d.%d",Ot,otD);
  if(Ot >= maxOilT)
  {
    u8g2.setDrawColor(1);
    u8g2.drawBox(1,24,126,20);
    u8g2.setDrawColor(2);
    u8g2.drawStr((128-u8g2.getStrWidth("XXX: XXX.X"))/2,41,buf1);
  }
  else{
      u8g2.setDrawColor(1);
      u8g2.drawStr((128-u8g2.getStrWidth("XXX: XXX.X"))/2,41,buf1);
  }
 
  //BATTERY
  sprintf(buf1,"BAT: %3d.%d",Bt,BtD);
  if(Bt <= minBatVolt)
  {
    u8g2.setDrawColor(1);
    u8g2.drawBox(1,44,126,20);
    u8g2.setDrawColor(2);
    u8g2.drawStr((128-u8g2.getStrWidth("XXX: XXX.X"))/2,61,buf1);
  }
  else{
      u8g2.setDrawColor(1);
      u8g2.drawStr((128-u8g2.getStrWidth("XXX: XXX.X"))/2,61,buf1);
  }
  u8g2.sendBuffer();  
}
