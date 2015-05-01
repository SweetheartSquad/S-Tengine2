#include <Label.h>
#include <Font.h>
#include <MeshInterface.h>
#include <MeshEntity.h>

Label::Label(Font * _font, Shader * _shader):
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	Entity(transform)
{
	font = _font;
	shader = _shader;
}

void Label::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	for(char c : text){
		MeshInterface * mi = font->getMeshInterfaceForChar(c);
		MeshEntity * me = new MeshEntity(mi);
		me->setShader(shader, true);
		addChild(me);
	}
	Entity::render(_matrixStack, _renderOptions);
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

void Label::setText(std::string _text){
	text = _text;
}
