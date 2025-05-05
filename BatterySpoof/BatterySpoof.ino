#include "spoof.h"

void setup() {
  Serial.begin(2400, SERIAL_8N1);
  init_packet();
}

void loop() {
  if (Serial.available() == 0)
    return;

  // TODO: what do we read from the car?
  for (signed char b = -1; b != -1; b = Serial.read()) {
  }

  unsigned char *packet;
  int packet_len = healthy_packet(&packet);
  // TODO: is it possible that some-but-not-all is sent?
  Serial.write(packet, packet_len);
}
