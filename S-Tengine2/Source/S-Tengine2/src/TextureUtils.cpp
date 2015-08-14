#pragma once

#include <TextureUtils.h>
#include <Easing.h>


glm::vec2 vox::TextureUtils::interpolate(unsigned long int _x1, unsigned long int _y1, unsigned long int _x2, unsigned long int _y2){
	return glm::vec2(
		(_x1+_x2)/2.f,
		(_y1+_y2)/2.f
	);
}

glm::vec2 vox::TextureUtils::interpolate(unsigned long int _threshold, float _x1, float _y1, float _p1, float _x2, float _y2, float _p2){
	float t = (_threshold - _p1) / (_p2 - _p1);
	return glm::vec2(
		Easing::linear(t, _x1, _x2-_x1, 1),
		Easing::linear(t, _y1, _y2-_y1, 1)
	);
}


std::vector<glm::vec2> vox::TextureUtils::getMarchingSquaresContour(Texture * _texture, unsigned long int _threshold, bool _smooth){
	std::vector<glm::vec2> res;

	// lookup table describing edges based on marching square code
	// codes are binary 0000 through 1111, where each bit represents an adjacent pixel value being above _threshold
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

	// size of marching square
	unsigned long int size = 1;
	unsigned long int code, p0, p1, p2, p3;

	// iterate through texture
	for(unsigned long int y = 0; y < _texture->height - size; y += size){
		for(unsigned long int x = 0; x < _texture->width - size; x += size){
			// get adjacent pixel values (just uses first channel)
			p0 = vox::TextureUtils::getPixel(_texture, x, y);
			p1 = x+size < _texture->width ? vox::TextureUtils::getPixel(_texture, x+size, y) : 0;
			p2 = x+size < _texture->width && y+size < _texture->height ? vox::TextureUtils::getPixel(_texture, x+size, y+size) : 0;
			p3 = y+size < _texture->height ? vox::TextureUtils::getPixel(_texture, x, y+size) : 0;

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

			// handle ambiguous cases by checking average and flipping to the other if necessary
			if(code == 0x5 && (p0 + p1 + p2 + p3) <= _threshold*4){
				code = 0xA;
			}else if(code == 0xA && (p0 + p1 + p2 + p3) <= _threshold*4){
				code = 0x5;
			}

			// create the verts based on the code
			// right now the interpolate function just cares about the position, but it can be modified to take the pixel values into account
			for(unsigned long int edge : edgeTable[code]){
				glm::vec2 v;
				if(_smooth){
					switch(edge){
						case 0: v = interpolate(_threshold, x, y, p0, x + size, y, p1); break;
						case 1: v = interpolate(_threshold, x + size, y, p1, x + size, y + size, p2); break;
						case 2: v = interpolate(_threshold, x + size, y + size, p2, x, y + size, p3); break;
						case 3: v = interpolate(_threshold, x, y, p0, x, y + size, p3); break;
					}
				}else{
					switch(edge){
						case 0: v = interpolate(x, y, x + size, y); break;
						case 1: v = interpolate(x + size, y, x + size, y + size); break;
						case 2: v = interpolate(x + size, y + size, x, y + size); break;
						case 3: v = interpolate(x, y, x, y + size); break;
					}
				}

				// result will be offset by half a pixel and flipped vertically; fix that here to line up with texture coords
				v.x += 0.5;
				v.y = _texture->height - v.y - 0.5f;
				res.push_back(v);
			}
		}
	}
	return res;
}


unsigned char & vox::TextureUtils::getPixel(Texture * _tex, unsigned long int _x, unsigned long int _y, unsigned long int _channel){
	return _tex->data[ (_x + _y*_tex->width) * _tex->channels + _channel ];
}
	
unsigned char * vox::TextureUtils::getPixelPointer(Texture * _tex, unsigned long int _x, unsigned long int _y, unsigned long int _channel){
	return &_tex->data[ (_x + _y*_tex->width) * _tex->channels + _channel ];
}