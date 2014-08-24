#pragma 

#include <vector>
#include "Joint.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>

using namespace std;

class SkeletonData
{
public:
	SkeletonData(void);
	void SaveSkeleton(vector<Joint*> &m_joints);
	~SkeletonData(void);

protected:
	
};

