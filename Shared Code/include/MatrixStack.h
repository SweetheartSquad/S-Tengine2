#pragma once

#include <vector>
#include <glm\glm.hpp>

class MatrixStack{

public:

	MatrixStack();
	~MatrixStack();

	/** Model matrix stack (replaces the OpenGL stack) */
	std::vector<glm::mat4> matrixStack;
	/** Current model matrix */
	glm::mat4 currentModelMatrix;
	/** View Matrix*/
	glm::mat4 viewMatrix;
	/**Projection matrix*/ 
	glm::mat4 projectionMatrix;

	//void calculateModelMatrix();
	/** Pushes the current model matrix onto the stack */
	void pushMatrix();
	/** Restores the current model matrix to the last stored value and pops it off the stack */
	void popMatrix();
	/** Returns the value of the current model matrix */
	glm::mat4 getCurrentMatrix();
	/** Sets the calue of the current model matrix to the identity matrix */
	void resetCurrentMatrix();
	/** Clears the model matrix stack */
	void clearMatrixStack();

	/** currentModelMatrix = currentModelMatrix * _scaleMatrix */
	void scale(			glm::mat4 _scaleMatrix);
	/** currentModelMatrix = currentModelMatrix * _rotationMatrix */
	void rotate(		glm::mat4 _rotationMatrix);
	/** currentModelMatrix = currentModelMatrix * _translationMatrix */
	void translate(		glm::mat4 _translationMatrix);
	/** currentModelMatrix = currentModelMatrix * _modelMatrix */
	void applyMatrix(	glm::mat4 _modelMatrix);
};