#ifndef ESP_NOW_HANDLER_H
#define ESP_NOW_HANDLER_H

#include <esp_now.h>
#include <WiFi.h>

// Message structure
typedef struct {
  char command[20];  // "request_paper" or "cancel_paper"
  int value;         // Table number
} Message;

// Initialize ESP-NOW receiver
void initEspNowReceiver();

// Receive callback - NEW V3.0 SIGNATURE
// (Your error log confirmed you need this specific version)
void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len);

// Send function
bool sendEspNowMessage(const uint8_t *peerMac, const Message &msg);

#endif
