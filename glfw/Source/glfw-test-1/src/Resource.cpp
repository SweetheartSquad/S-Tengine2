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
		"\\n?([v]{1}[t]{1})\\s*([-]?\\d*[\\.]{1}\\d*)\\s([-]?\\d*[\\.]{1}\\d)*");

	std::smatch match;

	std::vector<double> verts;
	std::vector<double> normals;
	std::vector<double> uvs;

	while (std::regex_search (fileSrc, match, regex)) {
		for(int i=1; i<match.size(); i++){
			if(static_cast<std::string>(match[i]).compare("v") == 0){
				verts.push_back(atof(static_cast<std::string>(match[i]).c_str()));		
			}else if(static_cast<std::string>(match[i]).compare("vn") == 0){
				normals.push_back(atof(static_cast<std::string>(match[i]).c_str()));
			}else if(static_cast<std::string>(match[i]).compare("vt") == 0){
				uvs.push_back(atof(static_cast<std::string>(match[i]).c_str()));
			}
		}
		fileSrc = match.suffix().str();
	}
		
	std::cout<<"sd";
}