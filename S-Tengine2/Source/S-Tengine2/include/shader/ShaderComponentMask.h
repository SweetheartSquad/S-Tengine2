#pragma once 

#include <shader\ShaderComponent.h>
class Texture;
class ShaderComponentMask : public ShaderComponent{
private:
	Texture * maskTex;
	GLint maskTexLoc;
public:
	Texture * getMaskTex();
	void setMaskTex(Texture * _maskTex);

	ShaderComponentMask(Shader * _shader, Texture * _maskTex = nullptr);
	~ShaderComponentMask();
	
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void load() override;
	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};