#include "StandardFrameBuffer.h"
#include <GLFW/glfw3.h>

StandardFrameBuffer::StandardFrameBuffer( bool _autoRelase):
	FrameBufferInterface(std::vector<FrameBufferChannel>(), 1, 1, _autoRelase)
{
	frameBufferChannels.push_back(FrameBufferChannel(GL_RGB, GL_COLOR_ATTACHMENT0, GL_RGB,  FrameBufferChannel::TEXTURE, GL_BYTE));
	frameBufferChannels.push_back(FrameBufferChannel(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, FrameBufferChannel::RENDER_BUFFER, 0));

	int width, height;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
	resize(width, height);
}

StandardFrameBuffer::~StandardFrameBuffer(){
}

GLuint StandardFrameBuffer::getTextureId(){
	return frameBufferChannels.at(0).id;
}

GLuint StandardFrameBuffer::getRenderBufferId(){
	return frameBufferChannels.at(1).id;
}