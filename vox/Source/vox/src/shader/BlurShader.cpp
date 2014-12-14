#include "shader/BlurShader.h"

BlurShader::BlurShader(bool _autoRelease)
	:Shader(getVertString(), getFragString(), _autoRelease)
{
}

std::string BlurShader::getVertString(){
	return
		"#version 150\n"
		"#extension GL_ARB_explicit_attrib_location : enable\n"

		"layout(location = 1) in vec2 aVertexPosition;\n"
		"layout(location = 2) in vec2 aVertexUVs;\n"
		"out vec2 Texcoord;\n"
		"void main() {\n"
			"Texcoord = aVertexUVs;\n"
			"gl_Position = vec4(aVertexPosition, 0.0, 1.0);\n"
		"}";
}

std::string BlurShader::getFragString(){
	return
		"#version 150\n"
		"in vec2 Texcoord;\n"
		"out vec4 outColor;\n"
		"uniform sampler2D texFramebuffer;\n"

		"const float blurSizeH = 1.0 / 2000.0;\n"
		"const float blurSizeV = 1.0 / 2000.0;\n"
		"void main() {\n"
			"vec4 sum = vec4(0.0);\n"
			"for (int x = -4; x <= 4; x++)\n"
				"for (int y = -4; y <= 4; y++)\n"
					"sum += texture(\n"
						"texFramebuffer,\n"
						"vec2(Texcoord.x + x * blurSizeH, Texcoord.y + y * blurSizeV)\n"
					") / 81.0;\n"
			"outColor = sum;\n"
		"}\n";
}