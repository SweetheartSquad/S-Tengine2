#pragma once

#include<gl/glew.h>

class FrameBufferChannel{
public:
	enum ChannelType{
		TEXTURE,
		RENDER_BUFFER
	} channelType;

	GLint internalFormat;
	GLenum attachmentType;
	GLenum size;

	/**_internalFormat refers to the format OpenGL uses to store the channel. Possible base types are:
	* GL_ALPHA
	* GL_LUMINANCE 
	* GL_LUMINANCE_ALPHA
	* GL_RGB
	* GL_RGBA
	* GL_DEPTH_COMPONENT
	* For more explicit types refer to table two at the following link : https://www.khronos.org/opengles/sdk/docs/man3/html/glTexImage2D.xhtml
	*
	* The _channelType refers to whether the data is saved as a texture or a render buffer.
	* Render buffers are not accessible from shaders.
	*
	* The _size refers to the data type used for the texture. This value is not relevant for a render buffer
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
	* _atachmentType refers to the achment type to the frame buffer
	* The possible values are :
	* GL_COLOR_ATTACHMENT0  -- Always use 0 as this value will be incremented internally, 
	* GL_DEPTH_ATTACHMENT, 
	* GL_STENCIL_ATTACHMENT, 
	* GL_DEPTH_STENCIL_ATTACHMENT
	*/
	FrameBufferChannel(GLint _internalFormat, GLenum _attachmentType, ChannelType _channelType, GLenum _size);
};

