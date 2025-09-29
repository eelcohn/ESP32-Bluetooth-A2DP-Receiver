# ESP32-Bluetooth-A2DP-Receiver

ESP32 based Bluetooth A2DP receiver

### Parts list:
* ESP32 development board
* PCM5102A
* RGB LED

### FAQ
* What do the colors of the LED mean?
  * Red: the device is starting up
  * Yellow: the device is ready, but no bluetooth client is connected and also the WiFi is not connected
  * Green: the device is ready, but no bluetooth client is connected but WiFi is connected
  * Cyan: the device is ready, and a bluetooth client is connecting to or disconnecting from the device
  * Blue: the device is ready, and a bluetooth client is connected to the device

### Troubleshooting
* My board (**ESP32-D0WD-V3**) doesn't appeared in boards list in Arduino IDE: select **AI Thinker ESP32-CAM**

### Based upon:
https://github.com/pschatzmann/ESP32-A2DP
http://www.winko-erades.nl/using-an-esp32-and-pcm5102-as-a-bluetooth-audio-receiver/
