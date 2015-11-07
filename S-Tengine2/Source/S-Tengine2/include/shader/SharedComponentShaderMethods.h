#pragma once

#include <GL/glew.h>

class NodeRenderable;
class RenderOptions;

namespace sweet{
	class MatrixStack;
}

class SharedComponentShaderMethods {
public:
	static void configureLights(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable);
	static void configureMaterials(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable);
};