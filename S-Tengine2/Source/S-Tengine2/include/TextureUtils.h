#pragma once

#include <Texture.h>
#include <glm\glm.hpp>

namespace vox{

class TextureUtils abstract{
public:
	// returns the vertices of contours in the image
	// returned vertices are in pairs forming edges; i.e. {edge1.v1, edge1.v2, edge2.v1, edge2.v2, etc.}
	static std::vector<glm::vec2> getMarchingSquaresContour(Texture * _tex, unsigned long int _threshold = 128, bool _smooth = false);
	
	// returns the pixel value at the given coordinates (passed by reference)
	// returned value is *_tex->data[ (_x + _y*_tex->width) * _tex->channels) + _channel]
	// note: no validation on image bounds or pixel depth
	static unsigned char & getPixel(Texture * _tex, unsigned long int _x, unsigned long int _y, unsigned long int _channel = 0);
	
	// returns a pointer to the pixel value at the given coordinates
	// returned pointer is _tex->data[ (_x + _y*_tex->width) * _tex->channels) + _channel]
	// note: no validation on image bounds or pixel depth
	static unsigned char * getPixelPointer(Texture * _tex, unsigned long int _x, unsigned long int _y, unsigned long int _channel = 0);

private:
	static glm::vec2 interpolate(unsigned long int _x1, unsigned long int _y1, unsigned long int _x2, unsigned long int _y2);
	static glm::vec2 interpolate(unsigned long int _threshold, float _x1, float _y1, float _p1, float _x2, float _y2, float _p2);
};

};