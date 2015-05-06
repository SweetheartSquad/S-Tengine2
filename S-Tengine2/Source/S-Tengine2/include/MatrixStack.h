#pragma once

#include <vector>
#include <glm\glm.hpp>


namespace vox{
/***********************************************
*
* A matrix stack used for matrix operations such
* as translation, scaling and rotation
*
***********************************************/

class MatrixStack{
private:

	/** View Matrix*/
	glm::mat4 viewMatrix;
	/**Projection matrix*/ 
	glm::mat4 projectionMatrix;

	/** Current Model-View-Projection matrix */
	glm::mat4 mvp;
	bool mvpDirty;
	/** Current View-Projection matrix */
	glm::mat4 vp;
	bool vpDirty;
public:
	/** Current model matrix */
	glm::mat4 currentModelMatrix;

	MatrixStack();
	~MatrixStack();

	/** Model matrix stack (replaces the OpenGL stack) */
	std::vector<glm::mat4> matrixStack;
	
	const glm::mat4 * getMVP();
	const glm::mat4 * getVP();
	
	void setViewMatrix(const glm::mat4 * _viewMatrix);
	void setProjectionMatrix(const glm::mat4 * _projectionMatrix);

	/** Pushes the current model matrix onto the stack */
	void pushMatrix();
	/** Restores the current model matrix to the last stored value and pops it off the stack */
	void popMatrix();
	/** Returns the value of the current model matrix */
	const glm::mat4 * getModelMatrix();
	const glm::mat4 * getViewMatrix();
	const glm::mat4 * getProjectionMatrix();
	/** Sets the calue of the current model matrix to the identity matrix */
	void resetCurrentMatrix();
	/** Clears the model matrix stack */
	void clearMatrixStack();

	/**
	* currentModelMatrix = currentModelMatrix * _scaleMatrix 
	*
	* @param _scaleMatrix The scale matrix to apply
	*/
	void scale(			glm::mat4 _scaleMatrix);

	/**
	* currentModelMatrix = currentModelMatrix * _rotationMatrix 
	*
	* @param _scaleMatrix The rotation matrix to apply
	*/
	void rotate(		glm::mat4 _rotationMatrix);

	/**
	* currentModelMatrix = currentModelMatrix * _translationMatrix 
	*
	* @param _scaleMatrix The translation matrix to apply
	*/
	void translate(		glm::mat4 _translationMatrix);

	/**
	* currentModelMatrix = currentModelMatrix * _modelMatrix 
	*
	* @param _scaleMatrix The model matrix to apply
	*/
	void applyMatrix(	glm::mat4 _modelMatrix);
};
};