#include "GLUtils.h"
#include <string>

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
		checkForGlError(false);
	}
}