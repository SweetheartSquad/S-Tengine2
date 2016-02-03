#pragma once

#include <TextureUtils.h>
#include <Easing.h>

glm::vec2 sweet::TextureUtils::interpolate(unsigned long int _x1, unsigned long int _y1, unsigned long int _x2, unsigned long int _y2){
	return glm::vec2(
		(_x1+_x2)/2.f,
		(_y1+_y2)/2.f
	);
}

glm::vec2 sweet::TextureUtils::interpolate(unsigned long int _threshold, float _x1, float _y1, float _p1, float _x2, float _y2, float _p2){
	float t = (_threshold - _p1) / (_p2 - _p1);
	return glm::vec2(
		Easing::linear(t, _x1, _x2-_x1, 1),
		Easing::linear(t, _y1, _y2-_y1, 1)
	);
}


std::vector<glm::vec2> sweet::TextureUtils::getMarchingSquaresContour(Texture * _texture, unsigned long int _threshold, bool _smooth, bool _includeNormals){
	std::vector<glm::vec2> res;

	// lookup table describing edges based on marching square code
	std::vector<unsigned long int> edgeTable[16];
	//edgeTable[0x0] is empty b/c it's completely outside
	edgeTable[0x1].push_back(0); edgeTable[0x1].push_back(3);
	edgeTable[0x2].push_back(0); edgeTable[0x2].push_back(1);
	edgeTable[0x3].push_back(1); edgeTable[0x3].push_back(3);
	edgeTable[0x4].push_back(1); edgeTable[0x4].push_back(2);
	edgeTable[0x5].push_back(0); edgeTable[0x5].push_back(1); edgeTable[0x5].push_back(2); edgeTable[0x5].push_back(3);
	edgeTable[0x6].push_back(0); edgeTable[0x6].push_back(2);
	edgeTable[0x7].push_back(2); edgeTable[0x7].push_back(3);
	edgeTable[0x8].push_back(2); edgeTable[0x8].push_back(3);
	edgeTable[0x9].push_back(0); edgeTable[0x9].push_back(2);
	edgeTable[0xA].push_back(0); edgeTable[0xA].push_back(3); edgeTable[0xA].push_back(1); edgeTable[0xA].push_back(2);
	edgeTable[0xB].push_back(1); edgeTable[0xB].push_back(2);
	edgeTable[0xC].push_back(1); edgeTable[0xC].push_back(3);
	edgeTable[0xD].push_back(0); edgeTable[0xD].push_back(1);
	edgeTable[0xE].push_back(0); edgeTable[0xE].push_back(3);
	//edgeTable[0xF] is empty b/c it's completely inside

	bool reverse[16][2] = {
		{0,	0},
		{true,	0},
		{false,	0},
		{true,	0},
		{false,	0},
		{false,	false},
		{false,	0},
		{true,	0},
		{false,	0},
		{true,	0},
		{false,	true},
		{true,	0},
		{false,	0},
		{true,	0},
		{false,	0},
		{0,	0}
	};
	bool flipped = false;

	// size of marching square
	unsigned long int size = 1;

	// codes are binary 0000 through 1111, where each bit represents an adjacent pixel value being above _threshold
	unsigned long int code,
		p0, p1, p2, p3;

	// iterate through texture
	for(unsigned long int y = 0; y < _texture->height - size; y += size){
		for(unsigned long int x = 0; x < _texture->width - size; x += size){
			// get adjacent pixel values (just uses first channel)
			p0 = sweet::TextureUtils::getPixel(_texture, x, y);
			p1 = x+size < _texture->width ? sweet::TextureUtils::getPixel(_texture, x+size, y) : 0;
			p2 = x+size < _texture->width && y+size < _texture->height ? sweet::TextureUtils::getPixel(_texture, x+size, y+size) : 0;
			p3 = y+size < _texture->height ? sweet::TextureUtils::getPixel(_texture, x, y+size) : 0;

			// determine code for the current square location
			code = 0;
			if(p0 > _threshold){
				code += 1; // xxx1
			}if(p1 > _threshold){
				code += 2; // xx1x
			}if(p2 > _threshold){
				code += 4; // x1xx
			}if(p3 > _threshold){
				code += 8; // 1xxx
			}
			
			flipped = false;
			// handle ambiguous cases by checking average and flipping to the other if necessary
			if(code == 0x5 && (p0 + p1 + p2 + p3) <= _threshold*4){
				code = 0xA;
				flipped = true;
			}

			if(code == 0xA && (p0 + p1 + p2 + p3) <= _threshold*4){
				code = 0x5;
				//flipped = true;
			}

			// create the verts based on the code
			// right now the interpolate function just cares about the position, but it can be modified to take the pixel values into account
			for(unsigned long int i = 0 ; i < edgeTable[code].size(); i += 2){
				unsigned long int vertType1 = edgeTable[code].at(i);
				unsigned long int vertType2 = edgeTable[code].at(i+1);
				glm::vec2 v1, v2;
				if(_smooth){
					switch(vertType1){
						case 0: v1 = interpolate(_threshold, x, y, p0, x + size, y, p1); break;
						case 1: v1 = interpolate(_threshold, x + size, y, p1, x + size, y + size, p2); break;
						case 2: v1 = interpolate(_threshold, x + size, y + size, p2, x, y + size, p3); break;
						case 3: v1 = interpolate(_threshold, x, y, p0, x, y + size, p3); break;
					}
					switch(vertType2){
						case 0: v2 = interpolate(_threshold, x, y, p0, x + size, y, p1); break;
						case 1: v2 = interpolate(_threshold, x + size, y, p1, x + size, y + size, p2); break;
						case 2: v2 = interpolate(_threshold, x + size, y + size, p2, x, y + size, p3); break;
						case 3: v2 = interpolate(_threshold, x, y, p0, x, y + size, p3); break;
					}
				}else{
					switch(vertType1){
						case 0: v1 = interpolate(x, y, x + size, y); break;
						case 1: v1 = interpolate(x + size, y, x + size, y + size); break;
						case 2: v1 = interpolate(x + size, y + size, x, y + size); break;
						case 3: v1 = interpolate(x, y, x, y + size); break;
					}
					switch(vertType2){
						case 0: v2 = interpolate(x, y, x + size, y); break;
						case 1: v2 = interpolate(x + size, y, x + size, y + size); break;
						case 2: v2 = interpolate(x + size, y + size, x, y + size); break;
						case 3: v2 = interpolate(x, y, x, y + size); break;
					}
				}

				// results will be offset by half a pixel and flipped vertically; fix that here to line up with texture coords
				v1.x += 0.5;
				v1.y = _texture->height - v1.y - 0.5f;
				v2.x += 0.5;
				v2.y = _texture->height - v2.y - 0.5f;
				res.push_back(v1);
				res.push_back(v2);
				if(_includeNormals){
					res.push_back((v2 - v1) * glm::vec2(-1,1) * (float)((flipped ? !reverse[code][i/2] : reverse[code][i/2]) * 2 - 1));
				}
			}
		}
	}
	return res;
}

