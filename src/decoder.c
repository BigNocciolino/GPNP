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
    FILE *nema_file, *xml_file;
    struct GPRMC rmc;

    // XMl nodes
    mxml_node_t *xml;
    mxml_node_t *gpx;
    mxml_node_t *trk;
    mxml_node_t *trkseg;
    mxml_node_t *trkpt;
    mxml_node_t *time;

    xml = mxmlNewXML("1.0");
    gpx = mxmlNewElement(xml, "gpx");
        trk = mxmlNewElement(gpx, "trk");
            trkseg = mxmlNewElement(trk, "trkseg");

    
    if (argc > 2) {
       file_path = argv[1];
       xml_path = argv[2];
    }else {
        printf("Usage:\n./decoder [file.txt] [out.gpx]\n");
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
    char element[MAX_ELEMENT_LEN];

    while(fgets(sentence, sizeof(sentence), nema_file) != NULL) {
        if (sentence[0] == '$') {
            // This is a NEMA sentece
            strncpy(sent_id, sentence, 6);
            if (strcmp(sent_id, "$GPRMC") == 0) {
                // No newline char sience the sentece have it
                parse_rmc(&rmc, sentence);
                trkpt = mxmlNewElement(trkseg, "trkpt");
                memset(element, 0, sizeof(element));
                sprintf(element, "%f", rmc.lat.latitude);
                mxmlElementSetAttr(trkpt, "lat", element);
                memset(element, 0, sizeof(element));
                sprintf(element, "%f", rmc.lng.longitude);
                mxmlElementSetAttr(trkpt, "lon", element);
                time = mxmlNewElement(trkseg, "time");
                mxmlNewText(time, 0, rmc.time);
            }
        } 
    } 

    xml_file = fopen(xml_path, "w");
    if (!xml_file) {
        printf("Could not open gpx file %s\n", xml_path);
        // mabye exit
    }else {
        mxmlSaveFile(xml, xml_file, MXML_NO_CALLBACK);
        fclose(xml_file);
    }

    fclose(nema_file);

    return 0;
}