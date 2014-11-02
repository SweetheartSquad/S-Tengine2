#pragma once 

class RenderSurfaceFilter{

private:	
	RenderSurfaceFilter();
	virtual ~RenderSurfaceFilter() = 0;

public:
	virtual void update() = 0;

};