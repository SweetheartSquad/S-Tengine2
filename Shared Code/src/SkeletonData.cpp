#pragma once

#include "SkeletonData.h"
#include "Transform.h"
#include "Animation.h"
#include "Easing.h"
#include "SceneRoot.h"

void SkeletonData::SaveSkeleton(std::string directory, std::string fileName, SceneRoot * _sceneRoot) {
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

				jointFile << "{" << "\"joints\": " << "[" << std::endl;

				//write the joints to file (loop through roots and recurse through children)
				for(unsigned long int i = 0; i < _sceneRoot->children.size(); ++i){
					Joint * j = dynamic_cast<Joint * >(_sceneRoot->children.at(i));
					jointFile << writeJoint(j);
					if (j != _sceneRoot->children.back()) {
						jointFile << ",";
					}
					jointFile << std::endl;
				}

				jointFile << "]}" << std::endl;
				jointFile.close();
			}catch (std::exception ex){
				if (jointFile.is_open()){
					jointFile.close();
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
			unsigned int i = 0;
			for( JsonTree::ConstIter joint = jointsJson.begin(); joint != jointsJson.end(); ++joint ) {
				// Apparently, getKey DOESN't return an index if there is no key? (Even though it says it will in the json.h header...)
				//JsonTree jJson = jointsJson.getChild(joint->getKey());
				JsonTree jJson = jointsJson.getChild(i);
				Joint * j = readJoint(jJson);
				joints.push_back(j);
				i++;
			}
		}catch (std::exception ex) {
			throw ex;
		}
	}else{
		throw std::exception("File does not exist!");
	}
	return joints;
}

std::string SkeletonData::writeJoint(Joint * j, unsigned int indent) {

	std::stringstream json;
	indent++;
	json << std::string(indent * 3, ' ') << "{" << std::endl;
	indent++;

	json << std::string(indent * 3, ' ') << "\"id\": " << j->id << "," << std::endl;

	if (j->parent != nullptr) {
		json << std::string(indent * 3, ' ') << "\"parent\": " << dynamic_cast<Joint *>(j->parent)->id << "," << std::endl;
	}

	// Transform: Object
	json << std::string(indent * 3, ' ') << "\"transform\":{" << std::endl;
	indent++;
	json << std::string(indent * 3, ' ') << "\"pos\": " << "{" << "\"x\": " << j->getPos().x << ", " << "\"y\": " << j->getPos().y << ", " << "\"z\": " << j->getPos().z << "}," << std::endl;
	json << std::string(indent * 3, ' ') << "\"orientation\": " << "{" << "\"x\": " << j->transform->orientation.x << ", " << "\"y\": " << j->transform->orientation.y << ", " << "\"z\": " << j->transform->orientation.z << ", " << "\"w\": " << j->transform->orientation.w << "}," << std::endl;
	json << std::string(indent * 3, ' ') << "\"scaleVector\": " << "{" << "\"x\": " << j->transform->scaleVector.x << ", " << "\"y\": " << j->transform->scaleVector.y << ", " << "\"z\": " << j->transform->scaleVector.z << "}" << std::endl;
	indent--;
	json << std::string(indent * 3, ' ') << "}," << std::endl;

	// Voxels: Array
	json << std::string(indent * 3, ' ') << "\"voxels\": " << "[" << std::endl;
	for (unsigned long int i = 0; i < j->voxels.size(); ++i) {
		Voxel * v = j->voxels.at(i);
		json << writeVoxel(v, indent);
		if (i != j->voxels.size() - 1) {
			json << ",";
		}
		json << std::endl;
	}
	json << std::string(indent * 3, ' ') << "]," << std::endl;

	// Animations: Objects
	json << std::string(indent * 3, ' ') << "\"animations\":{" << std::endl;
	json << writeAnimations(j, indent);
	json << std::string(indent * 3, ' ') << "}," << std::endl;

	// Children: Array
	json << std::string(indent * 3, ' ') << "\"children\": " << "[" << std::endl;
	for(unsigned long int i = 0; i < j->children.size(); ++i) {
		Joint * child = dynamic_cast<Joint *>(j->children.at(i));
		json << writeJoint(child, indent);
		if (j->children.size() != 0 && child->id != dynamic_cast<Joint *>(j->children.back())->id) {
			json << ",";
		}
		json << std::endl;
	}
	json << std::string(indent * 3, ' ') << "]" << std::endl;
	indent--;
	json << std::string(indent * 3, ' ') << "}";
	std::string blah = json.str();
	return json.str();
}

