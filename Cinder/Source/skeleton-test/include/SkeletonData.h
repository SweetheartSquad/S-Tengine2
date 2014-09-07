#pragma 
#pragma comment(lib, "Shlwapi.lib")

#include "shlwapi.h"
#include "Cinder/app/App.h"
#include <vector>
#include "Joint.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cerrno>

#include <filesystem>
#include <regex>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include "cinder/Json.h"

using namespace std;
using namespace rapidjson;
using namespace ci;

class SkeletonData
{
public:
	SkeletonData(void);
	
	void SaveSkeleton(string directory, string fileName, vector<Joint*> &m_joints);
	vector<Joint*> LoadSkeleton(string filePath);
	vector<Joint*> LoadSkeletonJson(string filePath);

	void validateDirectory(string &directory);
	void validateFileName(string &fileName);

	~SkeletonData(void);

protected:
	string boneJson(Joint* b);
};

