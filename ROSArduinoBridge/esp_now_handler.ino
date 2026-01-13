#include "esp_now_handler.h"

// Broadcast address
const uint8_t broadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Receive callback - NEW V3.0 COMPATIBLE
void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
  // Access MAC address from the new recv_info structure
  const uint8_t *mac_addr = recv_info->src_addr;

  if (len != sizeof(Message)) {
    return;  // Invalid packet size
  }

  Message msg;
  memcpy(&msg, data, sizeof(msg));

  // Forward to Raspberry Pi via serial
  if (strcmp(msg.command, "request_paper") == 0) {
    Serial.print("q ");
    Serial.print(msg.value);
    Serial.print("\n");
  } else if (strcmp(msg.command, "cancel_paper") == 0) {
    Serial.print("f ");
    Serial.print(msg.value);
    Serial.print("\n");
  }

  // Debugging (Optional)
  // char macStr[18];
  // snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
  //          mac_addr[0], mac_addr[1], mac_addr[2],
  //          mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.print("Debug: Received from "); Serial.println(macStr);
}

// Initialize ESP-NOW receiver
void initEspNowReceiver() {
  WiFi.mode(WIFI_STA);

  // FIXED: Changed esp_newlib_init() back to esp_now_init()
  if (esp_now_init() != ESP_OK) {
    // Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);
}

// Send function
bool sendEspNowMessage(const uint8_t *peerMac, const Message &msg) {
  esp_err_t result = esp_now_send(peerMac, (uint8_t *)&msg, sizeof(msg));
  return (result == ESP_OK);
}
