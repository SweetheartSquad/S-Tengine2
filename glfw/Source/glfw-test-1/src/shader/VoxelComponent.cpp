#pragma once

#include "shader/VoxelComponent.h"
#include "MatrixStack.h"
#include "MeshInterface.h"
#include "RenderOptions.h"
#include "shader/ShaderVariables.h"

#include <GL/glew.h>

#include <glm/glm.hpp>

VoxelComponent::VoxelComponent(){
}

std::string VoxelComponent::getVertexVariablesString(){
	return DEFINE + SHADER_COMPONENT_VOXEL + ENDL;
}

std::string VoxelComponent::getVertexBodyString(){
	return "gl_Position = vec4(aVertexPosition, 1.0);\n";
}

std::string VoxelComponent::getGeometryShader(){
	return 
		"#version 150\n"

		"layout(points) in;\n"
		"layout(triangle_strip, max_vertices = 24) out;\n"

		"in vec3 fragVertGeo[];\n"
		"in vec3 fragNormalGeo[];\n"
		"in vec4 fragColorGeo[];\n"
		"in vec2 fragUVGeo[];\n"
		"in vec4 shadowCoordGeo[];\n"

		"out vec3 fragVert;\n"
		"out vec3 fragNormal;\n"
		"out vec4 fragColor;\n"
		"out vec2 fragUV;\n"
		"out vec4 shadowCoord;\n"

		"uniform mat4 VP;\n"
		"uniform float resolution;\n"
		"uniform mat4 model;\n"

		"void rotate(in int i, inout vec3 inVec){\n"
			"if(i == 0){\n"
				"inVec = inVec;\n"
				"inVec = inVec;\n"
			"}else if(i == 1){\n"
				"inVec = vec3(-inVec.y, inVec.x, inVec.z);\n"
			"}else if(i == 2){\n"
				"inVec = vec3(-inVec.z, inVec.y, inVec.x);\n"
			"}\n"
		"}\n"

		"void main() {\n"

			"shadowCoord = shadowCoordGeo[0];\n"

			"fragVert = fragVertGeo[0];\n"
			"fragNormal = fragNormalGeo[0];\n"
			"fragColor = fragColorGeo[0];\n"

			"for(int i = 0; i < 3; ++i){\n"
				"for(int x = 0; x <= 1; ++x){\n"
			
					"fragNormal = vec3((x-0.5)*2, 0.0, 0);\n"
					"rotate(i, fragNormal);\n"

					"for(int y = 0; y <= 1; ++y){\n"
						"for(int z = 0; z <= 1; ++z){\n"
							"fragUV = vec2(y, z);\n"
					
							"vec3 cubeVert = vec3(x-0.5, y-0.5, z-0.5) * resolution;\n"
							"rotate(i, cubeVert);\n"
					
							"vec4 pos = model * gl_in[0].gl_Position.xyzw;\n"

							"pos.xyz = pos.xyz - mod(pos.xyz, vec3(resolution, resolution, resolution));\n"
					
							"gl_Position = VP * (pos + vec4(cubeVert, 0.0));\n"
					
							"EmitVertex();\n"
						"}\n"
					"}\n"
					"EndPrimitive();\n"
				"}\n"
			"}\n"
		"}\n";
}

//TOOD make resolution configurable
void VoxelComponent::configureUniforms(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption,  NodeRenderable* _nodeRenderable){
	glm::mat4 vp = _matrixStack->projectionMatrix * _matrixStack->viewMatrix;
	GLuint vpUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), "VP");
	GLuint resolutionUniformLocation = glGetUniformLocation(_renderOption->shader->getProgramId(), "resolution");
	glUniformMatrix4fv(vpUniformLocation, 1, GL_FALSE, &vp[0][0]);
	glUniform1f(resolutionUniformLocation, 0.15);
}
