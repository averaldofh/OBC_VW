#define wheelDiameter 65          //unit: cm
#define pin_sensor 19             //pin where the wheel sensor is attached
#define sw_pin 13                 //pin where the control switch is attached
#define FuelSensorPin 34          //pin where the fuel level sensor is attached
#define TempSensorPin 35          //pin where the NTC is attached
#define batSensorPin 36           //pin where the battery voltage is attached
#define ignPin 39                 //pin where the ignition voltage is attached
#define maxOilT 145               //defines the max oil temperature
#define minBatVolt 10             //define min battery voltage for alert purposes


#include <AceButton.h>            //button library
#include <U8g2lib.h>              //u8g2 Library
#include "EEPROM.h"               //EEPROM library
#include "Speedo.h"               //file which contains the rpm and speed functions (by interlinkknight)
#include "screens.h"              //file which contains the drawing functions for the 128x64 screen
#include "Fuel.h"                 //file which contains the fuel sensing function
#include "Temps.h"                //file which contains the temp sensing function
#include "Battery.h"              //file which contains the Battery voltage measuring function
using namespace ace_button;       //button library

float kmConv = wheelDiameter * 0.001885;  //determines the constant value for converting rpm to km/h
int spd,modo=0,telaAtiva=0;               //variables: speed, screen mode, and active screen
double odoT=0,odot;                       //variables to store odometers value
float oilT=0, batStats;                   //variable to store the oil temp and bat voltage
uint32_t tempo;                           //time variable
unsigned int odo, odoc, trip1=0;          //variables to store odometers and trip values
unsigned int trip2=0, fuelqty=0;          //variables to store trip and fuel quantity values
unsigned int lastTrip1,lastTrip2;         //variables to set trip values
char buf[32];                             //general use buffer
bool mainDrawn;
long timepass;
RTC_DATA_ATTR bool firstRun=1;            //bool variable to set some things at first run
RTC_DATA_ATTR int odoDS = 0;              //variable to store odometer in the deepsleep state
RTC_DATA_ATTR int odocDS = 0;             //variable to store hundreds in the deepsleep state
RTC_DATA_ATTR int t1DS = 0;               //variable to store the trip1 in the deepsleep state
RTC_DATA_ATTR int t2DS = 0;               //variable to store the trip2 in the deepsleep state

AceButton modeSw(sw_pin);                           //button configuration
void handleEvent(AceButton*, uint8_t, uint8_t);     //button configuration

void setup() 
{ 
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 1);
  u8g2.begin();
  draw_splash();
  EEPROM.begin(1000);
    if(odoDS!=0)
      {
      odo = odoDS;
      odot = odocDS;
      lastTrip1 = t1DS;
      lastTrip2 = t2DS;
      EEPROM.writeInt(0,odo);
      EEPROM.writeInt(5,odot);
      EEPROM.writeInt(9,lastTrip1);
      EEPROM.writeInt(13,lastTrip2);
      EEPROM.commit();
      }else{
        odo=EEPROM.readInt(0);
        odot=EEPROM.readInt(5);
        lastTrip1=EEPROM.readInt(9);
        lastTrip2=EEPROM.readInt(13);
      }
  minCal = EEPROM.readInt(20);
  maxCal = EEPROM.readInt(25);
  odoT = odo+(odot/10);
  pinMode(sw_pin,INPUT_PULLUP);
  pinMode(pin_sensor,INPUT_PULLUP);
  modeSw.setEventHandler(handleEvent);
   ButtonConfig* buttonConfig = modeSw.getButtonConfig();
   buttonConfig->setEventHandler(handleEvent);
   buttonConfig->setFeature(ButtonConfig::kFeatureClick);
   buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
 delay(2500);
}//setup


void loop() 
{ 
  attachInterrupt(digitalPinToInterrupt(pin_sensor), readPulses, RISING);    //reattaches interrupt  
  
 //----------------- ODOMETER UPDATE -----------------------//
 if(millis()-tempo>=1000)                                       //check if 1 second has passe
 {
 odoT += (getRpm() * kmConv / 3600) / ((millis()-tempo)/1000);   //then gets the distance travelled into odo
 tempo=millis();
 }  

 //----------------- DATA ACQUISITION ----------------------//
    spd = int(getRpm()*kmConv);    //get speed value

    oilT = getOilTemp();           //get new temperature value
 
    fuelqty = getFuelQt();         //get new percentage value
 
    batStats = getBatVolt();       //gets the battery voltage

 //------------------ DATA FORMATTING ---------------------//
 odot = odoT;                 //since sprintf in arduino IDE
 odo = (int)odot;             //isn't compatible with floats
 odot = odot - odo;           //i used this simple method
 odot = odot*10;              //to separate the odometer decimals
 odoc = (int)odot;            //to later print on screen
 trip1=odo-lastTrip1;         //Count how many km were travelled since last trip1 reset
 trip2=odo-lastTrip2;         //same for trip2

 //----------------- DISPLAY/EEPROM OPTIONS ----------------------//
 modeSw.check();                                          //Control button check routine
 
 detachInterrupt(0);                                      //detach interrupt while drawing on screen
 
   if(telaAtiva==0 && oilT<=maxOilT && batStats >= minBatVolt)                      //if in main screen and oilT below maximum
     { drawMain(spd,odo,odoc,trip1,trip2,modo,fuelqty); }                           //draw main screen
   if(telaAtiva==1 || oilT>maxOilT || batStats < minBatVolt)                       //if in temp screen OR oilT gets above maximum
     { drawStats(oilT, batStats);}                                                 //draw temps screen

  if(!digitalRead(ignPin))
  {
    odoDS = odo;
    odocDS = odoc;
    t1DS = lastTrip1;
    t2DS = lastTrip2;
    esp_deep_sleep_start();
  }

} //loop



//---------- control button routine -------------//
void handleEvent(AceButton* /* button */, uint8_t eventType,
    uint8_t /* buttonState */) {
  switch (eventType) {
    case AceButton::kEventClicked:
      if(telaAtiva==0 && modo<=1)modo++;else modo=0;
      if(telaAtiva==1){telaAtiva=0;modo=0;}
      break;
    case AceButton::kEventLongPressed:
      if(telaAtiva==0 && modo==1){trip1=0;lastTrip1=odo;}
      if(telaAtiva==0 && modo==2){trip2=0;lastTrip2=odo;}
      if(telaAtiva==0 && modo==0){telaAtiva=1;} 
      break;
  }
}
