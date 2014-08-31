#include "Vox.h"

std::vector<glm::mat4>* vox::matrixStack = new std::vector<glm::mat4>;
glm::mat4 vox::currentModelMatrix = glm::mat4(1);

double vox::lastTimestamp = 0;
double vox::deltaTimeCorrection = 1;

/////////// Matrix Stack Begin //////////////
void vox::popMatrix(){
	if(matrixStack->size() > 0){	
		if(matrixStack->size()>0){
			currentModelMatrix = matrixStack->at(matrixStack->size()-1);
			matrixStack->pop_back();
		}else{
			currentModelMatrix = glm::mat4(1);
			throw;
		}
	}else{
		currentModelMatrix = glm::mat4(1);
		throw;
	}
}

void vox::pushMatrix(){
	matrixStack->push_back(currentModelMatrix);
}

glm::mat4 vox::getCurrentMatrix(){
	return currentModelMatrix;
}

void vox::scale(glm::mat4 scaleMatrix){
	currentModelMatrix = currentModelMatrix * scaleMatrix;
}

void vox::rotate(glm::mat4 rotationMatrix){
	currentModelMatrix = currentModelMatrix * rotationMatrix;
}

void vox::translate(glm::mat4 translationMatrix){
	currentModelMatrix = currentModelMatrix * translationMatrix;
}

void vox::applyMatrix(glm::mat4 modelMatrix){
	currentModelMatrix = currentModelMatrix * modelMatrix;
}

void vox::clearMatrixStack(){
	matrixStack->clear();
}

/////////// Delta Time Begin //////////////
void vox::calculateDeltaTimeCorrection(){
	double targetFrameDuration = static_cast<double>(1)/FPS;
	double time = glfwGetTime();
	double deltaTime = time - lastTimestamp;
	deltaTimeCorrection = deltaTime/(targetFrameDuration);
	lastTimestamp = time;
}

void vox::destruct(){
	delete matrixStack;
}
