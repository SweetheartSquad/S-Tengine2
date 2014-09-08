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

	void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) override;
	void update(void) override;

	void setColour(float red, float green, float blue, float alpha);
	void setLeftColour(float red, float green, float blue, float alpha);
	void setRightColour(float red, float green, float blue, float alpha);
	void setTopColour(float red, float green, float blue, float alpha);
	void setBottomColour(float red, float green, float blue, float alpha);
	void setFrontColour(float red, float green, float blue, float alpha);
	void setBackColour(float red, float green, float blue, float alpha);
	void setVertexColour(int index, float red, float green, float blue, float alpha);
};

