#pragma once


#include <string>

/**
* Entity for sound attachment
*/
class EnSound{

public:
	string filename;
	ofSoundPlayer soundPlayer;
	bool started;

	EnSound(string filename, bool multiplay, bool loop) : filename(filename){
		started = false;
		// music must be streamed; otherwise it won't work on android
		soundPlayer.loadSound(filename,true);
		soundPlayer.setMultiPlay(multiplay);
		soundPlayer.setLoop(loop);
		soundPlayer.setVolume(1.0f);
	}

	bool Ended(){
		return started && !soundPlayer.getIsPlaying();
	}

	void Play(){
		soundPlayer.play();
		started = true;
	}

};



