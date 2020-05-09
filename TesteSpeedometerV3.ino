#define wheelDiameter 65          //unit: cm
#define pin_sensor 19             //pin where the wheel sensor is attached
#define FuelSensorPin 34          //pin where the fuel level sensor is attached
#define TempSensorPin 35          //pin where the NTC is attached
#define batSensorPin 36           //pin where the battery voltage is attached
#define ignPin 32                 //pin where the ignition voltage is attached
#define maxOilT 130               //defines the max oil temperature
#define minBatVolt 10             //define min battery voltage for alert purposes
#define Vin 3.5
#include "TesteSpeedometerV3_menu.h"
#include <soc/rtc.h>
#include <U8g2lib.h>              //u8g2 Library
#include "EEPROM.h"               //EEPROM library
#include "Speedo.h"               //file which contains the rpm and speed functions (by interlinkknight)
#include "screens.h"              //file which contains the drawing functions for the 128x64 screen
#include "readings.h"
#include "Main.h"
const char secureMenuPassword[] PROGMEM = "1234";
ReadOnlyAuthenticationManager roAuth(secureMenuPassword);

void setup() 
{  
  menuMgr.setAuthenticator(&roAuth);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 1);     //enables deep-sleep feature and define the wakeup pin at 32. 
  oled.begin();                                     //Init display
  draw_splash();                                    //draw a VW on the screen
  renderer.setResetCallback([]{renderer.takeOverDisplay(odoLoop);});
  EEPROM.begin(1000);                               //init the eeprom
    if(odoDS!=0)                                    //check if there is something on the deepsleep memory
      {                                             //indicating a deepsleep state
      odo = odoDS;                                  //then recover these values to the main variables
      odot = odocDS;                                //to show the last data
      lastTrip1 = t1DS;
      lastTrip2 = t2DS;
      EEPROM.writeInt(0,odo);                       //also writes it to the eeprom in case the battery gets diconnected
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
  odoT = odo+(float(odot)/10);
  pinMode(pin_sensor,INPUT_PULLUP);
  pinMode(ignPin,INPUT_PULLUP);
   setupMenu();
   delay(2500);
   menuSleepTimer.setCurrentValue(slpTimSet/1000);
   menuMaxOilTemp.setCurrentValue(maxOT);
   switches.addSwitch(18, butvoid,NO_REPEAT,false);
   switches.onRelease(18, buttonCheck);
}//setup

void butvoid(unsigned char pin, bool held){}

void loop() 
{ 
  taskManager.runLoop();
}

void CALLBACK_FUNCTION setOilTemp(int id) {
    maxOT = menuMaxOilTemp.getCurrentValue();
}

void CALLBACK_FUNCTION setSleepTimer(int id) {
    slpTimSet = menuSleepTimer.getCurrentValue();
    slpTimSet *= 1000;
}

void CALLBACK_FUNCTION returnToMain(int id) {
  delay(200);
  renderer.takeOverDisplay(odoLoop);
}

void CALLBACK_FUNCTION setZfuel(int id) {
    uint32_t zF=0;
     for(int i = 0 ; i < 400 ; i++)
     { zF += analogRead(FuelSensorPin); }
     zF = zF/400;
    EEPROM.writeInt(20,zF);
    EEPROM.commit();
}

void CALLBACK_FUNCTION setMfuel(int id) {
    uint32_t fF=0;
     for(int i = 0 ; i < 400 ; i++)
     { fF += analogRead(FuelSensorPin); }
     fF = fF/400;
    EEPROM.writeInt(25,fF);
    EEPROM.commit();
}
