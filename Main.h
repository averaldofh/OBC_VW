#include <BaseRenderers.h>
float kmConv = wheelDiameter * 0.001885;  //determines the constant value for converting rpm to km/h
int spd,modo=0,telaAtiva=0,slpTimSet=5000;               //variables: speed, screen mode, and active screen
int maxOT = maxOilT;
double odoT=0,odot;                       //variables to store odometers value
float oilT=0, batStats;                   //variable to store the oil temp and bat voltage
uint32_t tempo;                           //time variable
unsigned int odo, odoc, trip1=0;          //variables to store odometers and trip values
unsigned int trip2=0, fuelqty=0;          //variables to store trip and fuel quantity values
unsigned int lastTrip1,lastTrip2;         //variables to set trip values
char buf[32];                             //general use buffer
bool mainDrawn, gvB=0, tk=0, bf=0;
long timepass;
RTC_DATA_ATTR int odoDS = 0;              //variable to store odometer in the deepsleep state
RTC_DATA_ATTR int odocDS = 0;             //variable to store hundreds in the deepsleep state
RTC_DATA_ATTR int t1DS = 0;               //variable to store the trip1 in the deepsleep state
RTC_DATA_ATTR int t2DS = 0;               //variable to store the trip2 in the deepsleep state
RenderPressMode renderFnPressType;
void buttonCheck(unsigned char pin, bool heldDown) {
   switch(heldDown){
    case 0: if(telaAtiva==0 && modo<=1)modo++;else modo=0;
            if(telaAtiva==1){telaAtiva=0;modo=0;}bf=0;break;
    case 1: if(telaAtiva==0 && modo==1){trip1=0;lastTrip1=odo;}
            if(telaAtiva==0 && modo==2){trip2=0;lastTrip2=odo;}
            if(telaAtiva==1){bf=1;gvB=1;}
            if(telaAtiva==0 && modo==0){telaAtiva=1;delay(20);}break;
   }
  }
void odoLoop(unsigned int encoderValue, RenderPressMode st){
extern U8g2MenuRenderer renderer;
  attachInterrupt(digitalPinToInterrupt(pin_sensor), readPulses, RISING);    //reattaches interrupt  

 //----------------- ODOMETER UPDATE -----------------------//
 if(millis()-tempo>=1000)                                       //check if 1 second has passed
 {
 odoT += (spd/*getRpm()*/ * kmConv / 360) / ((millis()-tempo)/1000);   //then gets the distance travelled into odo
 tempo=millis();
 }  

 //----------------- DATA ACQUISITION ----------------------//
    spd = 360;//int(getRpm()*kmConv);    //get speed value
    oilT = 120;//getOilTemp();           //get new temperature value 
    fuelqty = getFuelQt();         //get new percentage value 
    getLiters();
    batStats = 12;//getBatVolt();       //gets the battery voltage

 //------------------ DATA FORMATTING ---------------------//
 odot = odoT;                 //since sprintf in arduino IDE
 odo = (int)odot;             //isn't compatible with floats
 odot = odot - odo;           //i used this simple method
 odot = odot*10;              //to separate the odometer decimals
 odoc = (int)odot;            //to later print on screen
 trip1=odo-lastTrip1;         //Count how many km were travelled since last trip1 reset
 trip2=odo-lastTrip2;         //same for trip2

 //----------------- DISPLAY/EEPROM OPTIONS ----------------------//                                        
 detachInterrupt(digitalPinToInterrupt(pin_sensor));                                //detach interrupt while drawing on screen

   if(telaAtiva==0 && oilT<=maxOT && batStats > minBatVolt)                      //if in main screen and oilT below maximum
     { drawMain(spd,odo,odoc,trip1,trip2,modo,fuelqty); }                           //draw main screen
   if(telaAtiva==1 || oilT>maxOT || batStats <= minBatVolt)                       //if in temp screen OR oilT gets above maximum
     { drawStats(oilT, batStats);}                                                 //draw temps screen
   
  uint32_t lt = millis();
  while(!digitalRead(ignPin))
  { if(millis() - lt >= slpTimSet){ 
    odoDS = odo;
    odocDS = odoc;
    t1DS = lastTrip1;
    t2DS = lastTrip2;
    esp_deep_sleep_start();}
  }
  if(gvB){gvB=0;renderer.giveBackDisplay();}
} //loop
