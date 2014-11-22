#include "Light.h"
#include "Transform.h"

Light::Light():
	NodeTransformable(new Transform()),
	Node()
{
}

Light::Light(Transform * _transform) :
	NodeTransformable(_transform),
	Node()
{
}

Light::~Light() {
	delete transform;
	transform = nullptr;
}

void Light::update(){
	//Copy the transform to the data struct
	data.position = transform->translationVector;
}