# OBC_VW
an atempt to get an on-board computer in an '69 VW Beetle.

this sketch for testing purposes, is runing ok on a ESP32 with built-in OLED screen. 
still under development.

The speed will be calculated after the wheels RPM. 

Wheels RPM will be measured by an Hall Effect Sensor and 4 magnets. Thanks to youtube.com/user/interlinkknight for this awesome
piece of code.

A thermistor will be used in the oil heat exchanger to measure the temperature.

A new Fuel tank sensor will be used instead of the old one. Probably a potentiometer. 

Still need to implement an eeprom to save the odometer. using the internal eeprom from esp32 gave me erros when used
EEPROM.commit(); (aka reboots).
