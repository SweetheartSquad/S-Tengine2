#pragma once

#include <scenario/Option.h>

Option::Option(Json::Value _json, Scenario * _scenario) :
	text(_json.get("text", "").asString()),
	link(_json.get("link", "END").asString())
{
}

Option::~Option(){

}