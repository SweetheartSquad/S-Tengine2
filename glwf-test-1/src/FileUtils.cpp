#include "FileUtils.h"

std::string FileUtils::voxReadFile(const char* filename){
	std::ifstream file;
	std::string contents = "";

	//Open file
	file.open(filename, std::ios::in);
	if(file.is_open()){
		std::cout << "File \"" << filename << "\" opened for reading." << std::endl;
		std::string line;
		while ( std::getline (file, line) ){
			std::cout << line << '\n';
			contents.append(line);
		}
		file.close();
	}else{
		std::cout << "ERROR: file \"" << filename << "\" could not be opened for reading." << std::endl;
	}

	return contents;
	
	//Move the file pointer to the end of the file ad determine the length
	/*fseek(fp, 0, SEEK_END);
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
	return contents;*/
}