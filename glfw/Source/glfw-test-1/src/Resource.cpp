#pragma once

#include <SOIL.h>
#include <regex>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

#include <soil.h>

#include "MeshInterface.h"
#include "Resource.h"
#include "FileUtils.h"

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

TriMesh* Resource::loadMeshFromObj(std::string _objSrc){

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