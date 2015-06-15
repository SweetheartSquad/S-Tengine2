#pragma once

#include <TextLabel.h>
#include <GL/glew.h>
#include <MeshFactory.h>
#include <Font.h>

TextLabel::TextLabel(BulletWorld* _world, Scene* _scene, Font* _font, Shader* _textShader, float _width):
	NodeUI(_world, _scene),
	Entity(),
	NodeBulletBody(_world),
	font(_font),
	textShader(_textShader),
	lines(new Transform()),
	updateRequired(false)
{
	autoResizingWidth = true;
	setWidth(_width);
	childTransform->addChild(lines, false);
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

void TextLabel::invalidateAllLines(){
	for(unsigned long int i = 0; i < lines->children.size(); ++i) {
		Line * line = dynamic_cast<Line *>(lines->children.at(i));
		line->invalidate();
	}
}

void TextLabel::update(Step * _step){
	if(updateRequired) {
		Line * curLine = getLine();
		lines->addChild(curLine, false);
		float curY = 0.f;
		for(unsigned long int i = 0; i < text.size(); ++i){
			if(!curLine->canFit(font->getGlyphWidthHeight(text.at(i)).x)){
				curY -= font->getLineHeight();
				curLine = getLine();
				lines->addChild(curLine);
			}
			curLine->insertChar(text.at(i));	
			curLine->translate(0.f, curY, 0.f, false);
		}
		updateRequired = false;
	}
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
	invalidateAllLines();	
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

Line* TextLabel::getLine() {
	Line * line = nullptr;
	if(unusedLines.size() > 0) {
		line = dynamic_cast<Line *>(unusedLines.back());
		unusedLines.pop_back();
	}else {
		line = new Line(this);
	}
	return line;
}

Line * TextLabel::currentLine(){
	return dynamic_cast<Line *>(lines->children.back());
}

void TextLabel::autoResizeHeight(){
	setHeight(font->getLineHeight());
}

void TextLabel::autoResizeWidth(){
	float width = 0;
	for(unsigned long int i = 0; i < lines->children.size(); ++i){
		Line * l = dynamic_cast<Line *>(lines->children.at(i));
		width = std::max(width, l->width);
	}
	setWidth(width);
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

Line::Line(TextLabel * _label):
	Transform(),
	label(_label),
	width(0.f),
	inUse(true)
{}

void Line::invalidate(){
	unusedGlyphs.insert(unusedGlyphs.end(), children.begin(), children.end());
	children.clear();
	width = 0.f;
	label->lines->removeChild(this);
	label->unusedLines.push_back(this);
}

void Line::insertChar(wchar_t _char){
	GlyphMeshEntity * glyph = nullptr;
	Glyph * glyphMesh = label->font->getMeshInterfaceForChar(_char);
	if(unusedGlyphs.size() > 0) {
		Transform * oldTrans = dynamic_cast<Transform *>(unusedGlyphs.back());
		glyph = dynamic_cast<GlyphMeshEntity *>(oldTrans->children.at(0));
		glyph->childTransform->children.clear();
		glyph->setGlyphMesh(glyphMesh);
		children.push_back(oldTrans);
		oldTrans->translate(width, 0.f, 0.f, false);
		unusedGlyphs.pop_back();
	}else{
		glyph = new GlyphMeshEntity(
				glyphMesh,
				label->textShader,
				_char);
		addChild(glyph)->translate(width, 0.f, 0.f, false);
	}
	glyph->setVisible(true);
	width += glyphMesh->advance.x/64.f;
	
	glyph->unload();
	glyph->load();
}

bool Line::canFit(float _width){
	if(abs(width - INFINITE_WIDTH) > 0.005){
		if(_width + width > label->getWidth(false, false)){
			return false;
		}
	}
	return true;
}