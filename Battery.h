float batVolt;

float getBatVolt()
{
  for(int i = 0 ; i < 50 ; i++)
  { batVolt += analogRead(batSensorPin); }
  batVolt = batVolt/50;
  batVolt = (batVolt * 5) / 4096;
  batVolt = batVolt / 0.3125;               //needed because the voltage divider
  return batVolt;
}
