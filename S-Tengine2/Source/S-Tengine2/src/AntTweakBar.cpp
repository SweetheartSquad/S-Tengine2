#pragma once

// If the engine is being compiled for release,
// all of the AntTweakBar functions will be redefined
// such that they don't actually do anything
//

#if _MSC_VER == 1700
	#ifndef _DEBUG
		#define _USE_THIS_THING
	#endif
#endif

#ifdef _USE_THIS_THING

#include <AntTweakBar.h>

int TwWindowSize(int width, int height){return 0;}

int TwTerminate(){return 0;}
int TwMouseMotion(int mouseX, int mouseY){return 0;}
int TwInit(TwGraphAPI, void*){return 0;}
int TwMouseWheel(int){return 0;}
int TwEventMouseButtonGLFW(int, int){return 0;}
int TwEventKeyGLFW(int, int){return 0;}
int TwEventCharGLFW(int, int){return 0;}
int TwDraw(){return 0;}
TwBar * TwNewBar(const char*){return nullptr;}
int TwAddVarCB(TwBar*, const char *, TwType, TwSetVarCallback, TwGetVarCallback, void *, const char *){return 0;}
int TwAddSeparator(TwBar*, const char *, const char *){return 0;}

#endif