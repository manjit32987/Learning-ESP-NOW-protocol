#include <esp_now.h>
#include <WiFi.h>

// Define a struct that matches the transmitter
struct PacketData {
  byte ledState;
};

PacketData incomingData;

const int ledPin = 4;  // Onboard LED (GPIO 2)

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));
  Serial.print("Received LED state: ");
  Serial.println(incomingData.ledState);

  digitalWrite(ledPin, incomingData.ledState == 1 ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Ready to receive data via ESP-NOW");
}

void loop() {
  // Nothing here, all work happens in OnDataRecv()
}
