//thermistor NTC 
//coefficients obtained using datasheet and this website
//https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html
// steinhart-hart
//
//aircraft sensor to 3v3, 22ohm to gnd voltage divider
//full tank gives aroun 40ohm, empty around 5
//giving us voltage values that range from 2.7V(empty) and 1V (full)
float batVolt;
float rb1 = 47.0/*56k*/, rb2 = 8.5/*10k*/;
uint32_t fuelqtt;
int minCal, maxCal;
double shA = 0.0006739775689;
double shB = 0.0002201819877;
double shC = 0.0000000943151063;
float Vout, TempC,Rout;
float Rt = 100000;
int rawADC;
bool minfof,maxfof;

float getOilTemp(){
  rawADC=0;
  for(int k = 0 ; k<100 ; k++)
  {
  rawADC += analogRead(TempSensorPin);
  }
  rawADC /= 100;
  Vout=Vin*((float)rawADC/4096.0); // calc for ntc
  Rout=(Rt*Vout/(Vin-Vout));
  Rout=log(Rout);
  TempC = (1 / (shA + (shB * Rout) + (shC * pow(Rout,3) ) ) )-273.15; 
return TempC;
}

int getFuelQt(bool a){
  
  for(int i = 0 ; i < 1000 ; i++)
  { fuelqtt += analogRead(FuelSensorPin); }
  fuelqtt = fuelqtt/1000;
  if(a){return fuelqtt;}else{
  if(fuelqtt>minCal){fuelqtt=minCal;minfof=1;}else if(fuelqtt<maxCal){fuelqtt=maxCal;maxfof=1;}
  fuelqtt = map(fuelqtt,minCal,maxCal,0,100);
  return fuelqtt;}
}

float getBatVolt()
{
  for(int i = 0 ; i < 100 ; i++)
  { batVolt += analogRead(batSensorPin); }
  batVolt = batVolt/100;
  batVolt = (batVolt * Vin) / 4096;
  batVolt = batVolt / (rb2 / (rb1+rb2));               //needed because the voltage divider
  return batVolt;
}

int getLiters(){
  int fL = map(getFuelQt(0),0,100,0,40);
  return fL;
}
