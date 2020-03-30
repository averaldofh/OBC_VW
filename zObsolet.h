/* void printOled()
{
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.drawFrame(0,0,u8g2.getDisplayWidth(),u8g2.getDisplayHeight());
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  sprintf(buf,"SPD: %01d km/h",spd);
  u8g2.drawStr(2,10,buf);
  sprintf(buf,"ODO: %05d.%d km",odo,odoc);
  u8g2.drawStr(2,20,buf);
  u8g2.sendBuffer();          // transfer internal memory to the display
}
*/
/*
bool drawFuelWarning(int qty)
{
  qty = map(qty,0,100,0,40);
  u8g2.clearBuffer();
  u8g2.drawFrame(0,0,u8g2.getDisplayWidth(),u8g2.getDisplayHeight());
  u8g2.setFont(font6);
  u8g2.drawStr((128-u8g2.getStrWidth("WARNING"))/2,18,"WARNING");
  u8g2.setFont(font4);
  u8g2.drawStr((128-u8g2.getStrWidth("FUEL BELOW"))/2,30,"FUEL BELOW");
  sprintf(buf1,"%02d",qty);
  u8g2.drawStr((128-u8g2.getStrWidth("XX"))/2,48,buf1);
  u8g2.drawStr((128-u8g2.getStrWidth("Liters"))/2,62,"Liters");
  u8g2.sendBuffer();
  if(millis()-tempoF>=5000)
  {
  return 0;
  }
  else
  {
    return 1;
    tempoF=millis();
  }
}
*/

/*bool updMain(int vel, int odo, int odoc, int trip1, int trip2, int type)
{

  u8g2.setFont(font1);
  u8g2.setFontMode(0);
  sprintf(buf1,"%3d",vel);
  u8g2.drawStr(1,47,buf1);
  
     switch(type)
  {
    case 0: u8g2.setFont(font2);
            u8g2.drawStr(105,56,"TOTAL"); 
            u8g2.setFont(font5);
            sprintf(buf1,"%05d.%d km",odo,odoc);
            u8g2.drawStr(3,62,buf1)
            ;break;
    case 1: u8g2.setFont(font2);
            u8g2.drawStr(105,62,"TRIP 1"); 
            u8g2.setFont(font5);
            sprintf(buf1,"%05d km",trip1);
            u8g2.drawStr(3,62,buf1);break;
    case 2: u8g2.setFont(font2);
            u8g2.drawStr(105,62,"TRIP 2"); 
            u8g2.setFont(font5);
            sprintf(buf1,"%05d km",trip2);
            u8g2.drawStr(3,62,buf1);break;
  }
  u8g2.sendBuffer();
  return 1;
}
*/

/*
 ///float Vin=3.3;     // [V] 
//float Rt=100000;    // Resistor t [ohm]
//double Vout=0.0;    // Vout in A0 
//float Rout=0.0;    // Rout in A0
//float TempC=0.0;   // variable output
//float R0=100000;    // value of rct in T0 [ohm]
//float t0=298.15;   // use T0 in Kelvin [K]
//float t1=273.15;      // [K] in datasheet 0º C
//float t2=373.15;      // [K] in datasheet 100° C
//float RT1=326600;   // [ohms]  resistence in T1
//float RT2=6792;    // [ohms]   resistence in T2
//float beta=0.0;    // initial parameters [K]
//float Rinf=0.0;    // initial parameters [ohm]   
//float TempK=0.0;   // variable output

//float getOilTemp(){
//
//  beta=(log(RT1/RT2))/((1/t1)-(1/t2));
//  Rinf=R0*exp(-beta/t0);
//  
//  Vout=Vin*((float)(analogRead(TempSensorPin))/4096.0); // calc for ntc
//  Rout=(Rt*Vout/(Vin-Vout));
//
//  TempK=(beta/log(Rout/Rinf)); // calc for temperature
//  TempC=TempK-273.15;
//
//  return TempC;
//}

 */
