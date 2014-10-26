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
	static void SaveSkeleton(std::string directory, std::string fileName, std::vector<Joint *> &m_joints);
	static std::vector<Joint *> LoadSkeleton(std::string filePath);

	static void validateDirectory(std::string & directory);
	static void validateFileName(std::string & fileName);

protected:
	static std::string writeJoint(Joint * b);
	static Joint * readJoint(JsonTree joint, Joint * parent = nullptr);
};

