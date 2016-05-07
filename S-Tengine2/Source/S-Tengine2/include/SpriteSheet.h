#pragma once

#include <map>
#include <node/NodeLoadable.h>

class Texture;
class SpriteSheetAnimationDefinition;

class SpriteSheet : public NodeLoadable{
public:
	std::map<std::string, SpriteSheetAnimationDefinition *> animations;
	Texture * texture;

	explicit SpriteSheet(Texture * _texture);
	~SpriteSheet();
	
	void addAnimation(std::string _name, SpriteSheetAnimationDefinition * _animation);
	void addAnimation(std::string _name, unsigned long int _start, unsigned long int _end, float _width, float _height, float _secondsPerFrame);
	
	virtual void load() override;
	virtual void unload() override;
};