Joint * SkeletonData::readJoint(JsonTree joint, Joint * parent) {
	
	Joint * j = new Joint(parent);
	std::vector<NodeChild *> children;
	std::vector<Voxel *> voxels;

	j->id = joint.getChild( "id" ).getValue<int>();
	app::console() << "id: " << j->id << std::endl;
	
	// Transform: Object
	JsonTree transform = joint.getChild("transform");

	JsonTree pos = transform.getChild("pos");
	app::console() << " jt_pos: x = " << pos.getChild("x").getValue<float>() << " y = " << pos.getChild("y").getValue<float>() << " pos: z = " << pos.getChild("z").getValue<float>() << std::endl;
	j->setPos(glm::vec3(pos.getChild("x").getValue<float>(), pos.getChild("y").getValue<float>(), pos.getChild("z").getValue<float>()), false);
	app::console() << " pos: x = " << j->getPos().x << " y = " << j->getPos().y << " pos: z = " << j->getPos().z << std::endl;
	
	JsonTree orientation = transform.getChild("orientation");
	j->transform->orientation = glm::quat(orientation.getChild("w").getValue<float>(), orientation.getChild("x").getValue<float>(), orientation.getChild("y").getValue<float>(), orientation.getChild("z").getValue<float>());
	app::console() << " orientation: x = " << j->transform->orientation.x << " y = " << j->transform->orientation.y << " z = " << j->transform->orientation.z << " w = " << j->transform->orientation.w << std::endl;
	
	JsonTree scale = transform.getChild("scaleVector");
	j->transform->scaleVector = glm::vec3(scale.getChild("x").getValue<float>(), scale.getChild("y").getValue<float>(), scale.getChild("z").getValue<float>());
	app::console() << " scale: x = " << j->transform->scaleVector.x << " y = " << j->transform->scaleVector.y << " z = " << j->transform->scaleVector.z << std::endl;
	
	// Voxels: Array
	JsonTree voxelsJson = joint.getChild("voxels");
	unsigned int voxel_index = 0;
	for (JsonTree::ConstIter voxel = voxelsJson.begin(); voxel != voxelsJson.end(); ++voxel) {
		// Apparently, getKey DOESN't return an index if there is no key?
		//JsonTree cJson = childrenJson.getChild(child->getKey());
		JsonTree vJson = voxelsJson.getChild(voxel_index);
		readVoxel(vJson, j);
		voxel_index++;
	}

	// Animations: Objects
	readAnimations(joint.getChild("animations"), j);
	
	// Children: Array
	JsonTree childrenJson = joint.getChild("children");
	unsigned int children_index = 0;
	for( JsonTree::ConstIter child = childrenJson.begin(); child != childrenJson.end(); ++child ) {
		// Apparently, getKey DOESN't return an index if there is no key?
		//JsonTree cJson = childrenJson.getChild(child->getKey());
		JsonTree cJson = childrenJson.getChild(children_index);
		Joint * c = readJoint(cJson, j);
		children.push_back(c);
		children_index++;
	}
	j->children = children;

	return j;
}

std::string SkeletonData::writeAnimations(Joint * j, unsigned int indent){
	std::stringstream json;
	indent++;

	json << writeAnimation(j->translateX, "translateX", indent) << "," << std::endl;
	json << writeAnimation(j->translateY, "translateY", indent) << "," << std::endl;
	json << writeAnimation(j->translateZ, "translateZ", indent) << "," << std::endl;
	json << writeAnimation(j->rotateX, "rotateX", indent) << "," << std::endl;
	json << writeAnimation(j->rotateY, "rotateY", indent) << "," << std::endl;
	json << writeAnimation(j->rotateZ, "rotateZ", indent) << "," << std::endl;
	json << writeAnimation(j->rotateW, "rotateW", indent) << "," << std::endl;
	json << writeAnimation(j->scaleX, "scaleX", indent) << "," << std::endl;
	json << writeAnimation(j->scaleY, "scaleY", indent) << "," << std::endl;
	json << writeAnimation(j->scaleZ, "scaleZ", indent) << std::endl;

	return json.str();
}

void SkeletonData::readAnimations(JsonTree animations, Joint * j){

	readAnimation(animations.getChild("translateX"), j->translateX);
	readAnimation(animations.getChild("translateY"), j->translateY);
	readAnimation(animations.getChild("translateZ"), j->translateZ);
	readAnimation(animations.getChild("rotateX"), j->rotateX);
	readAnimation(animations.getChild("rotateY"), j->rotateY);
	readAnimation(animations.getChild("rotateZ"), j->rotateZ);
	readAnimation(animations.getChild("rotateW"), j->rotateW);
	readAnimation(animations.getChild("scaleX"), j->scaleX);
	readAnimation(animations.getChild("scaleY"), j->scaleY);
	readAnimation(animations.getChild("scaleZ"), j->scaleX);
}

