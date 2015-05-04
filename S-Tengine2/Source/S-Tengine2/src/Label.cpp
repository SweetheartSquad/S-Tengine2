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
#include <MeshFactory.h>

Label::Label(Font * _font, Shader * _shader):
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

	while(childButNotReally->children.size() > 0){
		delete childButNotReally->children.back();
		childButNotReally->children.pop_back();
	}

	for(char c : text){
		Glyph * mi = font->getMeshInterfaceForChar(c);
		Transform * t = new Transform();
		MeshEntity * me = new MeshEntity(mi);
		me->setShader(shader, true);
		t->addChild(me);
		childButNotReally->addChild(t);
		t->translate(acc, 0.f, 0.f);
		acc += mi->advance.x/64;
	}
}

void Label::setText(std::string _text){
	text = _text;
	textDirty = true;
	updateText();
}
