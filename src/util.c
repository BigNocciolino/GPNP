#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
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

// remove_ext: removes the "extension" from a file spec.
//   myStr is the string to process.
//   extSep is the extension separator.
//   pathSep is the path separator (0 means to ignore).
// Returns an allocated string identical to the original but
//   with the extension removed. It must be freed when you're
//   finished with it.
// If you pass in NULL or the new string can't be allocated,
//   it returns NULL.

char *remove_ext (char* myStr, char extSep, char pathSep) {
    char *retStr, *lastExt, *lastPath;

    // Error checks and allocate string.
    if (myStr == NULL) return NULL;
    if ((retStr = malloc (strlen (myStr) + 1)) == NULL) return NULL;

    // Make a copy and find the relevant characters.
    strcpy (retStr, myStr);
    lastExt = strrchr (retStr, extSep);
    lastPath = (pathSep == 0) ? NULL : strrchr (retStr, pathSep);

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

    // Return the modified string.
    return retStr;
}