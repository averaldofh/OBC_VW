#include <U8g2lib.h>  //u8g2 Library
#include "splash.h"   //file containing the drawing data
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C       oled(U8G2_R0, 16, 15, 4);             //heltec onboard display
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, /* reset=*/ U8X8_PIN_NONE,15,4);  //external 1.3 Oled Display

#define font1 u8g2_font_inb38_mn // 38px numbers of speedometer
#define font2 u8g2_font_trixel_square_tf //4px total trip1 trip2
#define font3 u8g2_font_profont10_tf //6px fuel low 
#define font4 u8g2_font_helvB12_tf //12px km/h temps
#define font5 u8g2_font_tenfatguys_tf //10px odometer numbers
#define font6 u8g2_font_profont22_tf //14px oil
#define splash_width 128
#define splash_height 64
extern int maxOilT;
extern float minBatVolt,wheelDiameter;   
extern int maxOT, telaAtiva, minCal, maxCal;
extern volatile bool f,f1,up;
extern bool minfof, maxfof;
char buf1[128];
uint32_t tempoF;
int getFuelQt(bool a);

//function to read encoder values
float encUpd(float a, bool b){
 if(f1)
  {
    if(up)
     {if(!b)a++;else a += 0.5;}
    else
     {if(!b)a--;else a -= 0.5;}
    f1=0;  
  }
 return a;
}

//------------------------------------SPLASH SCREEN
  void draw_splash(){
oled.clearBuffer();
oled.drawXBMP(0,0,splash_width,splash_height,splash_bits);  
oled.sendBuffer();
}

//------------------------------------SLEEP COUNTER
void drawSleepCounter(int t){
oled.clearBuffer();
oled.setFont(font4);
oled.drawStr(64-(oled.getStrWidth("Sleeping in...")/2),20,"Sleeping in...");
sprintf(buf1,"%d seg",t);
oled.setFont(font6);
oled.setCursor(64-(oled.getStrWidth(buf1)/2),60);
oled.print(buf1);
oled.sendBuffer();
}

//------------------------------------MAIN SCREEN
bool drawMain(int vel, int odo, int odoc, int trip1, int trip2, int type, int fqty){ 
  int fqty1 = map(fqty,0,100,0,27);
  oled.clearBuffer();
 // oled.drawFrame(0,0,oled.getDisplayWidth(),oled.getDisplayHeight());
  oled.setFont(font4);
  oled.drawStr(128-oled.getStrWidth("km/h"),48,"km/h");
  oled.setFont(font3);
  oled.drawStr(102,12,"FUEL");
  oled.drawFrame(98,13,28,10);
  oled.drawBox(99,14,fqty1,9);
   if(fqty1<=3){
  oled.drawStr(104,21,"LOW");
   }
  oled.setFont(font2);
  fqty = map(fqty,0,100,0,40);
  sprintf(buf1,"%d L",fqty);
  oled.drawStr(105,30,buf1);
  oled.setFont(font1);
  oled.setFontMode(1);
  sprintf(buf1,"%3d",vel);
  oled.drawStr(3,45,buf1);
  
     switch(type)
  {
    case 0: oled.setFont(font2);
            oled.drawStr(107,56,"TOTAL"); 
            oled.setFont(font5);
            sprintf(buf1,"%05d.%d km",odo,odoc);
            oled.drawStr(5,62,buf1)
            ;break;
    case 1: oled.setFont(font2);
            oled.drawStr(107,62,"TRIP 1"); 
            oled.setFont(font5);
            sprintf(buf1,"%05d km",trip1);
            oled.drawStr(5,62,buf1);break;
    case 2: oled.setFont(font2);
            oled.drawStr(107,62,"TRIP 2"); 
            oled.setFont(font5);
            sprintf(buf1,"%05d km",trip2);
            oled.drawStr(5,62,buf1);break;
  }
  oled.sendBuffer();
  return 1; 
}