// TODO: define start scanning direction
std::vector<glm::vec2> sweet::TextureUtils::getTracedContour(Texture * _tex, unsigned long int _threshold){
	std::vector<glm::vec2> contour;
	
	// [direction][p1, p2, p3][x, y]
	const char moves[4][3][3] = {
		{ {-1, -1}, {0, -1}, {1, -1} }, // TOP
		{ {1, -1}, {1, 0}, {1, 1} }, // RIGHT
		{ {1, 1}, {0, 1}, {-1, 1} }, // BOTTOM
		{ {-1, 1}, {-1, 0}, {-1, -1} } // LEFT
	};

	glm::vec2 s;
	for (int x = 0; x < _tex->width; ++x){
		for (int y = 0; y < _tex->height; ++y){
			if (sweet::TextureUtils::getPixel(_tex, x, y) <= _threshold){
				s = glm::vec2(x,y);
				contour.push_back(glm::vec2(s.x, _tex->height - s.y));
				break;
			}
		}
		if (contour.size() > 0){
			break;
		}
	}

	if (contour.size() > 0){
		// stuff
		glm::vec2 p = s;
		glm::vec2 p1;
		glm::vec2 p2;
		glm::vec2 p3;
		int direction = 0;
		bool found;
		do {
			// max 3 tries allowed in search
			found = false;
			for (unsigned int numRotates = 0; numRotates < 3; numRotates++){
				// Find p1, p2, and p3
				p1 = glm::vec2(p.x + moves[direction][0][0], p.y + moves[direction][0][1]);
				p2 = glm::vec2(p.x + moves[direction][1][0], p.y + moves[direction][1][1]);
				p3 = glm::vec2(p.x + moves[direction][2][0], p.y + moves[direction][2][1]);

				bool p1V = p1.x >= 0 && p1.x < _tex->width && p1.y >= 0 && p1.y < _tex->height;
				bool p2V = p2.x >= 0 && p2.x < _tex->width && p2.y >= 0 && p2.y < _tex->height;
				bool p3V = p3.x >= 0 && p3.x < _tex->width && p3.y >= 0 && p3.y < _tex->height;

				if (p1V && sweet::TextureUtils::getPixel(_tex, p1.x, p1.y) <= _threshold){
					contour.push_back(glm::vec2(p1.x, _tex->height - p1.y));
					// move up and left
					p = p1;
					direction = ((direction - 1) % 4 + 4) % 4;
					found = true;
				}
				else if (p2V && sweet::TextureUtils::getPixel(_tex, p2.x, p2.y) <= _threshold){
					contour.push_back(glm::vec2(p2.x, _tex->height - p2.y));
					// move up
					p = p2;
					found = true;
				}
				else if (p3V && sweet::TextureUtils::getPixel(_tex, p3.x, p3.y) <= _threshold){
					contour.push_back(glm::vec2(p3.x, _tex->height - p3.y));
					// move right and left
					p = p3;
					found = true;
				}
				if (found){
					break;
				}else{
					direction = (direction + 1) % 4;
				}
				
			}

			if (!found){
				// Pixel is isolated, and probably the starting pixel
				break;
			}
		} while (!(p.x == s.x && p.y == s.y));

		// If the function actually found a contour, remove the last point? (which is s again)
		if (found){
			contour.pop_back();
		}
	}
	return contour;
}


