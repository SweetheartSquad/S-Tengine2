#pragma once

/***************************************************************
*
* A data struct for use with framebuffers and render surfaces
*
****************************************************************/

struct FrameBufferVertex{
	
	float x, y;
	float u, v;

	FrameBufferVertex(float _x, float _y, float _u, float _v){
		x = _x;
		y = _y;
		u = _u;
		v = _v;
	}
};