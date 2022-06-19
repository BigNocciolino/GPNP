#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <nema_parser.h>

char *convert_to_ISO(struct Date *d, struct Time *t) {
    char *iso_timestamp = malloc(sizeof(char) * 25); 
    sprintf(iso_timestamp, "%d-%d-%dT%d:%d:%d+00Z", d->year, d->month, d->day, t->hour, t->minutes, t->seconds);
    return iso_timestamp;
}

void removeChar(char *str, char garbage) {
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) {
            dst++;
        }
    }
    *dst = '\0';
}