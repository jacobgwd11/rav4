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

bool is_digit(const char c) { return '0' <= c && c <= '9'; }

float parse_num(const char *s) {
  while (s[0] && is_whitespace(s[0])) {
    s += 1;
  }

  if (!s[0] || !is_digit(s[0])) { // no number
    return -1;
  }

  float out = 0;
  for (; s[0] && is_digit(s[0]); s += 1) {
    out = 10 * out + (s[0] - '0');
  }
  if ('.' == s[0]) {
    s += 1;
    float place = 1.0 / 10;
    if (!s[0] || !is_digit(s[0])) {
      return -1;
    }
    for (; s[0] && is_digit(s[0]); s += 1) {
      out += place * (s[0] - '0');
      place /= 10;
    }
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
  int cmd_i = 0;
  char next_char = command[cmd_i++];
  if (!next_char) {
    return "error: Expected something after voltage command (e.g. V12 8.5 or V "
           "10)";
  }
  bool all_cells;
  int cell_num;
  if (is_whitespace(next_char)) {
    all_cells = true;
  } else if (is_digit(next_char)) {
    all_cells = false;
    cell_num = next_char - '0';
    next_char = command[cmd_i++];
    if (is_digit(next_char)) {
      cell_num = (10 * cell_num) + next_char - '0';
      next_char = command[cmd_i++];
    }
    if (!next_char || !is_whitespace(next_char)) {
      return "error: Expected space following cell number (e.g. V12 8)";
    }
    if (cell_num < 1 || cell_num > 24) {
      return "error: Expected a cell number from 1 to 24";
    }
  } else {
    return "error: Expected space or cell number from 1-24 after V (e.g. V2 "
           "16)";
  }
  const float v = parse_num(command + cmd_i);
  if (-1 == v) {
    return "error: Voltage command expects a single number (e.g. V 13.1)";
  }
  if (all_cells) {
    set_voltage(v);
  } else {
    set_voltage(v, cell_num - 1);
  }
  return "Set voltage.";
}

const char *temperature_command(const char *command) {
  char next_char = command[0];
  if (!next_char || !('1' <= next_char && next_char <= '4')) {
    return "error: Expected temperature sensor number immediately after 'T'";
  }
  if (!command[1] || !is_whitespace(command[1])) {
    return "error: Expected space after temperature command";
  }
  const float t = parse_num(command + 2);
  if (-1 == t) {
    return "error: Temperature command expects a single number (e.g. T2 24)";
  }
  set_temperature(t, next_char - '1');
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
           "  V[N] [number] -- set voltage (supports V1-V24, or just V for "
           "all)\n"
           "  T[N] [number] -- set temperature (supports T1–T4)";
  }
}
