#pragma once

#include "MeshEntity.h"
#include <Box2D/Box2D.h>

class BitmapFont;
class Scene;
class Shader;
class Box2DWorld;
class Box2DSprite;
class CharacterComponent;
class Texture;

class ComponentTexture{
public:
	Texture * texture;
	float width;
	float height;

	ComponentTexture(Texture * _texture, float _width, float _height);
};

class Character : public MeshEntity{
public:
	bool ai;
	static int16 gGroupIndex;
	int16 groupIndex;
	int16 categoryBits; // Box2D entity category
	int16 maskBits;		// Box2D collides only with
	float componentScale;
	
	Box2DWorld * world;

	std::vector<CharacterComponent **> components;

	explicit Character(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, bool _ai = true);
	
	~Character();

	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	void update(Step* _step) override;

	virtual void setShader(Shader * _shader, bool _configureDefaultVertexAttributes) override;

	virtual void addToScene(Scene * _scene);

	void translateComponents(glm::vec3 _translateVector);
};