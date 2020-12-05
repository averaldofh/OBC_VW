float wheelDiameter;
int spd,modo=0,telaAtiva=0;               //variables: speed, screen mode, and active screen
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
RTC_DATA_ATTR uint16_t slpTimSet=5000;         //variable to store the trip2 in the deepsleep state

void odoLoop(){
  attachInterrupt(digitalPinToInterrupt(pin_sensor), readPulses, RISING);    //attach interrupt  
  getRpm();
 //----------------- ODOMETER UPDATE -----------------------//
 if(millis()-tempo>=1000)                                       //check if 1 second has passed
 {
 odoT += (((RPM * wheelDiameter * 0.001885) / 360) / ((millis()-tempo)/1000))/10;   //then gets the distance travelled into odo
 tempo=millis();
 }  

 //----------------- DATA ACQUISITION ----------------------//
    spd = int(RPM*wheelDiameter * 0.001885);    //get speed value
    oilT = getOilTemp();           //get new temperature value 
    fuelqty = getFuelQt(0);         //get new percentage value. the parameter 0 will return the quantity in percentage. if set to 1 returns the rawADC value. Leave it 0.
    batStats = getBatVolt();       //gets the battery voltage

 //------------------ DATA FORMATTING ---------------------//
 odot = odoT;                 //since sprintf in arduino IDE
 odo = (int)odot;             //isn't compatible with floats, (i've read this and believed. didnt try)
 odot = odot - odo;           //i used this simple method
 odot = odot*10;              //to separate the odometer decimals
 odoc = (int)odot;            //to later print on screen
 trip1=odo-lastTrip1;         //Count how many km were travelled since last trip1 reset
 trip2=odo-lastTrip2;         //same for trip2

 //----------------- DISPLAY/EEPROM OPTIONS ----------------------//   
 //displays the right screen base on previous 'menu' system or pre-selectd values                                     

   if(telaAtiva==0)                      
     { drawMain(spd,odo,odoc,trip1,trip2,modo,fuelqty); }                         
   else if(telaAtiva==1 || oilT>maxOT)                       
     { drawStats(oilT, batStats);}                                                 
   else if(telaAtiva==2)
     {telaTempSet();}
   else if(telaAtiva==3)
     {telaWheelD();}
   else if(telaAtiva==4)
     {fuelCalMin();}
   else if(telaAtiva==5)
     {fuelCalMax();}   

//------------------- DeepSleep detection     
  uint32_t lt = millis();
  while(!digitalRead(ignPin)){    //when the ignition goes low waits for x seconds before go deepsleep
    telaAtiva=0;
    drawSleepCounter((slpTimSet-(millis()-lt))/1000);   //show remaining time on display
    if(millis() - lt >= slpTimSet){                     //if x seconds is reached, save to rtc memory and enters deepsleep
    odoDS = odo;
    odocDS = odoc;
    t1DS = lastTrip1;
    t2DS = lastTrip2;
    oled.setPowerSave(1);
    esp_deep_sleep_start();}
  }
} //loop
