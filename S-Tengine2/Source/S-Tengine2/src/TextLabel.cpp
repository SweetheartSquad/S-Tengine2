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
	setAutoresizeHeight();
	setWidth(_width);
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
	text += _text;
	updateRequired = true;
}

std::wstring TextLabel::getText(){
	return text;
}

void TextLabel::autoResizeHeight(){
	setHeight(font->getLineHeight());
}

void TextLabel::autoResizeWidth(){
	setWidth(lineWidth);
}

void TextLabel::invalidate(){
	unusedGlyphs.insert(unusedGlyphs.end(), contents->children.begin(), contents->children.end());
	while(contents->children.size() > 0){
		Transform * trans = dynamic_cast<Transform *>(contents->children.back());
		for(unsigned long int i = 0; i < trans->children.size(); ++i) {
			// We need to make sure that the glyphs aren't keeping to references to the wrong parents
			GlyphMeshEntity * gme = dynamic_cast<GlyphMeshEntity *>(trans->children.at(i));
			while (gme->childTransform->children.size() > 0) {
				gme->childTransform->removeChild(gme->childTransform->children.back());
			}
		}
		contents->removeChild(contents->children.back());
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
		glyph->setGlyphMesh(glyphMesh);
		glyph->parents.clear();
		contents->addChild(oldTrans, false);
		oldTrans->translate(lineWidth, 0.f, 0.f, false);
		unusedGlyphs.pop_back();
		/*
		* This is an interesting issue that makes sense but we haven't had often.
		* Usually we call set shader on a new mesh entity which does this
		* Since we're reusing mesh entities configureDefaultVertexAttributes
		* doesn't get called on a mesh when it's swapped in since we've already set the shader.
		* For this reason we do it here. We check dirty since this indicates that this is the first time 
		* the mesh is being used
		*/
		if(glyphMesh->dirty) {
			glyphMesh->configureDefaultVertexAttributes(textShader);
		}
	}else{
		glyph = new GlyphMeshEntity(
				glyphMesh,
				textArea->textShader,
				_char);
		contents->addChild(glyph)->translate(lineWidth, 0.f, 0.f, false);
		glyph->load();
	}
	lineWidth += glyphMesh->advance.x/64.f;
}

bool TextLabel::canFit(float _width){
	if(!widthMode == kAUTO){
		if(_width + lineWidth > textArea->getWidth(false, false)){
			return false;
		}
	}
	return true;
}

// GlyphMeshEntity definitions //

GlyphMeshEntity::GlyphMeshEntity(Glyph * _mesh, Shader* _shader, wchar_t _character):
	MeshEntity(_mesh, _shader),
	character(_character),
	inUse(true),
	glyph(_mesh)
{
}

void GlyphMeshEntity::setGlyphMesh(Glyph * _mesh){
	childTransform->addChild(_mesh, false);
	glyph = _mesh;
}
