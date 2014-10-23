#include "FrameBufferInterface.h"

FrameBufferInterface::FrameBufferInterface(){
	load();
}

FrameBufferInterface::~FrameBufferInterface(){
}

void FrameBufferInterface::load(){
	glGenFramebuffers(1, &frameBufferId);
	bindFrameBuffer();
	glGenTextures(1, &textureBufferId);
	bindTextureBuffer();
}

void FrameBufferInterface::unload(){
	glDeleteFramebuffers(1, &frameBufferId);
	frameBufferId = 0;
}

void FrameBufferInterface::bindTextureBuffer(){
	glBindTexture(GL_TEXTURE_2D, textureBufferId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBufferId, 0);

	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
}

void FrameBufferInterface::bindFrameBuffer(){
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
}

void FrameBufferInterface::configureDefaultShaderAttributes(){

}

GLenum FrameBufferInterface::checkFrameBufferStatus(){
	return glCheckFramebufferStatus(frameBufferId);
}
