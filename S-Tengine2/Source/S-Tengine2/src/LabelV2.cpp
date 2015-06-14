#pragma once

#include <LabelV2.h>
#include <GL/glew.h>
#include <MeshFactory.h>
#include <Font.h>

LabelV2::LabelV2(BulletWorld* _world, Scene* _scene, Font* _font, Shader* _textShader, Shader* _backgroundShader, float _width):
	NodeUI(_world, _scene),
	MeshEntity(MeshFactory::getPlaneMesh()),
	NodeBulletBody(_world),
	font(_font),
	textShader(_textShader),
	backgroundShader(_backgroundShader),
	width(0.f),
	lines(new Transform()),
	updateRequired(false)
{
	// Add initial line
	childTransform->addChild(lines, false);
	lines->addChild(new Line(this), false);
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
	for(unsigned long int i = 0; i < lines->children.size(); ++i) {
		Line * line = dynamic_cast<Line *>(lines->children.at(i));
		if(line != nullptr) {
			line->invalidate();
		}
	}
}

void LabelV2::update(Step* _step){
	if(updateRequired) {
		Line * curLine = currentLine();
		for(unsigned long int i = 0; i < text.size(); ++i){
			curLine->insertChar(text.at(i));
		}
		updateRequired = false;
	}
	NodeUI::update(_step);
}

void LabelV2::unload(){
	NodeUI::load();
	textShader->load();
	backgroundShader->load();
	font->unload();
}

void LabelV2::load(){
	NodeUI::unload();
	textShader->unload();
	backgroundShader->unload();
	font->load();
}

void LabelV2::setText(std::wstring _text){
	text = _text;
	invalidateAllLines();	
	updateRequired = true;
}

void LabelV2::appendText(std::wstring _text){
}

std::wstring LabelV2::getText(){
	return text;
}

void LabelV2::updateAlignment(){
}

//void LabelV2::setAlignment(Alignment _alignment){
//}

float LabelV2::getMeasuredWidth(){
	return 0.f;
}

float LabelV2::getMeasuredHeight(){
	return 0.f;
}

Line* LabelV2::currentLine(){
	return dynamic_cast<Line *>(lines->children.back());
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

Line::Line(LabelV2 * _label):
	Transform(),
	label(_label),
	width(0.f),
	inUse(true)
{}

void Line::invalidate(){
	unusedGlyphs.insert(unusedGlyphs.end(), children.begin(), children.end());
	children.clear();
	width = 0.f;
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
	glyph->load();
}