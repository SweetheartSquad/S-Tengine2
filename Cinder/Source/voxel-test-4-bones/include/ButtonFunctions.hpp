#include "CinderApp.h"
#include "Commands/CMD_SetTime.h"
#include "TrackBar.h"

class ButtonFunctions{
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
	
    static void VOXEL_ShowHide(CinderApp * _app);
    static void VOXEL_Preview(CinderApp * _app);
    static void VOXEL_Selectable(CinderApp * _app);
	
	static void TIME_Decrement(CinderApp * _app);
	static void TIME_PlayPause(CinderApp * _app);
	static void TIME_Increment(CinderApp * _app);

	static void SPACE_Object(CinderApp * _app);
	static void SPACE_World(CinderApp * _app);
};

void ButtonFunctions::MODE_Select(CinderApp * _app){
	_app->mode = CinderApp::UImode::kSELECT;
};
void ButtonFunctions::MODE_Translate(CinderApp * _app){
	_app->mode = CinderApp::UImode::kTRANSLATE;
};
void ButtonFunctions::MODE_Rotate(CinderApp * _app){
	_app->mode = CinderApp::UImode::kROTATE;
};
void ButtonFunctions::MODE_Scale(CinderApp * _app){
	_app->mode = CinderApp::UImode::kSCALE;
};

void ButtonFunctions::MODE_CreateJoints(CinderApp * _app){
	_app->mode = CinderApp::UImode::kCREATE;
};

void ButtonFunctions::MODE_PaintVoxels(CinderApp * _app){
	_app->mode = CinderApp::UImode::kPAINT_VOXELS;
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

void ButtonFunctions::VOXEL_ShowHide(CinderApp * _app){
    _app->viewJointsOnly = !_app->viewJointsOnly;
};

void ButtonFunctions::VOXEL_Preview(CinderApp * _app){
    _app->voxelPreviewMode = !_app->voxelPreviewMode;
};

void ButtonFunctions::VOXEL_Selectable(CinderApp * _app){
	_app->voxelSelectMode = !_app->voxelSelectMode;
};

void ButtonFunctions::TIME_Decrement(CinderApp * _app){
	if(UI::time-1 < _app->timelineTrackbar->min){
		_app->cmdProc->executeCommand(new CMD_SetTime(&UI::time, _app->timelineTrackbar->max, false));
	}else{
		_app->cmdProc->executeCommand(new CMD_SetTime(&UI::time, UI::time-1-fmod(UI::time, _app->timelineTrackbar->step), false));
	}
};

void ButtonFunctions::TIME_PlayPause(CinderApp * _app){
	_app->play = !_app->play;
	if(!_app->play){
		_app->cmdProc->executeCommand(new CMD_SetTime(&UI::time, (_app->timelineTrackbar->step/2.f)-fmod((_app->timelineTrackbar->step/2.f)+UI::time, _app->timelineTrackbar->step), true));
		_app->cmdProc->endCompressing();
	}else{
		_app->cmdProc->startCompressing();
	}
};

void ButtonFunctions::TIME_Increment(CinderApp * _app){
	if(UI::time+1 > _app->timelineTrackbar->max){
		_app->cmdProc->executeCommand(new CMD_SetTime(&UI::time, _app->timelineTrackbar->min, false));
	}else{
		_app->cmdProc->executeCommand(new CMD_SetTime(&UI::time, UI::time+1-fmod(UI::time, _app->timelineTrackbar->step), false));
	}
};

void ButtonFunctions::SPACE_Object(CinderApp * _app){
	_app->translateSpace = kOBJECT;
};

void ButtonFunctions::SPACE_World(CinderApp * _app){
	_app->translateSpace = kWORLD;
};