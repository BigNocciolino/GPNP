#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mxml.h>

#include "nema_parser.h"
#include "utils.h"
#include "gpx_creator.h"

int main(int argc, char **argv) {

    char *file_path = NULL;
    char *xml_path = NULL;
    
    if (argc > 2) {
       file_path = argv[1];
       xml_path = argv[2];
    }else {
        printf("Usage:\n./decoder [file.txt] [out.gpx]\n");
        exit(1);
    }

    if (write_to_file(file_path, xml_path) != 0) {
        exit(1);
    }

    return 0;
}