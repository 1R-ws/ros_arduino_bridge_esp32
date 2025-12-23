// sensors.ino
#include <Arduino.h>
#include <HX711.h>
#include <Preferences.h>

// HX711 pins
const int HX_DOUT = 16;
const int HX_SCK  = 4;

// Global objects
HX711 scale;
Preferences prefs;

float calibration_factor = 399;  // Adjust this to your load cell

void initWeightSensor() {
  scale.begin(HX_DOUT, HX_SCK);
  scale.set_scale(calibration_factor);

  prefs.begin("hx711", false);
  long saved_offset = prefs.getLong("offset", 0);

  if (saved_offset != 0) {
    scale.set_offset(saved_offset);
    Serial.println("Weight sensor: Loaded saved offset");
  } else {
    Serial.println("Weight sensor: No offset found, performing tare...");
    scale.tare(20);  // Average 20 readings for better stability
    prefs.putLong("offset", scale.get_offset());
    Serial.println("Weight sensor: Tare complete, offset saved");
  }

  Serial.println("Weight sensor ready");
}

float readWeight() {
  if (scale.is_ready()) {
    // 10 readings average for stability
    return scale.get_units(10);
  }
  return -9999.0;  // Error value if not ready
}