#include "spoof.hpp"

void setup() {
  Serial.begin(2400, SERIAL_8N1);
  init_packet();
}

#if OSCILLATE_VOLTAGE
#define OSCILLATE_PERIOD 60000 // one minute
unsigned long last_switch = millis();
bool osc_high = false;
#endif

void loop() {
  if (Serial.available() == 0)
    return;

  // TODO: what do we read from the car?
  for (signed char b = -1; b != -1; b = Serial.read()) {
  }

#if OSCILLATE_VOLTAGE
  if (millis() - last_switch > OSCILLATE_PERIOD) {
    osc_high = !osc_high;
    last_switch = millis();
    set_voltage(osc_high ? 14 : 11);
  }
#endif

  unsigned char *packet;
  int packet_len = healthy_packet(&packet);
  // TODO: is it possible that some-but-not-all is sent?
  Serial.write(packet, packet_len);
}
