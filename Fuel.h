uint32_t fuelqtt;
uint8_t lastfuel;
int minCal, maxCal;

int getFuelQt(){
  
  for(int i = 0 ; i < 100 ; i++)
  { fuelqtt += analogRead(FuelSensorPin); }
  fuelqtt = fuelqtt/100;
  return fuelqtt;
}
