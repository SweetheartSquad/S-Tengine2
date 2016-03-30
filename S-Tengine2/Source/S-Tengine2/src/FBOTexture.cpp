#pragma once

#include <FBOTexture.h>
#include <FrameBufferInterface.h>

FBOTexture::FBOTexture(FrameBufferInterface * _frameBuffer, bool _storeData, int _frameBufferChannelIndex, bool _autoRelease, bool _useMipmaps) :
	NodeResource(_autoRelease),
	fbo(_frameBuffer),
	fboChannelIndex(_frameBufferChannelIndex),
	ProgrammaticTexture(nullptr, _autoRelease, _useMipmaps)
{
	refresh();
}

void FBOTexture::refresh(){
	// if there is already data in the texture, free it
	if(data != nullptr){
		unloadImageData();
	}

	// resize in case the FBO has different dimensions
	resize(fbo->width, fbo->height, fbo->frameBufferChannels.at(fboChannelIndex).numChannels);
	// get the new data
	setTextureData(fbo->getPixelData(fboChannelIndex));
}