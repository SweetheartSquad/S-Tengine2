#pragma once

#include <iostream>

#include "Entity.h"

class Cube:public Entity{
public:
	virtual char * getTypename(){
		return "Cube";
	}

	Cube(glm::vec3 center, float size);
	~Cube(void);

	void draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix) override;
	void update(void) override;

	void setColour(float _red, float _green, float _blue, float _alpha);
	void setLeftColour(float _red, float _green, float _blue, float _alpha);
	void setRightColour(float _red, float _green, float _blue, float _alpha);
	void setTopColour(float _red, float _green, float _blue, float _alpha);
	void setBottomColour(float _red, float _green, float _blue, float _alpha);
	void setFrontColour(float _red, float _green, float _blue, float _alpha);
	void setBackColour(float _red, float _green, float _blue, float _alpha);
	void setVertexColour(int _index, float _red, float _green, float _blue, float _alpha);
};
