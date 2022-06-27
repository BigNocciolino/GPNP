#ifndef GPX_CREATOR_H
#define GPX_CREATOR_H
    
// This function returns 1 if the input file can not be opened
// -1 if the output file can not be openend
// 0 if everything ges correctoly
int write_to_file(char *nema_file_path, char *gpx_file_path);

#endif