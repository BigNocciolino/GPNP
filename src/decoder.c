#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "gpx_creator.h"
#include "util.h"

void print_help();
void addLastSeparator(char *str);

int main(int argc, char **argv) {

    char *file_path = NULL;
    char *xml_path = NULL;
    
    if (argc > 1) {
        for (int i=1; i<argc; i++) {
            if (!strcmp(argv[i], "-i")) {
                if (argv[++i] == NULL) return 1;
                file_path = (char *)malloc(strlen(argv[i])+1);
                strcpy(file_path, argv[i]);
            }else if (!strcmp(argv[i], "-o")) {
                if (argv[++i] == NULL) return 1;
                xml_path = (char *)malloc(strlen(argv[i])+1);
                strcpy(xml_path, argv[i]);
            }else {
                print_help();
            }
        }
    }else {
        print_help();
        exit(1);
    }

    if (xml_path == NULL) {
        xml_path = (char *)malloc(sizeof(file_path)+1);
        strcpy(xml_path, file_path);
    }

    // Check if the input string is a path
    if (isDirectory(file_path)) {
        addLastSeparator(file_path);
        // Check if the output string is a path
        if (!isDirectory(xml_path)) {
            if (mkdir(xml_path, 0700) && errno != EEXIST) {
                printf("Error creating output dir: %s\n", xml_path); 
                exit(1);
            }
            printf("Created output dir: %s\n", xml_path); 
            addLastSeparator(xml_path);
        }else {
            addLastSeparator(xml_path);
        }
        DIR *directory;
        struct dirent *entry;

        directory = opendir(file_path);
        if (directory == NULL) {
            printf("Can not open dir: %s\n", xml_path);
            exit(1);
        }

        while ( (entry = readdir(directory)) ) {
            if (!(entry->d_type == DT_DIR)) {
                // Build the actual file path
                char *full_in_path = malloc(strlen(file_path) + strlen(entry->d_name) + 2);
                sprintf(full_in_path, "%s%s", file_path, entry->d_name);
                printf("Parsing input file: %s\n", full_in_path);
                // Build the output file path
                char *full_out_path = malloc(strlen(xml_path) + strlen(entry->d_name) +2 + 5);
                sprintf(full_out_path, "%s%s", xml_path, entry->d_name);
                change_ext(full_out_path, "gpx");
                printf("Outputting in file: %s\n", full_out_path);
                if (write_to_file(full_in_path, full_out_path) != 0) {
                    free(full_in_path);
                    free(full_out_path);
                    closedir(directory);
                    exit(1);
                }
                free(full_in_path);
                free(full_out_path);
            }
        }
        closedir(directory);
    }else {
        if (write_to_file(file_path, xml_path) != 0) {
            printf("ERROR\n");
        }
    }

    free(file_path);
    free(xml_path);
    return 0;
}

void print_help() {
    printf("Usage:\n./decoder -i [file.txt] -o [out.gpx]\n");
}

// add the separator at the end of a path string
void addLastSeparator(char *str) {
    if (str[strlen(str) - 1] != '/') {
        str = (char *)realloc(str, (sizeof(str) + 1));
        // file_path[strlen(file_path) + 1] = "/";
        strncpy((str+strlen(str)), "/", sizeof(str));
    }
    return;
}