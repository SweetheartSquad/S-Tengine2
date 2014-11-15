#include "FrameBufferChannel.h"

FrameBufferChannel::FrameBufferChannel(GLint _internalFormat, GLenum _attachmentType, GLenum _format, ChannelType _channelType, GLenum _size):
	internalFormat(_internalFormat),
	channelType(_channelType),
	attachmentType(_attachmentType),
	size(_size),
	format(_format)
{
}