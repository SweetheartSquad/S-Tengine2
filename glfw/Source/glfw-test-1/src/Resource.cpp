#include "Resource.h"
#include <FileUtils.h>
#include <MeshInterface.h>

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

unsigned char* Resource::loadImage(const char* _src, int _width, int _height, int _SOILLoadMode){
	return SOIL_load_image(_src, &_width, &_height, 0, _SOILLoadMode);
}

void Resource::freeImageData(unsigned char* _image){
	SOIL_free_image_data(_image);
}

void Resource::loadMeshFromObj(std::string _objSrc){
	std::string fileSrc = FileUtils::voxReadFile(_objSrc);
	std::istringstream stream(fileSrc);
	std::smatch match;
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<Face> faces;
	std::string line;
	std::regex faceRegex("(\\d{1})[/]{1}(\\d?)[/]{1}(\\d?)");
	bool faceFormatChecked = false;
	ObjFaceFormat faceStructure = VERTS;

	while (std::getline(stream, line)) {
		char* lineId = new char[line.size()];
		sscanf(line.c_str(), "%s", lineId);
		std::string lineIdStr(lineId);
		if(lineIdStr.compare("v") == 0){
			glm::vec3 tempVert(1);
			sscanf(line.c_str(), "%*s%f %f %f", &tempVert.x, &tempVert.y, &tempVert.z);
			verts.push_back(tempVert);
		}else if(lineIdStr.compare("vn") == 0){
			glm::vec3 tempNorm(1);
			sscanf(line.c_str(), "%*s%f %f %f", &tempNorm.x, &tempNorm.y, &tempNorm.z);
			normals.push_back(tempNorm);
		}else if(lineIdStr.compare("vt") == 0){
			glm::vec2 tempUv(1);
			sscanf(line.c_str(), "%*s%f %f", &tempUv.x, &tempUv.y);
			uvs.push_back(tempUv);
		}else if(lineIdStr.compare("f") == 0){
			if(!faceFormatChecked){
				std::smatch matches;
				std::regex_search(line, matches, faceRegex);
				if(matches[2].str().size()>0 && matches[3].str().size()>0){
					faceStructure = VERTS_UVS_NORMALS;
				}else if(matches[2].str().size()>0 && !matches[3].str().size()>0){
					faceStructure = VERTS_UVS;
				}else if(!matches[2].str().size()>0 && matches[3].str().size()>0){
					faceStructure = VERTS_NORMALS;
				}
				faceFormatChecked = true;
			}else{
				Face face;
				std::string faceVert;
				std::string faceVerts;
				switch(faceStructure){
				case VERTS : faceVert = " %d//";
					faceVerts = "%*s" + faceVert + faceVert + faceVert;
					sscanf(line.c_str(), faceVerts.c_str(), &face.f1Vert, &face.f2Vert, &face.f2Vert);
					break;
				case VERTS_UVS : faceVert = " %d/%d/";
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
				faces.push_back(face);
			}

			TriMesh mesh();
			for(Face face : faces){
			}
		}
	}
}