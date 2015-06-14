#pragma once

#include <iostream>
#include "MeshEntity.h"

/*****************************
*
* A simple plane Entity.
*
******************************/

class Plane : public MeshEntity{
public:
	virtual char * getTypename(){
		return "Plane";
	}

	/**
	* Constructs the plane. This is where the plane's verticies, normals and UV's are initialized
	*
	* @param _center The plane's center position in world space
	* @param _size The plane's scale
	*/
	Plane(glm::vec3 _center = glm::vec3(0.f, 0.f, 0.f), float _scale = 1.f);
	~Plane(void);

	/**
	* Sets the color for the entire plane.
	*
	* @param _red The value for the red channel. Value from 0 to 1
	* @param _green The value for the green channel. Value from 0 to 1
	* @param _blue The value for the blue channel. Value from 0 to 1
	* @param _alpha The value for the alpha channel. Value from 0 to 1
	*/
	void setColour(float _red, float _green, float _blue, float _alpha);

	/**
	* Sets the color for the vertex located at _index in the plane's vertices vector.

	* @param _index The index of the vertex
	* @param _red The value for the red channel. Value from 0 to 1
	* @param _green The value for the green channel. Value from 0 to 1
	* @param _blue The value for the blue channel. Value from 0 to 1
	* @param _alpha The value for the alpha channel. Value from 0 to 1
	*/
	void setVertexColour(int _index, float _red, float _green, float _blue, float _alpha);
}; 