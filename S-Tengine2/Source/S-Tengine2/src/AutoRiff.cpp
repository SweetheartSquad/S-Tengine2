#pragma once

#include <AutoRiff.h>
#include <Step.h>
#include <NumberUtils.h>
#include <sstream>

int AutoRiff::scales[] = {
	0,	2,	4,	5,	7,	9,	11,	12,	13,	//kIONIAN
	0,	2,	3,	5,	7,	9,	10,	2,	13,	//kDORIAN
	0,	1,	3,	5,	7,	8,	10,	12,	13,	//kPHRYGIAN
	0,	2,	4,	6,	7,	9,	11,	12,	13,	//kLYDIAN
	0,	2,	4,	5,	7,	9,	11,	12,	13,	//kMIXOLYDIAN
	0,	2,	3,	5,	7,	8,	10,	12,	13,	//kAEOLIAN
	0,	1,	3,	5,	6,	8,	10,	12,	13	//kLOCRIAN
};



Note::Note(int _generationMin, int _generationMax){
	// generate either a whole note, a half note, a quarter note, or an eigth note
	switch(sweet::NumberUtils::randomInt(0, 3)){
		case 0: length = 1; break;
		case 1: length = 0.5; break;
		case 2: length = 0.25; break;
		case 3: length = 0.125; break;
	}
	// generate a random number on the scale (will be used as an array accessor for AutoRiff::scales
	pitch = sweet::NumberUtils::randomInt(_generationMin, _generationMax);
	// make some notes rests instead of actual notes
	volume = sweet::NumberUtils::randomInt(0, 1);

	//length = 0.25;
	//volume = 1;
}

Note::Note(float _length, int _pitch, float _volume) :
	length(_length),
	pitch(_pitch),
	volume(_volume)
{
	if(length <= 0.001){
		throw;
	}
}

std::string Note::toString(){
	std::stringstream ss;
	ss << (volume == 1 ? "X" : (volume == 0.5 ? "x" : (volume == 0.25 ? "*" : "0")));
	for(unsigned long int i = 1; i < length*128; ++i){
		ss << "-";
	}
	return ss.str();
}

AutoRiff::AutoRiff(OpenAL_Sound * _instrument, Mode _mode) :
	instrument(_instrument),
	pitch(0),
	nextNote(0),
	mode(_mode),
	bpm(120),
	timeSignatureBottom(4),
	timeSignatureTop(4),
	bars(4),
	curTime(0),
	curNote(0),
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


void AutoRiff::update(Step * _step){
	if(notes.size() > 0){
		instrument->update(_step);

		curTime += _step->deltaTime;

		if(curTime > nextNote){
			curNote += 1;
			// looping
			while(curNote >= notes.size()){
				curNote -= notes.size();
				curTime -= getLengthInSeconds();
				nextNote -= getLengthInSeconds();
			}
			nextNote += notes.at(curNote).length * getSecondsPerBeat();

			// play the note
			int notePitch = scales[mode*8 + notes.at(curNote).pitch];
			instrument->setPitch(pow(2, (pitch+notePitch)/13));
			instrument->setGain(notes.at(curNote).volume);
			instrument->play();
		}
	}
}

void AutoRiff::generate(){
	notes.clear();

	// fill the length of the riff
	float curLength = 0;
	float totalLength = getLengthInBeats();
	while(curLength < totalLength){
		notes.push_back(Note(generationMin, generationMax));
		curLength += notes.back().length;
	}
	if(curLength > totalLength){
		notes.back().length = curLength - totalLength;
	}

	curNote = 0;
	curTime = 0;
	nextNote = notes.front().length * getSecondsPerBeat();
}

float AutoRiff::getLengthInBeats() const{
	return bars * (float)timeSignatureTop / timeSignatureBottom;
}

float AutoRiff::getLengthInSeconds() const{
	return getLengthInBeats() * getSecondsPerBeat();
}

