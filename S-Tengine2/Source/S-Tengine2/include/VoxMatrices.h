#pragma once

#include <vector>

#include <glm\glm.hpp>

namespace vox{
	/** Model matrix stack (replaces the OpenGL stack) */
	extern std::vector<glm::mat4> matrixStack;
	/** Current model matrix */
	extern glm::mat4 currentModelMatrix;

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

	/** Calculates the value with which to correct for differences in framerate */
	void scale(glm::mat4 _scaleMatrix);
	void rotate(glm::mat4 _rotationMatrix);
	void translate(glm::mat4 _translationMatrix);
	void applyMatrix(glm::mat4 _modelMatrix);
};