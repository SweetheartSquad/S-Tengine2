#include "CinderApp.h"
#include "CMD_SetTime.h"

class ButtonFunctions
{
public:

	static void MODE_Select(CinderApp * _app);
	static void MODE_Translate(CinderApp * _app);
	static void MODE_Rotate(CinderApp * _app);
	static void MODE_Scale(CinderApp * _app);
	static void MODE_CreateJoints(CinderApp * _app);
	static void MODE_PaintVoxels(CinderApp * _app);

	static void CHANNEL_0(CinderApp * _app);
	static void CHANNEL_1(CinderApp * _app);
	static void CHANNEL_2(CinderApp * _app);
	static void CHANNEL_3(CinderApp * _app);

	static void TIME_Decrement(CinderApp * _app);
	static void TIME_Increment(CinderApp * _app);
};

void ButtonFunctions::MODE_Select(CinderApp * _app){
	_app->mode = CinderApp::UImode::SELECT;
	_app->params->setOptions("UI Mode", "label=`SELECT`");
};
void ButtonFunctions::MODE_Translate(CinderApp * _app){
	_app->mode = CinderApp::UImode::TRANSLATE;
	_app->params->setOptions("UI Mode", "label=`TRANSLATE`");
};
void ButtonFunctions::MODE_Rotate(CinderApp * _app){
	_app->mode = CinderApp::UImode::ROTATE;
	_app->params->setOptions("UI Mode", "label=`ROTATE`");
};
void ButtonFunctions::MODE_Scale(CinderApp * _app){
	_app->mode = CinderApp::UImode::SCALE;
	_app->params->setOptions("UI Mode", "label=`SCALE`");
};

void ButtonFunctions::MODE_CreateJoints(CinderApp * _app){
	_app->mode = CinderApp::UImode::CREATE;
	_app->params->setOptions("UI Mode", "label=`CREATE`");
};

void ButtonFunctions::MODE_PaintVoxels(CinderApp * _app){
	_app->mode = CinderApp::UImode::PAINT_VOXELS;
	_app->params->setOptions("UI Mode", "label=`PAINT VOXELS`");
};

void ButtonFunctions::CHANNEL_0(CinderApp * _app){
	_app->channel = 0;
};

void ButtonFunctions::CHANNEL_1(CinderApp * _app){
	_app->channel = 1;
};

void ButtonFunctions::CHANNEL_2(CinderApp * _app){
	_app->channel = 2;
};

void ButtonFunctions::CHANNEL_3(CinderApp * _app){
	_app->channel = 3;
};

void ButtonFunctions::TIME_Decrement(CinderApp * _app){
	_app->cmdProc->executeCommand(new CMD_SetTime(&UI::time, 1, true));
};

void ButtonFunctions::TIME_Increment(CinderApp * _app){
	_app->cmdProc->executeCommand(new CMD_SetTime(&UI::time, -1, true));
};

