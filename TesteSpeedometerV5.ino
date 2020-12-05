//----------------HARDWARE MAP
#define pin_sensor GPIO_NUM_19             //pin where the wheel sensor is attached
#define FuelSensorPin GPIO_NUM_34          //pin where the fuel level sensor is attached
#define TempSensorPin GPIO_NUM_35          //pin where the NTC is attached
#define batSensorPin GPIO_NUM_36           //pin where the battery voltage is attached
#define ignPin GPIO_NUM_39                 //pin where the ignition voltage is attached
#define encA GPIO_NUM_22                   //pin where the A (CLK) pin is connected
#define encB GPIO_NUM_21                  //pin where the B (DT) pin is connected
#define encS GPIO_NUM_23                    //pin where the switch pin is connected
#define ledOil GPIO_NUM_17                 //pin connected to an LED for general purpose
#define Vin 3.3                            //measured vcc voltage
//----------------LIBRARIES
#include <soc/rtc.h>             //libraries for rtc peripherals 
#include <U8g2lib.h>             //u8g2 Library
#include "myOta.h"               //Over-the-air updates
#include "EEPROM.h"              //EEPROM library
#include "Speedo.h"              //file which contains the rpm and speed functions (by interlinkknight)
#include "screens.h"             //file which contains the drawing functions for the 128x64 screen
#include "readings.h"            //functions and calculations 
#include "Main.h"                //the mains functions
#include <AceButton.h>           //library to manage the button
//----------------Consts and Vars
int maxOilT = 130;               //defines the max oil temperature
float minBatVolt = 10.0;         //define min battery voltage for alert purposes
using namespace ace_button;      //required by acebutton library
AceButton button(encS);          //create an acebutton instance
void handleEvent(AceButton*, uint8_t, uint8_t); //creates a handler for button events
volatile bool up,f,f1,ol;        //some general use variables

void encRead(){  //interrupt on encoder pinA to get the direction the encoder rotated
  delayMicroseconds(200);
  if(digitalRead(encA))
   up = digitalRead(encB);
  else
   up = !digitalRead(encB);
  f1=1;
}

void setup() 
{  
  pinMode(pin_sensor,INPUT_PULLUP); //define sensor pin as input
  pinMode(ignPin,INPUT_PULLUP); //define ignPin as input
  pinMode(ledOil, OUTPUT);      //define led as output
  pinMode(encS, INPUT_PULLUP);  //define encoder switch as input
  pinMode(encA,INPUT_PULLUP);   //define encoder A pin as input
  pinMode(encB,INPUT_PULLUP);   //define encoder B pin as input
  attachInterrupt(digitalPinToInterrupt(encA), encRead, CHANGE);  //configue interrupt on pin encA
  ButtonConfig* buttonConfig = button.getButtonConfig();  //config acebutton library
  buttonConfig->setEventHandler(handleEvent);             //set callback function
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);  //enable simple click button
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress); //enable long press button
  esp_sleep_enable_ext0_wakeup(ignPin, 1);     //enables deep-sleep feature and define the wakeup pin at ignPin. 
  oled.begin();                                     //Init display
  draw_splash();                                    //draw a VW on the screen
  if (button.isPressedRaw()) {myOTA();while(digitalRead(encS)){ArduinoOTA.handle();}} //check if button is pressed on boot, if so, enters OTA mode until next reset (or update)
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
  wheelDiameter=EEPROM.readInt(30);
  odoT = odo+(float(odot)/10);
  delay(2500);
}//setup

void loop() 
{ 
  odoLoop();
  button.check();
}

//created kind of a menu system using the button presses and different screens. Here is the logic:
//when in main screen(0), press to change the odometer to display TOTAL -> TRIP1 -> TRIP2 -> TOTAL...
//but if long pressed, goes to status screen(1), press once to return to main screen(0).
//When in Status Screen(1), long press enters max temperature setting(2), and single click to save and return.
//At temp setting screen(2), single click saves and goes to wheel diameter screen(3), and single click again to save and return to main(0). 
//Here's kind of a hidden screen, not to be accidentaly altered, when in temp settings(2), long press goes to min fuel calibration(4).
//at Min fuel calibration, you have your eeprom value and the actual reading, wait for it to stabilize and click once. the new value will be saved to eeprom.
//to calibrate the full tank, acces the min fuel calibration screen and HOLD the button, it will jump to max fuel calibration screen(5). click to save. 
// Main Screen = 0 -- Status Screen = 1 -- MaxTemp Setting = 2 -- Wheel Diamter Setting = 3 -- Empty Tank Setting = 4 -- Full Tank Setting = 5

void handleEvent(AceButton*, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventClicked:
      if(telaAtiva==0 && modo<=1)modo++;else modo=0;
      if(telaAtiva==1){telaAtiva=0;modo=0;maxOT = maxOilT;}
      else if(telaAtiva==2){telaAtiva=3;}
      else if(telaAtiva==3){telaAtiva=0;modo=0;maxOT = maxOilT;EEPROM.writeInt(30,wheelDiameter);EEPROM.commit();}
      else if(telaAtiva==4){minCal=getFuelQt(1);EEPROM.writeInt(20,minCal);EEPROM.commit();telaAtiva=0;minfof=0;}
      else if(telaAtiva==5){maxCal=getFuelQt(1);EEPROM.writeInt(25,maxCal);EEPROM.commit();telaAtiva=0;maxfof=0;}
      break;
    case AceButton::kEventLongPressed:
      if(telaAtiva==0 && modo==1){trip1=0;lastTrip1=odo;}
      else if(telaAtiva==0 && modo==2){trip2=0;lastTrip2=odo;}
      else if(telaAtiva==1){telaAtiva=2;}
      else if(telaAtiva==2){telaAtiva=4;}
      else if(telaAtiva==4){telaAtiva=5;}
      else if(telaAtiva==0 && modo==0){telaAtiva=1;}
      break;
  }
}
