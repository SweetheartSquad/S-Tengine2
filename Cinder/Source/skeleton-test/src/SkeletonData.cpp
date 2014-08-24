#include "SkeletonData.h"


SkeletonData::SkeletonData(void)
{
}

void SkeletonData::SaveSkeleton(vector<Joint *> &m_joints) {
	//console() << "Begin" << endl;
	ofstream boneFile;
	boneFile.open("BONES.txt");
	std::stringstream json = std::stringstream();

	boneFile << "{" << endl;
	for(Joint * b : m_joints)
	{ 

		//json << "{" << endl;
		boneFile << " {" << endl;
		boneFile << "  \"id\":" << b->id << "," << endl;
		boneFile << "  \"pos\":"<< "{\"x:\"" << b->pos.x << ", \"y\":" << b->pos.y << ", \"z:\"" << b->pos.z << "}," << endl;
		boneFile << "  \"rotation\":"<< "{\"x\":" << b->rotation.v.x << ", \"y\":" << b->rotation.v.y << ", \"z\":" << b->rotation.v.z << ", \"w\":" << b->rotation.w << "}" << endl;
		boneFile << " }";
		if (b->id != m_joints.back()->id) {
			boneFile << ",";
		}
		boneFile << endl;
		//json << "pos:"<< "{x:" << b->pos.x << ",y:" << b->pos.y << ",z:" << b->pos.z << "}" << endl;
	}
	boneFile << "}" << endl;
	boneFile.close();
}

SkeletonData::~SkeletonData(void)
{
}
