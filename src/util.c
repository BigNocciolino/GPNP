#include "util.h"
#include <stdint.h>
#include <stdio.h>
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