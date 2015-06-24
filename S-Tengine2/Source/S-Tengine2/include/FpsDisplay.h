#pragma once

#include <TextArea.h>

class FpsDisplay : public TextArea {
public:

	float currentFps;
	float avgFps;

	FpsDisplay(BulletWorld * _world, Scene * _scene, Font * font, Shader * _textShaders);

	void update(Step* _step) override;

private:

	int numFrames;
	double lastTime;
	int numCycles;
	int totalFrames; 
};