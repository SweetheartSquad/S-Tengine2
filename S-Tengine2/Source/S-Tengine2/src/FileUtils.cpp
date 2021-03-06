#pragma once

#include <FileUtils.h>
#include <Log.h>

std::string sweet::FileUtils::readFile(const std::string & _filename){
	std::ifstream file(_filename, std::ios::in);
	std::stringstream contents;

	if(file.is_open()){
		Log::info("File \"" + _filename + "\" opened for reading.");
		contents << file.rdbuf();
		file.close();
		Log::info("File \"" + _filename + "\" read.");
	}else{
		Log::error("File \"" + _filename + "\" could not be opened for reading.");
	}

	return contents.str();
}

bool sweet::FileUtils::createDirectoryIfNotExists(const std::string & _src){
	return CreateDirectoryA(_src.c_str(), NULL) != ERROR_PATH_NOT_FOUND;
}

bool sweet::FileUtils::createFileIfNotExists(const std::string & _src){
	if (fileExists(_src)){
		// File already exists
		return true;
	}
	std::ofstream file(_src);
	if (!file){
		Log::error("File \"" + _src + "\" could not be created.");
		return false;
	}
	// File was created
	return true;
}

bool sweet::FileUtils::fileExists(const std::string & _src){
	if (std::ifstream(_src)){
		return true;
	}else{
		return false;
	}
}