#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "Gala";//YOUR WIFI NAME
const char* password = "adeepaisbest";PASSWORD

const char* host = "api.thingspeak.com";
const char* apiKey = "YOURAPI";

void setup() {
  Serial.begin(9600); // Initialize serial communication with the ATmega328P
  delay(10);
  
  // Connect to Wi-Fi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (Serial.available() > 0) {
    // Read the value sent from the ATmega328P
    String data = Serial.readStringUntil('\n');
    int firstCommaIndex = data.indexOf(',');
    int secondCommaIndex = data.indexOf(',', firstCommaIndex + 1);
    
    if (firstCommaIndex > 0 && secondCommaIndex > firstCommaIndex) {
      int orientation = data.substring(0, firstCommaIndex).toInt();
      int motionDetected = data.substring(firstCommaIndex + 1, secondCommaIndex).toInt();
      float distance = data.substring(secondCommaIndex + 1).toFloat();
      
      if (orientation >= 0 && motionDetected >= 0 && distance >= 0) { // Ensure valid values are read
        WiFiClient client;
        const int httpPort = 80;
        if (!client.connect(host, httpPort)) {
          Serial.println("Connection failed");
          return;
        }
        
        // Construct the GET request URL
        String url = "/update?api_key=" + String(apiKey) + "&field1=" + String(orientation) + "&field2=" + String(motionDetected) + "&field3=" + String(distance);
        
        Serial.print("Requesting URL: ");
        Serial.println(url);
        
        // Send the request to the server
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" + 
                     "Connection: close\r\n\r\n");
        
        unsigned long timeout = millis();
        while (client.available() == 0) {
          if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
          }
        }
        
        // Read the response from the server
        while (client.available()) {
          String line = client.readStringUntil('\r');
          Serial.print(line);
        }
        
        Serial.println();
        Serial.print("Sent values: ");
        Serial.print("Orientation = ");
        Serial.print(orientation);
        Serial.print(", Motion = ");
        Serial.print(motionDetected);
        Serial.print(", Distance = ");
        Serial.println(distance);
        Serial.println("Closing connection");
      }
    }
  }
  
  delay(100); // Small delay to allow for serial communication
}