//------------------------------------STATUS SCREEN
void drawStats(float ot, float batStat){
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
  //oled.drawFrame(2,0,oled.getDisplayWidth()-1,oled.getDisplayHeight());
  oled.setFont(font6);
  oled.setFontMode(1);
  oled.drawStr((128-oled.getStrWidth("STATS"))/2,18,"STATS");
  sprintf(buf1,"OIL: %3d.%d",Ot,otD);
  if(Ot >= maxOT)
  {
    digitalWrite(ledOil,HIGH);
    oled.setDrawColor(1);
    oled.drawBox(1,24,127,20);
    oled.setDrawColor(2);
    oled.drawStr((128-oled.getStrWidth("XXX: XXX.X"))/2,41,buf1);
  }
  else{
      digitalWrite(ledOil,LOW);
      oled.setDrawColor(1);
      oled.drawStr((128-oled.getStrWidth("XXX: XXX.X"))/2,41,buf1);
  }
 
  //BATTERY
  sprintf(buf1,"BAT: %3d.%d",Bt,BtD);
  if(Bt <= minBatVolt)
  {
    oled.setDrawColor(1);
    oled.drawBox(1,44,127,20);
    oled.setDrawColor(2);
    oled.drawStr((128-oled.getStrWidth("XXX: XXX.X"))/2,61,buf1);
  }
  else{
      oled.setDrawColor(1);
      oled.drawStr((128-oled.getStrWidth("XXX: XXX.X"))/2,61,buf1);
  }
  oled.sendBuffer();  
}

//------------------------------------CONFIG MENU
//----------------Max Oil Temperature set Screen
void telaTempSet()
{
  oled.clearBuffer();
  oled.setFont(font6);
  oled.setFontMode(1);
  oled.drawStr((128-oled.getStrWidth("TempSet"))/2,18,"TempSet");
  maxOilT = encUpd(maxOilT,0);
  sprintf(buf1,"Temp: %d",maxOilT);
  oled.drawStr((128-oled.getStrWidth("Temp: 000"))/2,40,buf1);
  oled.setFont(font3);
  if(minfof){oled.drawStr(100,64,"mF!");}
  if(maxfof){oled.drawStr(120,64,"MF!");}
  oled.sendBuffer();
}

void telaWheelD()
{
  oled.clearBuffer();
  oled.setFont(font6);
  oled.setFontMode(1);
  oled.drawStr((128-oled.getStrWidth("WheelDiam"))/2,18,"WheelDiam");
  wheelDiameter = encUpd(wheelDiameter,1);
  sprintf(buf1,"D: %.1f cm",wheelDiameter);
  oled.drawStr((128-oled.getStrWidth("D: 00.0 cm"))/2,40,buf1);
  oled.sendBuffer();
}

void fuelCalMin(){
  oled.clearBuffer();
  oled.setFont(font6);
  oled.drawStr((128-oled.getStrWidth("min fuel"))/2,18,"min fuel");
  oled.setFont(font3);
  sprintf(buf1,"old: %d",minCal);
  oled.drawStr(5,35,buf1);
  sprintf(buf1, "new: %d",getFuelQt(1));
  oled.drawStr(5,55,buf1);
  oled.sendBuffer();
}

void fuelCalMax(){
  oled.clearBuffer();
  oled.setFont(font6);
  oled.drawStr((128-oled.getStrWidth("max fuel"))/2,18,"max fuel");
  oled.setFont(font3);
  sprintf(buf1,"old: %d",maxCal);
  oled.drawStr(5,35,buf1);
  sprintf(buf1, "new: %d",getFuelQt(1));
  oled.drawStr(5,55,buf1);
  oled.sendBuffer();
}

void OTAStartScreen(bool a){
  oled.clearBuffer();
  oled.setFont(font6);
  oled.drawStr((128-oled.getStrWidth("OTA"))/2,18,"OTA");
  oled.setFont(font3);
  if(a)oled.drawStr(5,35,"Awaiting...");
  oled.sendBuffer();
}

void OTAProgressScreen(int percent){
  oled.clearBuffer();
  oled.setFont(font6);
  oled.drawStr((128-oled.getStrWidth("PROGRESS"))/2,18,"PROGRESS");
  sprintf(buf1,"%d%%",percent);
  oled.drawStr((128-oled.getStrWidth("50%"))/2,64,buf1);
  oled.drawFrame(3,30,124,12);
  percent=map(percent,0,100,0,122);
  oled.drawBox(4,30,percent,12);
  oled.sendBuffer();
}

void OTAEndScreen(){
  oled.clearBuffer();
  oled.setFont(font6);
  oled.drawStr((128-oled.getStrWidth("OTA END"))/2,41,"OTA END");
  oled.sendBuffer();
}

void OTAErrorScreen(){
    oled.clearBuffer();
  oled.setFont(font6);
  oled.drawStr((128-oled.getStrWidth("OTA ERROR"))/2,41,"OTA ERROR");
  oled.sendBuffer();
}
