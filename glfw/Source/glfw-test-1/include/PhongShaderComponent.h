#pragma once

#include "ShaderComponent.h"

/******************************************************************************
*
* Adds phong material support to the shader that this component is added to.
*
* In order for the phong effect to occur the Mesh that is being rendered must
* have a material applied to it
*
* See ShaderComponent for method descriptions
*
*******************************************************************************/

class PhongShaderComponent : public ShaderComponent{
public:
	PhongShaderComponent();
	~PhongShaderComponent() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
};