#pragma once

#include <OpenALSound.h>

enum Mode{
	kIONIAN,
	kDORIAN,
	kPHRYGIAN,
	kLYDIAN,
	kMIXOLYDIAN,
	kAEOLIAN,
	kLOCRIAN
};

struct Note{
	float length;
	int pitch;
	float volume;

	Note();
};

class AutoRiff : public virtual NodeUpdatable, public virtual NodeChild{
public:
	Mode mode;
	OpenAL_Sound * instrument;

	static int scales[];

	// semi-tones above or below the original sound
	float pitch;


	float nextNote;

	float curTime;
	int curNote;


	// beats per minute
	float bpm;

	// number of bars in the riff
	int bars;
	
	// beats per bar
	int timeSignatureTop;
	// beat unit (note value which represents one beat)
	int timeSignatureBottom;


	std::vector<Note> notes;
	
	AutoRiff(OpenAL_Sound * _instrument, Mode _mode = kIONIAN);
	~AutoRiff();

	void update(Step * _step);

	void generateNotes();
	float getLengthInSeconds() const;
	float getLengthInBeats() const;
	float getSecondsPerBeat() const;
};