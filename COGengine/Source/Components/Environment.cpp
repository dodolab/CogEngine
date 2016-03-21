#include "Environment.h"
#include "InputAct.h"
#include "CogEngine.h"
#include "Utils.h"
#include "Events.h"

namespace Cog {

	void Environment::Init() {
		virtualAspectRatio = ((float)ofGetWindowSize().x) / ofGetWindowSize().y;
		OnScreenSizeChanged((int)ofGetWindowSize().x, (int)ofGetWindowSize().y);
		screenSizeChanged = false;
	}

	void Environment::Init(spt<ofxXml> xml) {
		this->Init();

		auto resCache = GETCOMPONENT(ResourceCache);
		Setting set = resCache->GetGlobalSettings("display");

		if (!set.name.empty()) {
			string aspectRatio = set.GetItemVal("aspect_ratio");

			if (!aspectRatio.empty()) {
				int dividerIndex = aspectRatio.find("/");

				if (dividerIndex == -1) {
					throw IllegalArgumentException(string_format("Error while parsing aspect ratio for environment; expected format xx/yy, found %s", aspectRatio.c_str()));
				}

				float firstVal = ofToFloat(aspectRatio.substr(0, dividerIndex));
				float secondVal = ofToFloat(aspectRatio.substr(dividerIndex + 1));

				this->SetVirtualAspectRatio(firstVal / secondVal);
				CogLogInfo("Environment", "Aspect ratio set as %s",aspectRatio.c_str());
			}
		}
	}

	void Environment::OnScreenSizeChanged(int newWidth, int newHeight) {
		Vec2i originalSize = this->GetScreenSize();

		// set original values only once
		if (originalWidth == 0 && originalHeight == 0) {
			originalWidth = newWidth;
			originalHeight = newHeight;
		}

		screenSizeChanged = true;
		screenWidth = virtualWidth = newWidth;
		screenHeight = virtualHeight = newHeight;
		screenOrient = virtualAspectRatio > 1 ? ScreenOrient::LANDSCAPE : ScreenOrient::PORTRAIT;

		aspectRatio = ((float)screenWidth) / screenHeight;
		RecalcVirtualSize();

		float realVirtual = ((virtualWidth)/((float)virtualHeight));

		MLOGDEBUG("Environment", "Screen size set to %dx%d, virtual size %dx%d, aspect ratio %f ; virtual aspect ratio %f", 
			screenWidth, screenHeight, virtualWidth, virtualHeight, aspectRatio, realVirtual);

		if (CogEngine::GetInstance().stage->GetActualScene() != nullptr) {
			SendMessageToListeners(ACT_SCREEN_CHANGED, 0, new ValueChangeEvent<Vec2i>(originalSize, GetScreenSize()),nullptr);
		}
	}

	Vec2i Environment::GetMousePosition() {
		int x = ofGetMouseX();
		int y = ofGetMouseY();
		x -= (int)ofGetCurrentViewport().x;
		y -= (int)ofGetCurrentViewport().y;

		return Vec2i(x, y);
	}

	void Environment::OnKeyAction(int key, bool pressed) {

		if (pressed) {
			// key down

			// if pressed keys contains this key, remove it
			for (auto pKey : pressedKeys) {
				// key is already in the collection of pressed keys
				if (pKey->key == key) return;
			}

			pressedKeys.push_back(new InputAct(key));
			CogSendMessageToListeners(StringHash(ACT_KEY_PRESSED), 0, nullptr, nullptr, this->GetId());
		}
		else {
			// key up
			for (auto it = pressedKeys.begin(); it != pressedKeys.end(); ++it) {
				if ((*it)->key == key) {
					(*it)->ended = true;
					break;
				}
			}
		}
	}

	void Environment::OnMultiTouchButton(int x, int y, int button, bool pressed) {
		// user touches the screen with more fingers
		FixTouchPosition(x, y);

		if (pressed) {
			pressedPoints.push_back(new InputAct(button, Vec2i(x, y)));
		}
		else {
			for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
				if ((*it)->inputType == InputType::TOUCH && (*it)->touchId == button) {
					// change position
					(*it)->position = Vec2i(x, y);
					(*it)->ended = true;
					return;
				}
			}
		}
	}

	void Environment::OnMultiTouchMotion(int x, int y, int button) {
		// user moves fingers

		FixTouchPosition(x, y);

		for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
			if ((*it)->touchId == button && (*it)->inputType == InputType::TOUCH) {
				(*it)->position = Vec2i(x, y);
			}
		}
	}

	void Environment::OnSingleTouchButton(int x, int y, int button, bool pressed) {
		// user touches the screen

		FixTouchPosition(x, y);

		if (pressed) {
			pressedPoints.push_back(new InputAct(button, Vec2i(x, y)));
		}
		else {
			for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
				if ((*it)->inputType == InputType::TOUCH) {
					// change position
					(*it)->position = Vec2i(x, y);
					(*it)->ended = true;
					return;
				}
			}
		}
	}

	void Environment::OnSingleTouchMotion(int x, int y, int button) {

		FixTouchPosition(x, y);

		// user moves finger
		for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
			if ((*it)->touchId == button && (*it)->inputType == InputType::TOUCH) {
				(*it)->position = Vec2i(x, y);
			}
		}
	}

	void Environment::RemoveEndedProcesses() {
		screenSizeChanged = false;

		// remove released keys
		for (auto it = pressedKeys.begin(); it != pressedKeys.end();) {
			if ((*it)->ended) {
				InputAct* act = (*it);
				it = pressedKeys.erase(it);
				delete act;
				continue;
			}
			else if ((*it)->started) {
				(*it)->started = false;
			}
			// increment only if item hasn't been erased
			++it;
		}

		// remove released touches
		for (auto it = pressedPoints.begin(); it != pressedPoints.end();) {
			if ((*it)->ended) {
				InputAct* act = (*it);
				it = pressedPoints.erase(it);
				delete act;
				continue;
			}
			else if ((*it)->started) {
				(*it)->started = false;
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

	void Environment::RunThread(ofThread* thread) {
		runningThreads.push_back(thread);
		thread->startThread();
	}

	void Environment::RecalcVirtualSize() {

		if (abs(virtualAspectRatio - aspectRatio) > 0.01f) {

			// recalculate virtual width and height
			switch (screenOrient) {
			case ScreenOrient::LANDSCAPE:
				if (aspectRatio < virtualAspectRatio) {
					virtualWidth = screenWidth;
					virtualHeight = (int)(screenWidth / virtualAspectRatio);
				}
				else {
					virtualWidth = (int)(screenHeight*virtualAspectRatio);
					virtualHeight = screenHeight;
				}
				break;
			case ScreenOrient::PORTRAIT:

				if (aspectRatio < virtualAspectRatio) {
					virtualWidth = (int)(screenHeight / virtualAspectRatio);
					virtualHeight = screenHeight;
				}
				else {
					virtualWidth = screenWidth;
					virtualHeight = (int)(screenHeight*virtualAspectRatio);
				}

				break;
			}
		}
	}

	void Environment::FixTouchPosition(int& x, int& y) {
		x -= (int)ofGetCurrentViewport().x;
		y -= (screenHeight - (int)ofGetCurrentViewport().getHeight()) / 2;
	}

}// namespace