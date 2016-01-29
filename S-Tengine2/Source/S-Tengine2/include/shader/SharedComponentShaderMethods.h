#pragma once

#include <GL/glew.h>
#include <shader/ShaderVariables.h>

class NodeRenderable;
class RenderOptions;

namespace sweet{
	class MatrixStack;
}

class SharedComponentShaderMethods {
private:
	static std::string lightArrayAccessors[MAX_LIGHTS][8];
	static std::string materialArrayAccessors[MAX_MATERIALS][2];
	static bool staticInitialized;
	static bool staticInit();
public:
	static void configureLights(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable);
	static void configureMaterials(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable);
};