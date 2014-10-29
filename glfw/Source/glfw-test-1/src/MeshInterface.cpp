#include "MeshInterface.h"

MeshInterface::MeshInterface(GLenum polygonalDrawMode, GLenum drawMode):
	drawMode(drawMode),
	polygonalDrawMode(polygonalDrawMode),
	loaded(false),
	dirty(true),
	shouldRenderLights(true),
	shouldRenderTextures(true),
	shouldRenderExtras(true)
{
	load();
	clean();
}

MeshInterface::~MeshInterface(void){
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &iboId);
	for(Texture* t:textures){
		t->decrementAndDelete();
	}
	vaoId = 0;
	vboId = 0;
	iboId = 0;
	GLUtils::checkForError(0,__FILE__,__LINE__);
}

GLsizei MeshInterface::getStride(){
	return sizeof(Vertex);
}

GLsizei MeshInterface::getVertCount(){
	return vertices.size();
}

void MeshInterface::load(){
	if(!loaded){
		glBindVertexArray(0);

		// Vertex Array Object (VAO)
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		// Vertex Auffer Object (VBO)
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), drawMode);

		// Index Buffer Object (IBO)
		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), drawMode);

		// Initialize textures
		for (Texture * texture : textures){
			texture->load();
		}

		// Disable VAO
		glBindVertexArray(0);
		GLUtils::checkForError(true,__FILE__,__LINE__);
		loaded = true;
	}
}

void MeshInterface::unload(){
	if(loaded){
		glDeleteBuffers(1, &iboId);
		glDeleteBuffers(1, &vboId);
		glDeleteVertexArrays(1, &vaoId);
		for (Texture * texture : textures){
			texture->unload();
		}
		iboId = 0;
		vboId = 0;
		vaoId = 0;

		loaded = false;
		dirty = true;
	}
}

void MeshInterface::clean(){
	if(dirty){
		// Vertex Buffer Object (VBO)
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (vertices.size()), vertices.data(), GL_DYNAMIC_DRAW);
		GLUtils::checkForError(0,__FILE__,__LINE__);

		// Index Buffer Object (IBO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * (indices.size()), indices.data(), GL_DYNAMIC_DRAW);
		GLUtils::checkForError(0,__FILE__,__LINE__);
		dirty = false;
	}
}

void MeshInterface::render(Shader * _shader, glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*> _lights){
	if(glIsVertexArray(vaoId) == GL_TRUE){
		if(glIsBuffer(vboId) == GL_TRUE){
			if(glIsBuffer(iboId) == GL_TRUE){
				GLUtils::checkForError(0,__FILE__,__LINE__);
				// Bind VAO
				glBindVertexArray(vaoId);
				GLUtils::checkForError(0,__FILE__,__LINE__);

				// Specify _shader attributes
				glUseProgram(_shader->getProgramId());
				GLUtils::checkForError(0,__FILE__,__LINE__);

				//Model View Projection
				configureModelViewProjection(_shader, _projectionMatrix, _viewMatrix, _lights);

				if(shouldRenderTextures){
					configureTextures(_shader, _projectionMatrix, _viewMatrix, _lights);
				}
				if(shouldRenderLights){
					configureLights(_shader, _projectionMatrix, _viewMatrix, _lights);
				}
				if(shouldRenderExtras){
					configureExtras(_shader, _projectionMatrix, _viewMatrix, _lights);
				}

				//Alpha blending
				// Should these be here or only once in the main render loop?
				glEnable (GL_BLEND);
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				//Texture repeat
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				// Draw (note that the last argument is expecting a pointer to the indices, but since we have an ibo, it's actually interpreted as an offset)
				glDrawRangeElements(polygonalDrawMode, 0, vertices.size(), indices.size(), GL_UNSIGNED_INT, 0);
				//glDrawElements(drawMode, vertices.size(), GL_UNSIGNED_BYTE, 0);
				GLUtils::checkForError(0,__FILE__,__LINE__);

				// Disable VAO
				glBindVertexArray(0);
			}else{
				std::cout << "ibo bad" << std::endl << std::endl;
			}
		}else{
			std::cout << "vbo bad" << std::endl << std::endl;
		}
	}else{
		std::cout << "vao bad" << std::endl << std::endl;
	}
}

