#include "FileUtils.h"

std::string FileUtils::voxReadFile(std::string _filename){
	std::ifstream file(_filename, std::ios::in);
	std::stringstream contents;

	if(file.is_open()){
#ifdef _DEBUG
		std::cout << "File \"" << _filename << "\" opened for reading." << std::endl;
#endif
		contents << file.rdbuf();
		file.close();
#ifdef _DEBUG
		std::cout << "File \"" << _filename << "\" read." << std::endl;
#endif
	}else{
		contents << "\tERROR: file \"" << _filename << "\" could not be opened for reading." << std::endl;
	}

	return contents.str();
}