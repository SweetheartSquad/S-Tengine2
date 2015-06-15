#pragma once

#include <TextLabel.h>
#include <TextArea.h>
#include <GL/glew.h>
#include <MeshFactory.h>
#include <Font.h>

TextLabel::TextLabel(BulletWorld* _world, Scene* _scene, Font* _font, Shader* _textShader, float _width):
	NodeUI(_world, _scene),
	Entity(),
	NodeBulletBody(_world),
	font(_font),
	textShader(_textShader),
	updateRequired(false),
	lineWidth(0.f),
	inUse(true),
	textArea(nullptr)
{
	autoResizingHeight = true;
	if(_width == UI_INFINITE){
		autoResizingWidth = true;
	}else{
		setWidth(_width);
	}
}

void TextLabel::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
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

void TextLabel::update(Step * _step){
	NodeUI::update(_step);
}

void TextLabel::unload(){
	NodeUI::load();
	textShader->load();
	font->unload();
}

void TextLabel::load(){
	NodeUI::unload();
	textShader->unload();
	font->load();
}

void TextLabel::setText(std::wstring _text){
	text = _text;
	updateRequired = true;
}

void TextLabel::appendText(std::wstring _text){
}

std::wstring TextLabel::getText(){
	return text;
}

void TextLabel::updateAlignment(){
}

//void TextLabel::setAlignment(Alignment _alignment){
//}

void TextLabel::autoResizeHeight(){
	setHeight(font->getLineHeight());
}

void TextLabel::autoResizeWidth(){
	setWidth(lineWidth);
}

///////////////////////////////////////
// !!! GlyphMeshEntity definitions !!!
///////////////////////////////////////

GlyphMeshEntity::GlyphMeshEntity(Glyph * _mesh, Shader* _shader, wchar_t _character):
	MeshEntity(_mesh, _shader),
	character(_character),
	inUse(true),
	glyph(_mesh)
{
}

void GlyphMeshEntity::setGlyphMesh(Glyph* _mesh){
	childTransform->children.clear();
	childTransform->addChild(_mesh);
	glyph = _mesh;
}

///////////////////////////////////////
// !!! Line definitions !!!
///////////////////////////////////////

void TextLabel::invalidate(){
	unusedGlyphs.insert(unusedGlyphs.end(), contents->children.begin(), contents->children.end());
	while(contents->children.size() > 0){
		contents->children.back()->removeParent(contents);
		contents->children.pop_back();
	}
	lineWidth = 0.f;
	textArea->removeLine(this);
}

void TextLabel::insertChar(wchar_t _char){
	GlyphMeshEntity * glyph = nullptr;
	Glyph * glyphMesh = textArea->font->getMeshInterfaceForChar(_char);
	if(unusedGlyphs.size() > 0) {
		Transform * oldTrans = dynamic_cast<Transform *>(unusedGlyphs.back());
		glyph = dynamic_cast<GlyphMeshEntity *>(oldTrans->children.at(0));
		glyph->childTransform->children.clear();
		glyph->setGlyphMesh(glyphMesh);
		contents->addChild(oldTrans, false);
		oldTrans->translate(lineWidth, 0.f, 0.f, false);
		unusedGlyphs.pop_back();
	}else{
		glyph = new GlyphMeshEntity(
				glyphMesh,
				textArea->textShader,
				_char);
		contents->addChild(glyph)->translate(lineWidth, 0.f, 0.f, false);
	}
	glyph->setVisible(true);
	lineWidth += glyphMesh->advance.x/64.f;
	
	glyph->unload();
	glyph->load();
}

bool TextLabel::canFit(float _width){
	if(!autoResizingWidth){
		if(_width + lineWidth > textArea->getWidth(false, false)){
			return false;
		}
	}
	return true;
}