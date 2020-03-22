uint32_t fuelqtt;

int getFuelQt(){
  for(int i = 0 ; i < 50 ; i++)
  { fuelqtt += analogRead(FuelSensorPin); }
  fuelqtt = fuelqtt/50;
  fuelqtt = map(fuelqtt,0,4096,0,100);
 return fuelqtt;
}
