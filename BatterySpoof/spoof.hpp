#ifndef RAV4_SPOOF_HPP
#define RAV4_SPOOF_HPP

void init_packet();
int healthy_packet(unsigned char **);
unsigned char checksum(const unsigned char *arr, int len);
void set_voltage(int);
void set_temperature(int);

#endif
