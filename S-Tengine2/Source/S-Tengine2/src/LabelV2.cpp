#pragma once

#include <LabelV2.h>
#include <GL/glew.h>
#include <MeshFactory.h>

LabelV2::LabelV2(BulletWorld* _world, Scene* _scene, Font* _font, Shader* _textShader, Shader* _backgroundShader, WrapMode _wrapMode, float _width):
	NodeUI(_world, _scene),
	MeshEntity(MeshFactory::getPlaneMesh()),
	NodeBulletBody(_world),
	font(_font),
	textShader(_textShader),
	backgroundShader(_backgroundShader),
	wrapMode(wrapMode),
	width(0.f),
	updateRequired(false)
{
	// Add initial line
	childTransform->addChild(new Line());
}

void LabelV2::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	GLboolean depth = glIsEnabled(GL_DEPTH_TEST);
	if(depth == GL_TRUE){
		glDisable(GL_DEPTH_TEST);
	}
	NodeUI::render(_matrixStack, _renderOptions);
	if(depth == GL_TRUE){
		glEnable(GL_DEPTH_TEST);
	}
}

void LabelV2::invalidateAllLines(){
	for(unsigned long int i = 0; i < childTransform->children.size(); ++i) {
		Line * line = dynamic_cast<Line *>(childTransform->children.at(i));
		if(line != nullptr) {
			line->invalidate();
		}
	}
}

void LabelV2::update(Step* _step){
	if(updateRequired) {
		invalidateAllLines();	
		for(unsigned long int i = 0; i < text.size(); ++i) {
			//currentLine()->insertGlyph(text.at(i));
		}
	}
	NodeUI::update(_step);
}

void LabelV2::unload(){
}

void LabelV2::load(){
}

void LabelV2::setText(std::wstring _text){
}

void LabelV2::appendText(std::wstring _text){
}

std::wstring LabelV2::getText(){
	return text;
}

void LabelV2::updateAlignment(){
}

void LabelV2::setAlignment(Alignment _alignment){
}

float LabelV2::getMeasuredWidth(){
	return 0.f;
}

float LabelV2::getMeasuredHeight(){
	return 0.f;
}

Line* LabelV2::currentLine(){
	return dynamic_cast<Line *>(childTransform->children.back());
}


///////////////////////////////////////
// !!! GlyphMeshEntity definitions !!!
///////////////////////////////////////

GlyphMeshEntity::GlyphMeshEntity(MeshInterface* _mesh, Shader* _shader, float _width, wchar_t _character):
	MeshEntity(_mesh, _shader),
	width(_width),
	character(_character),
	inUse(false){}


///////////////////////////////////////
// !!! Line definitions !!!
///////////////////////////////////////

Line::Line():
	width(0.f){}

void Line::invalidate(){
	for(unsigned long int j = 0; j < children.size(); ++j) {
		GlyphMeshEntity * mesh = dynamic_cast<GlyphMeshEntity *>(children.at(j));
		if(mesh != nullptr) {
			mesh->inUse = false;
		}
	}
}

void Line::insertGlyph(GlyphMeshEntity * _glyph, Shader * _textShader){
	_glyph->setShader(_textShader, true);
	_glyph->setVisible(true);
	addChild(_glyph)->translate(width + _glyph->width, 0.f, 0.f);
}