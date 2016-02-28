#include "FrameBufferChannel.h"

FrameBufferChannel::FrameBufferChannel(GLint _internalFormat, GLenum _attachmentType, GLenum _format, ChannelType _channelType, GLenum _size):
	internalFormat(_internalFormat),
	channelType(_channelType),
	attachmentType(_attachmentType),
	size(_size),
	format(_format),
	numChannels(0)
{
	// derive the number of channels from the internal format
	switch(internalFormat){
	case GL_RGBA: numChannels = 4; break;
	case GL_RGB: numChannels = 3; break;
	case GL_RG: case GL_LUMINANCE_ALPHA: numChannels = 2; break;
	case GL_R: case GL_LUMINANCE: case GL_DEPTH_COMPONENT: numChannels = 1; break;
	default: throw "unsupported FBO format";
	}
}