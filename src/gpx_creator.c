#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <mxml.h>
#include "util.h"
#include "gpx_creator.h"

#include <nmea.h>
#include <nmea/gprmc.h>
#include <nmea/gpgga.h>

// This function returns 1 if the input file can not be opened
// -1 if the output file can not be openend
// 0 if everything ges correctoly
int write_to_file(char *nema_file_path, char *gpx_file_path) {
    FILE *nema_file;
    FILE *gpx_file;
    
    nmea_s *data;

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
        return 1;
    } 

    // Read the content
    char sentence[NMEA_MAX_LENGTH] = {0};
    char timestamp[35] = {0};

    while(fgets(sentence, sizeof(sentence), nema_file) != NULL) {
        data = nmea_parse(sentence, strlen(sentence), 0, 0);
        if (data != NULL) {
            if (NMEA_GPRMC == data->type) {
                nmea_gprmc_s *rmc = (nmea_gprmc_s *) data;
                // RMC
                trkpt = mxmlNewElement(trkseg, "trkpt");
                // Set all the attribute for trkpt
                mxmlElementSetAttrf(trkpt, "lat", "%f", rmc->latitude); 
                mxmlElementSetAttrf(trkpt, "lon", "%f", rmc->longitude);
                // Set time element
                time = mxmlNewElement(trkpt, "time");
                sprintf(timestamp, "%d-%d-%dT%d:%d:%d+00Z", rmc->date_time.tm_year, rmc->date_time.tm_mon, rmc->date_time.tm_mday,
                                                            rmc->date_time.tm_hour, rmc->date_time.tm_min, rmc->date_time.tm_sec);
                mxmlNewText(time, 0, timestamp);
                // Store the speed
                speed = mxmlNewElement(trkpt, "speed");
                mxmlNewTextf(speed, 0, "%f", kn_to_ms(rmc->gndspd_knots));
            }
            if (NMEA_GPGGA == data->type) { 
                nmea_gpgga_s *gga = (nmea_gpgga_s *) data;
                // GGA
                elevation = mxmlNewElement(trkpt, "ele");
                mxmlNewTextf(elevation, 0, "%f",gga->altitude);

                satellites = mxmlNewElement(trkpt, "sat");
                mxmlNewTextf(satellites, 0, "%d", gga->n_satellites);
            }
        }else {
            printf("Failed to parse sentence\n");
        }
    }

    gpx_file = fopen(gpx_file_path, "w");
    if (gpx_file) {
        mxmlSaveFile(xml, gpx_file, MXML_NO_CALLBACK);
        fclose(gpx_file);
        mxmlDelete(xml);
    }else {
        printf("Could not open gpx file %s\n", gpx_file_path);
        printf("ERROR: %s\n", strerror(errno));
        mxmlDelete(xml);
        nmea_free(data);
        fclose(nema_file);
        return -1;
    } 

    nmea_free(data);
    fclose(nema_file);
    return 0;
}