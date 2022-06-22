#include "nema_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/*

https://stackoverflow.com/a/1095006

*/

// Parse the bare minimum to obtain all data
void parse_rmc(GPRMC *data, char sentence[]) {
    // This keeps track of in witch element of the sentence we are
    int id = 1;
    size_t len = strlen(sentence);
    char buf[MAX_ELEMENT_LEN+1];
    char *elements[GPRMC_ELEMENTS-1];

    if (len <= SENT_MAX_LEN) {
        // We need to skip the first 6+1 for the comma chars (they are the nema id) 
        for (size_t i=7, start = i; i<len; i++) {
            if (sentence[i] == ',' || (id == GPRMC_ELEMENTS-1 && len-i == 1)) {
                // allocate the memory to store the string
                memset(buf, 0, sizeof(buf));
                strncpy(buf, sentence+start, i-start);
                elements[id-1] = malloc(strlen(buf));
                strcpy(elements[id-1], buf);
                // strcpy(elements[id-1]+(sizeof(buf)), "\0");
               id++;
               start = i+1;
            }
        }
        // Assign the element into the array to the struct
        format_time(&data->time, elements[0]);
        data->status = *elements[1];
        save_coor(&data->latitude, elements[2], *elements[3], LATITUDE);
        save_coor(&data->longitude, elements[4], *elements[5], LONGITUDE);
        data->speed = atof(elements[6]);
        data->track_angle = atof(elements[7]);
        format_date(&data->date, elements[8]);
        data->magnetic_variation.variation = atof(elements[9]);
        data->magnetic_variation.direction = *elements[10];
        data->postion_mode = *elements[11];
        strncpy(data->checksum, elements[11]+2, 2);
    }
    // Free memory
    for (int i=0; i<id-1; i++) {
        free(elements[i]);
    }
}

void parse_gga(GPGGA *data, char sentence[]) {
    int id = 1;
    size_t len = strlen(sentence);
    char buf[MAX_ELEMENT_LEN+1];
    char *elements[GPGGA_ELEMENTS-1];

    if (len <= SENT_MAX_LEN) {
        for (size_t i=7, start = i; i<len; i++) {
            if (sentence[i] == ',' || (id == GPGGA_ELEMENTS-1 && len-i == 1)) {
                // allocate the memory to store the string
                memset(buf, 0, sizeof(buf));
                strncpy(buf, sentence+start, i-start);
                elements[id-1] = malloc(strlen(buf));
                strcpy(elements[id-1], buf);
                // strcpy(elements[id-1]+(sizeof(buf)), "\0");
               id++;
               start = i+1;
            }
        }
            
        format_time(&data->time, elements[0]);
        save_coor(&data->latitude, elements[1], *elements[2], LATITUDE);
        save_coor(&data->longitude, elements[3], *elements[4], LONGITUDE);
        data->fix_quality = atoi(elements[5]);
        data->satellites = atoi(elements[6]);
        data->horizontal_dilution = atof(elements[7]);
        data->altitude = atof(elements[8]);
        //Skip one element, it is the M fixed char for altitude in meters
        data->height_of_geoid = atof(elements[10]);
        data->last_update = atof(elements[11]);
        data->DGPS_id = atoi(elements[12]);
        strncpy(data->checksum, elements[13]+1, 2);
    }
    for (int i=0; i<id-1; i++) {
        free(elements[i]);
    }
}

