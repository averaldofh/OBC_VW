# OBC_VW
**Author: averaldofh**

## What's this?
It is just an atempt to get an on-board computer in an '69 VW Beetle.
schematics: https://easyeda.com/averaldofh/obc-vw
## What this thing will do? (or should..)
It's going to simply show the speed, odometer, 2 parcial odometers, oil temperature, battery voltage and fuel level.

## How it works?
**Speed:** There's a hall effect sensor attached to the wheel hub, as well as 4 magnets attached to the wheels. These will give us the RPM of the wheels, then we can get the speed of the car.

**Odometer:** With speed and time we can sum our mileage very easily and store in non-volatile memory.

**Oil Temperature:** Still figuring out how i will attach an NTC to monitor oil temperature, probably at the oil cooler. But the NTC uses a Steinhart-Hart equation.

**Fuel level:** I used a scraped aircraft fuel sensor, which works between aprox 20 and 50 ohms.

**Battery Level:** Simplest thing on the whole project.


# this sketch for testing purposes, is runing ok on a ESP32 with built-in OLED screen. still under development.
![SPLASH_SCREEN](https://raw.githubusercontent.com/averaldofh/OBC_VW/master/images/OLED_Splash.png)
![MAIN_SCREEN](https://raw.githubusercontent.com/averaldofh/OBC_VW/master/images/OLED_Main.png)
![STATS_SCREEN](https://raw.githubusercontent.com/averaldofh/OBC_VW/master/images/OLED_StatsOK.png)
![STATS_SCREENOIL](https://raw.githubusercontent.com/averaldofh/OBC_VW/master/images/OLED_StatsOIL.png)
![STATS_SCREENBAT](https://raw.githubusercontent.com/averaldofh/OBC_VW/master/images/OLED_StatsBAT.png)

# Special Thanks
**Thanks to youtube.com/user/interlinkknight for his awesome piece of code, regarding RPM counting.**
