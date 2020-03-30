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
