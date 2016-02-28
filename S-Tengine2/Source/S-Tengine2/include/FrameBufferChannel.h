#pragma once

#include<gl/glew.h>

/****************************************************************
*
* A FrameBuffer Channel. May be a render buffer or a texture2D
* This class works in conjunction with FrameBufferInterface
*
***************************************************************/

class FrameBufferChannel{
public:
	/** Enum for a channels type. Either a texture2D or a render buffer*/
	enum ChannelType{
		TEXTURE,
		RENDER_BUFFER
	} channelType;

	/**The internal format of the buffer channel*/
	GLint  internalFormat;
	// the number of pixel channels in this FBO channel (e.g. GL_RGBA has 4 channels)
	unsigned long int numChannels;
	/**The attachement type of the buffer channel*/
	GLenum attachmentType;
	/**The size of te data type. eg GL_UNSIGNED_INT*/
	GLenum size;
	/**The format of the buffer channel*/
	GLenum format;
	/**
	* The id of te buffer channel. Depending on the type of channel this
	* may be a texture ID or a render buffer ID
	*/
	GLuint id;

	/**
	* @param _internalFormat Refers to the format OpenGL uses to store the channel. Possible base types are:
	* GL_ALPHA
	* GL_LUMINANCE
	* GL_LUMINANCE_ALPHA
	* GL_RGB
	* GL_RGBA
	* GL_DEPTH_COMPONENT
	* For more explicit types refer to table two at the following link : https://www.khronos.org/opengles/sdk/docs/man3/html/glTexImage2D.xhtml
	*
	* @param _channelType Refers to whether the data is saved as a texture or a render buffer.
	* Render buffers are not accessible from shaders.
	*
	* @param _size Refers to the data type used for the texture. This value is not relevant for a render buffer
	* The possible values are :
	* GL_UNSIGNED_BYTE,
	* GL_BYTE,
	* GL_UNSIGNED_SHORT,
	* GL_SHORT,
	* GL_UNSIGNED_INT,
	* GL_INT,
	* GL_FLOAT,
	* GL_UNSIGNED_BYTE_3_3_2,
	* GL_UNSIGNED_BYTE_2_3_3_REV,
	* GL_UNSIGNED_SHORT_5_6_5,
	* GL_UNSIGNED_SHORT_5_6_5_REV,
	* GL_UNSIGNED_SHORT_4_4_4_4,
	* GL_UNSIGNED_SHORT_4_4_4_4_REV,
	* GL_UNSIGNED_SHORT_5_5_5_1,
	* GL_UNSIGNED_SHORT_1_5_5_5_REV,
	* GL_UNSIGNED_INT_8_8_8_8,
	* GL_UNSIGNED_INT_8_8_8_8_REV,
	* GL_UNSIGNED_INT_10_10_10_2,
	* GL_UNSIGNED_INT_2_10_10_10_REV
	*
	* @param _atachmentType Refers to the achment type to the frame buffer
	* The possible values are :
	* GL_COLOR_ATTACHMENT0  -- Always use 0 as this value will be incremented internally,
	* GL_DEPTH_ATTACHMENT,
	* GL_STENCIL_ATTACHMENT,
	* GL_DEPTH_STENCIL_ATTACHMENT
	*
	* @param _format Specifies the format of the pixel data.
	* The possible values are :
	* GL_RED, GL_RED_INTEGER,
	* GL_RG, GL_RG_INTEGER,
	* GL_RGB, GL_RGB_INTEGER,
	* GL_RGBA, GL_RGBA_INTEGER,
	* GL_DEPTH_COMPONENT,
	* GL_DEPTH_STENCIL,
	* GL_LUMINANCE_ALPHA,
	* GL_LUMINANCE,
	* GL_ALPHA.
	*
	*/
	FrameBufferChannel(GLint _internalFormat, GLenum _attachmentType, GLenum _format, ChannelType _channelType, GLenum _size);
};