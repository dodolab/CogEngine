#include "CogEnvironment.h"
#include "CogInputAct.h"
#include "CogEngine.h"
#include "CogUtils.h"

namespace Cog {

	void CogEnvironment::Init() {
		screenSizeChanged = false;
		realWidth = width = ofGetWindowSize().x;
		realHeight = height = ofGetWindowSize().y;
		aspectRatio = ((float)realWidth) / realHeight;
		ReinitAspectRatio();
	}

	void CogEnvironment::OnScreenSizeChanged(int newWidth, int newHeight) {
		screenSizeChanged = true;
		realHeight = newHeight;
		realWidth = newWidth;
		ReinitAspectRatio();
	}


	void CogEnvironment::OnKeyAction(int key, bool pressed) {

		if (pressed) {
			// key down

			// if pressed keys contains this key, remove it
			for (auto pKey : pressedKeys) {
				// key is already in the collection of pressed keys
				if (pKey.key == key) return;
			}

			pressedKeys.push_back(CogInputAct(key));
		}
		else {
			// key up
			for (auto it = pressedKeys.begin(); it != pressedKeys.end(); ++it) {
				if ((*it).key == key) {
					(*it).ended = true;
					break;
				}
			}
		}
	}

	void CogEnvironment::OnMultiTouchButton(int x, int y, int button, bool pressed) {
		// user touches the screen with more fingers

		// todo: scale to format
		if (width != GetWidth()) x /= (float)(width / ((float)GetWidth()));

		if (pressed) {
			pressedPoints.push_back(CogInputAct(button, ofVec3f(x, y)));
		}
		else {
			for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
				if ((*it).inputType == CogInputType::TOUCH && (*it).touchId == button) {
					// change position
					(*it).position = ofVec3f(x, y);
					(*it).ended = true;
					return;
				}
			}
		}
	}

	void CogEnvironment::OnMultiTouchMotion(int x, int y, int button) {
		// user moves fingers
		// todo: scale to format
		if (width != GetWidth()) x /= (float)(width / ((float)GetWidth()));

		for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
			if ((*it).touchId == button && (*it).inputType == CogInputType::TOUCH) {
				(*it).position = ofVec3f(x, y);
			}
		}
	}

	void CogEnvironment::OnSingleTouchButton(int x, int y, int button, bool pressed) {
		// user touches the screen

		// todo: scale to format
		if (width != GetWidth()) x /= (float)(width / ((float)GetWidth()));

		if (pressed) {
			pressedPoints.push_back(CogInputAct(button, ofVec3f(x, y)));
		}
		else {
			for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
				if ((*it).inputType == CogInputType::TOUCH) {
					// change position
					(*it).position = ofVec3f(x, y);
					(*it).ended = true;
					return;
				}
			}
		}
	}

	void CogEnvironment::OnSingleTouchMotion(int x, int y, int button) {
		// todo: scale to format
		if (width != GetWidth()) x /= (float)(width / ((float)GetWidth()));

		// user moves finger
		for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
			if ((*it).touchId == button && (*it).inputType == CogInputType::TOUCH) {
				(*it).position = ofVec3f(x, y);
			}
		}
	}

	void CogEnvironment::RemoveEndedProcesses() {
		screenSizeChanged = false;

		// remove released keys
		for (auto it = pressedKeys.begin(); it != pressedKeys.end();) {
			if ((*it).ended) {
				it = pressedKeys.erase(it);
				continue;
			}
			else if ((*it).started) {
				(*it).started = false;
			}
			// increment only if item hasn't been erased
			++it;
		}

		// remove released touches
		for (auto it = pressedPoints.begin(); it != pressedPoints.end();) {
			if ((*it).ended) {
				it = pressedPoints.erase(it);
				continue;
			}
			else if ((*it).started) {
				(*it).started = false;
			}
			// increment only if item hasn't been erased
			++it;
		}

		// remove ended sounds
		for (auto it = playedSounds.begin(); it != playedSounds.end();) {
			if ((*it)->Ended()) {
				it = playedSounds.erase(it);

				continue;
			}
			// increment only if item hasn't been erased
			++it;
		}

		// remove ended threads
		for (auto it = runningThreads.begin(); it != runningThreads.end();) {
			if (!(*it)->isThreadRunning()) {
				delete (*it);
				it = runningThreads.erase(it);
				continue;
			}
			// increment only if item hasn't been erased
			++it;
		}

	}

	void CogEnvironment::RunThread(ofThread* thread) {
		runningThreads.push_back(thread);
		thread->startThread();
	}

}