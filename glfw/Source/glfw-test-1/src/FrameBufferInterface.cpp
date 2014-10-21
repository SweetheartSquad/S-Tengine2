#include "FrameBufferInterface.h"

FrameBufferInterface::FrameBufferInterface(){
	load();
}

FrameBufferInterface::~FrameBufferInterface(){
}

void FrameBufferInterface::load(){
	glGenFramebuffers(1, &frameBufferId);
	glGenTextures(1, &textureBufferId);
	bindTextureBuffer();
	bindFrameBuffer();
}

void FrameBufferInterface::unload(){
	glDeleteFramebuffers(1, &frameBufferId);
	frameBufferId = 0;
}

void FrameBufferInterface::bindTextureBuffer(){
	glBindTexture(GL_TEXTURE_2D, textureBufferId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 200, 200, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBufferId, 0);
}

void FrameBufferInterface::bindFrameBuffer(){
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
}

GLenum FrameBufferInterface::checkFrameBufferStatus(){
	return glCheckFramebufferStatus(frameBufferId);
}
