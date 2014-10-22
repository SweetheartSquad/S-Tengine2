#pragma once

#include<iostream>
#include "Entity.h"

class Plane : public Entity{
public:
	virtual char * getTypename(){
		return "Plane";
	}

	Plane(glm::vec3 _center, float _size);
	~Plane(void);

	void draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*> _lights) override;
	void update(void) override;

	void setColour(float _red, float _green, float _blue, float _alpha);
	void setVertexColour(int _index, float _red, float _green, float _blue, float _alpha);
}; 