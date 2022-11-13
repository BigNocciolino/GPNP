#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#include <sys/stat.h>
#include <nmea.h>

float kn_to_ms(float kn) {
    return kn * MS_CONVERSION;
}

float degrees_to_decimal(int degrees, double minutes, char cardinal, uint8_t type) {
    // Decimal = DEgress + Mnutes/60 + Seconds/3600
    float decimal = 0;
    minutes = minutes / 60;
    decimal = minutes + degrees;
    
    if (type == LAT_TYPE) {
        if (cardinal == NMEA_CARDINAL_DIR_SOUTH) {
            decimal *= -1;
        }
    }else if (type == LON_TYPE) {
        if (cardinal == NMEA_CARDINAL_DIR_WEST ) {
            decimal *= -1;
        }
    }
    return decimal; 
}

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

// Remove the last extension mdifying the input string
void remove_ext (char* myStr, char extSep, char pathSep) {
    char *lastExt, *lastPath;

    // Error checks and allocate string.
    assert(myStr != NULL);

    // Make a copy and find the relevant characters.
    lastExt = strrchr (myStr, extSep);
    lastPath = (pathSep == 0) ? NULL : strrchr (myStr, pathSep);

    // If it has an extension separator.
    if (lastExt != NULL) {
        // and it's to the right of the path separator.
        if (lastPath != NULL) {
            if (lastPath < lastExt) {
                // then remove it.
                *lastExt = '\0';
            }
        } else {
            // Has extension separator with no path separator.
            *lastExt = '\0';
        }
    }
}

void change_ext(char *str, const char *newExt) {
    remove_ext(str, '.', '/');
    sprintf(str, "%s.%s", str, newExt);
}