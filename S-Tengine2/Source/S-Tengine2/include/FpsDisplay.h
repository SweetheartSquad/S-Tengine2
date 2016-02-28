#pragma once

#include <TextArea.h>

class FpsDisplay : public TextArea {
public:
	float avgFps;

	FpsDisplay(BulletWorld * _world, Font * font, Shader * _textShaders);

	void update(Step* _step) override;

private:

	unsigned long int numFrames;
	double lastTime;
	unsigned long int numCycles;
	unsigned long int totalFrames; 
};