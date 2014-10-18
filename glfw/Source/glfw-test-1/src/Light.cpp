#include "Light.h"

Light::Light():
	transform(new Transform()),
	Node()
{
}

Light::Light(Transform * _transform) :
	transform(_transform),
	Node()
{
}

Light::~Light() {
	delete transform;
	transform = nullptr;
}

void Light::draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix){
	//push transform
	vox::pushMatrix();
	vox::applyMatrix(transform->getModelMatrix());
	//pop transform
	vox::popMatrix();
}

void Light::update(){
	//Copy the transform to the data struct
	data.position = transform->translationVector;
}