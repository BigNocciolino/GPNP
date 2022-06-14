#ifndef NEMA_PARSER_H
#define NEMA_PARSER_H

#define NEMA_IDS 5
// This include $ char
#define NEMA_ID_LEN 7
// The max leght for each nema element in a senctence
#define MAX_ELEMENT_LEN 10
// This include the $ char and the newline char
#define SENT_MAX_LEN 81
// Number of elements in a GPRMC sentece
#define GPRMC_ELEMENTS 12
// TODO mabye try to use enumerations
static const char *NEMA[NEMA_IDS] = {"$GPRMC", "$GPVTG", "$GPGGA", "$GPGSV", "$GPGLL"};

struct Latitude {
    float latitude;
    char direction;
};

struct Longitude {
    float longitude;
    char direction;
};

struct Magnetic_Variation {
    float variation;
    char direction;
};

struct GPRMC
{
    char time[6];
    char status;
    struct Latitude lat;
    struct Longitude lng;
    // The speed is stored in knots
    float speed;
    float track_angle;
    char date[6];
    struct Magnetic_Variation magnetic_variation;
    char postion_mode;
    char checksum[2];
};


void parse_rmc(struct GPRMC *data, char sentence[]);
    
#endif