#pragma once

#include "MeshEntity.h"
#include <Box2D/Box2D.h>
#include <node/NodeResource.h>
#include <node\NodeShadable.h>

class BitmapFont;
class Scene;
class LayeredScene;
class Shader;
class Box2DWorld;
class Box2DSprite;
class CharacterComponent;
class Texture;
class TextureSampler;

class Box2DSuperSprite : public virtual Entity, public virtual NodeShadable{
public:
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

	// saves the shader on this supersprite and sets it on all its components
	virtual void setShader(Shader * _shader, bool _configureDefaultVertexAttributes);
	
	virtual void translateComponents(glm::vec3 _translateVector);
	virtual void translateComponents(float _x, float _y, float _z);
	void setUserData(void * _data);

	// Sets the group index on all components
	void setGroupIndex(int16 _groupIndex);

	/**
	* Adds a component to the components list. If the shader of this is not null and the shader
	* of the component is null then the shader of this will be applied to the component
	*/
	virtual void addComponent(Box2DSprite ** _component);

	void snapComponents(Box2DSprite * _sprite);
};