#ifndef UTILS_H
#define UTILS_H

#include "nmea_parser.h"
#include <stddef.h>

char *convert_to_ISO(struct Date *d, struct Time *t);
uint8_t sent_calc_checksum(const char *sentence);
int sent_has_checksum(const char *sentence, size_t length);

#endif 