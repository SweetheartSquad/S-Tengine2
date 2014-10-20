#pragma once

#include "CMD_Parent.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"

CMD_Parent::CMD_Parent(std::vector<Joint *> * _joints) :
	joints(_joints)
{
}

void CMD_Parent::execute(){
}

void CMD_Parent::unexecute(){
	
}

CMD_Parent::~CMD_Parent(void){
}