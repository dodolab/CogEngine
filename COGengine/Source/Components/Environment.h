#pragma once

#include "InputAct.h"
#include "Component.h"

namespace Cog {

	enum class ScreenOrient {
		PORTRAIT, LANDSCAPE
	};

	/**
	* Environment controller for device events (especially inputs)
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
		vector<spt<Sound>> playedSounds;
		// screen orientation
		ScreenOrient screenOrient;

		// scaled screen width
		int virtualWidth;
		// scaled screen height
		int virtualHeight;
		// real screen width
		int screenWidth;
		// real screen height
		int screenHeight;
		// virtual aspect ratio
		float virtualAspectRatio;
		// real aspect ratio
		float aspectRatio;

		// first width and height
		int originalWidth = 0;
		int originalHeight = 0;

		// collection of running threads
		vector<ofThread*> runningThreads;
	public:


		/**
		* Initializes environment controller
		*/
		void Init();

		/**
		* Initializes environment controller, using xml
		*/
		void Init(spt<ofxXml> xml);

		/**
		* Adds a new sound
		*/
		void AddSound(spt<Sound> sound) {
			playedSounds.push_back(sound);
		}

		/**
		* Plays sound and adds it to the collection
		*/
		void PlaySound(spt<Sound> sound) {
			sound->Play();
			playedSounds.push_back(sound);
		}

		/**
		* Returns true, if screen size has changed
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
		vector<spt<Sound>>& GetPlayedSounds() {
			return playedSounds;
		}

		bool IsKeyPressed(int key) {
			for (InputAct* act : pressedKeys) {
				if (act->key == key) return true;
			}

			return false;
		}

		int GetScreenWidth() {
			return screenWidth;
		}

		int GetVirtualWidth() {
			return virtualWidth;
		}

		int GetScreenHeight() {
			return screenHeight;
		}

		int GetVirtualHeight() {
			return virtualHeight;
		}

		int GetOriginalWidth() {
			return originalWidth;
		}

		int GetOriginalHeight() {
			return originalHeight;
		}

		float GetVirtualAspectRatio() {
			return virtualAspectRatio;
		}

		float GetScreenAspectRatio() {
			return aspectRatio;
		}

		ScreenOrient GetScreenOrientation() {
			return screenOrient;
		}

		/**
		* Gets width and height in 2D vector
		*/
		Vec2i GetScreenSize() {
			return Vec2i(screenWidth, screenHeight);
		}

		Vec2i GetVirtualScreenSize() {
			return Vec2i(virtualWidth, virtualHeight);
		}


		/**
		* Sets virtual aspect ratio
		*/
		void SetVirtualAspectRatio(float ratio) {
			this->virtualAspectRatio = ratio;
			RecalcVirtualSize();
		}

		/**
		* Handler for key action
		* @param key id of key
		* @param pressed indicates, if the key is pressed or released
		*/
		void OnKeyAction(int key, bool pressed);

		/**
		* Handler for screen size change
		* @param newWidth new screen width
		* @param newHeight new screen height
		*/
		void OnScreenSizeChanged(int newWidth, int newHeight);

		/**
		* Gets actual position of the mouse
		*/
		Vec2i GetMousePosition();

		/**
		* Handler for multitouch press
		* @param x coordinate in x axis
		* @param y coordinate in y axis
		* @param button id of pressed button
		* @param pressed indicates, if the button is pressed or released
		*/
		void OnMultiTouchButton(int x, int y, int button, bool pressed);

		/**
		* Handler for multitouch motion
		* @param x coordinate in x axis
		* @param y coordinate in y axis
		* @param button id of button
		*/
		void OnMultiTouchMotion(int x, int y, int button);

		/**
		* Handler for single touch press
		* @param x coordinate in x axis
		* @param y coordinate in y axis
		* @param button id of pressed button
		* @param pressed indicates, if the button is pressed or released
		*/
		void OnSingleTouchButton(int x, int y, int button, bool pressed);

		/**
		* Handler for single touch motion
		* @param x coordinate in x axis
		* @param y coordinate in y axis
		* @param button id of pressed button
		*/
		void OnSingleTouchMotion(int x, int y, int button);

		/**
		* Removes processes that already ended from collection
		*/
		void RemoveEndedProcesses();

		/**
		* Runs a new thread
		* @param thread thread to run
		*/
		void RunThread(ofThread* thread);

		virtual void Update(const uint64 delta, const uint64 absolute) {
			RemoveEndedProcesses();
		}

	protected:

		/**
		* Reinitializes virtual width and height
		* according to the actual aspect ratio
		*/
		void RecalcVirtualSize();

		void FixTouchPosition(int& x, int& y);
	};

}// namespace