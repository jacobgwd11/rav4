#include <cassert>
#include <iostream>

#include "../spoof.hpp"

void test_checksum() {
  constexpr unsigned char arr[4] = {1, 1, 2, 4};

  assert(1 == checksum(arr, 1));
  assert(0 == checksum(arr, 2));
  assert(6 == checksum(arr, 4));
}

void test_healthy_packet() {
  init_packet();

  unsigned char *packet;
  const int length = healthy_packet(&packet);

  assert(255 == packet[0]);
  assert(length - 2 == packet[1]);
  assert(62 == length);
  assert(0x31 == packet[2]);

  unsigned char sum = 0;
  for (int i = 1; i <= packet[1]; i++) {
    sum ^= packet[i];
  }
  assert(packet[packet[1] + 1] == sum);

  for (int i = 3; i < 3 + 24 * 2; i += 2) {
    const int voltage = (packet[i + 1] << 8) + packet[i];
    assert(14292 == voltage);
  }

  for (int i = 3 + 24 * 2; i < 3 + 24 * 2 + 4 * 2; i += 2) {
    const int temperature = (packet[i + 1] << 8) + packet[i];
    assert(6703 == temperature);
  }
}

void test_set_voltage() {
#if OSCILLATE_VOLTAGE
  init_packet();
  set_voltage(13);

  unsigned char *packet;
  const int length = healthy_packet(&packet);

  for (int i = 3; i < 3 + 24 * 2; i += 2) {
    const int voltage = (packet[i + 1] << 8) + packet[i];
    assert(13000 == voltage);
  }

  assert(0xff == checksum(packet, length));
#endif
}

void run_tests() {
  test_checksum();
  test_healthy_packet();
  test_set_voltage();
}

int main(int argc, char **argv) {
  run_tests();
  std::cout << "[BatterySpoof] All tests passed!" << std::endl;
}
