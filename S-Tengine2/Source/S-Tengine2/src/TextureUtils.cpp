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

std::vector<glm::vec2> sweet::TextureUtils::getCurves(Texture * _texture, unsigned long int _threshold, unsigned long int _smooth){
	std::vector<glm::vec2> verts;
	for (unsigned long x = 0; x < _texture->width; ++x){
		for (unsigned long y = 0; y < _texture->height; ++y){
			//if (isEdge)
		}
	}
	return verts;
}

bool sweet::TextureUtils::isEdge(Texture * _texture, unsigned long int _x, unsigned long int _y, unsigned long int _threshold){
	float dx = 1.f / _texture->width;
	float dy = 1.f / _texture->height;

	int k = -1;
	float delta;
	long pixels[9]; // one channel 

	for (int x = -1; x < 2; ++x){
		for (int y = -1; y < 2; ++y){
			++k;
			// get average intensity
			pixels[k] = TextureUtils::getPixel(_texture, x, y);
		}
	}

	// average blah
	delta = (abs(pixels[1] - pixels[7]) +
		abs(pixels[5] - pixels[3]) +
		abs(pixels[0] - pixels[8]) +
		abs(pixels[2] - pixels[6])
		) / 4.;

	return delta <= _threshold;
}


unsigned char & sweet::TextureUtils::getPixel(Texture * _tex, unsigned long int _x, unsigned long int _y, unsigned long int _channel){
	return _tex->data[ (_x + _y*_tex->width) * _tex->channels + _channel ];
}
	
unsigned char * sweet::TextureUtils::getPixelPointer(Texture * _tex, unsigned long int _x, unsigned long int _y, unsigned long int _channel){
	return &_tex->data[ (_x + _y*_tex->width) * _tex->channels + _channel ];
}