#pragma 
#pragma comment(lib, "Shlwapi.lib")

#include "shlwapi.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <regex>

#include <Cinder/app/App.h>
#include <cinder/Json.h>

#include "Joint.h"

using namespace std;
using namespace ci;

class SkeletonData
{
public:
	SkeletonData(void);
	
	void SaveSkeleton(string directory, string fileName, vector<Joint*> &m_joints);
	vector<Joint*> LoadSkeleton(string filePath);

	void validateDirectory(string &directory);
	void validateFileName(string &fileName);

	~SkeletonData(void);

protected:
	string writeJoint(Joint* b);
	Joint* readJoint(JsonTree joint, Joint * parent = nullptr);
};

