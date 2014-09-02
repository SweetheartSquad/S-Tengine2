#pragma once

struct Vertex{
	float x,y,z;
	float red,green,blue,alpha;
	float nx, ny, nz;
	float u,v;
	Vertex(float x, float y, float z, 
					  float red, float green, float blue, float alpha, 
					  float normalX, float normalY, float normalZ, 
					  float u, float v){
		this->x = x;
		this->y = y;
		this->z = z;
		this->red = red;
		this->green = green;
		this->blue = blue;
		this->alpha = alpha;
		this->nx = normalX;
		this->ny = normalY;
		this->nz = normalZ;
		this->u = u;
		this->v = v;
	}

	Vertex(float x, float y, float z, 
				float red, float green, float blue, float alpha){
		this->x = x;
		this->y = y;
		this->z = z;
		this->red = red;
		this->green = green;
		this->blue = blue;
		this->alpha = alpha;
		this->nx = 0.0f;
		this->ny = 0.0f;
		this->nz = 0.0f;
		this->u = 0.0f;
		this->v = 0.0f;
	}

	Vertex(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
		this->red = (float)(std::rand()%255)/255.f;
		this->green = (float)(std::rand()%255)/255.f;
		this->blue = (float)(std::rand()%255)/255.f;

		std::cout << red << " " << green << " " << blue << std::endl;

		this->alpha = 1.0f;
		this->nx = 0.0f;
		this->ny = 0.0f;
		this->nz = 0.0f;
		this->u = 0.0f;
		this->v = 0.0f;
	}
};