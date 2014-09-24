#include "Resource.h"
#include <FileUtils.h>


Resource::Resource(){}
Resource::~Resource(){}

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
	std::vector<glm::vec3> faces;

	std::regex vertRegex    ("([v]{1})\\s{1}([-]?\\d*[\\.]{1}\\d*)\\s([-]?\\d*[\\.]{1}\\d*)\\s{1}([-]?\\d*[\\.]{1}\\d*)");
	std::regex normalRegex  ("([v]{1}[n]{1})\\s{1}([-]?\\d*[\\.]{1}\\d*)\\s([-]?\\d*[\\.]{1}\\d*)\\s{1}([-]?\\d*[\\.]{1}\\d*)");
	std::regex uvRegex      ("([v]{1}[t]{1})\\s{1}([-]?\\d*[\\.]{1}\\d*)\\s([-]?\\d*[\\.]{1}\\d*)");
	std::regex faceRegex    ("([f]{1})\\s{1}(\\d*)[/]{1}(\\d*)[/]{1}(\\d*)\\s{1}(\\d*)[/]{1}(\\d*)[/]{1}(\\d*)\\s{1}(\\d*)[/]{1}(\\d*)[/]{1}(\\d*)");

	std::string line;    
    /*while (std::getline(stream, line)) { 
		if(std::regex_search(line, match, vertRegex)){
			verts.push_back(glm::vec3(atof(static_cast<std::string>(match[1]).c_str()),
							atof(static_cast<std::string>(match[2]).c_str()),
							atof(static_cast<std::string>(match[3]).c_str())));
		}else if(std::regex_search(line, match, normalRegex)){
			normals.push_back(glm::vec3(atof(static_cast<std::string>(match[1]).c_str()),
							atof(static_cast<std::string>(match[2]).c_str()),
							atof(static_cast<std::string>(match[3]).c_str())));	
		}else if(std::regex_search(line, match, uvRegex)){
			uvs.push_back(glm::vec2(atof(static_cast<std::string>(match[1]).c_str()),
							atof(static_cast<std::string>(match[2]).c_str())));	
		}else if(std::regex_search(line, match, faceRegex)){
			faces.push_back(glm::vec3(atof(static_cast<std::string>(match[1]).c_str()),
							atof(static_cast<std::string>(match[2]).c_str()),
							atof(static_cast<std::string>(match[3]).c_str())));
			faces.push_back(glm::vec3(atof(static_cast<std::string>(match[4]).c_str()),
							atof(static_cast<std::string>(match[5]).c_str()),
							atof(static_cast<std::string>(match[6]).c_str())));
			faces.push_back(glm::vec3(atof(static_cast<std::string>(match[7]).c_str()),
							atof(static_cast<std::string>(match[8]).c_str()),
							atof(static_cast<std::string>(match[9]).c_str())));
		}
    }*/

	while (std::getline(stream, line)) { 
		char* lineId = new char[line.size()];
		sscanf(line.c_str(), "%s", lineId);
		std::string lineIdStr(lineId);
		if(lineIdStr.compare("v") == 0){
			glm::vec3 tempVert;
			sscanf(line.c_str(), "%f %f %f", &tempVert.x, &tempVert.y, &tempVert.z);
			verts.push_back(tempVert);
		}else if(lineIdStr.compare("vn") == 0){
			glm::vec3 tempNorm;
			sscanf(line.c_str(), "%f %f %f", &tempNorm.x, &tempNorm.y, &tempNorm.z);
			normals.push_back(tempNorm);
		}else if(lineIdStr.compare("vt") == 0){
			glm::vec2 tempUv;
			sscanf(line.c_str(), "%f %f", &tempUv.x, &tempUv.y);
			uvs.push_back(tempUv);		
		}else if(lineIdStr.compare("f") == 0){
		
		}
    }

	std::cout<<"asd";
}