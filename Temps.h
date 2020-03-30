float Vin=3.3;     // [V]        
 double shA = 0.000763017695;
 double shB = 0.0002062009672;
 double shC = 0.0000001461161;
 float Vout, TempC,Rout;
 float Rt = 112000;
 int rawADC;


// steinhart-hart
float getOilTemp(){
  rawADC=0;
  for(int k = 0 ; k<4500 ; k++)
  {
  rawADC += analogRead(TempSensorPin);
  }
  rawADC /= 4500;
  Vout=Vin*((float)rawADC/4096.0); // calc for ntc
  Rout=(Rt*Vout/(Vin-Vout));
  Rout=log(Rout);
  TempC = (1 / (shA + (shB * Rout) + (shC * pow(Rout,3) ) ) )-273.15; 
return TempC;
}


