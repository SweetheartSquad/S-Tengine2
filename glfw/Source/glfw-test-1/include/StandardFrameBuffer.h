#pragma once

#include "FrameBufferInterface.h"

class StandardFrameBuffer : public FrameBufferInterface{
public:
	explicit StandardFrameBuffer(bool _autoRelase);
	virtual ~StandardFrameBuffer();
};