# Printer ESP32-CAM Manager
This provides a fully function camera monitor and management interface for a 3D printer. This can be powered by the printer or separately. 
This has been fully tested using a ultimaker style home made printer, REPRAP RAMPS 1.6 and Marlin firmware with a single extruder and heated bed.

The hardware is based on a ESP32-CAM (AI thinker ESP32-CAM)
The software was developed with the Arduino IDE

The original camera streaming video software for the camera was developed by Rui Santos & Sara Santos - Random Nerd Tutorials
This I have adapted to provide to include a reliable interrupt driven serial interface from the printer, real time video stream from the ESP32 camera and a web server to provide a fully functional management interface. I have tried to modularise it so it can be adapted for other printers and web interfaces.

Particular attention has been developed to ensure simultanious camera and serial performance. This project then provides a Octoprint style functionality with some additions.

A fully functional web interface covering:

ESP32-CAM reset, light.

Printer reset output (not tested).

Real time image stream of the printer.

Real time Bed and extruder temperatures.

SD Card support, reading and SD file printing.

A Action panel having various controls for the management of the printer.

A Command panel to provide the operator full command capability and logging of all data (both rx and tx) of the printer interface.


![image](https://github.com/user-attachments/assets/0709e7e8-839d-4ccd-827f-a204b7d65628)

![image](https://github.com/user-attachments/assets/94c114ee-b2b1-4910-8ae4-ca88eb97109d)

![image](https://github.com/user-attachments/assets/c8e6b942-5034-4add-bb6d-6c5964714379)

![image](https://github.com/user-attachments/assets/c51942cc-989d-4c36-a047-d52a1aacefd7)


![image](https://github.com/user-attachments/assets/f73917e0-b846-48fd-b1d5-55e15f40fcf3)


The Hardware

ESP32-CAM with camera and wifi aerial (AI thinker ESP32 CAM with 

level shifter (4 Channel IIC I2C Logic Level 5V 3.3V Converter)

buck power module (converter 12v printer PSU to 5.6V for ESP)

FTDI module for programming and debug.

Isolation switch for printer interface.

Programming push button (external).

Power switch.

Jumper field (used for testing to read or listen to messages from the printer or computer interface)


![printer_CAM](https://github.com/user-attachments/assets/0f339c00-12bf-4e90-a097-c10990f55b1b)



