#define wheelDiameter 65          //unit: cm
#define pin_sensor 19             //pin where the wheel sensor is attached
#define sw_pin 13                 //pin where the control switch is attached
#define FuelSensorPin 37
#define TempSensorPin 36
#include <AceButton.h>            //button library
#include <U8g2lib.h>              //u8g2 Library
#include "Speedo.h"               //file which contains the rpm and speed functions (by interlinkknight)
#include "screens.h"              //file which contains the drawing functions for the 128x64 screen
#include "Fuel.h"                 //file which contains the fuel sensing function
#include "Temps.h"                //file which contains the temp sensing function
#include "EEPROM.h"
#define maxOilT 145               //defines the max oil temperature
using namespace ace_button;       

float kmConv = wheelDiameter * 0.001885;  //determines the constant value for converting rpm to km/h
int spd,modo=0,telaAtiva=0;               //variables: speed, screen mode, and active screen
double odoT=0,odot;                       //variables to store odometers value
float oilT=0;                             //variable to store the oil temp
uint32_t tempo;                           //time variable
unsigned int odo, odoc, trip1=0;          //variables to store odometers and trip values
unsigned int trip2=0, fuelqty=0;          //variables to store trip and fuel quantity values
unsigned int lastTrip1,lastTrip2;         //variables to set trip values
char buf[32];                             //general use buffer
bool mainDrawn,firstRun=1;                //bool variable to set some things at first run

AceButton modeSw(sw_pin);
void handleEvent(AceButton*, uint8_t, uint8_t);

void setup() 
{ 
  EEPROM.begin(1000);
  delay(1000);
  Serial.begin(115200);
  odoT = (double)EEPROM.readDouble(0);
  trip1 = (int)EEPROM.readInt(8);
  trip2 = (int)EEPROM.readInt(12);
  u8g2.begin();
  pinMode(sw_pin,INPUT_PULLUP);
  pinMode(pin_sensor,INPUT_PULLUP);
  modeSw.setEventHandler(handleEvent);
  attachInterrupt(digitalPinToInterrupt(pin_sensor), readPulses, RISING); //Counter restarted  
  delay(1000);
  ButtonConfig* buttonConfig = modeSw.getButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
 if(firstRun){
 lastTrip1=trip1;
 lastTrip2=trip2;
 firstRun=0;
 }
}

void readPulses()  // The interrupt runs this to calculate the period between pulses:
{
  PeriodBetweenPulses = micros() - LastTimeWeMeasured;  // Current "micros" minus the old "micros" when the last pulse happens.
  LastTimeWeMeasured = micros();  // Stores the current micros so the next time we have a pulse we would have something to compare with.
  if(PulseCounter >= AmountOfReadings)  // If counter for amount of readings reach the set limit:
  {
    PeriodAverage = PeriodSum / AmountOfReadings;  // Calculate the final period dividing the sum of all readings by the
    PulseCounter = 1;  // Reset the counter to start over. The reset value is 1 because its the minimum setting allowed (1 reading).
    PeriodSum = PeriodBetweenPulses;  // Reset PeriodSum to start a new averaging operation.
    int RemapedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);  // Remap the period range to the reading range.
    RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);  // Constrain the value so it doesn't go below or above the limits.
    AmountOfReadings = RemapedAmountOfReadings;  // Set amount of readings as the remaped value.
  }
  else
  {
    PulseCounter++;  // Increase the counter for amount of readings by 1.
    PeriodSum = PeriodSum + PeriodBetweenPulses;  // Add the periods so later we can average.
  }
}  // End of Pulse_Event.

void loop() 
{ 
 //----------------- ODOMETER UPDATE -----------------------//
 if(millis()-tempo>=1000)                                       //check if 1 second has passe
 {
 odoT += (getRpm() * kmConv / 3600) / ((millis()-tempo)/1000);   //then gets the distance travelled into odo
 tempo=millis();
 }  

 //----------------- DATA ACQUISITION ----------------------//
 spd = int(getRpm()*kmConv);       //get speed value
 
   if(getOilTemp() - oilT > 1 || getOilTemp() - oilT < 1)   //if oiltemp has changed more than half degree celsius
    {  oilT = getOilTemp(); }      //get new temperature value
 
   if(getFuelQt() - fuelqty > 2 || getFuelQt() - fuelqty < 2)   //if fuelquantity has changed more than 2% 
    { fuelqty = getFuelQt(); }     //get new percentage value

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
 
   if(telaAtiva==0 && oilT<=maxOilT)                      //if in main screen and oilT below maximum
     { drawMain(spd,odo,odoc,trip1,trip2,modo,fuelqty);}  //draw main screen
   if(telaAtiva==1 || oilT>maxOilT)                       //if in temp screen OR oilT gets above maximum
     { drawTemp(oilT); }                                  //draw temps screen

 attachInterrupt(digitalPinToInterrupt(pin_sensor), readPulses, RISING);    //reattaches interrupt
    
}

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
