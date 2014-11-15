#pragma once

#include "FrameBufferInterface.h"

class DepthFrameBuffer : public FrameBufferInterface{
public:
	explicit DepthFrameBuffer(bool _autoRelase);
	~DepthFrameBuffer();
};