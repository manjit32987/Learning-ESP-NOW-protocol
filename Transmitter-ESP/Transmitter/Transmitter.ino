#include <esp_now.h>
#include <WiFi.h>

// Replace with the receiver's MAC address
uint8_t receiverMacAddress[] = {0x2C, 0xBC, 0xBB, 0x92, 0xCE, 0x84};  // 6:52:42.770 -> ESP32 MAC Address: 2C:BC:BB:92:CE:84

// Define a struct to send
struct PacketData {
  byte ledState;
};

PacketData data;

const int buttonPin = 15;  // GPIO15 for button
bool lastButtonState = HIGH;
bool ledState = false;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin);

  // Detect button press (from HIGH to LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    ledState = !ledState;  // Toggle the LED state
    data.ledState = ledState;

    esp_now_send(receiverMacAddress, (uint8_t *)&data, sizeof(data));
    Serial.print("Button Pressed. Sending value: ");
    Serial.println(data.ledState);
    delay(300);  // simple debounce
  }

  lastButtonState = currentButtonState;
}
