#pragma once

#include <rng.h>
#include <NumberUtils.h>
#include <algorithm>

float sweet::NumberUtils::pingPong(float _val, float _min, float _max){
	float l = _max - _min; 
	float L = 2 * l;
	float T = fmod(_val, L);
 
	if(0 <= T && T < l){
		return T + _min;
	}else{
		return L - T + _min;
	}
}

float sweet::NumberUtils::randomFloat(float _min, float _max){
	return _min + randomFloat() * (_max - _min +1);
	return 0;
}
float sweet::NumberUtils::randomFloat(){
	return (float)ran_arr_cycle()/(INT_MAX/2);
}

int sweet::NumberUtils::randomInt(int _min, int _max){
	return _min + ran_arr_cycle() % (_max - _min +1);
}
int sweet::NumberUtils::randomInt(){
	return ran_arr_cycle();
}



long int sweet::NumberUtils::p[512];

void sweet::NumberUtils::seed(unsigned long int _seed){
	ran_start(_seed);

	// stuff for perlin noise permutation array
	for(unsigned long int i = 0; i < 256; ++i){
		ran_array(p, 256);
	}
	for(unsigned long int i = 0; i < 256; ++i){
		p[i] %= 256;
		p[256+i] = p[i];
	}
}

// C++ port of improved perlin function found at http://mrl.nyu.edu/~perlin/noise/
// ORIGINAL JAVA REFERENCE IMPLEMENTATION OF IMPROVED NOISE - COPYRIGHT 2002 KEN PERLIN
double sweet::NumberUtils::pNoise(double _x, double _y, double _z){
	
	// unit cube that contains point
	int X = (int)std::floor(_x) & 255,
		Y = (int)std::floor(_y) & 255,
		Z = (int)std::floor(_z) & 255;
	
	// relative XYZ of point in cube
	_x -= std::floor(_x);
	_y -= std::floor(_y);
	_z -= std::floor(_z);

	// compute fade curves for XYZ
	double 
		u = fade(_x),
		v = fade(_y),
		w = fade(_z);

	// hash coordinates of the 8 cube corners
	 int A	= p[X]+Y,
		 AA	= p[A]+Z,
		 AB	= p[A+1]+Z,
		 B	= p[X+1]+Y,
		 BA	= p[B]+Z,
		 BB	= p[B+1]+Z;
	 // and add blended results from 8 corners of cube
	 return lerp(w, lerp(v, lerp(u,  grad(p[AA],	_x,		_y,		_z),  
                                     grad(p[BA],	_x-1,	_y,		_z)),
                             lerp(u, grad(p[AB],	_x,		_y-1,	_z),
                                     grad(p[BB],	_x-1,	_y-1,	_z))),
                     lerp(v, lerp(u, grad(p[AA+1],	_x,		_y,		_z-1),
                                     grad(p[BA+1],	_x-1,	_y,		_z-1)),
                             lerp(u, grad(p[AB+1],	_x,		_y-1,	_z-1),
                                     grad(p[BB+1],	_x-1,	_y-1,	_z-1))));
}

double sweet::NumberUtils::fade(double _t) { return _t * _t * _t * (_t * (_t * 6 - 15) + 10); }
double sweet::NumberUtils::lerp(double _t, double _a, double _b) { return _a + _t * (_b - _a); }
double sweet::NumberUtils::grad(int _hash, double _x, double _y, double _z) {
    int h = _hash & 15;                      // Convert LO 4 bits of hash code into 12 gradient directions
    double u = h<8 ? _x : _y,
           v = h<4 ? _y : h==12||h==14 ? _x : _z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}