void MeshInterface::configureTextures(Shader * _shader, glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*> _lights){
	// Pass the _shader the number of textures
	glUniform1i(glGetUniformLocation(_shader->getProgramId(), GL_UNIFORM_ID_NUM_TEXTURES), textures.size());
	// Bind each texture to the texture sampler array in the frag _shader
	for(unsigned long int i = 0; i < textures.size(); i++){
		glUniform1i(glGetUniformLocation(_shader->getProgramId(), GL_UNIFORM_ID_TEXTURE_SAMPLER), i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures.at(i)->textureId);
	}
}

void MeshInterface::configureLights(Shader * _shader, glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*> _lights){
	glm::mat4 model = vox::currentModelMatrix;
	GLuint modelUniformLocation = glGetUniformLocation(_shader->getProgramId(), GL_UNIFORM_ID_MODEL_MATRIX);
	glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);

	// Pass the _shader the number of lights
	glUniform1i(glGetUniformLocation(_shader->getProgramId(), GL_UNIFORM_ID_NUM_LIGHTS), _lights.size());

	//Pass the paramaters for each light to the _shader
	for(unsigned long int i = 0; i < _lights.size(); i++){
		const char * pos = GLUtils::buildGLArryReferenceString(GL_UNIFORM_ID_LIGHTS_POSITION, i);
		const char * ins = GLUtils::buildGLArryReferenceString(GL_UNIFORM_ID_LIGHTS_INTENSITIES, i);
		GLuint lightUniformLocation = glGetUniformLocation(_shader->getProgramId(), pos);
		glUniform3f(lightUniformLocation, _lights.at(i)->data.position.x, _lights.at(i)->data.position.y, _lights.at(i)->data.position.z);
		GLuint intensitiesUniformLocation = glGetUniformLocation(_shader->getProgramId(), ins);
		glUniform3f(intensitiesUniformLocation, _lights.at(i)->data.intensities.x, _lights.at(i)->data.intensities.y, _lights.at(i)->data.intensities.z);
	}
}

void MeshInterface::configureModelViewProjection(Shader* _shader, glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*> _lights){
	glm::mat4 mvp = _projectionMatrix * _viewMatrix * vox::currentModelMatrix;
	GLuint mvpUniformLocation = glGetUniformLocation(_shader->getProgramId(),  GL_UNIFORM_ID_MODEL_VIEW_PROJECTION);
	glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	GLUtils::checkForError(0,__FILE__,__LINE__);
}

void MeshInterface::configureExtras(Shader * shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, std::vector<Light*> _lights){
}

void MeshInterface::configureDefaultVertexAttributes(Shader *_shader){
	GLUtils::configureVertexAttributes(_shader->get_aVertexPosition(), 3, 0, vaoId, getStride());
	GLUtils::configureVertexAttributes(_shader->get_aVertexColor(), 4, sizeof(float) * 3, vaoId, getStride());
	GLUtils::configureVertexAttributes(_shader->get_aVertexNormals(), 3, sizeof(float) * 7, vaoId, getStride());
	GLUtils::configureVertexAttributes(_shader->get_aVertexUVs(), 2, sizeof(float) * 10, vaoId, getStride());
}

void MeshInterface::pushVert(Vertex _vertex){
	vertices.push_back(_vertex);
	dirty = true;
}

void MeshInterface::pushTexture2D(Texture* _texture){
	++_texture->referenceCount;
	textures.push_back(_texture);
}

void MeshInterface::setNormal(unsigned long int _vertId, float _x, float _y, float _z){
	vertices.at(_vertId).nx = _x;
	vertices.at(_vertId).ny = _y;
	vertices.at(_vertId).nz = _z;

	dirty = true;
}

void MeshInterface::setUV(unsigned long _vertId, float _u, float _v){
	vertices.at(_vertId).u = _u;
	vertices.at(_vertId).v = _v;
}
void TriMesh::pushTri(GLuint _v0, GLuint _v1, GLuint _v2){
	indices.push_back(_v0);
	indices.push_back(_v1);
	indices.push_back(_v2);

	dirty = true;
}
void QuadMesh::pushQuad(GLuint _v0, GLuint _v1, GLuint _v2, GLuint _v3){
	indices.push_back(_v0);
	indices.push_back(_v1);
	indices.push_back(_v2);
	indices.push_back(_v3);

	dirty = true;
}