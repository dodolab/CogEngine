#pragma once

#include "InputAct.h"
#include "Component.h"
#include "Vec2i.h"
#include "Soundfx.h"

namespace Cog {

	class AsyncProcess;

	/**
	* Screen orientation
	*/
	enum class ScreenOrient {
		PORTRAIT, LANDSCAPE
	};

	/**
	* Environment controller for device events (keys, gestures, sounds)
	*/
	class Environment : public  Component {

	private:

		// flag for screen size change
		bool screenSizeChanged;
		// collection of actually pressed keys
		vector<InputAct*> pressedKeys;
		// collection of actually pressed buttons
		vector<InputAct*> pressedPoints;
		// collection of played sounds
		vector<Soundfx*> playedSounds;
		// collection of running threads
		vector<AsyncProcess*> runningThreads;
		// screen orientation
		ScreenOrient screenOrient;
		// mouse scrolling
		Vec2i scroll = Vec2i(0);
		// scaled screen width
		int virtualWidth;
		// scaled screen height
		int virtualHeight;
		// real screen width
		int screenWidth;
		// real screen height
		int screenHeight;
		// virtual aspect ratio (customizable)
		float virtualAspectRatio;
		// real aspect ratio
		float aspectRatio;

		// initial width and height
		int initialWidth = 0;
		int initialHeight = 0;
		// indicator whether sounds should be muted
		bool muteSounds = false;
	public:

		~Environment() {
			for (auto snd : playedSounds) {
				delete snd;
			}
		}

		/**
		* Initializes environment controller
		*/
		void OnInit();


		/**
		* Adds a new sound
		*/
		void AddSound(Soundfx* sound) {
			playedSounds.push_back(sound);
		}

		/**
		* Plays sound and adds it to the collection
		*/
		void PlaySound(Soundfx* sound) {
			if (!muteSounds) {
				sound->Play();
				playedSounds.push_back(sound);
			}
		}

		/**
		* Returns true, if screen size has changed since the last update
		*/
		bool ScreenSizeChanged() {
			return screenSizeChanged;
		}

		/**
		* Gets collection of currently pressed keys
		*/
		vector<InputAct*>& GetPressedKeys() {
			return pressedKeys;
		}

		/**
		* Gets collection of currently pressed points
		*/
		vector<InputAct*>& GetPressedPoints() {
			return pressedPoints;
		}

		/**
		* Gets collection of currently played sounds
		*/
		vector<Soundfx*>& GetPlayedSounds() {
			return playedSounds;
		}

		/**
		* Stops all sounds
		*/
		void StopAllSounds() {
			for (auto snd : playedSounds) {
				snd->Stop();
			}
		}

		/**
		* Gets actual mouse scrolling deltas
		*/
		Vec2i GetMouseScroll() const {
			return this->scroll;
		}

		/**
		* Returns true, if selected key is pressed
		*/
		bool IsKeyPressed(int key) {
			for (InputAct* act : pressedKeys) {
				if (act->key == key) return true;
			}

			return false;
		}

		/**
		* Gets width of the screen
		*/
		int GetScreenWidth() const {
			return screenWidth;
		}

		/**
		* Gets virtual width of the screen, adjusted
		* by the custom ratio
		*/
		int GetVirtualWidth() const {
			return virtualWidth;
		}

		/**
		* Gets height of the screen
		*/
		int GetScreenHeight() const {
			return screenHeight;
		}

		/**
		* Gets virtual height of the screen, adjusted
		* by the custom ratio
		*/
		int GetVirtualHeight() const {
			return virtualHeight;
		}

		/**
		* Gets initial width of the screen, set immediately
		* when application starts
		*/
		int GetInitialWidth() const {
			return initialWidth;
		}

		/**
		* Gets indicator whether sounds should be muted
		*/
		bool MuteSounds() const {
			return muteSounds;
		}

		/**
		* Sets indicator whether sounds should be muted
		*/
		void SetMuteSounds(bool muteSounds) {
			this->muteSounds = muteSounds;
		}

		/**
		* Gets initial height of the screen, set immediately
		* when application starts
		*/
		int GetInitialHeight() const {
			return initialHeight;
		}

		/**
		* Gets customizable aspect ratio
		*/
		float GetVirtualAspectRatio() const {
			return virtualAspectRatio;
		}

		/**
		* Gets real aspect ratio of the screen
		*/
		float GetScreenAspectRatio() const {
			return aspectRatio;
		}

		/**
		* Gets screen orientation
		*/
		ScreenOrient GetScreenOrientation() const {
			return screenOrient;
		}

		/**
		* Gets screen size in vector
		*/
		Vec2i GetScreenSize() {
			return Vec2i(screenWidth, screenHeight);
		}

		/**
		* Gets virtual screen size, adjusted by the
		* custom ratio
		*/
		Vec2i GetVirtualScreenSize() {
			return Vec2i(virtualWidth, virtualHeight);
		}


		/**
		* Sets virtual aspect ratio so that the excessing
		* part of the screen will be black
		*/
		void SetVirtualAspectRatio(float ratio) {
			this->virtualAspectRatio = ratio;
			RecalcVirtualSize();
		}

		/**
		* Handler for key action
		* @param key ascii id of key
		* @param pressed indicates whether the key is pressed or released
		*/
		void OnKeyAction(int key, bool pressed);

		/**
		* Handler for screen size change
		* @param newWidth new width of the screen
		* @param newHeight new height of the screen
		*/
		void OnScreenSizeChanged(int newWidth, int newHeight);

		/**
		* Gets actual position of the mouse
		*/
		Vec2i GetMousePosition();

		/**
		* Handler for multitouch 
		* @param x coordinate on x axis
		* @param y coordinate on y axis
		* @param button id of pressed button
		* @param pressed indicates whether the button is pressed or released
		*/
		void OnMultiTouchButton(int x, int y, int button, bool pressed);

		/**
		* Handler for multitouch motion
		* @param x coordinate on x axis
		* @param y coordinate on y axis
		* @param button id of button
		*/
		void OnMultiTouchMotion(int x, int y, int button);

		/**
		* Handler for single touch 
		* @param x coordinate on x axis
		* @param y coordinate on y axis
		* @param button id of pressed button
		* @param pressed indicates whether the button is pressed or released
		*/
		void OnSingleTouchButton(int x, int y, int button, bool pressed);

		/**
		* Handler for single touch motion
		* @param x coordinate on x axis
		* @param y coordinate on y axis
		* @param button id of pressed button
		*/
		void OnSingleTouchMotion(int x, int y, int button);

		/**
		* Handler for mouse scroll
		* @param x coordinate on x axis
		* @param y coordinate on y axis
		* @param scrollX delta of scrolling on x axis
		* @param scrollY delta of scrolling on y axis
		*/
		void OnSingleScroll(int x, int y, int scrollX, int scrollY);

		/**
		* Removes processes (threads), sounds and inputs that have just finished
		*/
		void RemoveEndedStuff();

		/**
		* Runs a new process
		* @param process process to run
		*/
		void RunProcess(AsyncProcess* process);


		virtual void Update(const uint64 delta, const uint64 absolute) {
			RemoveEndedStuff();
		}

	protected:

		/**
		* Reinitializes virtual width and height
		* according to the virtual aspect ratio
		*/
		void RecalcVirtualSize();

		/**
		* Fixes touch position according to the view offset,
		* because in the case of a virtual ratio, position
		* of the mouse is displaced
		*/
		void FixTouchPosition(int& x, int& y);
	};

}// namespace