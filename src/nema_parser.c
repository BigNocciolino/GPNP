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
    char *elements[GPRMC_ELEMENTS];

    if (len <= SENT_MAX_LEN) {
        // We need to skip the first 6+1 for the comma chars (they are the nema id) 
        for (size_t i=7, start = i; i<len; i++) {
            if (sentence[i] == ',' || (id == GPRMC_ELEMENTS && len-i == 1)) {
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
