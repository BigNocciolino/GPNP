#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "nmea_parser.h"

char *convert_to_ISO(struct Date *d, struct Time *t) {
    char *iso_timestamp = malloc(sizeof(char) * 35); 
    sprintf(iso_timestamp, "%d-%d-%dT%d:%d:%d+00Z", d->year, d->month, d->day, t->hour, t->minutes, t->seconds);
    return iso_timestamp;
}

uint8_t sent_calc_checksum(const char *sentence) {
    const char *n = sentence + 1; // Plus one, skip '$'
    uint8_t chk = 0;

    /* While current char isn't '*' or sentence ending (newline) */
    while ('*' != *n && '\n' != *n) {
        if ('\0' == *n || n - sentence > SENT_MAX_LEN) {
            /* Sentence too long or short */
            return 0;
        }
        chk ^= (uint8_t) *n;
        n++;
    }
    return chk;
}

int sent_has_checksum(const char *sentence, size_t length) {
    // 5 beacouse the sentence end with *7A\n\t
	if ('*' == sentence[length - 5]) {
		return 0;
	}

	return -1;
}

// int
// nmea_validate(const char *sentence, size_t length, int check_checksum)
// {
// 	const char *n;

// 	/* should have atleast 9 characters */
// 	if (9 > length) {
// 		return -1;
// 	}

// 	/* should be less or equal to 82 characters */
// 	if (NMEA_MAX_LENGTH < length) {
// 		return -1;
// 	}

// 	/* should start with $ */
// 	if ('$' != *sentence) {
// 		return -1;
// 	}

// 	/* should end with \r\n, or other... */
// 	if (NMEA_END_CHAR_2 != sentence[length - 1] || NMEA_END_CHAR_1 != sentence[length - 2]) {
// 		return -1;
// 	}

// 	/* should have a 5 letter, uppercase word */
// 	n = sentence;
// 	while (++n < sentence + 6) {
// 		if (*n < 'A' || *n > 'Z') {
// 			/* not uppercase letter */
// 			return -1;
// 		}
// 	}

// 	/* should have a comma after the type word */
// 	if (',' != sentence[6]) {
// 		return -1;
// 	}

// 	/* check for checksum */
// 	if (1 == check_checksum && 0 == nmea_has_checksum(sentence, length)) {
// 		uint8_t actual_chk;
// 		uint8_t expected_chk;
// 		char checksum[3];

// 		checksum[0] = sentence[length - 4];
// 		checksum[1] = sentence[length - 3];
// 		checksum[2] = '\0';
// 		actual_chk = nmea_get_checksum(sentence);
// 		expected_chk = (uint8_t) strtol(checksum, NULL, 16);
// 		if (expected_chk != actual_chk) {
// 			return -1;
// 		}
// 	}

// 	return 0;
// }
