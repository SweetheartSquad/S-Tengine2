#pragma once

#include <SOIL.h>
#include <regex>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

#include <soil.h>

#include <json/json.h>

#include "MeshInterface.h"
#include "Resource.h"
#include "FileUtils.h"
#include "VoxelJoint.h"
#include "VoxelMesh.h"
#include "Animation.h"

Resource::Resource(){}
Resource::~Resource(){}

enum ObjFaceFormat {
	VERTS,
	VERTS_UVS,
	VERTS_NORMALS,
	VERTS_UVS_NORMALS
};

struct Face{
	int f1Vert, f1Uv, f1Norm,
		f2Vert, f2Uv, f2Norm,
		f3Vert, f3Uv, f3Norm;
};

struct FizzXAtlas{
	
};

struct FizzXImage{
	std::string path;
	std::string level;
	float x;
	float y;
	float rotation;
	float scaleX;
	float scaleY;
	float zDepth;
	std::string body;
	std::string atlas;
};

struct FizzXFixture{
	b2FixtureDef fixtureDef;
	std::string name;
};

struct FizzXBody{
	b2BodyDef bodyDef;
	std::string name;
	std::string image;
	std::vector<FizzXFixture> fixtures;
};

unsigned char* Resource::loadImage(const char* _src, int _width, int _height, int _SOILLoadMode, int * _channels){
	unsigned char* res = SOIL_load_image(_src, &_width, &_height, _channels, _SOILLoadMode);
	if(res == 0){
		throw;
	}
	return res;
}

void Resource::freeImageData(unsigned char* _image){
	SOIL_free_image_data(_image);
}

