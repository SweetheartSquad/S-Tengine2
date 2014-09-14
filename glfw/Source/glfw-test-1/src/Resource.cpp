#include "Resource.h"

Resource::Resource(){}

Resource::~Resource(){}

unsigned char* Resource::loadImage(const char* _src, int _width, int _height, int _SOILLoadMode){
	return SOIL_load_image(_src, &_width, &_height, 0, _SOILLoadMode);
}

void Resource::freeImageData(unsigned char* _image){
	SOIL_free_image_data(_image);
}