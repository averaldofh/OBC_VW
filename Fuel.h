uint32_t fuelqtt;
uint8_t lastfuel;
int minCal, maxCal;

int getFuelQt(){
  
  for(int i = 0 ; i < 50 ; i++)
  { fuelqtt += analogRead(FuelSensorPin); }
  fuelqtt = fuelqtt/50;
  
 if(lastfuel-fuelqtt>100 || fuelqtt-lastfuel>100)
 {
    lastfuel = fuelqtt;
    fuelqtt = map(fuelqtt,minCal-10,maxCal+10,0,100);
    return fuelqtt;
 }
 else {
   return lastfuel;
  }
}
