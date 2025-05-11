#include "spoof.hpp"

constexpr int PACKET_LEN = 62;
unsigned char HEALTHY_PACKET[PACKET_LEN];

unsigned char checksum(const unsigned char *arr, const int len) {
  unsigned char out = 0;
  for (int i = 0; i < len; i++) {
    out ^= arr[i];
  }
  return out;
}

void init_packet() {
  int i = 0;
  HEALTHY_PACKET[i++] = 255;            // header
  HEALTHY_PACKET[i++] = PACKET_LEN - 2; // header: length
  HEALTHY_PACKET[i++] = 49;             // header: unknown
  for (int j = 0; j < 24; j++) {
    // voltage
    HEALTHY_PACKET[i++] = 212; // low byte
    HEALTHY_PACKET[i++] = 55;  // high byte
  }
  for (int j = 0; j < 4; j++) {
    // temperature
    // TODO: entirely made-up. What is reasonable?
    HEALTHY_PACKET[i++] = 47; // low byte
    HEALTHY_PACKET[i++] = 26; // high byte
  }
  HEALTHY_PACKET[i++] = 0; // padding to reach full length?
  HEALTHY_PACKET[i++] = 0;

  HEALTHY_PACKET[i] = checksum(HEALTHY_PACKET + 1, i - 1);
  i++;
}

int healthy_packet(unsigned char **packet) {
  *packet = HEALTHY_PACKET;
  return PACKET_LEN;
}

void set_voltage(int voltage) {
  voltage *= 1000;
  unsigned char low = voltage & 0xff;
  unsigned char high = (voltage >> 8) & 0xff;

  int i = 3;
  for (int j = 0; j < 24; j++) {
    HEALTHY_PACKET[i++] = low;
    HEALTHY_PACKET[i++] = high;
  }
  HEALTHY_PACKET[PACKET_LEN - 1] = checksum(HEALTHY_PACKET + 1, PACKET_LEN - 2);
}

void set_temperature(int temp, int ind) {
  temp *= 1000;
  unsigned char low = temp & 0xff;
  unsigned char high = (temp >> 8) & 0xff;

  int i = 3 + 24 * 2 + ind * 2;
  HEALTHY_PACKET[i++] = low;
  HEALTHY_PACKET[i++] = high;
  HEALTHY_PACKET[PACKET_LEN - 1] = checksum(HEALTHY_PACKET + 1, PACKET_LEN - 2);
}
