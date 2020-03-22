float Vin=4.9;     // [V]        
float Rt=92000;    // Resistor t [ohm]
float R0=100000;    // value of rct in T0 [ohm]
float t0=298.15;   // use T0 in Kelvin [K]
double Vout=0.0;    // Vout in A0 
float Rout=0.0;    // Rout in A0
// use the datasheet to get this data.
float t1=273.15;      // [K] in datasheet 0º C
float t2=373.15;      // [K] in datasheet 100° C
float RT1=326600;   // [ohms]  resistence in T1
float RT2=6792;    // [ohms]   resistence in T2
float beta=0.0;    // initial parameters [K]
float Rinf=0.0;    // initial parameters [ohm]   
float TempK=0.0;   // variable output
float TempC=0.0;   // variable output


float getOilTemp(){
  
  beta=(log(RT1/RT2))/((1/t1)-(1/t2));
  Rinf=R0*exp(-beta/t0);

  Vout=Vin*((float)(analogRead(TempSensorPin))/4096.0); // calc for ntc
  Rout=(Rt*Vout/(Vin-Vout));

  TempK=(beta/log(Rout/Rinf)); // calc for temperature
  TempC=TempK-273.15;

  return TempC;
}
