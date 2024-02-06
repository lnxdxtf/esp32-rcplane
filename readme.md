# RCPLANE ESP32-ESP32-CONTROLLER

## Features: 

- [X] Controller base TX (XboxOne, Xbox360, PS3, PS4,..) | Just config to your controller
- [x] Transmissor signals to Plane. (Working for now just on wire connecteds)
- [~] Try improve latency from ~100ms to ~20ms
- [~] Video Streaming + WebServer for VR Oculus(Smartphone app)
- [~] System navigation (GPS, GYRO, ACCEL, ...)


This is a simple project that can be modified to config your own plane type;

## MODULES: 
- 2x ESP32 (1x ESP32 to Plane and 1X ESP32-S3 to Controller)
- 2x nrf24l01-pa-lna
- 1x ESC (xA) + BEC (5V)
- 1x BLDC 
- 1x LiPo Battery (+7.4V +1100mah +20C + 2S)
- 2x Servo
- 3D Printed plane (<a href="https://www.thingiverse.com/thing:6365819">Used GASB-FOUR</a>)

ESP32-S3 (CONTROLLER)
<img src="/assets/imgs/ESP32S3_Pinout.png"/>

ESP-WROOM-32 (PLANE)
<img src="/assets/imgs/ESP-wroom-32-Pinout.jpg"/>


nrf24l01-pa-lna-wireless_pinout
<img src="/assets/imgs//nrf24l01-pa-lna-wireless_pinout.jpg"/>
For ESP32 Check the arduino_pins.h (to see the SPI pins | VSPI)
go to #include<SPI.h> and then you will find the #include<arduino_pins.h>
get the pins(MOSI, MISO, SS, SCK)

