#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mxml.h>

#include "nema_parser.h"

void print_rmc(struct GPRMC *rmc) {
    printf("Time: %s\n", rmc->time);
    printf("Status: %c\n", rmc->status);
    printf("Latitude: %5.4f,%c\n", rmc->lat.latitude, rmc->lat.direction);
    printf("Longitude: %5.4f,%c\n", rmc->lng.longitude, rmc->lng.direction);
    printf("Speed: %3.2f\n", rmc->speed);
    printf("Data: %s\n", rmc->date);
    printf("Mode: %c\n", rmc->postion_mode);
    printf("Checksum: %s\n", rmc->checksum);
    return;
}

int main(int argc, char **argv) {

    char *file_path = NULL;
    char *xml_path = NULL;
    FILE *nema_file;
    struct GPRMC rmc;
    
    if (argc > 1) {
       file_path = argv[1];
    }else {
        printf("Usage:\n./decoder [file]\n");
        exit(1);
    }

    nema_file = fopen(file_path, "r");
    if (!nema_file) {
        printf("Culd not open file %s\n", file_path);
        exit(1);
    }

    // Read the content
    char sentence[SENT_MAX_LEN];
    char sent_id[NEMA_ID_LEN];

    while(fgets(sentence, sizeof(sentence), nema_file) != NULL) {
        if (sentence[0] == '$') {
            // This is a NEMA sentece
            strncpy(sent_id, sentence, 6);
            if (strcmp(sent_id, "$GPRMC") == 0) {
                // No newline char sience the sentece have it
                parse_rmc(&rmc, sentence);
                print_rmc(&rmc);
            }
        } 
    } 

    fclose(nema_file);

    return 0;
}