#include "GLUtils.h"

// check OpenGL error
void GLUtils::checkForError(bool _printSuccess = false, std::string _file = "", int _line = 0){
	GLenum err;
	std::string errString;
	while ((err = glGetError()) != GL_NO_ERROR) {
		if(_printSuccess || err != GL_NO_ERROR){
			switch(err) {
			case GL_NO_ERROR:						errString="NO_ERROR"; break;
			case GL_INVALID_OPERATION:				errString="INVALID_OPERATION"; break;
			case GL_INVALID_ENUM:					errString="INVALID_ENUM"; break;
			case GL_INVALID_VALUE:					errString="INVALID_VALUE"; break;
			case GL_OUT_OF_MEMORY:					errString="OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  errString="INVALID_FRAMEBUFFER_OPERATION"; break;
			default: errString = "dunno, but something went wrong?";	break;
			}
			std::cout << "\tFile: " << _file.c_str() <<", Line: "<<_line << ", Log: " << errString.c_str() << std::endl;
			//std::cout << glewGetErrorString(err) << std::endl;
		}
	}
}