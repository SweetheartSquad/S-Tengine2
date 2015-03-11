#pragma once

#include "MeshEntity.h"
#include <Box2D/Box2D.h>

class BitmapFont;
class Scene;
class LayeredScene;
class Shader;
class Box2DWorld;
class Box2DSprite;
class CharacterComponent;
class Texture;

class TextureSampler{
public:

	Texture * texture;

	float width;
	float height;

	TextureSampler(Texture * _texture, float _width, float _height);
};

class Box2DSuperSprite : public MeshEntity {
public:
	
	static int16 gGroupIndex;
	int16 groupIndex;
	int16 categoryBits; // Box2D entity category
	int16 maskBits;		// Box2D collides only with
	float componentScale;
	
	Box2DWorld * world;
	Scene * scene;
	signed long int sceneLayer;

	std::vector<Box2DSprite **> components;

	// Component to which others are relative (ex: character torso, item handle)
	Box2DSprite * rootComponent;

	explicit Box2DSuperSprite(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	
	~Box2DSuperSprite();

	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack) override;
	virtual void update(Step* _step) override;

	virtual void setShader(Shader * _shader, bool _configureDefaultVertexAttributes) override;
	
	virtual void addToScene(Scene * _scene);

	virtual void addToLayeredScene(LayeredScene * _scene, unsigned long int _layer);

	void translateComponents(glm::vec3 _translateVector);
	void setUserData(void * _data);

	// Sets the group index on all components
	void setGroupIndex(int16 _groupIndex);

	void addComponent(Box2DSprite * _component);
};