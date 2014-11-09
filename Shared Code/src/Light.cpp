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

void Light::draw(MatrixStack * _matrixStack){
	//push transform
	_matrixStack->pushMatrix();
	_matrixStack->applyMatrix(transform->getModelMatrix());
	//pop transform
	_matrixStack->popMatrix();
}

void Light::update(){
	//Copy the transform to the data struct
	data.position = transform->translationVector;
}