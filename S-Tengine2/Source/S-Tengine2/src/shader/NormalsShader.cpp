#include <shader/NormalsShader.h>

#include <MatrixStack.h>
#include <shader/ShaderVariables.h>
#include <RenderOptions.h>

#include <GL/glew.h>

#include <glm/mat4x2.hpp>
#include <glm/gtc/matrix_transform.inl>

NormalsShader::NormalsShader(bool _autoRelease)  :
	Shader(getVertString(), getFragString(), _autoRelease)
{
}

void NormalsShader::configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) {
	const glm::mat4 * vp = _matrixStack->getVP();
	glUniformMatrix4fv(glGetUniformLocation(_renderOption->shader->getProgramId(), GL_UNIFORM_ID_VIEW_PROJECTION.c_str()), 1, GL_FALSE, &(*vp)[0][0]);
}

std::string NormalsShader::getVertString(){
	return 
		"#version 150\n"

		"in vec3 " + GL_ATTRIBUTE_ID_VERTEX_POSITION + SEMI_ENDL +
        "in vec4 " + GL_ATTRIBUTE_ID_VERTEX_COLOR + SEMI_ENDL +
        "in vec3 " + GL_ATTRIBUTE_ID_VERTEX_NORMALS + SEMI_ENDL +

		"uniform mat4 " + GL_UNIFORM_ID_VIEW_PROJECTION + ";\n"

		"out vec3 normalVec;\n"

		"void main(){\n"
			"gl_Position = " + GL_UNIFORM_ID_VIEW_PROJECTION + " * vec4(" + GL_ATTRIBUTE_ID_VERTEX_POSITION  + ", 1.0);\n"
			"normalVec = " + GL_ATTRIBUTE_ID_VERTEX_NORMALS + SEMI_ENDL + 
		"}\n";
}

std::string NormalsShader::getFragString(){
	return	
		"#version 150\n"

		"in vec3 normalVec;\n"

		"out vec4 normals" + SEMI_ENDL + 

		"void main(){\n"
			"normals = vec4(normalVec, 1);\n"
		"}\n";
}