TriMesh * Resource::loadMeshFromObj(std::string _objSrc){

	std::istringstream stream(FileUtils::voxReadFile(_objSrc));
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	std::smatch match;
	std::regex faceRegex("(\\d{1})[/]?(\\d?)[/]?(\\d?)[\\s]{1}");

	bool faceFormatChecked  = false;
	bool hasUvs			    = false;
	bool hasNorms           = false;

	ObjFaceFormat faceStructure = VERTS;
	TriMesh* mesh = new TriMesh(GL_TRIANGLES, GL_STATIC_DRAW);

	int maxchars = 8192;
	std::vector<char> buf(maxchars);

	while(stream.peek() != -1) {
		stream.getline(&buf[0], maxchars);
		std::string line(&buf[0]);
		if (line.size() > 0) {
			if (line[line.size()-1] == '\n') line.erase(line.size()-1);
		}
		if (line.size() > 0) {
			if (line[line.size()-1] == '\r') line.erase(line.size()-1);
		}	
		char p1 = -1;
		char p2 = -1;
		if(line.size() >= 1){
			p1 = line[0];	
		}
		if(line.size() >= 1){
			p2 = line[1];	
		}		
		if(p1 == -1 || p2 == -1){
			continue;	
		}
		if(p1 == '\0' || p1 == '#'){
			continue;
		}
		if(p1 == 'v' && p2 == ' '){
			glm::vec3 tempVert(1);
			sscanf(line.c_str(), "%*s %f %f %f", &tempVert.x, &tempVert.y, &tempVert.z);
			verts.push_back(tempVert);
		}else if(p1 == 'v' && p2 == 'n'){
			glm::vec3 tempNorm(1);
			sscanf(line.c_str(), "%*s %f %f %f", &tempNorm.x, &tempNorm.y, &tempNorm.z);
			normals.push_back(tempNorm);
		}else if(p1 == 'v' && p2 == 't'){
			glm::vec2 tempUv(1);
			sscanf(line.c_str(), "%*s %f %f", &tempUv.x, &tempUv.y);
			uvs.push_back(tempUv);
		}else if(p1 == 'f' && p2 == ' '){
			if(!faceFormatChecked){
				std::smatch matches;
				std::regex_search(line, matches, faceRegex);
				if(matches[2].str().size()>0 && matches[3].str().size()>0){
					faceStructure = VERTS_UVS_NORMALS;
					hasUvs = true;
					hasNorms = true;
				}else if(matches[2].str().size()>0 && !matches[3].str().size()>0){
					faceStructure = VERTS_UVS;
					hasUvs = true;
				}else if(!matches[2].str().size()>0 && matches[3].str().size()>0){
					faceStructure = VERTS_NORMALS;
					hasNorms = true;
				}
				faceFormatChecked = true;
			}
			Face face;
			std::string faceVert;
			std::string faceVerts;
			switch(faceStructure){
			case VERTS : faceVert = " %d";
				faceVerts = "%*s" + faceVert + faceVert + faceVert;
				sscanf(line.c_str(), faceVerts.c_str(), &face.f1Vert, &face.f2Vert, &face.f3Vert);
				break;
			case VERTS_UVS : faceVert = " %d/%d";
				faceVerts = "%*s" + faceVert + faceVert + faceVert;
				sscanf(line.c_str(), faceVerts.c_str(), &face.f1Vert, &face.f1Uv,
					&face.f2Vert, &face.f2Uv,
					&face.f3Vert, &face.f3Uv);
				break;
			case VERTS_NORMALS : faceVert = " %d//%d";
				faceVerts = "%*s" + faceVert + faceVert + faceVert;
				sscanf(line.c_str(), faceVerts.c_str(), &face.f1Vert, &face.f1Norm,
					&face.f2Vert, &face.f2Norm,
					&face.f3Vert, &face.f3Norm);
				break;
			case VERTS_UVS_NORMALS : faceVert = " %d/%d/%d";
				faceVerts = "%*s" + faceVert + faceVert + faceVert;
				sscanf(line.c_str(), faceVerts.c_str(), &face.f1Vert, &face.f1Uv, &face.f1Norm,
					&face.f2Vert, &face.f2Uv, &face.f2Norm,
					&face.f3Vert, &face.f3Uv, &face.f3Norm);
				break;
			}
			Vertex v1(verts.at(face.f1Vert - 1));
			Vertex v2(verts.at(face.f2Vert - 1));
			Vertex v3(verts.at(face.f3Vert - 1));
			if(hasUvs){
				glm::vec2 uv = uvs.at(face.f1Uv - 1);
				v1.u = uv.x;
				v1.v = uv.y;
				uv = uvs.at(face.f2Uv - 1);
				v2.u = uv.x;
				v2.v = uv.y;
				uv = uvs.at(face.f3Uv - 1);
				v3.u = uv.x;
				v3.v = uv.y;
			}
			if(hasNorms){
				glm::vec3 norm = normals.at(face.f1Norm - 1);
				v1.nx = norm.x;
				v1.ny = norm.y;
				v1.nz = norm.z;
				norm = normals.at(face.f2Norm - 1);
				v2.nx = norm.x;
				v2.ny = norm.y;
				v2.nz = norm.z;
				norm = normals.at(face.f3Norm - 1);
				v3.nx = norm.x;
				v3.ny = norm.y;
				v3.nz = norm.z;
			}

			mesh->pushVert(v1);
			mesh->pushVert(v2);
			mesh->pushVert(v3);

			mesh->pushTri(mesh->vertices.size()-3,
				mesh->vertices.size()-2,
				mesh->vertices.size()-1);
		}
	}
	return mesh;
}

