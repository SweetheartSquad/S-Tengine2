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
	std::regex regex("\\n?([v]{1})\\s*([-]?\\d*[\\.]{1}\\d*)\\s([-]?\\d*[\\.]{1}\\d)*\\s*([-]?\\d*[\\.]{1}\\d)*|"
		"\\n?([v]{1}[n]{1})\\s*([-]?\\d*[\\.]{1}\\d*)\\s([-]?\\d*[\\.]{1}\\d)*\\s*([-]?\\d*[\\.]{1}\\d)*|"
		"\\n?([v]{1}[t]{1})\\s*([-]?\\d*[\\.]{1}\\d*)\\s([-]?\\d*[\\.]{1}\\d)*|"
		"\\n?([f]{1})\\s*(\\d*)[/]?(\\d*)[/]?(\\d*)\\s*(\\d*)[/]?(\\d*)[/]*(\\d*)\\s*(\\d*)[/]?(\\d*)[/]*(\\d*)\\s*");

	std::smatch match;

	std::vector<double> verts;
	std::vector<double> normals;
	std::vector<double> uvs;
	std::vector<double> faces;
	int count = 0;
	std::regex_search (fileSrc, match, regex);
	while (std::regex_search (fileSrc, match, regex)) {
		for(int i=0; i<match.size(); i++){
			if(static_cast<std::string>(match[i]).compare("v") == 0){
				verts.push_back(atof(static_cast<std::string>(match[i + 1]).c_str()));
				verts.push_back(atof(static_cast<std::string>(match[i + 2]).c_str()));
				verts.push_back(atof(static_cast<std::string>(match[i + 3]).c_str()));
			}else if(static_cast<std::string>(match[i]).compare("vn") == 0){
				normals.push_back(atof(static_cast<std::string>(match[i + 1]).c_str()));
				normals.push_back(atof(static_cast<std::string>(match[i + 2]).c_str()));
				normals.push_back(atof(static_cast<std::string>(match[i + 3]).c_str()));
			}else if(static_cast<std::string>(match[i]).compare("vt") == 0){
				uvs.push_back(atof(static_cast<std::string>(match[i + 1]).c_str()));
				uvs.push_back(atof(static_cast<std::string>(match[i + 2]).c_str()));
			}else if(static_cast<std::string>(match[i]).compare("f") == 0){
				faces.push_back(atof(static_cast<std::string>(match[i + 1]).c_str()));
				faces.push_back(atof(static_cast<std::string>(match[i + 2]).c_str()));
				faces.push_back(atof(static_cast<std::string>(match[i + 3]).c_str()));
				faces.push_back(atof(static_cast<std::string>(match[i + 4]).c_str()));
				faces.push_back(atof(static_cast<std::string>(match[i + 5]).c_str()));
				faces.push_back(atof(static_cast<std::string>(match[i + 6]).c_str()));
				faces.push_back(atof(static_cast<std::string>(match[i + 7]).c_str()));
				faces.push_back(atof(static_cast<std::string>(match[i + 8]).c_str()));
				faces.push_back(atof(static_cast<std::string>(match[i + 9]).c_str()));
			}
			count++;
		}
		fileSrc = match.suffix().str();
	}
	std::cout<<"asd";
}