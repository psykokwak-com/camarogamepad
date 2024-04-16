# 2012 CAMARO as an XBOX gamepad


## What is it

I am compiling here the documentation to create a device that allows using a 2012 Camaro as a game controller.

**Important:** This is an experiment, there is no guarantee that it will work or that it will not cause damage to the car or other equipment.


## Result

*(Click on the image)*  
[![2012 Camaro as a controller for VR simracing](https://img.youtube.com/vi/-EMXueu6ryw/0.jpg)](https://www.youtube.com/watch?v=-EMXueu6ryw)


## Hardware used

- **Car:** 2012 Chevrolet camaro. It should work with any 5gen Camaro.
- **Computer:** Any Windows/Linux(?) computer that is able to handle a XBOX USB controller. See [ArduinoXInput](https://github.com/dmadison/ArduinoXInput) for more informations
- **Controller:** An [Arduino pro micro](https://www.amazon.fr/arduino-pro-micro/s?k=arduino+pro+micro+16mhz) or any arduino board with a ATmega32U4 MCU.
- **CAN shield:** A [MCP2515 breakout board](https://www.amazon.fr/s?k=mcp2515+breakout). (I like [this one](https://joy-it.net/en/products/SBC-CAN01) because it works with 5V and 3.3V)
- **OBD2 connector:** A [simple OBD2 connector](https://www.amazon.fr/LoongGate-OBD-II-Broches-Mettre-Ouvert/dp/B075QDV3FX/).


## Wiring

![image](https://github.com/psykokwak-com/camarogamepad/assets/11871065/f6d65f79-5285-49d6-b6e1-ad199dbba69c)  
[*Sparkfun Arduino pro micro hookup guide*](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide)

![image](https://github.com/psykokwak-com/camarogamepad/assets/11871065/1f98880f-2bcf-4ff4-994e-3597aea6797a)


## Software

1. [Install Arduino IDE](https://docs.arduino.cc/software/ide-v1/tutorials/Windows/)
2. Add the [sparkfun Arduino pro micro board](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/all#installing-windows)
4. Install the libraries dependencies (from the libraries manager): [ArduinoXInput](https://github.com/dmadison/ArduinoXInput), [Arduino-MCP2515](https://github.com/autowp/arduino-mcp2515)
5. **Important**: Follow the [instructions and install the XInput boards](https://www.partsnotincluded.com/how-to-emulate-an-xbox-controller-with-arduino-xinput/) from the ArduinoXInput library.  
   *(Failure to follow these instructions will result in the device not functioning and potentially bricking your Arduino board.)*
7. Flash the [CamaroGamePad.ino](https://github.com/psykokwak-com/camarogamepad/blob/main/XBOXController/CamaroGamePad.ino) sketch to the arduino


## CAN reverse engineering

### Hardware setup

- [ESP32-DevkitC board](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html)
- The same MCP2515 breakout board as before
  + with SPI pin 5 (CS) and pin 21 (INT)
  + with OBD2 pin 6 (CAN-H) and pin 14 (CAN-L)


### Software Setup

- Flash the ESP32 board with the [canhacker.ino](https://github.com/psykokwak-com/camarogamepad/blob/main/CanHacker/canhacker.ino) sketch
- SavvyCan with custom build (See https://github.com/collin80/SavvyCAN/issues/764)
- Configure a new connection :
  + LAWICEL
  + Serial Port Speed : 2000000
  + CAN Bus Speed : 500000
 
### CAN ID
For now, here's what I've found (CAMARO 2012):

**Brake:**
- CAN id: `0x0BE`
- Index: 1
- Size: 1 byte
- Value: [0-255]
- Unit: %
  
**Accelerator:**
- CAN id : `0x1A1`
- Index: 6
- Size: 1 byte
- Value: [0-255]
- Unit: %

**Steering wheel:**
- CAN id : `0x1E5`
- Index: 5,6
- Size: 2 bytes
- Value: [-9000:9000]
- Unit: degrees * 10

*If you've identified any other useful IDs, let me know.*

### More ?

- https://www.camaro5.com/forums/showpost.php?p=10072820&postcount=4
- https://docs.google.com/spreadsheets/d/1pFdixF6W0XK4SR6pXHIlZaFRo9qM5XmAWOK7JcAQdDM/htmlview
- https://forum.realdash.net/t/multi-bus-data-with-obdlink-ex/3872
- https://pinoutguide.com/CarElectronics/gm_car_obd_ii_pinout.shtml
