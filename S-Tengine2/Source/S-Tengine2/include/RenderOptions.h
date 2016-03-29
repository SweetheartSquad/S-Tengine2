#pragma once

#include <node\Node.h>
#include <vector>
#include <GL\glew.h>
#include <StandardFrameBuffer.h>

class Shader;
class Light;

struct ViewPortDimensions {
	int x;
	int y;
	int width;
	int height;
};

class RenderOptions : public Node{
public:

	RenderOptions(Shader * _shader, std::vector<Light *> * _lights, Shader * _overrideShader = nullptr);

	~RenderOptions();

	std::vector<Light*> * lights;

	GLuint currentVao;
	GLuint currentlyBoundShaderId;

	Shader * shader;
	Shader * overrideShader;

	GLuint  depthBufferId;
	GLuint  normalBufferId;

	bool alphaSorting;
	bool alphaEnabled;
	bool depthEnabled;

	// View port values
	ViewPortDimensions viewPortDimensions;

	// OpenGL clear colour - Defaults to black
	float clearColour[4];

	/**
	* Clears the currently bound buffer using the valuses of @see clearColour
	*
	* Clears the following bits GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT
	*/
	void clear();

	// sets the OpenGL clear colour (values are 0-1)
	void setClearColour(float _r, float _g, float _b, float _a);

	//Sets the glViewport and stores the values for quick retrival
	void setViewPort(int _x, int _y, int _w, int _h);
};