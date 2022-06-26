#include "nema_parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void write_to_file(char *nema_file_path, char *gpx_file_path) {
    FILE *nema_file;
    // take most of the data 
    GPRMC rmc;
    // Take the satellite number
    GPGGA gga;

    nema_file = fopen(nema_file_path, "r");
    if (!nema_file) {
        printf("Culd not open file: %s\n", nema_file_path);
        printf("ERROR: %s\n", strerror(errno));
        exit(1);
    } 

    // Read the content
    char sentence[SENT_MAX_LEN];
    char sent_id[NEMA_ID_LEN+1];
    char element[MAX_ELEMENT_LEN];

    while(fgets(sentence, sizeof(sentence), nema_file) != NULL) {
        // Check if is a valid nema sentence
        if (sentence[0] == '$') {
            // claer the current buffer 
            memset(sent_id, 0, sizeof(sent_id)); 
            // get the id of the sentence
            strncpy(sent_id, sentence, 6);
            // Find if the is the correct one
            if (strcmp(sent_id, "$GPRMC") == 0) {
                parse_rmc(&rmc, sentence);
            }
        }
    }

    fclose(nema_file);
}