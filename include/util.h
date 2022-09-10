#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

#define MS_CONVERSION 0.514444
#define LON_TYPE 1
#define LAT_TYPE 0

float kn_to_ms(float kn);
float degrees_to_decimal(int degrees, double minutes, char cardinal, uint8_t type);
int isDirectory(const char *path);

#endif