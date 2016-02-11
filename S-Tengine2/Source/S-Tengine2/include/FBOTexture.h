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
	// NOTE: Does not call bufferData; you must call this manually if you plan on rendering the updated texture
	void refresh();
};