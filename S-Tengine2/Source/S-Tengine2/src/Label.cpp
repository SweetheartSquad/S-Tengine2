#pragma once

#include <Label.h>
#include <Font.h>
#include <MeshInterface.h>
#include <MeshEntity.h>
#include <node\NodeChild.h>
#include <shader/BaseComponentShader.h>
#include <shader/ShaderComponentText.h>
#include <shader/ShaderComponentTexture.h>
#include <ostream>
#include <string>

Label::Label(Font * _font, Shader * _shader):
	NodeTransformable(new Transform()),
	Entity(transform),
	textDirty(false)
{
	font = _font;
	++font->referenceCount;
	shader = _shader;
	++shader->referenceCount;
}

void Label::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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
	font->load();
	Entity::load();
}

Label::~Label(){
	font->decrementAndDelete();
	shader->decrementAndDelete();
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

	while(children.size() > 0){
		delete children.back();
		children.pop_back();
	}

	for(char c : text){
		MeshInterface * mi = font->getMeshInterfaceForChar(c);
		MeshEntity * me = new MeshEntity(mi);
		me->setShader(shader, true);
		addChildAtIndex(me, 0); // Needs to render in reverse so that letters overlap properly, so letters are added to the start of the array
		me->transform->translate(acc, 0.f, 0.f);
		//glm::vec2 offset = font->getGlyphWidthHeight(c) + font->getGlyphXY(c);
		font->loadGlyph(c);
		acc += font->face->glyph->advance.x/64;//offset.x;
	}

	//This is important for things like calligraphy fonts
	//children.reverse(children.size());
}

void Label::setText(std::string _text){
	text = _text;
	textDirty = true;
	updateText();
}
