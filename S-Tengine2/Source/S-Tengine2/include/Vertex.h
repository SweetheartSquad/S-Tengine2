#pragma once

#include <glm/glm.hpp>
#include <ostream>

struct Vertex{
	/** Position */
	float x, y, z;
	/** Colour */
	float red, green, blue, alpha;
	/** Normal */
	float nx, ny, nz;
	/** Texture UV */
	float u, v;

	Vertex(float _x, float _y, float _z,
		float _red, float _green, float _blue, float _alpha,
		float _normalX, float _normalY, float _normalZ,
		float _u, float _v) :
	x(_x),
		y(_y),
		z(_z),
		red(_red),
		green(_green),
		blue(_blue),
		alpha(_alpha),
		nx(_normalX),
		ny(_normalY),
		nz(_normalZ),
		u(_u),
		v(_v)
	{
	}

	Vertex(float _x, float _y, float _z,
		float _red, float _green, float _blue, float _alpha) :
	x(_x),
		y(_y),
		z(_z),
		red(_red),
		green(_green),
		blue(_blue),
		alpha(_alpha),
		nx(0.f),
		ny(0.f),
		nz(0.f),
		u(0.f),
		v(0.f)
	{
	}

	explicit Vertex(glm::vec3 _vert) :
		x(_vert.x),
		y(_vert.y),
		z(_vert.z),
		red(1.f),
		green(1.f),
		blue(1.f),
		alpha(1.f),
		nx(0.f),
		ny(0.f),
		nz(0.f),
		u(0.f),
		v(0.f)
	{
	}

	Vertex(glm::vec3 _vert, glm::vec2 _uvs) :
		x(_vert.x),
		y(_vert.y),
		z(_vert.z),
		red(1.f),
		green(1.f),
		blue(1.f),
		alpha(1.f),
		nx(0.f),
		ny(0.f),
		nz(0.f),
		u(_uvs.x),
		v(_uvs.y)
	{
	}

	Vertex(glm::vec3 _vert, glm::vec3 _normal) :
		x(_vert.x),
		y(_vert.y),
		z(_vert.z),
		red(1.f),
		green(1.f),
		blue(1.f),
		alpha(1.f),
		nx(_normal.x),
		ny(_normal.y),
		nz(_normal.z),
		u(0.f),
		v(0.f)
	{
	}

	Vertex(glm::vec3 _vert, glm::vec2 _uvs, glm::vec3 _normal) :
		x(_vert.x),
		y(_vert.y),
		z(_vert.z),
		red(1.f),
		green(1.f),
		blue(1.f),
		alpha(1.f),
		nx(_normal.x),
		ny(_normal.y),
		nz(_normal.z),
		u(_uvs.x),
		v(_uvs.y)
	{
	}

	/** Will generate random vertex colours */
	Vertex(float _x, float _y, float _z) :
		x(_x),
		y(_y),
		z(_z),
		red(1.f),
		green(1.f),
		blue(1.f),
		alpha(1.f),
		nx(0.f),
		ny(0.f),
		nz(0.f),
		u(0.f),
		v(0.f)
	{
	}

	friend std::ostream& operator<<(std::ostream& os, const Vertex& obj){
		return os
			<< "x: " << obj.x
			<< " y: " << obj.y
			<< " z: " << obj.z
			<< " red: " << obj.red
			<< " green: " << obj.green
			<< " blue: " << obj.blue
			<< " alpha: " << obj.alpha
			<< " nx: " << obj.nx
			<< " ny: " << obj.ny
			<< " nz: " << obj.nz
			<< " u: " << obj.u
			<< " v: " << obj.v;
	}
};