unsigned char & sweet::TextureUtils::getPixel(Texture * _tex, unsigned long int _x, unsigned long int _y, unsigned long int _channel){
	return _tex->data[ (_x + _y*_tex->width) * _tex->channels + _channel ];
}
	
unsigned char * sweet::TextureUtils::getPixelPointer(Texture * _tex, unsigned long int _x, unsigned long int _y, unsigned long int _channel){
	return &_tex->data[ (_x + _y*_tex->width) * _tex->channels + _channel ];
}



void sweet::TextureUtils::setPixel(Texture * _tex, unsigned long int _x, unsigned long int _y, glm::uvec4 _colour){
	unsigned char * p = getPixelPointer(_tex, _x, _y, 0);
	*(p + 0) = _colour.r;
	*(p + 1) = _colour.g;
	*(p + 2) = _colour.b;
	*(p + 3) = _colour.a;
}
void sweet::TextureUtils::setPixel(Texture * _tex, unsigned long int _x, unsigned long int _y, glm::uvec3 _colour){
	unsigned char * p = getPixelPointer(_tex, _x, _y, 0);
	*(p + 0) = _colour.r;
	*(p + 1) = _colour.g;
	*(p + 2) = _colour.b;
}
void sweet::TextureUtils::setPixel(Texture * _tex, unsigned long int _x, unsigned long int _y, glm::uvec2 _colour){
	unsigned char * p = getPixelPointer(_tex, _x, _y, 0);
	*(p + 0) = _colour.r;
	*(p + 1) = _colour.g;
}
void sweet::TextureUtils::setPixel(Texture * _tex, unsigned long int _x, unsigned long int _y, unsigned char _colour){
	unsigned char * p = getPixelPointer(_tex, _x, _y, 0);
	*(p + 0) = _colour;
}

unsigned char sweet::TextureUtils::getMax(Texture * _tex, unsigned long int _channel){
	unsigned char res = 0;
	for(unsigned long int x = 0; x < _tex->width; ++x){
	for(unsigned long int y = 0; y < _tex->height; ++y){
		res = glm::max(res, getPixel(_tex, x, y, _channel));
	}
	}
	return res;
}

unsigned char sweet::TextureUtils::getMin(Texture * _tex, unsigned long int _channel){
	unsigned char res = 255;
	for(unsigned long int x = 0; x < _tex->width; ++x){
	for(unsigned long int y = 0; y < _tex->height; ++y){
		res = glm::min(res, getPixel(_tex, x, y, _channel));
	}
	}
	return res;
}