#include "SkeletonData.h"


SkeletonData::SkeletonData(void)
{
}

void SkeletonData::SaveSkeleton(string directory, string fileName, vector<Joint *> &m_joints) {
	try{
		//Validate directory
		validateDirectory(directory);

		//Validate filename
		validateFileName(fileName);

		//Create dir
		if( PathFileExistsA(directory.c_str()) == TRUE)  { 
			app::console() << "dir created/exists" << endl;
			ofstream boneFile;
			try {
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
					boneFile << "  \"pos\":"<< "{\"x:\"" << b->getPos().x << ", \"y\":" << b->getPos().y << ", \"z:\"" << b->getPos().z << "}," << endl;
					boneFile << "  \"orientation\":"<< "{\"x\":" << b->orientation.v.x << ", \"y\":" << b->orientation.v.y << ", \"z\":" << b->orientation.v.z << ", \"w\":" << b->orientation.w << "}" << endl;
					boneFile << " }";
					if (b->id != m_joints.back()->id) {
						boneFile << ",";
					}
					boneFile << endl;
				}
				boneFile << "}" << endl;
				boneFile.close();
			}catch (exception ex){
				if(boneFile != NULL){
					if (boneFile.is_open()){
						boneFile.close();
					}
				}
				throw ex;
			}
		}else{
				app::console() << "Does Not exist!\n" << endl;
				throw exception("Directory does not exist!");
		}
	}catch(exception ex){
		throw ex;
	}
}

vector<Joint*> SkeletonData::LoadSkeleton(string filePath) {
	try{
		ifstream boneFile(filePath);
		if (boneFile.good())
		{
			// read away
		}else{
			throw exception("File does not exist.");
		}
	}catch (exception ex){
		throw ex;
	}
}

void SkeletonData::validateDirectory(string directory) {
	if( PathFileExistsA(directory.c_str()) == TRUE)  { 
		if ( directory.back() != (char)"/") {
			directory += "/";
		}
		//this->directory = directory;
		// directory ok!
	}else{
		app::console() << "Directory does not exist!" << endl;
		throw exception("Directory does not exist!");
	}
}

void SkeletonData::validateFileName(string fileName) {
	
	regex rgx_name("[\\\\/<>\|\":\?\*]+");
	regex rgx_ext("^.*\..*$");
	try{
		if(fileName.empty()){
			throw exception("Filename is empty");
		}
		if(regex_match(fileName.begin(),fileName.end(),rgx_name)){
			throw exception("Invalid filename: filename cannot contain \\/<>|\":?*");
		}
		if(!regex_match(fileName.begin(),fileName.end(),rgx_ext)){
			app::console() << "Missing extension" << endl;
			fileName.append(".txt");
		}
		//this->fileName = fileName;
		// fileName ok!
	}catch(exception ex){
		throw ex;
	}
}

SkeletonData::~SkeletonData(void)
{
}
