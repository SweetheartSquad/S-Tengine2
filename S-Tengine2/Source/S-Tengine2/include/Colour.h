#pragma once

#include <glm\glm.hpp>

class Colour : public glm::uvec3{
public:
	// 255,255,255
	Colour();
	// 0-255 range
	Colour(unsigned char _r, unsigned char _g, unsigned char _b);
	// 0-255 range
	Colour(glm::uvec3 _rgb);

	// returns a darker shade of this colour
	Colour darker();
	// returns a brighter shade of this colour
	Colour brighter();

	// converts to this colour to HSV, adds _hsv, then converts back to RGB and returns the result
	// HSV is in range (0-360, 0-100, 0-100)
	Colour hsvMod(glm::uvec3 _hsv);
	
	// Converts this colour to HSV colour space
	// HSV is in range (0-360, 0-100, 0-100)
	static glm::uvec3 rgbToHsv(glm::uvec3 _rgb);

	// Converts this colour to HSV colour space
	// HSV is in range (0-360, 0-100, 0-100)
	static glm::uvec3 hsvToRgb(glm::uvec3 _hsv);

	static Colour getRandomInRgbRange(glm::uvec3 _min = glm::uvec3(0,0,0), glm::uvec3 _max = glm::uvec3(255,255,255));
	static Colour getRandomFromRgbMean(glm::uvec3 _mean = glm::uvec3(128,128,128), glm::uvec3 _halfRange = glm::uvec3(0,0,0));
};