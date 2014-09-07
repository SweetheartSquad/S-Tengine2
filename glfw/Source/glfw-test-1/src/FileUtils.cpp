#include "FileUtils.h"

std::string FileUtils::voxReadFile(std::string filename){
	std::ifstream file(filename, std::ios::in);
	std::stringstream contents;

	if(file.is_open()){
		std::cout << "File \"" << filename << "\" opened for reading." << std::endl;
		contents << file.rdbuf();
		file.close();
		std::cout << "File \"" << filename << "\" read." << std::endl;
	}else{
		contents << "\tERROR: file \"" << filename << "\" could not be opened for reading." << std::endl;
	}
	std::cout << contents;

	return contents.str();
}