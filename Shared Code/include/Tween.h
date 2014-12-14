#pragma once

#include "node/Node.h"
#include "Easing.h"
#include <glm/gtx/quaternion.hpp>

template<typename T>
class Tween : public Node
{
public:
	Tween(float deltaTime, T deltaValue, Easing::Type _interpolation);
	~Tween();

	float deltaTime;
	T deltaValue;

	Easing::Type interpolation;


private:

};

template<typename T>
Tween<T>::Tween(float _deltaTime, T _deltaValue, Easing::Type _interpolation) :
	deltaTime(_deltaTime),  
	deltaValue(_deltaValue), 
	interpolation(_interpolation)
{
}

template<typename T>
Tween<T>::~Tween(){
}