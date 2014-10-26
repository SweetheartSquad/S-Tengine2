#pragma once

#include "SkeletonData.h"

void SkeletonData::SaveSkeleton(std::string directory, std::string fileName, std::vector<Joint *> & joints) {
	try{
		//Validate directory
		validateDirectory(directory);

		//Validate filename
		validateFileName(fileName);

		//Create dir
		if( PathFileExistsA(directory.c_str()) == TRUE)  { 
			app::console() << "dir created/exists" << std::endl;
			std::ofstream jointFile;
			try {
				jointFile.open(directory.append(fileName));
				std::stringstream json = std::stringstream();

				jointFile << "{\"joints\":[" << std::endl;

				//store the root joints in a temporary vector
				std::vector<Joint *> roots;
				for(Joint * j : joints){
					if(j->parent == nullptr){
						roots.push_back(j);
					}
				}

				//write the joints to file (loop through roots and recurse through children)
				for(Joint * j : roots){
					jointFile << writeJoint(j);
					if (j->id != roots.back()->id) {
						jointFile << ",";
					}
					jointFile << std::endl;
				}

				jointFile << "]}" << std::endl;
				jointFile.close();
			}catch (std::exception ex){
				if(jointFile != NULL){
					if (jointFile.is_open()){
						jointFile.close();
					}
				}
				throw ex;
			}
		}else{
				app::console() << "Does Not exist!\n" << std::endl;
				throw std::exception("Directory does not exist!");
		}
	}catch(std::exception ex){
		throw ex;
	}
}

std::vector<Joint *> SkeletonData::LoadSkeleton(std::string filePath) {
	//Not sure about this error catching setup
	std::vector<Joint*> joints;

	if( PathFileExistsA(filePath.c_str()) == TRUE)  { 
		try{
			JsonTree doc = JsonTree(loadFile(filePath));

			JsonTree jointsJson = doc.getChild( "joints" );
			Joint * parent = nullptr;
			for( JsonTree::ConstIter joint = jointsJson.begin(); joint != jointsJson.end(); ++joint ) {
				JsonTree jJson = jointsJson.getChild(joint->getKey());
				Joint * j = readJoint(jJson);
				joints.push_back(j);
			}
		}catch (std::exception ex) {
			throw ex;
		}
	}else{
		throw std::exception("File does not exist!");
	}
	return joints;
}

std::string SkeletonData::writeJoint(Joint * j) {
	std::stringstream json;
	json << " {" << std::endl;
	json << "  \"id\":" << j->id << "," << std::endl;
	if (j->parent != nullptr) {
		json << "  \"parent_id\":" << j->parent->id << "," << std::endl;
	}
	json << "  \"pos\":"<< "{\"x\":" << j->getPos().x << ", \"y\":" << j->getPos().y << ", \"z\":" << j->getPos().z << "}," << std::endl;
	json << "  \"orientation\":" << "{\"x\":" << j->transform.orientation.x << ", \"y\":" << j->transform.orientation.y << ", \"z\":" << j->transform.orientation.z << ", \"w\":" << j->transform.orientation.w << "}," << std::endl;
	
	json << "  \"children\":" << "[" << std::endl;
	for(Joint * c : j->children) {
		json << writeJoint(c);
		if (j->children.size() != 0 && c->id != j->children.back()->id) {
			json << ",";
		}
	}
	
	json << "]" << std::endl;
	
	json << " }" << std::endl;

	return json.str();
}

Joint * SkeletonData::readJoint(JsonTree joint, Joint * parent) {
	
	Joint * j = new Joint(parent);
	std::vector<Joint *> children;

	//j->parent = parent;
	j->id = joint.getChild( "id" ).getValue<int>();
		app::console() << "id: " << j->id << std::endl;
	JsonTree pos = joint.getChild("pos");
		app::console() << " jt_pos: x = " << pos.getChild("x").getValue<float>() << " y = " << pos.getChild("y").getValue<float>() << " pos: z = " << pos.getChild("z").getValue<float>() << std::endl;
	j->setPos(Vec3d(pos.getChild("x").getValue<float>(), pos.getChild("y").getValue<float>(), pos.getChild("z").getValue<float>()), false);
		app::console() << " pos: x = " << j->getPos().x << " y = " << j->getPos().y << " pos: z = " << j->getPos().z << std::endl;
	JsonTree orientation = joint.getChild("orientation");
	j->transform.orientation = glm::quat(orientation.getChild("x").getValue<float>(), orientation.getChild("y").getValue<float>(), orientation.getChild("z").getValue<float>(), orientation.getChild("w").getValue<float>());
		app::console() << " orientation: x = " << j->transform.orientation.x << " y = " << j->transform.orientation.y << " z = " << j->transform.orientation.z << " w = " << j->transform.orientation.w << std::endl;

	JsonTree childrenJson = joint.getChild("children");
	for( JsonTree::ConstIter child = childrenJson.begin(); child != childrenJson.end(); ++child ) {
		JsonTree cJson = childrenJson.getChild(child->getKey());
		Joint * c = readJoint(cJson, j);
		children.push_back(c);
	}
	j->children = children;

	return j;
}

void SkeletonData::validateDirectory(std::string & directory) {
	if( PathFileExistsA(directory.c_str()) == TRUE)  { 
		if ( directory.back() != (char)"/") {
			directory += "/";
		}
		//this->directory = directory;
		// directory ok!
	}else{
		app::console() << "Directory does not exist!" << std::endl;
		throw std::exception("Directory does not exist!");
	}
}

void SkeletonData::validateFileName(std::string & fileName) {
	std::regex rgx_name("[\\\\/<>\|\":\?\*]+");
	std::regex rgx_ext("^.*\\.json$");
	try{
		app::console() << "validateFileName" << std::endl;
		if(fileName.empty()){
			app::console() << "Filename is empty" << std::endl;
			throw std::exception("Filename is empty");
		}
		if(regex_match(fileName.begin(),fileName.end(),rgx_name)){
			app::console() << "Invalid filename" << std::endl;
			throw std::exception("Invalid filename: filename cannot contain \\/<>|\":?*");
		}
		if(!regex_match(fileName.begin(),fileName.end(),rgx_ext)){
			app::console() << "Missing extension" << std::endl;
			fileName.append(".json");
		}
		//this->fileName = fileName;
		// fileName ok!
	}catch(std::exception ex){
		throw ex;
	}
}