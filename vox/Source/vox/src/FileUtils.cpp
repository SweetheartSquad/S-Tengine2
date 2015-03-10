#include "FileUtils.h"

std::string FileUtils::voxReadFile(std::string _filename){
	std::ifstream file(_filename, std::ios::in);
	std::stringstream contents;

	if(file.is_open()){
		std::cout << "File \"" << _filename << "\" opened for reading." << std::endl;
		contents << file.rdbuf();
		file.close();
		std::cout << "File \"" << _filename << "\" read." << std::endl;
	}else{
		contents << "\tERROR: file \"" << _filename << "\" could not be opened for reading." << std::endl;
	}
	//std::cout << contents.str();

	return contents.str();
}