#include "nema_parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <mxml.h>
#include "utils.h"
#include "gpx_creator.h"

void write_to_file(char *nema_file_path, char *gpx_file_path) {
    FILE *nema_file;
    FILE *gpx_file;
    // take most of the data 
    GPRMC rmc;
    // Take the satellite number
    GPGGA gga;

    mxml_node_t *xml;
    mxml_node_t *gpx;
    mxml_node_t *trk;
    mxml_node_t *trkseg;
    mxml_node_t *trkpt;
    mxml_node_t *time;
    mxml_node_t *speed;
    mxml_node_t *elevation;
    mxml_node_t *satellites;

    // start the build of the gpx file
    xml = mxmlNewXML("1.0");
    gpx = mxmlNewElement(xml, "gpx");
    trk = mxmlNewElement(gpx, "trk");
    trkseg = mxmlNewElement(trk, "trkseg");

    nema_file = fopen(nema_file_path, "r");
    if (!nema_file) {
        printf("Culd not open file: %s\n", nema_file_path);
        printf("ERROR: %s\n", strerror(errno));
        exit(1);
    } 

    // Read the content
    char sentence[SENT_MAX_LEN];
    char sent_id[NEMA_ID_LEN+1];

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
                
                trkpt = mxmlNewElement(trkseg, "trkpt");
                // Set all the attribute for trkpt
                mxmlElementSetAttrf(trkpt, "lat", "%f", rmc.latitude); 
                mxmlElementSetAttrf(trkpt, "lon", "%f", rmc.longitude);
                // Set time element
                time = mxmlNewElement(trkpt, "time");
                char *iso_timestamp = convert_to_ISO(&rmc.date, &rmc.time);
                mxmlNewText(time, 0, iso_timestamp);
                // cler the array dinamically allocated
                free(iso_timestamp);
                // Store the speed
                speed = mxmlNewElement(trkpt, "speed");
                mxmlNewTextf(speed, 0, "%f", rmc.speed);
            }
            if (strcmp(sent_id, "$GPGGA") == 0) {
                parse_gga(&gga, sentence);

                elevation = mxmlNewElement(trkpt, "ele");
                mxmlNewTextf(elevation, 0, "%f", gga.altitude);

                satellites = mxmlNewElement(trkpt, "sat");
                mxmlNewTextf(satellites, 0, "%d", gga.satellites);
            }
        }
    }

    gpx_file = fopen(gpx_file_path, "w");
    if (gpx_file) {
        mxmlSaveFile(xml, gpx_file, MXML_NO_CALLBACK);
        fclose(gpx_file);
    }else {
        printf("Could not open gpx file %s\n", gpx_file_path);
        printf("ERROR: %s\n", strerror(errno));
    } 

    fclose(nema_file);
}