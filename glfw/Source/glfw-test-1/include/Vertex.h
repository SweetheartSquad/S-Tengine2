#pragma once

struct Vertex{
	float x,y,z;
	float red,green,blue,alpha;
	float nx, ny, nz;
	float u,v;
	Vertex(float _x, float _y, float _z,
		float _red, float _green, float _blue, float _alpha,
		float _normalX, float _normalY, float _normalZ,
		float _u, float _v){
			this->x = _x;
			this->y = _y;
			this->z = _z;
			this->red = _red;
			this->green = _green;
			this->blue = _blue;
			this->alpha = _alpha;
			this->nx = _normalX;
			this->ny = _normalY;
			this->nz = _normalZ;
			this->u = _u;
			this->v = _v;
	}

	Vertex(float _x, float _y, float _z,
		float _red, float _green, float _blue, float _alpha){
			this->x = _x;
			this->y = _y;
			this->z = _z;
			this->red = _red;
			this->green = _green;
			this->blue = _blue;
			this->alpha = _alpha;
			this->nx = 0.0f;
			this->ny = 0.0f;
			this->nz = 0.0f;
			this->u = 0.0f;
			this->v = 0.0f;
	}

	//will generate random verte_x colours
	Vertex(float _x, float _y, float _z){
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->red = static_cast<float>(std::rand()%255)/255.f;
		this->green = static_cast<float>(std::rand()%255)/255.f;
		this->blue = static_cast<float>(std::rand()%255)/255.f;
		this->alpha = 1.0f;
		this->nx = 0.0f;
		this->ny = 0.0f;
		this->nz = 0.0f;
		this->u = 0.0f;
		this->v = 0.0f;
	}
};