VoxelJoint * parseJoint(Json::Value _node, Json::ArrayIndex _index){

	VoxelJoint * mainJoint = new VoxelJoint(_node[_index]["id"].asInt(), new VoxelMesh(GL_STATIC_DRAW), new Transform());
	
	mainJoint->transform->translationVector = glm::vec3(
		_node[_index]["transform"]["pos"].get("x", 0).asFloat(),
		_node[_index]["transform"]["pos"].get("y", 0).asFloat(),
		_node[_index]["transform"]["pos"].get("z", 0).asFloat());
	mainJoint->transform->orientation = glm::quat(
		_node[_index]["transform"]["orientation"].get("w", 1).asFloat(),
		_node[_index]["transform"]["orientation"].get("x", 0).asFloat(),
		_node[_index]["transform"]["orientation"].get("y", 0).asFloat(),
		_node[_index]["transform"]["orientation"].get("z", 0).asFloat());
	mainJoint->transform->scaleVector = glm::vec3(
		_node[_index]["transform"]["scaleVector"].get("x", 1).asFloat(),
		_node[_index]["transform"]["scaleVector"].get("y", 1).asFloat(),
		_node[_index]["transform"]["scaleVector"].get("z", 1).asFloat());
	
	mainJoint->translateX->hasStart = _node[_index]["animations"]["translateX"].get("hasStart", 0).asBool();
	mainJoint->translateY->hasStart = _node[_index]["animations"]["translateY"].get("hasStart", 0).asBool();
	mainJoint->translateZ->hasStart = _node[_index]["animations"]["translateZ"].get("hasStart", 0).asBool();

	mainJoint->translateX->startValue = _node[_index]["animations"]["translateX"].get("startValue", 0).asFloat();
	mainJoint->translateY->startValue = _node[_index]["animations"]["translateY"].get("startValue", 0).asFloat();
	mainJoint->translateZ->startValue = _node[_index]["animations"]["translateZ"].get("startValue", 0).asFloat();

	mainJoint->translateX->loopType = static_cast<Animation<float>::LoopType>(_node[_index]["animations"]["translateX"].get("loopType", 0).asInt());
	mainJoint->translateY->loopType = static_cast<Animation<float>::LoopType>(_node[_index]["animations"]["translateY"].get("loopType", 0).asInt());
	mainJoint->translateZ->loopType = static_cast<Animation<float>::LoopType>(_node[_index]["animations"]["translateZ"].get("loopType", 0).asInt());

	Json::Value transXTweens = _node[_index]["animations"]["translateX"]["tweens"];
	for(Json::Value::ArrayIndex j = 0; j < transXTweens.size(); ++j){
		mainJoint->translateX->tweens.push_back(new Tween<float>(
				transXTweens[j].get("deltaTime", 0).asFloat(),
				transXTweens[j].get("deltaValue", 0).asFloat(),
				static_cast<Easing::Type>(transXTweens[j].get("interpolation", 0).asInt())));
	}

	Json::Value transYTweens = _node[_index]["animations"]["translateY"]["tweens"];
	for(Json::Value::ArrayIndex j = 0; j < transYTweens.size(); ++j){
		mainJoint->translateY->tweens.push_back(new Tween<float>(
				transYTweens[j].get("deltaTime", 0).asFloat(),
				transYTweens[j].get("deltaValue", 0).asFloat(),
				static_cast<Easing::Type>(transYTweens[j].get("interpolation", 0).asInt())));
	}

	Json::Value transZTweens = _node[_index]["animations"]["translateZ"]["tweens"];
	for(Json::Value::ArrayIndex j = 0; j < transZTweens.size(); ++j){
		mainJoint->translateZ->tweens.push_back(new Tween<float>(
				transZTweens[j].get("deltaTime", 0).asFloat(),
				transZTweens[j].get("deltaValue", 0).asFloat(),
				static_cast<Easing::Type>(transZTweens[j].get("interpolation", 0).asInt())));
	}
	
	mainJoint->rotate->hasStart = _node[_index]["animations"]["rotate"].get("hasStart", 0).asBool();

	mainJoint->rotate->startValue.w = _node[_index]["animations"]["rotate"]["startValue"].get("w", 1).asFloat();
	mainJoint->rotate->startValue.x = _node[_index]["animations"]["rotate"]["startValue"].get("x", 0).asFloat();
	mainJoint->rotate->startValue.y = _node[_index]["animations"]["rotate"]["startValue"].get("y", 0).asFloat();
	mainJoint->rotate->startValue.z = _node[_index]["animations"]["rotate"]["startValue"].get("z", 0).asFloat();

	mainJoint->rotate->loopType = static_cast<Animation<glm::quat>::LoopType>(_node[_index]["animations"]["rotate"].get("loopType", 0).asInt());

	Json::Value rotateTweens = _node[_index]["animations"]["rotate"]["tweens"];
	for(Json::Value::ArrayIndex j = 0; j < rotateTweens.size(); ++j){
		mainJoint->rotate->tweens.push_back(new Tween<glm::quat>(
				rotateTweens[j].get("deltaTime", 0).asFloat(),
				glm::quat(
					rotateTweens[j]["deltaValue"].get("w", 0).asFloat(),
					rotateTweens[j]["deltaValue"].get("x", 0).asFloat(),
					rotateTweens[j]["deltaValue"].get("y", 0).asFloat(),
					rotateTweens[j]["deltaValue"].get("z", 0).asFloat()
				),
				static_cast<Easing::Type>(rotateTweens[j].get("interpolation", 0).asInt())));
	}

	mainJoint->scaleX->hasStart = _node[_index]["animations"]["scaleX"].get("hasStart", 0).asBool();
	mainJoint->scaleY->hasStart = _node[_index]["animations"]["scaleY"].get("hasStart", 0).asBool();
	mainJoint->scaleZ->hasStart = _node[_index]["animations"]["scaleZ"].get("hasStart", 0).asBool();

	mainJoint->scaleX->startValue = _node[_index]["animations"]["scaleX"].get("startValue", 1).asFloat();
	mainJoint->scaleY->startValue = _node[_index]["animations"]["scaleY"].get("startValue", 1).asFloat();
	mainJoint->scaleZ->startValue = _node[_index]["animations"]["scaleZ"].get("startValue", 1).asFloat();

	mainJoint->scaleX->loopType = static_cast<Animation<float>::LoopType>(_node[_index]["animations"]["scaleX"].get("loopType", 0).asInt());
	mainJoint->scaleY->loopType = static_cast<Animation<float>::LoopType>(_node[_index]["animations"]["scaleY"].get("loopType", 0).asInt());
	mainJoint->scaleZ->loopType = static_cast<Animation<float>::LoopType>(_node[_index]["animations"]["scaleZ"].get("loopType", 0).asInt());

	Json::Value scaleXTweens = _node[_index]["animations"]["scaleX"]["tweens"];
	for(Json::Value::ArrayIndex j = 0; j < scaleXTweens.size(); ++j){
		mainJoint->scaleX->tweens.push_back(new Tween<float>(
				scaleXTweens[j].get("deltaTime", 0).asFloat(),
				scaleXTweens[j].get("deltaValue", 0).asFloat(),
				static_cast<Easing::Type>(scaleXTweens[j].get("interpolation", 0).asInt())));
	}

	Json::Value scaleYTweens = _node[_index]["animations"]["scaleY"]["tweens"];
	for(Json::Value::ArrayIndex j = 0; j < scaleYTweens.size(); ++j){
		mainJoint->scaleY->tweens.push_back(new Tween<float>(
				scaleYTweens[j].get("deltaTime", 0).asFloat(),
				scaleYTweens[j].get("deltaValue", 0).asFloat(),
				static_cast<Easing::Type>(scaleYTweens[j].get("interpolation", 0).asInt())));
	}

	Json::Value scaleZTweens = _node[_index]["animations"]["scaleZ"]["tweens"];
	for(Json::Value::ArrayIndex j = 0; j < scaleZTweens.size(); ++j){
		mainJoint->scaleZ->tweens.push_back(new Tween<float>(
				scaleZTweens[j].get("deltaTime", 0).asFloat(),
				scaleZTweens[j].get("deltaValue", 0).asFloat(),
				static_cast<Easing::Type>(scaleZTweens[j].get("interpolation", 0).asInt())));
	}

	Json::Value voxels =  _node[_index]["voxels"];

	for(Json::Value::ArrayIndex v = 0; v < voxels.size(); ++v){
		mainJoint->mesh->pushVert(Vertex(
				voxels[v].get("x", 0).asFloat(),
				voxels[v].get("y", 0).asFloat(),
				voxels[v].get("z", 0).asFloat(),
				1, 1, 1, 1
			));	
		static_cast<VoxelMesh *>(mainJoint->mesh)->pushPoint(v);
	}

	Json::Value children = _node[_index]["children"];

	for(Json::Value::ArrayIndex c = 0; c < children.size(); ++c){
		mainJoint->addChild(parseJoint(children, c));
	}

	return mainJoint;
}
	
	
VoxelJoint * Resource::loadVoxelModel(std::string _jsonSrc){
	std::string jsonString = FileUtils::voxReadFile(_jsonSrc);

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(jsonString, root, false);

	if(!parsedSuccess){
		std::cout << "Unable to parse json";	
	}

	Json::Value array = root["joints"];

	VoxelJoint * mainJoint = new VoxelJoint(0, new VoxelMesh(GL_STATIC_DRAW), new Transform, nullptr);
	std::vector<VoxelJoint *> joints;

	for(Json::ArrayIndex i = 0; i < array.size(); ++i)  {
		joints.push_back(parseJoint(array, i));
	}

	for(unsigned long int i = 0; i < joints.size(); ++i){
		mainJoint->addChild(joints.at(i));
	}
	
	return mainJoint;
}

