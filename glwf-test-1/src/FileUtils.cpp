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
		contents = "invalid file";
	}

	return contents;
}