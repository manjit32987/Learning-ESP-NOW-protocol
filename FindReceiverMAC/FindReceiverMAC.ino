#include <WiFi.h>

void setup() {
  // Start serial communication at 115200 baud rate
  Serial.begin(115200);
  
  // Wait for serial monitor to open
  while (!Serial);

  // Initialize the Wi-Fi module (does not need to connect to a network)
  WiFi.mode(WIFI_STA);  // Set Wi-Fi mode to Station (Client)
  
  // Print a welcome message
  Serial.println("ESP32 MAC Address Retriever");
  Serial.println("Type '1' to get the MAC address of the ESP32.");
}

void loop() {
  // Check if there's data available in the Serial Monitor
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the character input
    
    // If the input is '1', display the MAC address
    if (command == '1') {
      // Get the MAC address of the ESP32
      uint8_t mac[6];
      WiFi.macAddress(mac);

      // Print the MAC address in a human-readable format
      Serial.print("ESP32 MAC Address: ");
      for (int i = 0; i < 6; i++) {
        if (mac[i] < 16) {
          Serial.print("0");  // Print leading zeros for hex values less than 16
        }
        Serial.print(mac[i], HEX); // Print the MAC address in hexadecimal
        if (i < 5) {
          Serial.print(":");
        }
      }
      Serial.println(); // New line after printing the MAC address
    }
  }
}
