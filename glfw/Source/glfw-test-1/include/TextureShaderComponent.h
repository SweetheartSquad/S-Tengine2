#pragma once 

#include "ShaderComponent.h"

class TextureShaderComponent : ShaderComponent{
public:
	TextureShaderComponent();
	~TextureShaderComponent() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
};