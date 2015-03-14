#pragma once

#include <Structure.h>

class Hair: public Structure
{
public:
	Hair(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex);
	~Hair();

private:

};