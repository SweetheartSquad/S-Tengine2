#pragma once

#include <AutoMusic.h>
#include <Step.h>
#include <NumberUtils.h>
#include <sstream>

int AutoMusic::scales[] = {
	0,	2,	4,	5,	7,	9,	11,	12,	13,	//kIONIAN
	0,	2,	3,	5,	7,	9,	10,	2,	13,	//kDORIAN
	0,	1,	3,	5,	7,	8,	10,	12,	13,	//kPHRYGIAN
	0,	2,	4,	6,	7,	9,	11,	12,	13,	//kLYDIAN
	0,	2,	4,	5,	7,	9,	11,	12,	13,	//kMIXOLYDIAN
	0,	2,	3,	5,	7,	8,	10,	12,	13,	//kAEOLIAN
	0,	1,	3,	5,	6,	8,	10,	12,	13	//kLOCRIAN
};

AutoMusic::AutoMusic(float _lengthInBeats, int _pitch, float _volume) :
	lengthInBeats(_lengthInBeats),
	pitch(_pitch),
	volume(_volume)
{
}

void AutoMusic::update(Step * _step){

}

std::string AutoMusic::toString(){
	return "?";
}

AutoTrack::AutoTrack(float _bpm, unsigned long int _timeSignatureTop, unsigned long int _timeSignatureBottom, int _pitch, float _volume) :
	AutoMusic((float)_timeSignatureTop/_timeSignatureBottom, _pitch, _volume),
	timeSignatureTop(_timeSignatureTop),
	timeSignatureBottom(_timeSignatureBottom),
	bpm(_bpm)
{
}

AutoTrack::~AutoTrack(){
	clearComponents();
}

float AutoTrack::getSecondsPerBeat() const{
	// beats per minute == beats per second / 60
	// beats per second == 1 / seconds per beat
	// seconds per beat == 1 / (beats per minute / 60)
	return 1.f / (bpm / 60.f) * timeSignatureBottom;
}

void AutoTrack::clearComponents(){
	while(components.size() > 0){
		delete components.back();
		components.pop_back();
	}
}

void AutoTrack::generate(){
	lengthInSeconds = lengthInBeats * getSecondsPerBeat();

	curComponent = 0;
	curTime = 0;
	nextComponent = 0;//(components.size() > 0) ? components.front()->lengthInBeats * getSecondsPerBeat() : 0;
}


void AutoTrack::update(Step * _step){
	if(components.size() > 0){
		curTime += _step->deltaTime;

		if(curTime > nextComponent){
			curComponent += 1;
			// looping
			while(curComponent >= components.size()){
				curComponent -= components.size();
				curTime -= lengthInSeconds;
				nextComponent -= lengthInSeconds;
			}
			nextComponent += components.at(curComponent)->lengthInBeats * getSecondsPerBeat();

			// play the active component
			playComponent(components.at(curComponent));
		}
		// update the active component
		components.at(curComponent)->update(_step);
	}
}

void AutoTrack::playComponent(AutoMusic * _component){

}

std::string AutoTrack::toString(){
	std::stringstream ss;
	for(AutoMusic * c : components){
		ss << c->toString();
	}
	return ss.str();
}

void AutoTrack::randomProperties(){
	bpm = sweet::NumberUtils::randomInt(60, 240);
	timeSignatureBottom = sweet::NumberUtils::randomInt(2,8);
	timeSignatureTop = sweet::NumberUtils::randomInt(1,timeSignatureBottom+1);	
}

Note::Note(float _lengthInBeats, int _pitch, float _volume) :
	AutoMusic(_lengthInBeats, _pitch, _volume)
{
	if(lengthInBeats <= 0.001){
		throw;
	}
}

std::string Note::toString(){
	std::stringstream ss;
	ss << (volume == 1 ? "X" : (volume == 0.5 ? "x" : (volume == 0.25 ? "*" : "0")));
	for(unsigned long int i = 1; i < lengthInBeats*64; ++i){
		ss << "-";
	}
	return ss.str();
}

AutoRiff::AutoRiff(OpenAL_Sound * _instrument, Mode _mode) :
	AutoTrack(120, 4, 4, 0, 1),
	instrument(_instrument),
	mode(_mode),
	generationMax(8),
	generationMin(-8)
{
	if(instrument != nullptr){
		++instrument->referenceCount;
	}
}

AutoRiff::~AutoRiff(){
	instrument->decrementAndDelete();
}

void AutoRiff::playComponent(AutoMusic * _component){
	int notePitch = scales[mode*8 + _component->pitch];
	instrument->setPitch(pow(2, (pitch+notePitch)/13));
	instrument->setGain(_component->volume);
	instrument->play();
}



