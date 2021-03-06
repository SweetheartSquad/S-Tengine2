#pragma once

#include <shader\ShaderComponent.h>
#include <Texture.h>
#include "ShaderComponentDiffuse.h"

class ShaderComponentToon : public ShaderComponentDiffuse {
public:	
	ShaderComponentToon(ComponentShaderBase * _shader, Texture * _texture, bool _doubleSided);
	~ShaderComponentToon();
	
	virtual std::string getVertexVariablesString() override;
	virtual std::string getFragmentVariablesString() override;
	virtual std::string getVertexBodyString() override;
	virtual std::string getFragmentBodyString() override;
	virtual std::string getOutColorMod() override;
	virtual void load() override;
	virtual void unload() override;
	virtual void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

	virtual void setTexture(Texture * _texture);
	virtual Texture * const getTexture() const;

private:
	int levels;
	Texture * texture;
	GLint levelsLoc;
	GLint numTexturesLoc;
	GLint textureLoc;
};
