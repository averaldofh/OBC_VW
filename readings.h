//thermistor NTC 
//coefficients obtained using datasheet and this website
//https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html
// steinhart-hart
//
//A29 sensor to 3.3, 22ohm to gnd voltage divider
//full tank gives aroun 40ohm, empty around 5
//giving us voltage values that range from 2.7V(empty) and 1V (full)
float batVolt;
float rb1 = 56.00/*56k*/, rb2 = 10.00/*10k*/;
uint32_t fuelqtt;
int minCal, maxCal;
double shA = 0.0006739775689;
double shB = 0.0002201819877;
double shC = 0.0000000943151063;
float Vout, TempC,Rout;
float Rt = 100000;
int rawADC;

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

int getFuelQt(){
  
  for(int i = 0 ; i < 1000 ; i++)
  { fuelqtt += analogRead(FuelSensorPin); }
  fuelqtt = fuelqtt/1000;
  if(fuelqtt>minCal)fuelqtt=minCal;else if(fuelqtt<maxCal)fuelqtt=maxCal;
  fuelqtt = map(fuelqtt,minCal,maxCal,0,100);
  return fuelqtt;
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

void getLiters(){
  int fL = map(getFuelQt(),0,100,0,40);
  menuEstLiters.setCurrentValue(fL,1);
}
