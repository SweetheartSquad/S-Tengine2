#pragma once

#include <TextureSampler.h>
#include <Texture.h>

TextureSampler::TextureSampler(Texture * _texture, float _width, float _height, float _u, float _v) :
	NodeResource(true),
	texture(_texture),
	width(_width),
	height(_height),
	u(_u),
	v(_v)
{
}

void TextureSampler::load(){
	texture->load();
	NodeResource::load();
}

void TextureSampler::unload(){
	texture->unload();
	NodeResource::unload();
}