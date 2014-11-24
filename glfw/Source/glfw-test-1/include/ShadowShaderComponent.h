#pragma once

#include "ShaderComponent.h"

class ShadowShaderComponent : ShaderComponent{

public:
	ShadowShaderComponent();
	~ShadowShaderComponent() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	
};