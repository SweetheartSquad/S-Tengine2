#include <Label.h>
#include <Font.h>
#include <MeshInterface.h>
#include <MeshEntity.h>
#include <shader/BaseComponentShader.h>
#include <shader/ShaderComponentText.h>
#include <shader/ShaderComponentTexture.h>

Label::Label(Font * _font, Shader * _shader):
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	Entity(transform),
	textDirty(false)
{
	font = _font;
	shader = _shader;
}

void Label::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	GLboolean depth = glIsEnabled(GL_DEPTH_TEST);
	if(depth == GL_TRUE){
		glDisable(GL_DEPTH_TEST);
	}
	Entity::render(_matrixStack, _renderOptions);
	if(depth == GL_TRUE){
		glEnable(GL_DEPTH_TEST);
	}
}

void Label::update(Step * _step){
	Entity::update(_step);
}

void Label::unload(){
	Entity::unload();
	font->unload();
}

void Label::load(){
	Entity::load();
	font->load();
}

Label::~Label(){
}

void Label::appendText(std::string _text){
	text += _text;
	textDirty = true;
	updateText();
}

std::string Label::getText(){
	return text;
}

void Label::updateText(){
	float acc = 0.f;
	textDirty = false;
	// Don't need to delete the elements in the vector because they are being managed by the font
	children.clear();
	for(char c : text){
		MeshInterface * mi = font->getMeshInterfaceForChar(c);
		MeshEntity * me = new MeshEntity(mi);
		me->setShader(shader, true);
		addChild(me);
		me->transform->translate(acc, 0.f, 0.f);
		glm::vec2 offset = font->getGlyphWidthHeight(c) + font->getGlyphXY(c);
		acc += offset.x;
	}
	//Render in reverse so that letters overlap properly.
	//This is important for things like calligraphy fonts
	children.reserve(children.size());
}

void Label::setText(std::string _text){
	text = _text;
	textDirty = true;
	updateText();
}
