#pragma once

#include <FpsDisplay.h>
#include <MeshInterface.h>
#include <shader/Shader.h>

FpsDisplay::FpsDisplay(BulletWorld* _world, Scene* _scene, Font * _font, Shader * _textShader) :
	TextArea(_world, _scene, _font, _textShader, 300),
	numFrames(0),
	avgFps(0.0f),
	currentFps(0.0f),
	numCycles(0),
	lastTime(0.0),
	totalFrames(0)
{
}

void FpsDisplay::update(Step* _step) {

	if(!textShader->loaded) {
		textShader->load();
	}

	double currentTime = glfwGetTime();
	numFrames++;
	totalFrames++;
	if (currentTime - lastTime >= 1.0){
		numCycles++;
		currentFps = numFrames;
		avgFps = (float)totalFrames / (float)numCycles;
		std::wstring s(L"FPS: " + std::to_wstring(numFrames) + L"\n" + L"AVG FPS: " + std::to_wstring(avgFps));
		setText(s); 
		numFrames = 0;
		lastTime += 1.0;
	}
	TextArea::update(_step);
}