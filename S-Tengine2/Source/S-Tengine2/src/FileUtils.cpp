#pragma once

#include <FileUtils.h>
#include <Log.h>

std::string FileUtils::voxReadFile(std::string _filename){
	std::ifstream file(_filename, std::ios::in);
	std::stringstream contents;

	if(file.is_open()){
		Log::info("File \"" + _filename + "\" opened for reading.");
		contents << file.rdbuf();
		file.close();
		Log::info("File \"" + _filename + "\" read.");
	}else{
		contents << "\tERROR: file \"" << _filename << "\" could not be opened for reading." << std::endl;
	}

	return contents.str();
}