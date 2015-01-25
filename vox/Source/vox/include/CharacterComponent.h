#include "Box2DSprite.h"
#include "box2d/Box2D.h"

class CharacterComponent : public Box2DSprite{
public:
	
	float width;
	float height;
	float scale;

	CharacterComponent(float _componentScale, float _width, float _height, Texture * _texture, Box2DWorld * _world, b2BodyType _bodyType = b2_dynamicBody, bool _defaultFixture = true, Shader * _shader = nullptr, Transform * _transform = new Transform());
	
	float getCorrectedHeight();
	float getCorrectedWidth();

	void createFixture();
};