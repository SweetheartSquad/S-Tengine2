typedef struct Vertex
{
	float x,y,z;
	float red,green,blue,alpha;
	float nx, ny, nz;
	float u,v;
	Vertex(float x, float y, float z, 
					  float red, float green, float blue, float alpha, 
					  float normalX, float normalY, float normalZ, 
					  float u, float v)
	{
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
				float red, float green, float blue, float alpha)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->red = red;
		this->green = green;
		this->blue = blue;
		this->alpha = alpha;
		this->nx = 0;
		this->ny = 0;
		this->nz = 0;
		this->u = 0;
		this->v = 0;
	}

	Vertex(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->red = 1.0f;
		this->green = 1.0f;
		this->blue = 1.0f;
		this->alpha = 1.0f;
		this->nx = 0;
		this->ny = 0;
		this->nz = 0;
		this->u = 0;
		this->v = 0;
	}
};