Box2DLevel* Resource::loadFizzXLevel(std::string _jsonSrc){
	std::string jsonString = FileUtils::voxReadFile(_jsonSrc);

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(jsonString, root, false);

	if(!parsedSuccess){
		std::cout << "Unable to parse json";	
	}

	Json::Value box2D = root["box2d"];

	std::map<std::string, b2Body *>bodiesMap;
	std::map<std::string, Json::Value>jointsMap;
	std::map<std::string, Json::Value>imageMap;
	std::map<std::string, Json::Value>atlasMap;

	Json::Value bodies = box2D["bodies"]["body"];

	for(Json::ArrayIndex i = 0; i < bodies.size(); i++){
		FizzXBody body;
		body.bodyDef.position.x = bodies[i].get("x", 0).asFloat();
		body.bodyDef.position.x = bodies[i].get("y", 0).asFloat();
		body.bodyDef.bullet		= bodies[i].get("bullet", false).asBool();
		body.image				= bodies[i].get("image", "").asString();
		
		Json::Value fixtures = box2D["fixtures"]["fixture"];
		for(Json::ArrayIndex j = 0; j < fixtures.size(); j++){
			FizzXFixture fixture;
			fixture.name = fixtures[j].get("name", "").asString();
			fixture.fixtureDef.isSensor = fixtures[j].get("isSensor", false).asBool(); 
			fixture.fixtureDef.restitution = fixtures[j].get("restitution", 1).asFloat();
			fixture.fixtureDef.friction = fixtures[j].get("friction", 1).asFloat(); 
			fixture.fixtureDef.density = fixtures[j].get("density", 1).asFloat(); 
			if(fixtures[j].get("shapeType", "").asString() == "circleShape"){
				b2CircleShape circle;
				circle.m_p.x = fixtures[j].get("circleX", 1).asFloat();
				circle.m_p.y = fixtures[j].get("circleY", 1).asFloat();
				circle.m_radius = fixtures[j].get("circleRadius", 1).asFloat();
				fixture.fixtureDef.shape = &circle;
			}else{
				//Assume polygonal
				b2PolygonShape shape;
				Json::Value verts = fixtures[j]["vertex"];
				b2Vec2 * vertArr = new b2Vec2[]();
				for(int v = 0; v < verts.size(); v++){
					vertArr[v] = b2Vec2(verts.get("x", 0).asFloat(), verts.get("y", 0).asFloat());
				}
				shape.Set(vertArr, verts.size());
				fixture.fixtureDef.shape = &shape;
			}
			body.fixtures.push_back(fixture);
		}
	}
}