#ifndef RAV4_SPOOF_H
#define RAV4_SPOOF_H

void init_packet();
int healthy_packet(unsigned char **);
unsigned char checksum(unsigned char *, int);

#if OSCILLATE_VOLTAGE
void set_voltage(int);
#endif

#endif
