#pragma once 

#include "MeshEntity.h"
#include "Rectangle.h"

#include <map>
#include "SpriteSheetAnimation.h"

class Texture;
class TextureSampler;
class SpriteSheetAnimation;
class Rectangle;
class SpriteMesh;
class SpriteSheet;

class Sprite : public MeshEntity{
public:
	SpriteSheet * spriteSheet;
	SpriteSheetAnimationInstance * currentAnimation;
	bool playAnimation;

	explicit Sprite(Shader * _shader = nullptr);
	explicit Sprite(Texture * _texture, Shader * _shader = nullptr);
	explicit Sprite(TextureSampler *_textureSampler, Shader * _shader = nullptr);
	virtual ~Sprite();

	Vertex * getTopLeft();
	Vertex * getTopRight();
	Vertex * getBottomLeft();
	Vertex * getBottomRight();

	void setUvs(float _topLeftU, float _topLeftV, float _topRightU, float _topRightV, 
	float _bottomLeftU, float _bottomLeftV, float _bottomRightU, float _bottomRightV);
	void setUvs(sweet::Rectangle _rect);
	virtual void update(Step* _step) override;
	void setCurrentAnimation(std::string _name);
	
	void setPrimaryTexture(Texture * _texture);
	void setPrimaryTexture(TextureSampler * _textureSampler);

	void setSpriteSheet(SpriteSheet * _spriteSheet, std::string _currentAnimation);
};