#pragma once

#include <ProgrammaticTexture.h>

class FrameBufferInterface;

class FBOTexture : public ProgrammaticTexture{
private:
	FrameBufferInterface * fbo;
	int fboChannelIndex;
public:
	explicit FBOTexture(FrameBufferInterface * _frameBuffer, bool _storeData, int _frameBufferChannelIndex, bool _autoRelease, bool _useMipmaps = true);

	// copies the data from the FBO into the texture
	void refresh();
};