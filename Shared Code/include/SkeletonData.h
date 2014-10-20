#pragma once
#pragma comment(lib, "Shlwapi.lib")

#include "shlwapi.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <regex>

#include <cinder/app/App.h>
#include <cinder/Json.h>

#include "Joint.h"

using namespace ci;

class SkeletonData{
public:
	SkeletonData(void);
	
	void SaveSkeleton(std::string directory, std::string fileName, std::vector<Joint *> &m_joints);
	std::vector<Joint *> LoadSkeleton(std::string filePath);

	void validateDirectory(std::string & directory);
	void validateFileName(std::string & fileName);

	~SkeletonData(void);

protected:
	std::string writeJoint(Joint * b);
	Joint* readJoint(JsonTree joint, Joint * parent = nullptr);
};

