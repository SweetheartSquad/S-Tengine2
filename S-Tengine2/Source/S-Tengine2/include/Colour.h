#pragma once

#include <glm\glm.hpp>

class Colour : public glm::ivec3{
public:
	// 255,255,255
	Colour();
	// 0-255 range
	Colour(unsigned char _r, unsigned char _g, unsigned char _b);
	// 0-255 range
	Colour(glm::ivec3 _rgb);

	// returns a darker shade of this colour
	Colour darker();
	// returns a brighter shade of this colour
	Colour brighter();

	// converts to this colour to HSV, adds _hsv, then converts back to RGB and returns the result
	// HSV is in range (0-360, 0-100, 0-100)
	Colour hsvMod(glm::ivec3 _hsv);
	
	// Converts this colour to HSV colour space
	// HSV is in range (0-360, 0-100, 0-100)
	static glm::ivec3 rgbToHsv(glm::ivec3 _rgb);

	// Converts this colour to HSV colour space
	// HSV is in range (0-360, 0-100, 0-100)
	static glm::ivec3 hsvToRgb(glm::ivec3 _hsv);

	static Colour getRandomFromRgbRange(glm::ivec3 _min = glm::ivec3(0,0,0), glm::ivec3 _max = glm::ivec3(255,255,255));
	static Colour getRandomFromRgbMean(glm::ivec3 _mean = glm::ivec3(128,128,128), glm::ivec3 _halfRange = glm::ivec3(0,0,0));
	static Colour getRandomFromHsvRange(glm::ivec3 _min = glm::ivec3(0,0,0), glm::ivec3 _max = glm::ivec3(360,100,100));
	static Colour getRandomFromHsvMean(glm::ivec3 _mean = glm::ivec3(180,50,50), glm::ivec3 _halfRange = glm::ivec3(0,0,0));
};