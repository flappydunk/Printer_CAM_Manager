# Printer_CAM_Manager
This provides a fully function camera monitor and management interface for a 3D printer. This can be powered by the printer or seperatly. 
This has been fully tested using a  ultimaker type home made printer, REPRAP RAMPS 1.6 and Marlin firmware.

The hardware is based on a ESP32-CAM (AI thinker ESP32-CAM)
The software was developed with the Arduino IDE

The camera streming video software for the camera was developed by Rui Santos & Sara Santos - Random Nerd Tutorials
This I have adapted to provide to include a reliable serial interface to the printer, real time video stream from the ESP32 camera and a web server to provide a fully functional management interface. I have tried to modularise it so it can be adapted for other printers and web interfaces.

Particular attention has been developed to ensure simultanious camera and serial performance. This project then provides a Octoprint style functionality with some additions.
A fully functional web interface covering:
Real time image stream of the printer
Real time Bed and extruder temperatures
SD Card support, reading and SD file printing
A Action panel having various controls for the management of the printer.
A adhoc command panel to provide the operator full command capability and logging of all data (both rx and tx) of the printer interface.





![image](https://github.com/user-attachments/assets/0709e7e8-839d-4ccd-827f-a204b7d65628)



![image](https://github.com/user-attachments/assets/7938c408-79fc-4dae-95db-5b0e41cbf4d5)



![image](https://github.com/user-attachments/assets/c8e6b942-5034-4add-bb6d-6c5964714379)



![image](https://github.com/user-attachments/assets/c51942cc-989d-4c36-a047-d52a1aacefd7)


![image](https://github.com/user-attachments/assets/f73917e0-b846-48fd-b1d5-55e15f40fcf3)



