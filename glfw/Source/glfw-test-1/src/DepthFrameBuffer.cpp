#include "DepthFrameBuffer.h"
#include <GLFW/glfw3.h>

DepthFrameBuffer::DepthFrameBuffer(bool _autoRelase):
	FrameBufferInterface(std::vector<FrameBufferChannel>(), 1, 1, _autoRelase)
{
	frameBufferChannels.push_back(FrameBufferChannel(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, FrameBufferChannel::TEXTURE, GL_FLOAT));

	int width, height;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
	resize(width, height);
}

DepthFrameBuffer::~DepthFrameBuffer(){
}