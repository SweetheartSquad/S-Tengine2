#include "UiInteractable.h"

#include "CinderRenderOptions.h"

#include <cinder\gl\gl.h>

UiInteractable::UiInteractable(ci::Vec2i _pos, ci::Vec2i _size) :
    NodeSelectable(),
    pos(_pos),
    size(_size),
	isHovered(false),
	isDown(false),
	isActive(false),
	displayColor((float)(std::rand()%255)/255.f, (float)(std::rand()%255)/255.f, (float)(std::rand()%255)/255.f)
{
}

void UiInteractable::down(CinderApp * _app){
	isHovered = true;
	isDown = true;
	
    downHandler(_app);
}
void UiInteractable::up(CinderApp * _app){
	isDown = false;
	upHandler(_app);
}
void UiInteractable::in(){
	isHovered = true;
}
void UiInteractable::out(){
	isHovered = false;
}

void UiInteractable::pressProgrammatically(CinderApp * _app){
	in();
	down(_app);
	up(_app);
	out();
}

UiInteractable::~UiInteractable()
{
}