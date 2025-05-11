#include "spoof.hpp"
#if INTERACTIVE
#include "interactive.hpp"
#endif

#if OSCILLATE_VOLTAGE
#define OSCILLATE_PERIOD 60000 // one minute
unsigned long t0;
#endif

void setup() {
  Serial1.begin(2400, SERIAL_8N1);
  init_packet();
#if DEBUG_LOG || INTERACTIVE
  Serial.begin(9600);
#endif
#if OSCILLATE_VOLTAGE
  t0 = millis();
#endif
}

void loop() {
  if (Serial1.available() != 0) {
    car_request();
  }
#if INTERACTIVE
  else if (Serial.available() != 0) {
    interactive();
  }
#endif
}

void car_request() {
  delay(100);
#if DEBUG_LOG
  Serial.println("reading bytes:");
#endif
  for (signed int b = Serial1.read(); b != -1; b = Serial1.read()) {
    // read and ignore — "a few bytes, always the same"
#if DEBUG_LOG
    Serial.print("0x");
    Serial.println(b, HEX);
#endif
  }

#if OSCILLATE_VOLTAGE
  bool osc_high = (bool)(((millis() - t0) / OSCILLATE_PERIOD) % 2);
  set_voltage(osc_high ? 14 : 11);
#if DEBUG_LOG
  Serial.println(osc_high ? "voltage is high" : "voltage is low");
#endif
#endif

  unsigned char *packet;
  int packet_len = healthy_packet(&packet);
  Serial1.write(packet, packet_len);

#if DEBUG_LOG
  Serial.print("wrote ");
  Serial.print(packet_len);
  Serial.println(" bytes");
#endif
}

#if INTERACTIVE
void interactive() {
  String command = Serial.readStringUntil('\n');
  const char *response = handle_command(command.c_str());
  if (!response[0])
    return;

  Serial.println(response);
}
#endif
