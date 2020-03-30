uint32_t fuelqtt;
uint8_t lastfuel;
int minCal, maxCal;

int getFuelQt(){
  
  for(int i = 0 ; i < 50 ; i++)
  { fuelqtt += analogRead(FuelSensorPin); }
  fuelqtt = fuelqtt/50;
  return fuelqtt;
}
