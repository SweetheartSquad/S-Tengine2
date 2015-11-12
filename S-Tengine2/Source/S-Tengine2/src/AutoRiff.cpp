#pragma once

#include <AutoRiff.h>
#include <Step.h>
#include <NumberUtils.h>

int AutoRiff::scales[] = {
	0,	2,	4,	5,	7,	9,	11,	12,	13,	//kIONIAN
	0,	2,	3,	5,	7,	9,	10,	2,	13,	//kDORIAN
	0,	1,	3,	5,	7,	8,	10,	12,	13,	//kPHRYGIAN
	0,	2,	4,	6,	7,	9,	11,	12,	13,	//kLYDIAN
	0,	2,	4,	5,	7,	9,	11,	12,	13,	//kMIXOLYDIAN
	0,	2,	3,	5,	7,	8,	10,	12,	13,	//kAEOLIAN
	0,	1,	3,	5,	6,	8,	10,	12,	13	//kLOCRIAN
};

Note::Note(){
	// generate either a whole note, a half note, a quarter note, or an eigth note
	switch(sweet::NumberUtils::randomInt(0, 3)){
		case 0: length = 1; break;
		case 1: length = 0.5; break;
		case 2: length = 0.25; break;
		case 3: length = 0.125; break;
	}
	// generate a random number on the scale (will be used as an array accessor for AutoRiff::scales
	pitch = sweet::NumberUtils::randomInt(0, 8);
	// make some notes rests instead of actual notes
	volume = sweet::NumberUtils::randomInt(0, 1);

	//length = 0.25;
	//volume = 1;
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
	curNote(0)
{
	if(instrument != nullptr){
		++instrument->referenceCount;
	}
}

AutoRiff::~AutoRiff(){
	instrument->decrementAndDelete();
}


void AutoRiff::update(Step * _step){
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

void AutoRiff::generateNotes(){
	notes.clear();

	// fill the length of the riff
	float curLength = 0;
	float totalLength = getLengthInBeats();
	while(curLength < totalLength){
		notes.push_back(Note());
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