void AutoRiff::generate(){
	clearComponents();

	// fill the length of the riff
	float curLengthInBeats = 0;
	while(curLengthInBeats < lengthInBeats){

		float genLengthInBeats = 0;
		switch(sweet::NumberUtils::randomInt(0, 3)){
			case 0: genLengthInBeats = 1; break;
			case 1: genLengthInBeats = 0.5; break;
			case 2: genLengthInBeats = 0.25; break;
			case 3: genLengthInBeats = 0.125; break;
		}
		// generate a random number on the scale (will be used as an array accessor for AutoRiff::scales
		float genPitch = sweet::NumberUtils::randomInt(generationMin, generationMax);
		// make some notes rests instead of actual notes
		float genVolume = sweet::NumberUtils::randomInt(0, 1);

		components.push_back(new Note(genLengthInBeats, genPitch, genVolume));
		curLengthInBeats += components.back()->lengthInBeats;
	}
	if(curLengthInBeats > lengthInBeats){
		components.back()->lengthInBeats -= curLengthInBeats - lengthInBeats;
	}

	AutoTrack::generate();
}

AutoSnare::AutoSnare(OpenAL_Sound * _instrument) :
	AutoRiff(_instrument)
{
}

AutoKick::AutoKick(OpenAL_Sound * _instrument) :
	AutoRiff(_instrument)
{
}

AutoRide::AutoRide(OpenAL_Sound * _instrument) :
	AutoRiff(_instrument)
{
}

void AutoSnare::generate(){
	clearComponents();

	// generate snare hits
	float beatUnit = 1.f/timeSignatureBottom;
	for(unsigned long int i = 0; i < timeSignatureTop; ++i){
		
		int numSnare = 0;
		switch(sweet::NumberUtils::randomInt(0,2)){
			case 0:	numSnare = 1; break;
			case 1:	numSnare = 2; break;
			case 2:	numSnare = 4; break;
		}
		float l = beatUnit/numSnare;
		if(l < beatUnit && sweet::NumberUtils::randomInt(0,1)){
			l = beatUnit-l;
		}

		bool addSecondary = sweet::NumberUtils::randomInt(0, 1);
		

		if(i % 2 == 1){
			if(addSecondary){
				components.push_back(new Note(l, 0, 1)); // primary hit
				if(l < beatUnit){
					components.push_back(new Note(beatUnit-l, 0, 0.5)); // secondary hit
				}
			}else{
				components.push_back(new Note(beatUnit, 0, 1)); // primary hit for full beat
			}
		}else{
			if(addSecondary){
				components.push_back(new Note(beatUnit, 0, 0.5)); // secondary hit for full beat
			}else{
				 // full rest
				if(components.size() > 0){
					components.back()->lengthInBeats += beatUnit;
				}else{
					components.push_back(new Note(beatUnit, 0, 0));
				}
			}
		}
	}

	AutoTrack::generate();
}
void AutoKick::generate(){
	clearComponents();

	// generate kick hits
	float beatUnit = 1.f/timeSignatureBottom;
	//kick->notes.push_back(Note(0.25, 0, 1)); // first primary hit always on 1
	for(unsigned long int i = 0; i < timeSignatureTop; ++i){
		int numKick;
		switch(sweet::NumberUtils::randomInt(0,2)){
			case 0:	numKick = 1; break;
			case 1:	numKick = 2; break;
			case 2:	numKick = 4; break;
		}
		float l = beatUnit/numKick;
		components.push_back(new Note(l, 0, 0.5));
		if(l < beatUnit){
			components.push_back(new Note(beatUnit-l, 0, 1));
		}
	}

	AutoTrack::generate();
}
void AutoRide::generate(){
	clearComponents();

	// generate ride hits
	float beatUnit = 1.f/timeSignatureBottom;
	int numRide = 0;
	switch(sweet::NumberUtils::randomInt(0,2)){
		case 0:	numRide = 1; break; // one primary per beat
		case 1:	numRide = 2; break; // two primary per beat
		case 2:	numRide = 4; break; // four primary per beat
	}
	for(unsigned long int i = 0; i < timeSignatureTop; ++i){
		for(unsigned long int j = 0; j < numRide; ++j){
			bool addSecondary = !sweet::NumberUtils::randomInt(0, 2);
			if(addSecondary){
				components.push_back(new Note(beatUnit/numRide*0.5f, 0, 0.5)); // primary hit
				components.push_back(new Note(beatUnit/numRide*0.5f, 0, 0.25)); // secondary hit
			}else{
				components.push_back(new Note(beatUnit/numRide, 0, 0.5)); // primary hit
			}
		}
	}

	AutoTrack::generate();
}

