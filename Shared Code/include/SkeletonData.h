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
#include "Animation.h"

#include "Transform.h"
#include "Easing.h"
#include "SceneRoot.h"

using namespace ci;

class Voxel;
class SceneRoot;

class SkeletonData{
public:
	static void SaveSkeleton(std::string directory, std::string fileName, SceneRoot * _sceneRoot);
	static std::vector<Joint *> LoadSkeleton(std::string filePath);

	static void validateDirectory(std::string & directory);
	static void validateFileName(std::string & fileName);

protected:
	static std::string writeJoint(Joint * j, unsigned int indent = 0);
	static Joint * readJoint(JsonTree joint, Joint * parent = nullptr);

	static std::string writeAnimations(Joint * j, unsigned int indent = 0);
	static void readAnimations(JsonTree animations, Joint * j);
	
	static std::string writeAnimation(Animation<float> * a, std::string name, unsigned int indent = 0);
	static std::string writeAnimation(Animation<glm::quat> * a, std::string name, unsigned int indent = 0);
	
	static void readAnimation(JsonTree animation, Animation<float> * a);
	static void readAnimation(JsonTree animation, Animation<glm::quat> * a);
	
	static std::string writeTween(Tween<float> * t, int id, unsigned int indent = 0);
	static std::string writeTween(Tween<glm::quat> * t, int id, unsigned int indent = 0);
	
	static Tween<float> * readFloatTween(JsonTree tween);
	static Tween<glm::quat> * readQuaternionTween(JsonTree tween);

	static std::string writeVoxel(Voxel * v, unsigned int indent = 0);
	static void readVoxel(JsonTree voxel, Joint * parent);
};
