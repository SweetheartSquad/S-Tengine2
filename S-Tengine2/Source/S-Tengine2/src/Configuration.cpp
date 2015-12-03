#pragma once

#include <Configuration.h>
#include <FileUtils.h>
#include <Log.h>
#include <GLFW\glfw3.h>
Configuration::Configuration() :
	fullscreen(false),
	resolution(0),
	fps(0)
{
}

void Configuration::load(const std::string & _filename){
	// get the current video mode so that we can set the resolution defaults
	const GLFWvidmode * vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	bool fullscreenDefault = false;
	glm::uvec2 resolutionDefault = glm::uvec2(vidmode->width*0.9, vidmode->height*0.9);
	double fpsDefault = 60.0;


	Json::Reader reader;
	Json::Value json;
	bool parsingSuccessful;
	
	if(!FileUtils::fileExists(_filename)){
		// set defaults
		fullscreen = fullscreenDefault;
		resolution = resolutionDefault;
		fps = fpsDefault;

		// save defaults to file
		save(_filename);
	}
	FileUtils::createFileIfNotExists(_filename);
	
	std::string jsonLoaded = FileUtils::readFile(_filename);
	parsingSuccessful = reader.parse(jsonLoaded, json);

	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages()/* + "\n" + jsonLoaded*/);
	}else{
		fullscreen = json.get("fullscreen", fullscreenDefault).asBool();
		resolution = glm::uvec2(json["resolution"].get("x", resolutionDefault.x).asInt(), json["resolution"].get("y", resolutionDefault.y).asInt());
		fps = json.get("fps", fpsDefault).asDouble();
	}
}

void Configuration::save(const std::string & _filename){
	Json::Value json;
	json["fullscreen"] = fullscreen;
	json["resolution"]["x"] = resolution.x;
	json["resolution"]["y"] = resolution.y;
	json["fps"] = fps;
		
	std::ofstream log(_filename, std::ios_base::out);
	log << json;
	log.close();
}