#include "spoof.hpp"

bool is_whitespace(char c) {
  switch (c) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
    return true;
  default:
    return false;
  }
}

bool is_digit(char c) { return '0' <= c && c <= '9'; }

int parse_int(const char *s) {
  while (s[0] && is_whitespace(s[0])) {
    s += 1;
  }

  if (!s[0]) { // no number
    return -1;
  }

  int out = 0;
  for (; s[0] && is_digit(s[0]); s += 1) {
    out = 10 * out + (s[0] - '0');
  }

  while (s[0] && is_whitespace(s[0])) {
    s += 1;
  }

  if (s[0]) { // extra non-whitespace chars
    return -1;
  }
  return out;
}

const char *voltage_command(const char *command) {
  if (!command[0] || !is_whitespace(command[0])) {
    return "error: Expected space after voltage command";
  }
  int v = parse_int(command + 1);
  if (-1 == v) {
    return "error: Voltage command expects a single number (e.g. V 13)";
  }
  set_voltage(v);
  return "Set voltage.";
}

const char *temperature_command(const char *command) {
  if (!command[0] || !is_whitespace(command[0])) {
    return "error: Expected space after temperature command";
  }
  int t = parse_int(command + 1);
  if (-1 == t) {
    return "error: Temperature command expects a single number (e.g. T 24)";
  }
  set_temperature(t);
  return "Set temperature.";
}

const char *handle_command(const char *command) {
  while (command[0] && is_whitespace(command[0])) {
    command += 1;
  }

  if (!command[0]) { // no command
    return "";
  }

  switch (command[0]) {
  case 'V':
  case 'v':
    return voltage_command(command + 1);
  case 'T':
  case 't':
    return temperature_command(command + 1);
  default:
    return "Unknown command. Valid commands are:\n"
           "  V [number] -- set voltage (whole numbers only)\n"
           "  T [number] -- set temperature (whole numbers only)";
  }
}
