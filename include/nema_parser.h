#ifndef NEMA_PARSER_H
#define NEMA_PARSER_H

#include <stdint.h>

#define NEMA_IDS 5
// This include $ char
#define NEMA_ID_LEN 7
// The max leght for each nema element in a senctence
#define MAX_ELEMENT_LEN 20
// This include the $ char and the newline char
#define SENT_MAX_LEN 81
// Number of elements in a GPRMC sentece
#define GPRMC_ELEMENTS 12
// TODO mabye try to use enumerations
static const char *NEMA[NEMA_IDS] = {"$GPRMC", "$GPVTG", "$GPGGA", "$GPGSV", "$GPGLL"};

struct Time {
   int8_t hour;
   int8_t minutes;
   int8_t seconds; 
};

struct Date
{
    int8_t day;
    int8_t month;
    int8_t year;
};

struct Latitude {
    int degrees;
    int minutes;
    char direction;
};
typedef struct Latitude Latitude;

struct Longitude {
    int degrees;
    int minutes;
    char direction;
};
typedef struct Longitude Longitude;

struct Magnetic_Variation {
    float variation;
    char direction;
};
typedef struct Magnetic_Variation Magnetic_Variation;

struct GPRMC {
    struct Time time;
    char status;
    Latitude lat;
    Longitude lng;
    // The speed is stored in knots
    float speed;
    float track_angle;
    struct Date date;
    Magnetic_Variation magnetic_variation;
    char postion_mode;
    char checksum[2];
};
typedef struct GPRMC GPRMC;


void parse_rmc(struct GPRMC *data, char sentence[]);

// Utils
void format_time(struct Time *t, char time[]);
void format_date(struct Date *d, char date[]);
char *serialize_coords(int deg, int minutes);

void save_lat(Latitude *lat, char *data);
void save_lng(Longitude *lng, char *data);

#endif