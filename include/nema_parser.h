#ifndef NEMA_PARSER_H
#define NEMA_PARSER_H

#include <stdint.h>

#define LATITUDE 0
#define LONGITUDE 1

#define NEMA_IDS 5
// This include $ char
#define NEMA_ID_LEN 6
// The max leght for each nema element in a senctence
#define MAX_ELEMENT_LEN 20
// This include the $ char and the newline char
#define SENT_MAX_LEN 82
// Number of elements in a GPRMC sentece
//static const char *NEMA[NEMA_IDS] = {"$GPRMC", "$GPVTG", "$GPGGA", "$GPGSV", "$GPGLL"};

// GPRMC
#define GPRMC_ELEMENTS 13
// GPGGA
#define GPGGA_ELEMENTS 15
// GPVTG
#define GPVTG_ELEMENTS 10

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

struct Magnetic_Variation {
    float variation;
    char direction;
};
typedef struct Magnetic_Variation Magnetic_Variation;

// Recommended Minimum sentence C
struct GPRMC {
    struct Time time;
    char status;
    float latitude;
    float longitude;
    // The speed is stored in knots
    float speed;
    float track_angle;
    struct Date date;
    Magnetic_Variation magnetic_variation;
    char postion_mode;
    char checksum[2];
};
typedef struct GPRMC GPRMC;

// Global Positioning System Fix Data
struct GPGGA {
    struct Time time;
    float latitude;
    float longitude;
    int fix_quality;
    int satellites;
    float horizontal_dilution;
    // The altitude is stored in meters
    float altitude;
    float height_of_geoid;
    // Last update is in seconds
    float last_update;
    int DGPS_id;
    char checksum[2];
};
typedef struct GPGGA GPGGA;

struct GPVTG {
    float track_deg_true;
    float track_deg_magnetic;
    float speed_knots;
    float speed_kph;
    char indicator;
    char checksum[2];
};
typedef struct GPVTG GPVTG;

void parse_rmc(GPRMC *data, char sentence[]);
void parse_gga(GPGGA *data, char sentence[]);
void parse_vtg(GPVTG *data, char sentence[]);

// Utils
void format_time(struct Time *t, char time[]);
void format_date(struct Date *d, char date[]);

void save_coor(float *nema_coor, char *data, char direction, int type);

#endif