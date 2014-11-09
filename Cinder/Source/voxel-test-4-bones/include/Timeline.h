#pragma once

#include <vector>
#include "Keyframe.h"

namespace st{
	class Timeline
	{
	public:
		Timeline(void);
		~Timeline(void);

		std::vector<Keyframe> keyframes;
	};
};
