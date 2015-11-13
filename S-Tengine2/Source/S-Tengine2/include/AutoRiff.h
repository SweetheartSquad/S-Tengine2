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
	
	Note(int _generationMin, int _generationMax);
	Note(float _length, int _pitch, float _volume);

	std::string toString();
};

class AutoRiff : public virtual NodeUpdatable, public virtual NodeChild{
public:
	Mode mode;
	OpenAL_Sound * instrument;

	static int scales[];

	// semi-tones above or below the original sound
	float pitch;

	// number of notes above the original sample the riff is allowed to pitch shift to
	// e.g. a generationMax of 8 means that it can pitch up to one octave up
	int generationMax;
	// number of notes below the original sample the riff is allowed to pitch shift to
	// e.g. a generationMin of 8 means that it can pitch up to one octave down
	int generationMin;


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

	virtual void generate();
	float getLengthInSeconds() const;
	float getLengthInBeats() const;
	float getSecondsPerBeat() const;
};


class AutoSnare : public AutoRiff{
public:
	AutoSnare(OpenAL_Sound * _instrument);
	virtual void generate() override;
};

class AutoKick : public AutoRiff{
public:
	AutoKick(OpenAL_Sound * _instrument);
	virtual void generate() override;
};

class AutoRide : public AutoRiff{
public:
	AutoRide(OpenAL_Sound * _instrument);
	virtual void generate() override;
};

class AutoBeat : public virtual NodeUpdatable, public virtual NodeChild{
public:
	// plays the backbeat
	AutoRiff * snare;
	// plays the downbeats
	AutoRiff * kick;
	// plays at a constant interval
	AutoRiff * ride;

	AutoBeat(OpenAL_Sound * _snare, OpenAL_Sound * _kick, OpenAL_Sound * _ride);
	~AutoBeat();

	virtual void update(Step * _step) override;

	void generate();
	
	// note: changes only affects generation
	float bpm;
	// beats per bar
	int timeSignatureTop;
	// beat unit (note value which represents one beat)
	int timeSignatureBottom;
	float length;
};

class AutoDrums : public virtual NodeUpdatable, public virtual NodeChild{
public:
	// plays the backbeat
	OpenAL_Sound * snare;
	// plays the downbeats
	OpenAL_Sound * kick;
	// plays at a constant interval
	OpenAL_Sound * ride;

	std::vector<AutoBeat *> beats;
	int activeBeat;
	
	AutoDrums(OpenAL_Sound * _snare, OpenAL_Sound * _kick, OpenAL_Sound * _ride);
	~AutoDrums();

	virtual void update(Step * _step) override;

	void generate();
};

class AutoSong : public virtual NodeUpdatable, public virtual NodeChild{
	
};