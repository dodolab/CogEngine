#include "Environment.h"
#include "InputAct.h"
#include "CogEngine.h"
#include "Utils.h"
#include "Events.h"
#include "Settings.h"
#include "EntityStorage.h"
#include "ResourceCache.h"
#include "Stage.h"
#include "Scene.h"
#include "AsyncProcess.h"

namespace Cog {

	void Environment::OnInit() {
		virtualAspectRatio = ((float)ofGetWindowSize().x) / ofGetWindowSize().y;
		OnScreenSizeChanged((int)ofGetWindowSize().x, (int)ofGetWindowSize().y);
		screenSizeChanged = false;
	}

	void Environment::OnInit(spt<ofxXml> xml) {
		this->OnInit();

		auto resCache = GETCOMPONENT(ResourceCache);
		Setting set = resCache->GetGlobalSettings("display");

		if (!set.name.empty()) {
			string aspectRatio = set.GetItemVal("aspect_ratio");

			if (!aspectRatio.empty()) {
				// aspect ratio is defined in form xxx/yyy
				int dividerIndex = aspectRatio.find("/");

				if (dividerIndex == -1) {
					CogLogError("Environment","Error while parsing aspect ratio for environment; expected format xx/yy, found %s", aspectRatio.c_str());
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

		// set initial values only once
		if (initialWidth == 0 && initialHeight == 0) {
			initialWidth = newWidth;
			initialHeight = newHeight;
		}

		screenSizeChanged = true;
		screenWidth = virtualWidth = newWidth;
		screenHeight = virtualHeight = newHeight;
		screenOrient = virtualAspectRatio > 1 ? ScreenOrient::LANDSCAPE : ScreenOrient::PORTRAIT;

		aspectRatio = ((float)screenWidth) / screenHeight;
		RecalcVirtualSize();

		float realVirtual = ((virtualWidth)/((float)virtualHeight));

		COGLOGDEBUG("Environment", "Screen size set to %dx%d, virtual size %dx%d, aspect ratio %f ; virtual aspect ratio %f", 
			screenWidth, screenHeight, virtualWidth, virtualHeight, aspectRatio, realVirtual);

		// send message that screen has been changed
		if (CogEngine::GetInstance().stage->GetActualScene() != nullptr) {
			SendMessage(ACT_SCREEN_CHANGED, spt<ValueChangeEvent<Vec2i>>(new ValueChangeEvent<Vec2i>(originalSize, GetScreenSize())));
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
			SendMessage(ACT_KEY_PRESSED);
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
		
		FixTouchPosition(x, y);

		if (pressed) {
			// sometimes the first touch shows twice
			for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
				if ((*it)->touchId == button) {
					pressedPoints.erase(it);
					break;
				}
			}
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
		
		FixTouchPosition(x, y);

		for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
			if ((*it)->touchId == button && (*it)->inputType == InputType::TOUCH) {
				(*it)->position = Vec2i(x, y);
			}
		}
	}

	void Environment::OnSingleTouchButton(int x, int y, int button, bool pressed) {
		
		FixTouchPosition(x, y);

		if (pressed) {
			// sometimes the first button shows twice
			for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
				if ((*it)->touchId == button) {
					pressedPoints.erase(it);
					break;
				}
			}
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

	void Environment::OnSingleScroll(int x, int y, int scrollX, int scrollY) {
		this->scroll = Vec2i(scrollX, scrollY);
		SendMessage(ACT_MOUSE_SCROLLED);
	}

	void Environment::OnSingleTouchMotion(int x, int y, int button) {
		
		FixTouchPosition(x, y);

		for (auto it = pressedPoints.begin(); it != pressedPoints.end(); ++it) {
			if ((*it)->touchId == button && (*it)->inputType == InputType::TOUCH) {
				(*it)->position = Vec2i(x, y);
			}
		}
	}

	void Environment::RemoveEndedStuff() {
		
		screenSizeChanged = false;
		scroll = Vec2i(0);
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
			++it;
		}

		// remove released gestures
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
			++it;
		}

		// remove ended sounds
		for (auto it = playedSounds.begin(); it != playedSounds.end();) {
			if ((*it)->Ended()) {
				delete (*it);
				it = playedSounds.erase(it);

				continue;
			}
			++it;
		}

		// remove ended processes
		for (auto it = runningThreads.begin(); it != runningThreads.end();) {
			if (!(*it)->isThreadRunning()) {
				delete (*it);
				it = runningThreads.erase(it);
				continue;
			}
			++it;
		}
	}

	void Environment::RunProcess(AsyncProcess* thread) {
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
		// y axis is inverted
		y -= (screenHeight - (int)ofGetCurrentViewport().getHeight()) / 2;
	}

}// namespace