std::string SkeletonData::writeAnimation(Animation * a, std::string name, unsigned int indent){

	std::stringstream json;
	
	json << std::string(indent * 3, ' ') << "\"" << name << "\":{" << std::endl;
	indent++;

	// startValue
	json << std::string(indent * 3, ' ') << "\"startValue\": " << a->startValue << "," << std::endl;

	// hasStart
	json << std::string(indent * 3, ' ') << "\"hasStart\": " << a->hasStart << "," << std::endl;

	// tween array
	json << std::string(indent * 3, ' ') << "\"tweens\": " << "[" << std::endl;
	for(unsigned long int i = 0; i < a->tweens.size(); ++i) {
		Tween * tween = a->tweens.at(i);
		json << writeTween(tween, i, indent);
		if (i != a->tweens.size() - 1) {
			json << ",";
		}
		json << std::endl;
	}
	json << std::string(indent * 3, ' ') << "]," << std::endl;

	// loopType
	json << std::string(indent * 3, ' ') << "\"loopType\": " << a->loopType << std::endl;

	indent--;
	json << std::string(indent * 3, ' ') << "}";

	return json.str();
}

void SkeletonData::readAnimation(JsonTree animation, Animation * a){

	std::vector<Tween *> tweens;
	app::console() << animation.getKey() << std::endl;
	a->startValue = animation.getChild("startValue").getValue<float>();
	a->hasStart = animation.getChild("hasStart").getValue<bool>();
	a->referenceValue = a->startValue;

	JsonTree tweensJson = animation.getChild("tweens");
	unsigned int i = 0;
	for( JsonTree::ConstIter tween = tweensJson.begin(); tween != tweensJson.end(); ++tween ) {
		app::console() << "tween key: " << tween->getKey() << std::endl;
		// Apparently, getKey DOESN't return an index if there is no key?
		//JsonTree tJson = tweensJson.getChild(tween->getKey());
		JsonTree tJson = tweensJson.getChild(i);
		Tween * t = readTween(tJson);
		tweens.push_back(t);
		i++;
	}
	a->tweens = tweens;

	a->loopType = static_cast<Animation::LoopType>(animation.getChild("loopType").getValue<int>());
}

std::string SkeletonData::writeTween(Tween * t, int id, unsigned int indent){
	std::stringstream json;
	indent++;

	json << std::string(indent * 3, ' ') << "{" << std::endl;
	
	indent++;
	json << std::string(indent * 3, ' ') << "\"id\": " << id << "," << std::endl;
	json << std::string(indent * 3, ' ') << "\"deltaTime\": " << t->deltaTime << "," << std::endl;
	json << std::string(indent * 3, ' ') << "\"deltaValue\": " << t->deltaValue << "," << std::endl;
	json << std::string(indent * 3, ' ') << "\"interpolation\": " << t->interpolation << std::endl;
	indent--;

	json << std::string(indent * 3, ' ') << "}";

	return json.str();
}

Tween * SkeletonData::readTween(JsonTree tween){
	// get interpolation

	Tween * t = new Tween(tween.getChild("deltaTime").getValue<float>(), tween.getChild("deltaValue").getValue<float>(), static_cast<Easing::Type>(tween.getChild("interpolation").getValue<int>()));
	app::console() << "id: " << tween.getChild("id").getValue<int>() << " deltaTime: " << tween.getChild("deltaTime").getValue<float>() << " deltaValue: " << tween.getChild("deltaValue").getValue<float>() << std::endl;
	return t;
}

std::string SkeletonData::writeVoxel(Voxel * v, unsigned int indent){
	std::stringstream json;
	indent++;

	// Just the position vector
	json << std::string(indent * 3, ' ') << "{" << "\"x\": " << v->pos.x << ", " << "\"y\": " << v->pos.y << ", " << "\"z\": " << v->pos.z << "}";

	return json.str();
}

void SkeletonData::readVoxel(JsonTree voxel, Joint * parent){

	// create voxel, added to parent in constructor
	Voxel * v = new Voxel(Vec3d(voxel.getChild("x").getValue<float>(), voxel.getChild("y").getValue<float>(), voxel.getChild("z").getValue<float>()), parent);
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
	std::regex rgx_name("[\\\\/<>\|\"\?\*]+");
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