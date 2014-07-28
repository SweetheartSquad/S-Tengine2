#include "FileUtils.h"

char*FileUtils::voxReadFile(const char* file){
	
	FILE *currentFile = fopen(file, READ);
    fseek(currentFile, 0, SEEK_END);
    int count = (int)ftell(currentFile);
    
    rewind(currentFile);
    char *data = (char *)malloc(sizeof(char)*(count +1));
    count = (int)fread(data, sizeof(char), count, currentFile);
    data[count] = '\0';
    
    fclose(currentFile);
    
    return data;
}