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

class Animation;
class Tween;

class SkeletonData{
public:
	static void SaveSkeleton(std::string directory, std::string fileName, std::vector<Joint *> &m_joints);
	static std::vector<Joint *> LoadSkeleton(std::string filePath);

	static void validateDirectory(std::string & directory);
	static void validateFileName(std::string & fileName);

protected:
	static std::string writeJoint(Joint * j, unsigned int indent = 0);
	static Joint * readJoint(JsonTree joint, Joint * parent = nullptr);

	static std::string writeAnimations(Joint * j, unsigned int indent = 0);
	static void readAnimations(JsonTree animations, Joint * j);

	static std::string writeAnimation(Animation * a, std::string name, unsigned int indent = 0);
	static void readAnimation(JsonTree animation, Animation * a);

	static std::string writeTween(Tween * t, int id, unsigned int indent = 0);
	static Tween * readTween(JsonTree tween);
};