AutoBeat::AutoBeat(OpenAL_Sound * _snare, OpenAL_Sound * _kick, OpenAL_Sound * _ride) :
	AutoTrack(120, 4, 4, 0, 1),
	snare(new AutoSnare(_snare)),
	kick(new AutoKick(_kick)),
	ride(new AutoRide(_ride))
{
	snare->generationMin = 0;
	snare->generationMax = 0;
	
	kick->generationMin = 0;
	kick->generationMax = 0;
	
	ride->generationMin = 0;
	ride->generationMax = 0;
};

AutoBeat::~AutoBeat(){
	delete snare;
	delete kick;
	delete ride;
}

void AutoBeat::generate(){
	snare->clearComponents();
	kick->clearComponents();
	ride->clearComponents();
	
	//alSource3f(snare->instrument->source->sourceId, AL_POSITION, sweet::NumberUtils::randomFloat(-1, 1), 0, 0);
	//alSource3f(kick->instrument->source->sourceId, AL_POSITION, sweet::NumberUtils::randomFloat(-1, 1), 0, 0);
	//alSource3f(ride->instrument->source->sourceId, AL_POSITION, sweet::NumberUtils::randomFloat(-1, 1), 0, 0);

	snare->bpm = kick->bpm = ride->bpm = bpm;
	snare->timeSignatureTop = kick->timeSignatureTop = ride->timeSignatureTop = timeSignatureTop;
	snare->timeSignatureBottom = kick->timeSignatureBottom = ride->timeSignatureBottom = timeSignatureBottom;
	//snare->bars = kick->bars = ride->bars = 1;
	
	snare->generate();
	kick->generate();
	ride->generate();
	
	AutoTrack::generate();

	// even-out lengths
	/*float sl = snare->lengthInBeats;
	float kl = kick->lengthInBeats;
	float rl = ride->lengthInBeats;

	if(abs(sl - lengthInBeats) > FLT_EPSILON){
		snare->components.back()->lengthInBeats += lengthInBeats-sl;
	}if(abs(kl - lengthInBeats) > FLT_EPSILON){
		kick->components.back()->lengthInBeats += lengthInBeats-kl;
	}if(abs(rl - lengthInBeats) > FLT_EPSILON){
		ride->components.back()->lengthInBeats += lengthInBeats-rl;
	}*/

	std::cout << "Snare:\t" << snare->toString() << std::endl;
	std::cout << "Kick:\t" << kick->toString() << std::endl;
	std::cout << "Ride:\t" << ride->toString() << std::endl;
}

void AutoBeat::update(Step * _step){
	snare->update(_step);
	kick->update(_step);
	ride->update(_step);
}

AutoDrums::AutoDrums(OpenAL_Sound * _snare, OpenAL_Sound * _kick, OpenAL_Sound * _ride) :
	AutoTrack(120, 4, 4, 0, 1),
	snare(_snare),
	kick(_kick),
	ride(_ride)
{
}

void AutoDrums::generate(){
	clearComponents();

	int numBeats = 1;

	for(unsigned long int i = 0; i < numBeats; ++i){
		AutoBeat * b = new AutoBeat(snare, kick, ride);
		b->bpm = bpm;
		b->timeSignatureTop = timeSignatureTop;
		b->timeSignatureBottom = timeSignatureBottom;

		std::cout << "Beat: " << bpm << "bpm, " << timeSignatureTop << "/" << timeSignatureBottom << std::endl;

		b->generate();
		components.push_back(b);
	}

	AutoTrack::generate();

	lengthInBeats *= numBeats;
	lengthInSeconds *= numBeats;
}

//void AutoDrums::update(Step * _step){
//	components.at(0)->update(_step);
	//if(components.size() > 0){
	//	components.at(curComponent)->update(_step);
	//
	//	curTime += _step->deltaTime;
	//
	//	if(curTime > nextComponent){
	//		curComponent += 1;
	//		// looping
	//		while(activeBeat >= beats.size()){
	//			activeBeat -= beats.size();
	//			curTime -= getLengthInSeconds();
	//			nextBeat -= getLengthInSeconds();
	//		}
	//		nextBeat += beats.at(activeBeat)->length * getSecondsPerBeat();
	//
	//		// play the note
	//		/*int notePitch = scales[mode*8 + notes.at(curNote).pitch];
	//		instrument->setPitch(pow(2, (pitch+notePitch)/13));
	//		instrument->setGain(notes.at(curNote).volume);
	//		instrument->play();*/
	//	}
	//}
	//
	//
	//beats.at(activeBeat)->update(_step);
//}