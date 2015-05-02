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
	Entity::render(_matrixStack, _renderOptions);
}

void Label::update(Step * _step){
	if(textDirty){
		float acc = 0.f;
		textDirty = false;
		children.clear();
		for(char c : text){
			MeshInterface * mi = font->getMeshInterfaceForChar(c);
			MeshEntity * me = new MeshEntity(mi);
			me->setShader(shader, true);
			addChild(me);
			me->transform->translate(acc, 0.f, 0.f);
			glm::vec2 wH = font->getGlyphWidthHeight(c);
			acc += wH.x;
		}
	}
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

void Label::setText(std::string _text){
	text = _text;
	textDirty = true;
}
