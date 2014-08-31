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

#include <filesystem>
#include <regex>
#include <json.h>

using namespace std;

class SkeletonData
{
public:
	SkeletonData(void);
	
	void SaveSkeleton(string directory, string fileName, vector<Joint*> &m_joints);
	vector<Joint*> LoadSkeleton(string filePath);

	void validateDirectory(string dir);
	void validateFileName(string flnm);

	~SkeletonData(void);

protected:
};

