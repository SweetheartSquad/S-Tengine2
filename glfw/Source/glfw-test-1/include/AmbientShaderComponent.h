#pragma once

#include "ShaderComponent.h"

/**************************************************************************
*
* Adds ambient light to the shader that this compent is added to.
*
* If a component such as PhongShaderComponent
* is definded then this components effect will be negated
* This is because the phong component already implements ambient lighting
*
* See ShaderComponent for method descriptions
*
**************************************************************************/

class AmbientShaderComponent : public ShaderComponent{
public:
	AmbientShaderComponent();
	~AmbientShaderComponent() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
};