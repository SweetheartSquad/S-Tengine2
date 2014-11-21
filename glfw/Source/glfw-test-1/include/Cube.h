#pragma once

#include <iostream>

#include "Entity.h"

/**
* A simple cube Entity.
*/
class Cube : public Entity{
public:

	virtual char * getTypename(){
		return "Cube";
	}

	/**
	* Constructs the cube. This is where the Cube's verticies, normals and UV's are initialized
	*
	* @param _center The cube's center position in world space
	* @param _size The cube's scale
	*/
	Cube(glm::vec3 _center, float _scale);

	~Cube(void);

	/**
	* Sets the color for the entire cube.
	*
	* @param _red The value for the red channel. Value from 0 to 1
	* @param _green The value for the green channel. Value from 0 to 1
	* @param _blue The value for the blue channel. Value from 0 to 1
	* @param _alpha The value for the alpha channel. Value from 0 to 1
	*/
	void setColour(float _red, float _green, float _blue, float _alpha);

	/**
	* Sets the color for cube's left face
	*
	* @param _red The value for the red channel. Value from 0 to 1
	* @param _green The value for the green channel. Value from 0 to 1
	* @param _blue The value for the blue channel. Value from 0 to 1
	* @param _alpha The value for the alpha channel. Value from 0 to 1
	*/
	void setLeftColour(float _red, float _green, float _blue, float _alpha);

	/**
	* Sets the color for cube's right face.
	*
	* @param _red The value for the red channel. Value from 0 to 1
	* @param _green The value for the green channel. Value from 0 to 1
	* @param _blue The value for the blue channel. Value from 0 to 1
	* @param _alpha The value for the alpha channel. Value from 0 to 1
	*/
	void setRightColour(float _red, float _green, float _blue, float _alpha);

	/**
	* Sets the color for cube's top face.
	*
	* @param _red The value for the red channel. Value from 0 to 1
	* @param _green The value for the green channel. Value from 0 to 1
	* @param _blue The value for the blue channel. Value from 0 to 1
	* @param _alpha The value for the alpha channel. Value from 0 to 1
	*/
	void setTopColour(float _red, float _green, float _blue, float _alpha);

	/**
	* Sets the color for cube's bottom face.
	*
	* @param _red The value for the red channel. Value from 0 to 1
	* @param _green The value for the green channel. Value from 0 to 1
	* @param _blue The value for the blue channel. Value from 0 to 1
	* @param _alpha The value for the alpha channel. Value from 0 to 1
	*/
	void setBottomColour(float _red, float _green, float _blue, float _alpha);

	/**
	* Sets the color for cube's front face.
	*
	* @param _red The value for the red channel. Value from 0 to 1
	* @param _green The value for the green channel. Value from 0 to 1
	* @param _blue The value for the blue channel. Value from 0 to 1
	* @param _alpha The value for the alpha channel. Value from 0 to 1
	*/
	void setFrontColour(float _red, float _green, float _blue, float _alpha);

	/**
	* Sets the color for cube's back face.
	*
	* @param _red The value for the red channel. Value from 0 to 1
	* @param _green The value for the green channel. Value from 0 to 1
	* @param _blue The value for the blue channel. Value from 0 to 1
	* @param _alpha The value for the alpha channel. Value from 0 to 1
	* @return
	*/
	void setBackColour(float _red, float _green, float _blue, float _alpha);

	/**
	* Sets the color for the vertex located at _index in the cubes vertices vector.

	* @param _index The index of the vertex
	* @param _red The value for the red channel. Value from 0 to 1
	* @param _green The value for the green channel. Value from 0 to 1
	* @param _blue The value for the blue channel. Value from 0 to 1
	* @param _alpha The value for the alpha channel. Value from 0 to 1
	*/
	void setVertexColour(int _index, float _red, float _green, float _blue, float _alpha);
};
