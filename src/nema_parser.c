#include "nema_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

https://stackoverflow.com/a/1095006

*/

// Parse the bare minimum to obtain all data
// FIXME it not work with consecutive ,
// This not take the last value beacouse to take the value it needs to find the other ",". 
// So I need to find a way to take before
void parse_rmc(struct GPRMC *data, char sentence[]) {
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
        strncpy(data->time, elements[0], 6);
        data->status = *elements[1];
        data->lat.latitude = atof(elements[2]);
        data->lat.direction = *elements[3];
        data->lng.longitude = atof(elements[4]);
        data->lng.direction = *elements[5];
        data->speed = atof(elements[6]);
        data->track_angle = atof(elements[7]);
        strncpy(data->date, elements[8], 6);
        data->magnetic_variation.variation = atof(elements[9]);
        data->magnetic_variation.direction = *elements[10];
        data->postion_mode = *elements[11];
        strncpy(data->checksum, elements[11]+2, 2);
    }
    //free(elements);
}