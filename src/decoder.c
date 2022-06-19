#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mxml.h>

#include "nema_parser.h"
#include "utils.h"


int main(int argc, char **argv) {

    char *file_path = NULL;
    char *xml_path = NULL;
    FILE *nema_file, *xml_file;
    GPRMC rmc;

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
        printf("Culd not open file: %s\n", file_path);
        printf("ERROR: %s\n", strerror(errno));
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
                parse_rmc(&rmc, sentence);
                char *iso_timestamp = convert_to_ISO(&rmc.date, &rmc.time);
                char *ser_coords = serialize_coords(rmc.lat.degrees, rmc.lat.minutes);
                trkpt = mxmlNewElement(trkseg, "trkpt");
                memset(element, 0, sizeof(element));
                sprintf(element, "%s", ser_coords);
                mxmlElementSetAttr(trkpt, "lat", element);
                memset(element, 0, sizeof(element));
                ser_coords = serialize_coords(rmc.lng.degrees, rmc.lng.minutes);
                sprintf(element, "%s", ser_coords);
                mxmlElementSetAttr(trkpt, "lon", element);
                time = mxmlNewElement(trkseg, "time");
                mxmlNewText(time, 0, iso_timestamp);
                free(iso_timestamp);
                free(ser_coords);
            }
        } 
    } 

    xml_file = fopen(xml_path, "w");
    if (xml_file) {
        mxmlSaveFile(xml, xml_file, MXML_NO_CALLBACK);
        fclose(xml_file);
    }else {
        printf("Could not open gpx file %s\n", xml_path);
        printf("ERROR: %s\n", strerror(errno));
    }

    fclose(nema_file);

    return 0;
}