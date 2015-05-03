#include "GLUtils.h"
#include <string>

// check OpenGL error
/*void GLUtils::checkForError(bool _printSuccess = false, std::string _file = "", int _line = 0){
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
}*/

std::string GLUtils::buildGLArrayReferenceString(std::string _value, unsigned long int _index){
	std::string r;
	int i = 0;
	for(char c : _value){
		if(c != ']'){
			++i;
		}else{
			r += _value.substr(0, i);
			r += std::to_string(_index);
			r += _value.substr(i, _value.size() - 1);
			return r;
		}
	}
	throw "string not an array reference";
}

void GLUtils::configureVertexAttributes(GLint _vertexHandle, unsigned long _arity, int _bufferOffset, GLuint _vaoId, GLuint _vboId, GLsizei _stride){
	if (_vertexHandle != -1){
		GLint prev;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev);
		glBindVertexArray(_vaoId);

		GLint prev2;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prev2);
		glBindBuffer(GL_ARRAY_BUFFER, _vboId);

		glEnableVertexAttribArray(_vertexHandle);
		glVertexAttribPointer(_vertexHandle, _arity, GL_FLOAT, GL_FALSE, _stride, BUFFER_OFFSET(_bufferOffset));
		
		glBindVertexArray(prev);
		checkForGlError(0,__FILE__,__LINE__);
		GL_BGRA;
	}
}