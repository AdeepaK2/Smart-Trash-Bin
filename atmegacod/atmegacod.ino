#include <Wire.h>  // Wire library - used for I2C communication

int ADXL345 = 0x53; // I2C address for the ADXL345 accelerometer
const int pirSensorPin = 7; // Define the pin for the PIR sensor
int pirState = LOW;  // PIR sensor state

const int trigPin = 9; // Ultrasonic sensor pins
const int echoPin = 10;

float X_out, Y_out, Z_out;
float X_offset = 0, Y_offset = 0, Z_offset = 0;
float X_baseline = 0, Y_baseline = 0, Z_baseline = 0;

void setup() {
  Serial.begin(9600); // Start serial communication with ESP8266
  Wire.begin(); // Start the Wire library

  initializeADXL345();
  calibrateSensor();

  pinMode(pirSensorPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  checkOrientation();
  checkPIRSensor();
  float distance = calculateDistance();
  
  int orientation = (Z_out > Z_baseline - 1.5) ? 0 : 1; // 0 for standing, 1 for flipped
  int motionDetected = (pirState == HIGH) ? 1 : 0;

  // Send data in a format like: "orientation,motion,distance"
  Serial.print(orientation);
  Serial.print(",");
  Serial.print(motionDetected);
  Serial.print(",");
  Serial.println(distance);

  delay(15000); // Send data every 15 seconds
}

void initializeADXL345() {
  Wire.beginTransmission(ADXL345); 
  Wire.write(0x2D); 
  Wire.write(8); 
  Wire.endTransmission();
  delay(10);

  Wire.beginTransmission(ADXL345);
  Wire.write(0x31);
  Wire.write(0x08); 
  Wire.endTransmission();
  delay(10);
}

void calibrateSensor() {
  long X_sum = 0, Y_sum = 0, Z_sum = 0;
  int samples = 100;

  for (int i = 0; i < samples; i++) {
    readAccelerometer();
    X_sum += X_out;
    Y_sum += Y_out;
    Z_sum += Z_out;
    delay(10);
  }

  X_offset = X_sum / samples;
  Y_offset = Y_sum / samples;
  Z_offset = Z_sum / samples;

  X_baseline = X_offset;
  Y_baseline = Y_offset;
  Z_baseline = Z_offset;
  Z_offset -= Z_baseline - 1.0; // Assuming 1g on Z axis at rest
}

void readAccelerometer() {
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);

  if (Wire.available() == 6) {
    int16_t X_out_raw = Wire.read() | (Wire.read() << 8);
    int16_t Y_out_raw = Wire.read() | (Wire.read() << 8);
    int16_t Z_out_raw = Wire.read() | (Wire.read() << 8);

    X_out = X_out_raw * 0.0078;
    Y_out = Y_out_raw * 0.0078;
    Z_out = Z_out_raw * 0.0078;
  }
}

void checkOrientation() {
  readAccelerometer();
  X_out -= X_offset;
  Y_out -= Y_offset;
  Z_out -= Z_offset;
}

void checkPIRSensor() {
  int val = digitalRead(pirSensorPin);

  if (val == HIGH) {
    if (pirState == LOW) {
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH) {
      pirState = LOW;
    }
  }
}

float calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  return distance;
}
