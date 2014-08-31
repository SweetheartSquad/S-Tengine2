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

using namespace std;

class SkeletonData
{
public:
	SkeletonData(void);
	void setDirectory(string dir);
	void setFileName(string flnm);
	void SaveSkeleton(vector<Joint*> &m_joints);
	~SkeletonData(void);

protected:
	string directory;
	string fileName;
};