float AutoRiff::getSecondsPerBeat() const{
	// beats per minute == beats per second / 60
	// beats per second == 1 / seconds per beat
	// seconds per beat == 1 / (beats per minute / 60)
	return 1 / (bpm / 60) * timeSignatureBottom;
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
				notes.push_back(Note(l, 0, 1)); // primary hit
				if(l < beatUnit){
					notes.push_back(Note(beatUnit-l, 0, 0.5)); // secondary hit
				}
			}else{
				notes.push_back(Note(beatUnit, 0, 1)); // primary hit for full beat
			}
		}else{
			if(addSecondary){
				notes.push_back(Note(beatUnit, 0, 0.5)); // secondary hit for full beat
			}else{
				 // full rest
				if(notes.size() > 0){
					notes.back().length += beatUnit;
				}else{
					notes.push_back(Note(beatUnit, 0, 0));
				}
			}
		}
	}
}
void AutoKick::generate(){
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
		notes.push_back(Note(l, 0, 0.5));
		if(l < beatUnit){
			notes.push_back(Note(beatUnit-l, 0, 1));
		}
	}
}
void AutoRide::generate(){
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
				notes.push_back(Note(beatUnit/numRide*0.5f, 0, 0.5)); // primary hit
				notes.push_back(Note(beatUnit/numRide*0.5f, 0, 0.25)); // secondary hit
			}else{
				notes.push_back(Note(beatUnit/numRide, 0, 0.5)); // primary hit
			}
		}
	}
}

AutoBeat::AutoBeat(OpenAL_Sound * _snare, OpenAL_Sound * _kick, OpenAL_Sound * _ride) :
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
	snare->notes.clear();
	kick->notes.clear();
	ride->notes.clear();
	
	//alSource3f(snare->instrument->source->sourceId, AL_POSITION, sweet::NumberUtils::randomFloat(-1, 1), 0, 0);
	//alSource3f(kick->instrument->source->sourceId, AL_POSITION, sweet::NumberUtils::randomFloat(-1, 1), 0, 0);
	//alSource3f(ride->instrument->source->sourceId, AL_POSITION, sweet::NumberUtils::randomFloat(-1, 1), 0, 0);

	snare->bpm = kick->bpm = ride->bpm = bpm;
	snare->timeSignatureTop = kick->timeSignatureTop = ride->timeSignatureTop = timeSignatureTop;
	snare->timeSignatureBottom = kick->timeSignatureBottom = ride->timeSignatureBottom = timeSignatureBottom;
	snare->bars = kick->bars = ride->bars = 1;
	
	snare->generate();
	kick->generate();
	ride->generate();


	// even-out lengths
	length = (float)timeSignatureTop/timeSignatureBottom;
	float sl = snare->getLengthInBeats();
	float kl = kick->getLengthInBeats();
	float rl = ride->getLengthInBeats();

	if(abs(sl - length) > FLT_EPSILON){
		snare->notes.back().length += length-sl;
	}if(abs(kl - length) > FLT_EPSILON){
		kick->notes.back().length += length-sl;
	}if(abs(rl - length) > FLT_EPSILON){
		ride->notes.back().length += length-sl;
	}

	std::cout << "Snare:\t";
	float c = 0;
	for(Note n : snare->notes){
		std::cout << n.toString();
		c += n.length;
	}
	std::cout << " " << c << std::endl;

	std::cout << "Kick:\t";
	c = 0;
	for(Note n : kick->notes){
		std::cout << n.toString();
		c += n.length;
	}
	std::cout << " " << c << std::endl;

	std::cout << "Ride:\t";
	c = 0;
	for(Note n : ride->notes){
		std::cout << n.toString();
		c += n.length;
	}
	std::cout << " " << c << std::endl;
}

void AutoBeat::update(Step * _step){
	snare->update(_step);
	kick->update(_step);
	ride->update(_step);
}

AutoDrums::AutoDrums(OpenAL_Sound * _snare, OpenAL_Sound * _kick, OpenAL_Sound * _ride) :
	snare(_snare),
	kick(_kick),
	ride(_ride),
	activeBeat(0)
{
};

AutoDrums::~AutoDrums(){
	while(beats.size() > 0){
		delete beats.back();
		beats.pop_back();
	}
}

void AutoDrums::generate(){
	while(beats.size() > 0){
		delete beats.back();
		beats.pop_back();
	}
	float bpm = sweet::NumberUtils::randomInt(60, 240);
	int timeSignatureBottom = sweet::NumberUtils::randomInt(2,8);
	int timeSignatureTop = sweet::NumberUtils::randomInt(1,timeSignatureBottom+1);
	for(unsigned long int i = 0; i < 1; ++i){
		beats.push_back(new AutoBeat(snare, kick, ride));
		beats.back()->bpm = bpm;
		beats.back()->timeSignatureTop = timeSignatureTop;
		beats.back()->timeSignatureBottom = timeSignatureBottom;

		std::cout << "Beat: " << bpm << "bpm, " << timeSignatureTop << "/" << timeSignatureBottom << std::endl;

		beats.back()->generate();
	}

	activeBeat = 0;
}

void AutoDrums::update(Step * _step){
	beats.at(activeBeat)->update(_step);
}