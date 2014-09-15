#include "SkeletonData.h"


SkeletonData::SkeletonData(void)
{
}

void SkeletonData::SaveSkeleton(string directory, string fileName, vector<Joint *> &joints) {
	try{
		//Validate directory
		validateDirectory(directory);

		//Validate filename
		validateFileName(fileName);

		//Create dir
		if( PathFileExistsA(directory.c_str()) == TRUE)  { 
			app::console() << "dir created/exists" << endl;
			ofstream jointFile;
			try {
				jointFile.open(directory.append(fileName));
				std::stringstream json = std::stringstream();

				jointFile << "{\"joints\":[" << endl;
				for(Joint * j : joints)
				{ 
					jointFile << writeJoint(j);
					if (j->id != joints.back()->id) {
						jointFile << ",";
					}
					jointFile << endl;
				}
				jointFile << "]}" << endl;
				jointFile.close();
			}catch (exception ex){
				if(jointFile != NULL){
					if (jointFile.is_open()){
						jointFile.close();
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
	//Not sure about this eror catching setup
	vector<Joint*> joints;

	if( PathFileExistsA(filePath.c_str()) == TRUE)  { 
		try{
			JsonTree doc = JsonTree(loadFile(filePath));

			JsonTree jointsJson = doc.getChild( "joints" );
			Joint * parent = NULL;
			for( JsonTree::ConstIter joint = jointsJson.begin(); joint != jointsJson.end(); ++joint ) {
				JsonTree jJson = jointsJson.getChild(joint->getKey());
				Joint * j = readJoint(jJson);

				joints.push_back(j);
			}
		}catch (exception ex) {
			throw ex;
		}
	}else{
		throw new exception("File does not exist!");
	}
	return joints;
}

string SkeletonData::writeJoint(Joint* j) {
	std::stringstream json;
	json << " {" << endl;
	json << "  \"id\":" << j->id << "," << endl;
	if (j->parent != NULL) {
		json << "  \"parent_id\":" << j->parent->id << "," << endl;
	}
	json << "  \"pos\":"<< "{\"x\":" << j->getPos().x << ", \"y\":" << j->getPos().y << ", \"z\":" << j->getPos().z << "}," << endl;
	json << "  \"orientation\":" << "{\"x\":" << j->transform->orientation.x << ", \"y\":" << j->transform->orientation.y << ", \"z\":" << j->transform->orientation.z << ", \"w\":" << j->transform->orientation.w << "}," << endl;
	
	json << "  \"children\":" << "[" << endl;
	for(Joint * c : j->children) {
		json << writeJoint(c);
		if (j->children.size() != 0 && c->id != j->children.back()->id) {
			json << ",";
		}
	}
	
	json << "]" << endl;
	
	json << " }" << endl;

	return json.str();
}

Joint* SkeletonData::readJoint(JsonTree joint, Joint * parent) {
	
	Joint * j = new Joint();
	vector<Joint*> children;

	j->parent = parent;
	j->id = joint.getChild( "id" ).getValue<int>();
	JsonTree pos = joint.getChild("pos");
	j->setPos(Vec3f(pos.getChild("x").getValue<float>(),pos.getChild("y").getValue<float>(),pos.getChild("z").getValue<float>()));
	JsonTree orientation = joint.getChild("orientation");
	j->transform->orientation = glm::quat(orientation.getChild("x").getValue<float>(), orientation.getChild("y").getValue<float>(), orientation.getChild("z").getValue<float>(), orientation.getChild("w").getValue<float>());

	JsonTree childrenJson = joint.getChild("children");
	for( JsonTree::ConstIter child = childrenJson.begin(); child != childrenJson.end(); ++child ) {
		JsonTree cJson = childrenJson.getChild(child->getKey());
		Joint * c = readJoint(cJson, j);
		children.push_back(c);
	}
	j->children = children;

	return j;
}

void SkeletonData::validateDirectory(string &directory) {
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

void SkeletonData::validateFileName(string &fileName) {
	regex rgx_name("[\\\\/<>\|\":\?\*]+");
	regex rgx_ext("^.*\\.json$");
	try{
		app::console() << "validateFileName" << endl;
		if(fileName.empty()){
			app::console() << "Filename is empty" << endl;
			throw exception("Filename is empty");
		}
		if(regex_match(fileName.begin(),fileName.end(),rgx_name)){
			app::console() << "Invalid filename" << endl;
			throw exception("Invalid filename: filename cannot contain \\/<>|\":?*");
		}
		if(!regex_match(fileName.begin(),fileName.end(),rgx_ext)){
			app::console() << "Missing extension" << endl;
			fileName.append(".json");
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
