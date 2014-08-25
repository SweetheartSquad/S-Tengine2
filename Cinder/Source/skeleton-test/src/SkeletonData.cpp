#include "SkeletonData.h"


SkeletonData::SkeletonData(void)
{
}

void SkeletonData::SaveSkeleton(vector<Joint *> &m_joints) {
	//Create dir
	if( PathFileExistsA(directory.c_str()) == TRUE)  { 
		app::console() << "dir created/exists" << endl;

		ofstream boneFile;
		boneFile.open(directory.append(fileName));
		std::stringstream json = std::stringstream();

		boneFile << "{" << endl;
		for(Joint * b : m_joints)
		{ 

			//json << "{" << endl;
			boneFile << " {" << endl;
			boneFile << "  \"id\":" << b->id << "," << endl;
			if (b->parent != NULL) {
				boneFile << "  \"parent_id\":" << b->parent->id << "," << endl;
			}
			boneFile << "  \"pos\":"<< "{\"x:\"" << b->pos.x << ", \"y\":" << b->pos.y << ", \"z:\"" << b->pos.z << "}," << endl;
			boneFile << "  \"rotation\":"<< "{\"x\":" << b->rotation.v.x << ", \"y\":" << b->rotation.v.y << ", \"z\":" << b->rotation.v.z << ", \"w\":" << b->rotation.w << "}" << endl;
			boneFile << " }";
			if (b->id != m_joints.back()->id) {
				boneFile << ",";
			}
			boneFile << endl;
		}
		boneFile << "}" << endl;
		boneFile.close();
    }else{
            printf("Does Not exist!\n"); 
    }
}

void SkeletonData::setDirectory(string directory) {
	if( PathFileExistsA(directory.c_str()) == TRUE)  { 
		if ( directory.back() != (char)"/") {
			directory += "/";
		}
		this->directory = directory;
	}else{
		app::console() << "Directory does not exist!" << endl;
	}
}

void SkeletonData::setFileName(string fileName) {
	this->fileName = fileName;
}

SkeletonData::~SkeletonData(void)
{
}
