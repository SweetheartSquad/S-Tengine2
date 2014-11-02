#include "FrameBufferInterface.h"

FrameBufferInterface::FrameBufferInterface(unsigned long int _width, unsigned long int _height, bool _autoRelease):
	NodeResource(_autoRelease),
	width(_width),
	height(_height)
{
	load();
}

FrameBufferInterface::~FrameBufferInterface(){
}

void FrameBufferInterface::load(){
	glGenFramebuffers(1, &frameBufferId);
	bindFrameBuffer();

	glGenTextures(1, &textureBufferId);
	glBindTexture(GL_TEXTURE_2D, textureBufferId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBufferId, 0);

	glGenRenderbuffers(1, &renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferInterface::unload(){
	glDeleteFramebuffers(1, &frameBufferId);
	glDeleteTextures(1, &textureBufferId);
	glDeleteRenderbuffers(1, &renderBufferId);
	frameBufferId   = 0;
	renderBufferId  = 0;
	textureBufferId = 0;
}

void FrameBufferInterface::reload(){
	unload();
	load();
}

void FrameBufferInterface::resize(unsigned long _width, unsigned long _height){	
	if(width != _width || height != _height){
		width  = _width;
		height = _height;
		unload();
		load();	
	}
}

void FrameBufferInterface::bindFrameBuffer(){
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
}

GLenum FrameBufferInterface::checkFrameBufferStatus(){
	GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE:
			std::cout << "Framebuffer Complete";
			break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "An attachment could not be bound to frame buffer object!";
			break;

			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "Attachments are missing! At least one image (texture) must be bound to the frame buffer object!";
			break;

			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			std::cout << "The dimensions of the buffers attached to the currently used frame buffer object do not match!";
			break;

			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			std::cout << "The formats of the currently used frame buffer object are not supported or do not fit together!";
			break;

			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cout << "A Draw buffer is incomplete or undefinied. All draw buffers must specify attachment points that have images attached.";
			break;

			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cout << "A Read buffer is incomplete or undefinied. All read buffers must specify attachment points that have images attached.";
			break;

			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			std::cout << "All images must have the same number of multisample samples.";
			break;

			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS :
			std::cout << "If a layered image is attached to one attachment, then all attachments must be layered attachments. The attached layers do not have to have the same number of layers, nor do the layers have to come from the same kind of texture.";
			break;

			case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "Attempt to use an unsupported format combinaton!";
			break;

			default:
			std::cout << "Unknown error while attempting to create frame buffer object!";
			break;
		}
	return status;
}
