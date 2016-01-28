#pragma once

#include <FrameBufferInterface.h>
#include <Log.h>

#include <iostream>
#include <sstream>

#include <stb/stb_image_write.h>

std::stack<GLuint> FrameBufferInterface::fboStack;

void FrameBufferInterface::pushFbo(){
	pushFbo(this);
}

void FrameBufferInterface::pushFbo(GLuint _fboIdToBind){
	fboStack.push(_fboIdToBind);
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _fboIdToBind);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fboIdToBind);
}

void FrameBufferInterface::pushFbo(FrameBufferInterface * const _fboToBind){
	pushFbo(_fboToBind->frameBufferId);
}

void FrameBufferInterface::popFbo(){
	// the top of the stack is the currently bound FBO, so pop it immediately
	fboStack.pop();
	// the new top is the previously bound FBO, so we need to rebind it
	// if there is no top, we need to bind the default FBO instead
	GLuint fboToBind = (fboStack.size() > 0) ? fboStack.top() : 0;
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboToBind);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboToBind);
}

FrameBufferInterface::FrameBufferInterface(std::vector<FrameBufferChannel> _frameBufferChannels, unsigned long int _width, unsigned long int _height, bool _autoRelease):
	NodeResource(_autoRelease),
	width(_width),
	height(_height),
	frameBufferChannels(_frameBufferChannels),
	scaleModeMag(GL_LINEAR),
	scaleModeMin(GL_LINEAR)
{
	load();
}

FrameBufferInterface::~FrameBufferInterface(){
	unload();
}

void FrameBufferInterface::load(){
	unsigned long int colorAttachmentCount = 0;

	glGenFramebuffers(1, &frameBufferId);
	// we need to bind the framebuffer in order to configure it
	pushFbo();
	for(unsigned long int i = 0; i < frameBufferChannels.size(); i++){
		switch (frameBufferChannels.at(i).channelType){
		case FrameBufferChannel::TEXTURE :
			glGenTextures(1, &frameBufferChannels.at(i).id);
			glBindTexture(GL_TEXTURE_2D, frameBufferChannels.at(i).id);
			glTexImage2D(GL_TEXTURE_2D, 0, frameBufferChannels.at(i).internalFormat, width, height, 0,
				frameBufferChannels.at(i).format, frameBufferChannels.at(i).size, 0);

			// Texture scaling mode
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleModeMag);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleModeMin);
			if(frameBufferChannels.at(i).attachmentType == GL_COLOR_ATTACHMENT0){
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentCount, GL_TEXTURE_2D, frameBufferChannels.at(i).id, 0);
			}else{
				glFramebufferTexture2D(GL_FRAMEBUFFER, frameBufferChannels.at(i).attachmentType, GL_TEXTURE_2D, frameBufferChannels.at(i).id, 0);
			}
			break;
		case FrameBufferChannel::RENDER_BUFFER :
			glGenRenderbuffers(1, &frameBufferChannels.at(i).id);
			glBindRenderbuffer(GL_RENDERBUFFER, frameBufferChannels.at(i).id);
			glRenderbufferStorage(GL_RENDERBUFFER, frameBufferChannels.at(i).internalFormat, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, frameBufferChannels.at(i).attachmentType, GL_RENDERBUFFER, frameBufferChannels.at(i).id);
			break;
		}
	}
	// rebind previous fbo
	popFbo();
	
	NodeLoadable::load();
}

void FrameBufferInterface::unload(){
	glDeleteFramebuffers(1, &frameBufferId);
	frameBufferId = 0;
	for(unsigned long int i = 0; i < frameBufferChannels.size(); i++){
		switch (frameBufferChannels.at(i).channelType){
		case FrameBufferChannel::TEXTURE :
			glDeleteTextures(1, &frameBufferChannels.at(i).id);
			frameBufferChannels.at(i).id = 0;
			break;
		case FrameBufferChannel::RENDER_BUFFER :
			glDeleteRenderbuffers(1, &frameBufferChannels.at(i).id);
			frameBufferChannels.at(i).id = 0;
			break;
		}
	}
	
	NodeLoadable::unload();
}

void FrameBufferInterface::reload(){
	unload();
	load();
}

bool FrameBufferInterface::resize(unsigned long _width, unsigned long _height){
	if(width != _width || height != _height){
		width  = _width;
		height = _height;
		unload();
		load();
		return true;
	}
	return false;
}

GLenum FrameBufferInterface::checkFrameBufferStatus(){
	GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status) {
	case GL_FRAMEBUFFER_COMPLETE:
		Log::info("Framebuffer Complete");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		Log::warn("An attachment could not be bound to frame buffer object!");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		Log::warn("Attachments are missing! At least one image (texture) must be bound to the frame buffer object!");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		Log::warn("The dimensions of the buffers attached to the currently used frame buffer object do not match!");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		Log::warn("The formats of the currently used frame buffer object are not supported or do not fit together!");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		Log::warn("A Draw buffer is incomplete or undefinied. All draw buffers must specify attachment points that have images attached.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		Log::warn("A Read buffer is incomplete or undefinied. All read buffers must specify attachment points that have images attached.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		Log::warn("All images must have the same number of multisample samples.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS :
		Log::warn("If a layered image is attached to one attachment, then all attachments must be layered attachments. The attached layers do not have to have the same number of layers, nor do the layers have to come from the same kind of texture.");
		break;

	case GL_FRAMEBUFFER_UNSUPPORTED:
		Log::warn("Attempt to use an unsupported format combinaton!");
		break;

	default:
		Log::warn("Unknown error while attempting to create frame buffer object!");
		break;
	}
	return status;
}

void FrameBufferInterface::saveToFile(const char * _filename, unsigned long int _fbochannel){
	glBindTexture(GL_TEXTURE_2D, frameBufferChannels.at(_fbochannel).id);
	GLint textureWidth, textureHeight;
	unsigned long int bytes;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);
	bytes = textureWidth*textureHeight*4;
	
	GLubyte * pixels = (GLubyte *)malloc(bytes * sizeof(GLubyte));

	unsigned char * pixelsSOIL = (unsigned char *)malloc(bytes * sizeof(unsigned char));

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	for(unsigned long int i = 0; i < bytes; ++i){
		pixelsSOIL[i] = pixels[i];
	}
	
	std::stringstream ss;
	ss << "data/images/" << _filename;
	if(stbi_write_tga(ss.str().c_str(), textureWidth, textureHeight, 4, pixelsSOIL, 1)){
		Log::info("FBO \""+ss.str()+"\" saved");
	}else{
		Log::error("FBO \""+ss.str()+"\" not saved");
	}
	
	free(pixels);
	free(pixelsSOIL);
}

GLubyte * FrameBufferInterface::getPixelData(unsigned long int _fbochannel) {
	glBindTexture(GL_TEXTURE_2D, frameBufferChannels.at(_fbochannel).id);
	GLint textureWidth, textureHeight;
	unsigned long int bytes;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);
	bytes = textureWidth*textureHeight*4;
	
	GLubyte * pixels = (GLubyte *)malloc(bytes * sizeof(GLubyte));

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	return pixels;
}