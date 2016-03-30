#pragma once

#include <Configuration.h>
#include <FileUtils.h>
#include <Log.h>
#include <GLFW\glfw3.h>
#include <ctime>

Configuration::Configuration() :
	fullscreen(false),
	resolution(0),
	fps(0),
	monitor(0),
	rngSeed(0)
{
}

void Configuration::load(const std::string & _filename){
	// get the current video mode so that we can set the resolution defaults
	const GLFWvidmode * vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	bool fullscreenDefault = false;
	glm::uvec2 resolutionDefault = glm::uvec2(vidmode->width*0.9, vidmode->height*0.9);
	double fpsDefault = 60.0;
	signed long int rngSeedDefault = 0;
	signed long int monitorDefault = 0;


	Json::Reader reader;
	Json::Value json;
	bool parsingSuccessful;
	
	if(!sweet::FileUtils::fileExists(_filename)){
		// set defaults
		fullscreen = fullscreenDefault;
		resolution = resolutionDefault;
		fps = fpsDefault;

		// save defaults to file
		save(_filename);
	}
	sweet::FileUtils::createFileIfNotExists(_filename);
	
	std::string jsonLoaded = sweet::FileUtils::readFile(_filename);
	parsingSuccessful = reader.parse(jsonLoaded, json);

	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages()/* + "\n" + jsonLoaded*/);
	}else{
		fullscreen = json.get("fullscreen", fullscreenDefault).asBool();
		resolution = glm::uvec2(json["resolution"].get("x", resolutionDefault.x).asInt(), json["resolution"].get("y", resolutionDefault.y).asInt());
		fps = json.get("fps", fpsDefault).asDouble();
		rngSeed = json.get("rngSeed", rngSeedDefault).asInt();
		monitor = json.get("monitor", monitorDefault).asInt();
		Json::Value jsonGain = json["gain"];

		// gain defaults
		gain["master"] = gain["voice"] = gain["music"] = gain["sfx"] = gain["other"] = 1;

		for(auto g : jsonGain.getMemberNames()){
			gain[g] = jsonGain.get(g, 1.f).asFloat();
		}

		// if the rng seed is negative, it means we want to use the time as a seed
		if(rngSeed < 0){
			rngSeed = time(nullptr);
		}
	}
}

void Configuration::save(const std::string & _filename){
	Json::Value json;
	json["fullscreen"] = fullscreen;
	json["resolution"]["x"] = resolution.x;
	json["resolution"]["y"] = resolution.y;
	json["fps"] = fps;
	json["rngSeed"] = rngSeed;
	json["monitor"] = monitor;
	json["gain"] = Json::Value();
	for(auto g : gain){
		json["gain"][g.first] = g.second;
	}

	std::ofstream log(_filename, std::ios_base::out);
	log << json;
	log.close();
}