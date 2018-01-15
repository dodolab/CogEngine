#pragma once

#include "Definitions.h"
#include "ofSoundPlayer.h"

namespace Cog {

	/**
	* Sound or music file
	*/
	class Soundfx {
	private:
		// link to player
		ofSoundPlayer* soundPlayer;
		string filename;
		// indicator whether the sound started to play
		bool started;
		// volume with range 0-1
		// must be cached because app may crash on android (some kind of bug in OF)
		float volume;
		// indicator whether the sound plays in a loop
		// must be cached because soundPlayer doesn't provide getter
		bool loop;
	public:
		
		Soundfx(string filename) : filename(filename) {
			started = false;
			soundPlayer = new ofSoundPlayer();
			// music must be streamed; otherwise it won't work on android
			soundPlayer->load(filename, true);
			SetVolume(1.0f);
		}

		~Soundfx() {
			soundPlayer->stop();
		}

		/**
		* Gets indicator whether the sound has ended
		*/
		bool Ended() const {
			return started && !soundPlayer->isPlaying();
		}

		/**
		* Gets indicator whether the sound has started to play
		*/
		void Play() {
			soundPlayer->play();
			started = true;
		}

		/**
		* Gets indicator whether the sound has stopped
		*/
		void Stop() {
			soundPlayer->stop();
		}

		/**
		* Gets indicator whether the sound plays in a loop
		*/
		bool IsLoop() {
			return loop;
		}

		/**
		* Sets indicator whether the sound should play in a loop
		*/
		void SetLoop(bool loop) {
			this->loop = loop;
			soundPlayer->setLoop(loop);
		}

		/**
		* Sets volume in range <0,1>
		*/
		void SetVolume(float volume) {
			this->volume = volume;
			soundPlayer->setVolume(volume);
		}

		/**
		* Gets actual volume
		*/
		float GetVolume() const {
			return volume;
		}
	};

}// namespace


