## Smart Trash Bin Project

This project leverages the UNO WiFi R3 ATmega328P ESP8266 board to create a smart trash bin that monitors its status using multiple sensors. The system can detect bin orientation, motion, and fill level, sending this data to the cloud for analysis and alerts.

### Components

- **UNO WiFi R3 ATmega328P ESP8266 Board**: Integrates the ATmega328P microcontroller and ESP8266 Wi-Fi module for seamless connectivity and processing.
- **ADXL345 Accelerometer**: Detects the bin's orientation.
- **PIR Sensor**: Detects motion near the bin.
- **Ultrasonic Sensor**: Measures the fill level of the bin.

### Repository Structure

- **atmegacod**: Contains the code for the ATmega328P microcontroller.
- **espcode**: Contains the code for the ESP8266 module.
- **LICENSE**: MIT License.
- **README.md**: Project documentation.

### ATmega328P Code

The ATmega328P code initializes the sensors and reads their values in the main loop. It detects the bin's orientation, motion, and fill level and sends this data to the ESP8266 via serial communication.

### ESP8266 Code

The ESP8266 code connects to a Wi-Fi network and sends the received data from the ATmega328P to ThingSpeak. It constructs a GET request with the sensor data and handles the HTTP communication with the ThingSpeak server.

### How to Use

1. **Set Up the Hardware**: Connect the ADXL345 accelerometer, PIR sensor, and ultrasonic sensor to the UNO WiFi R3 ATmega328P ESP8266 board.
2. **Upload the Code**: Upload the `atmegacod` to the ATmega328P and the `espcode` to the ESP8266 part of the board.
3. **Configure Wi-Fi**: Update the `ssid` and `password` in the `espcode` with your Wi-Fi credentials.
4. **Run the System**: Power the system and monitor the data being sent to ThingSpeak.

### License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

---

Feel free to clone this repository and modify the code to suit your needs. Contributions are welcome! If you encounter any issues, please open an issue on GitHub.
