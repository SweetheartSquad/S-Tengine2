#include "Light.h"

Light::Light(Transform * _transform) :
	transform(_transform)
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