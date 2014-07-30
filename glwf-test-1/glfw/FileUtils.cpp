#include "FileUtils.h"

char*FileUtils::voxReadFile(const char* filename){
	//Open file
	FILE* fp = fopen(filename, READ);
	//Move the file pointer to the end of the file ad determine the length
	fseek(fp, 0, SEEK_END);
	long fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* contents = new char[fileLength+1];
	//zero out memory
	for(int i = 0; i < fileLength+1; i++){
		contents[i];
	}
	//Here we read the file
	fread(contents, 1, fileLength, fp);
	//Denote the end of the string
	contents[fileLength-6] = '\0';
	fclose(fp);

	std::cout << contents << std::endl;
	return contents;
}