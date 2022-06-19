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
        save_lat(&data->lat, elements[2]);
        data->lat.direction = *elements[3];
        save_lng(&data->lng, elements[4]);
        data->lng.direction = *elements[5];
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
    char *ptr;
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

// TODO parametrize the size
char *serialize_coords(int deg, int minutes) {
    char *buf = malloc(sizeof(char) * 20);
    // Make the number positive if is N or E
    sprintf(buf, "%d.%d", deg, minutes);
    return buf;
}

void save_lat(Latitude *lat, char *data) {
    int n;
    char buf[20] = {0};
    // remove starting 0
    n = strspn(data, "0");
    if(data[n] != '\0' ) {
        // Get the first 2 chars, they are degrees
        strncpy(buf, &data[n], 2);
        lat->degrees = atoi(buf);
        memset(buf, 0, sizeof(buf));
        // Get all the remaning chars, they also include the dot
        strncpy(buf, &data[n]+2, sizeof(buf)-2);
        removeChar(buf, '.');
        lat->minutes = atoi(buf);
    }
}

void save_lng(Longitude *lng, char *data) {
    int n;
    char buf[20] = {0};
    // remove starting 0
    n = strspn(data, "0");
    if(data[n] != '\0' ) {
        strncpy(buf, &data[n], 2);
        lng->degrees = atoi(buf);
        memset(buf, 0, sizeof(buf));
        strncpy(buf, &data[n]+2, sizeof(buf)-2);
        removeChar(buf, '.');
        lng->minutes = atoi(buf);
    }
}