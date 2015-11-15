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


class AutoMusic : public virtual NodeUpdatable, public virtual NodeChild{
public:
	static int scales[];

	float lengthInBeats;
	//float lengthInSeconds;
	// semi-tones above or below the original sound
	int pitch;
	float volume;

	AutoMusic(float _lengthInBeats, int _pitch, float _volume);

	virtual void update(Step * _step);

	virtual std::string toString();
};

class AutoTrack : public AutoMusic{
public:
	// beats per minute
	float bpm;
	float lengthInSeconds;
	
	// beats per bar
	unsigned long int timeSignatureTop;
	// beat unit (note value which represents one beat)
	unsigned long int timeSignatureBottom;



	float curTime;
	std::vector<AutoMusic *> components;
	int curComponent;
	float nextComponent;
	
	AutoTrack(float _bpm, unsigned long int _timeSignatureTop, unsigned long int _timeSignatureBottom, int _pitch, float _volume);
	~AutoTrack();

	virtual void update(Step * _step);

	// deletes all components and removes them from the component list
	void clearComponents();
	virtual void generate();
	virtual void playComponent(AutoMusic * _component);
	float getSecondsPerBeat() const;

	virtual std::string toString() override;
};

class Note : public AutoMusic{
public:
	Note(float _lengthInBeats, int _pitch, float _volume);

	virtual std::string toString() override;
};

class AutoRiff : public AutoTrack{
public:
	Mode mode;
	OpenAL_Sound * instrument;

	// number of notes above the original sample the riff is allowed to pitch shift to
	// e.g. a generationMax of 8 means that it can pitch up to one octave up
	int generationMax;
	// number of notes below the original sample the riff is allowed to pitch shift to
	// e.g. a generationMin of 8 means that it can pitch up to one octave down
	int generationMin;

	// number of bars in the riff
	//int bars;
	
	AutoRiff(OpenAL_Sound * _instrument, Mode _mode = kIONIAN);
	~AutoRiff();

	virtual void playComponent(AutoMusic * _component) override;
	virtual void generate() override;
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

class AutoBeat : public AutoTrack{
public:
	// plays the backbeat
	AutoSnare * snare;
	// plays the downbeats
	AutoKick * kick;
	// plays at a constant interval
	AutoRide * ride;

	AutoBeat(OpenAL_Sound * _snare, OpenAL_Sound * _kick, OpenAL_Sound * _ride);
	~AutoBeat();

	virtual void update(Step * _step) override;

	virtual void generate() override;
};

class AutoDrums : public AutoTrack{
public:
	// plays the backbeat
	OpenAL_Sound * snare;
	// plays the downbeats
	OpenAL_Sound * kick;
	// plays at a constant interval
	OpenAL_Sound * ride;
	
	AutoDrums(OpenAL_Sound * _snare, OpenAL_Sound * _kick, OpenAL_Sound * _ride);

	//virtual void update(Step * _step) override;

	virtual void generate() override;
};

/*class AutoSong : public virtual NodeUpdatable, public virtual NodeChild{
	
};*/