void parse_vtg(GPVTG *data, char sentence[]) {
    int id = 1;
    size_t len = strlen(sentence);
    char buf[MAX_ELEMENT_LEN+1];
    char *elements[GPVTG_ELEMENTS-1];

    if (len <= SENT_MAX_LEN) {
        for (size_t i=7, start = i; i<len; i++) {
            if (sentence[i] == ',' || (id == GPVTG_ELEMENTS-1 && len-i == 1)) {
                // allocate the memory to store the string
                memset(buf, 0, sizeof(buf));
                strncpy(buf, sentence+start, i-start);
                elements[id-1] = malloc(strlen(buf));
                strcpy(elements[id-1], buf);
                // strcpy(elements[id-1]+(sizeof(buf)), "\0");
               id++;
               start = i+1;
            }
        }
        
        printf("%s\n", sentence);
        for (int j=0; j<id-1; j++) {
            printf("%s,", elements[j]);
        }
        printf(" ]\n");

        data->track_deg_true = atof(elements[0]);
        // Skip T char
        data->track_deg_magnetic = atof(elements[2]);
        // Skip M char
        data->speed_knots = atof(elements[4]);
        // Skip N char
        data->speed_kph = atof(elements[6]);
        // Skip k char
        data->indicator = *elements[8];
        strncpy(data->checksum, elements[8]+1, 2);
    }
    for (int i=0; i<id-1; i++) {
        free(elements[i]);
    } 
}

void parse_gll(GPGLL *data, char sentence[]) {
    int id = 1;
    size_t len = strlen(sentence);
    char buf[MAX_ELEMENT_LEN+1];
    char *elements[GPGLL_ELEMENTS-1];

    if (len <= SENT_MAX_LEN) {
        for (size_t i=7, start = i; i<len; i++) {
            if (sentence[i] == ',' || (id == GPGLL_ELEMENTS-1 && len-i == 1)) {
                // allocate the memory to store the string
                memset(buf, 0, sizeof(buf));
                strncpy(buf, sentence+start, i-start);
                elements[id-1] = malloc(strlen(buf));
                strcpy(elements[id-1], buf);
                // strcpy(elements[id-1]+(sizeof(buf)), "\0");
               id++;
               start = i+1;
            }
        }

        printf("%s\n", sentence);
        for (int j=0; j<id-1; j++) {
            printf("%s,", elements[j]);
        }
        printf(" ]\n");

        save_coor(&data->latitude, elements[0], *elements[1], LATITUDE);
        save_coor(&data->longitude, elements[2], *elements[3], LONGITUDE);
        format_time(&data->time, elements[4]);
        data->status = *elements[5];
        strncpy(data->checksum, elements[6]+2, 2);
    }
}

// Utils
void format_time(struct Time *t, char time[]) {
    char buf[2];
    if (strlen(time) >= 6) {
        // We tecnically have all the arguments 
        // Hour
        strncpy(buf, time, 2);
        t->hour = atoi(buf);
        memset(buf, 0, sizeof(buf));
        strncpy(buf, time+2, 2);
        t->minutes = atoi(buf);
        memset(buf, 0, sizeof(buf));
        strncpy(buf, time+4, 2);
        t->seconds = atoi(buf);
    }
}

void format_date(struct Date *d, char date[]) {
    char buf[2];
    if (strlen(date) == 6) {
        // We tecnically have all the arguments 
        // Hour
        strncpy(buf, date, 2);
        d->day = atoi(buf);
        memset(buf, 0, sizeof(buf));
        strncpy(buf, date+2, 2);
        d->month = atoi(buf);
        memset(buf, 0, sizeof(buf));
        strncpy(buf, date+4, 2);
        d->year = atoi(buf);
    }
}


// both function are incorrect, look at https://it.wikipedia.org/wiki/WGS84
void save_coor(float *nema_coor, char *data, char direction, int type) {
    float l, ss;
    int dd;
    l = atof(data);

    // DD + SS /60
    // DD = int(float(l)/100)
    // SS = float(l) - DD *100
    // lat.value = DD + SS/60

    dd = (int) l / 100;
    ss = (l - dd*100);
    *nema_coor = dd + (ss/60);    
    if (type == LATITUDE) {
        if (direction == 'S') {
            *nema_coor *= -1;
        }
    }else if (type == LONGITUDE) {
        if (direction == 'W') {
            *nema_coor *= -1;
        }
    }
}
