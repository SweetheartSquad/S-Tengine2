#include <Text.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <Vox.h>
#include <GL/glew.h>
#include <Texture.h>
#include <vector>
#include <MeshEntity.h>
#include <MeshInterface.h>
#include <shader/ShaderComponentTexture.h>
#include <RenderOptions.h>
#include <MatrixStack.h>
#include <RenderSurface.h>
#include <OrthographicCamera.h>
#include <FollowCamera.h>

Text::Text(std::string _fontSrc):
	NodeLoadable(),
	NodeRenderable(),
	texture(nullptr),
	textureT(nullptr),
	frameBuffer(new StandardFrameBuffer(false)),
	frameBuffer2(true),
	renderSurface(new RenderSurface(new Shader("../assets/RenderSurface", false, true))),
	cam(-320, 320, -240, 240, -1000, 1000)
{
	face = nullptr;
	if(FT_New_Face(vox::freeTypeLibrary, "../assets/arial.ttf", 0, &face) != 0) {
		std::cerr << "Couldn't load font " << _fontSrc;
	}
 
	shader = new BaseComponentShader(true);
	shader->components.push_back(new ShaderComponentTexture(shader));
	shader->compileShader();
	m = new MeshEntity(new MeshInterface(GL_QUADS, GL_STATIC_DRAW));

	cam.farClip = 1000.f;
	cam.transform->rotate(90, 0, 1, 0, kWORLD);
	cam.transform->translate(5.0f, 1.5f, 22.5f);
	cam.yaw = 90.0f;
	cam.pitch = 10.0f;

	texture = new Texture("../assets/S-Tengine2_logo.png", 2048, 2048, true, true);
	
}

Text::~Text(){
}

void Text::load(){
	if(textureT != nullptr){
		textureT->load();
		texture->load();
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	NodeLoadable::load();

	frameBuffer->load();
	frameBuffer2.load();

	m->load();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Text::unload(){
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if(textureT != nullptr){
		textureT->unload();
		texture->unload();
	}
	NodeLoadable::unload();
	frameBuffer->unload();
	frameBuffer2.unload();
	m->unload();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Text::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	
	//if(textDirty || 1){
	textDirty = false;
	std::vector<MeshEntity> tempPlanes;

	//for(auto c : text){

		//unload();
		//load();
	

		/*cam.farClip = 1000.f;
		cam.transform->rotate(90, 0, 1, 0, kWORLD);
		cam.transform->translate(5.0f, 1.5f, 22.5f);
		cam.yaw = 90.0f;
		cam.pitch = -10.0f;*/

		//glClearColor(1,0,0,1);
		checkForGlError(0,__FILE__,__LINE__);

		//frameBuffer.resize(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		//frameBuffer.bindFrameBuffer();


		float x = 0.0f;
		float y = 0.0f;

		float sx = 2.0f/640.0f;
		float sy = 2.0f/480.0f;

		float vx = x + face->glyph->bitmap_left * sx;
		float vy = y + face->glyph->bitmap_top * sy;
		float w = face->glyph->bitmap.width * sx;
		float h = face->glyph->bitmap.rows * sy;

		m->mesh->vertices.clear();
		m->mesh->indices.clear();
		m->mesh->textures.clear();

		checkForGlError(0,__FILE__,__LINE__);

		m->setShader(shader, true);
		m->mesh->pushVert(Vertex(glm::vec3(vx, vy, -2.f), glm::vec2(0.f, 0.f)));
		m->mesh->pushVert(Vertex(glm::vec3(vx + w, vy, -2.f), glm::vec2(1.f, 0.f)));
		m->mesh->pushVert(Vertex(glm::vec3(vx + w, vy - h, -2.f), glm::vec2(1.f, 1.f)));
		m->mesh->pushVert(Vertex(glm::vec3(vx, vy-h, -2.f), glm::vec2(0.f, 1.f)));

		FT_Set_Pixel_Sizes(face, 0, 200);
		FT_GlyphSlot glyph = face->glyph;
		FT_Load_Char(face, text.at(0), FT_LOAD_RENDER);

		checkForGlError(0,__FILE__,__LINE__);
		
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		if(textureT == nullptr) {
			textureT = new Texture(glyph->bitmap, true, false);
		}else {
			//textureT->data = glyph->bitmap.buffer;
			//textureT->unload();
			//textureT->load();

			textureT = new Texture(glyph->bitmap, true, false);
		}
		
		checkForGlError(0,__FILE__,__LINE__);

		m->mesh->pushTexture2D(textureT);
		m->mesh->dirty = true;
		m->unload();
		m->load();
		//texture = new Texture(glyph->bitmap, true, false);

		RenderOptions * renderOptions = new RenderOptions(shader, new std::vector<Light *>());
		renderOptions->overrideShader = shader;
		vox::MatrixStack * ms = new vox::MatrixStack();
		//ms->applyMatrix(vox::matrixStack);
		ms->pushMatrix();
		ms->setProjectionMatrix(cam.getProjectionMatrix());
		ms->setViewMatrix(cam.getViewMatrix());
		
		m->update(&vox::step);

		m->render(ms, renderOptions);

		ms->popMatrix();


		checkForGlError(0,__FILE__,__LINE__);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	

		//renderSurface->render(frameBuffer.getTextureId(), frameBuffer2.getTextureId());

		//unload();
		//load();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		//frameBuffer.frameBufferId = texture->textureId;
		
	//}
	//	}
}

void Text::setText(std::string _text){
	text = _text;
	updateTexture();
	textDirty = true;
}

std::string Text::getText(){
	return text;
}

void Text::updateTexture(){
	
}