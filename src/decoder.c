#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "gpx_creator.h"

void print_help();

int main(int argc, char **argv) {

    char *file_path = NULL;
    char *xml_path = "out.gpx";
    
    if (argc > 1) {
        for (int i=1; i<argc; i++) {
            if (!strcmp(argv[i], "-i")) {
                file_path = argv[++i];
            }else if (!strcmp(argv[i], "-o")) {
                xml_path = argv[++i];
            }else {
                print_help();
            }
        }
    }else {
        print_help();
        exit(1);
    }

    if (write_to_file(file_path, xml_path) != 0) {
        exit(1);
    }

    return 0;
}

void print_help() {
    printf("Usage:\n./decoder -i [file.txt] -o [out.gpx]\n");
}