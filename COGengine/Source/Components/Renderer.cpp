#pragma once

#include "Renderer.h"
#include "Scene.h"

namespace Cog {

	void Renderer::BeginRender() {
		ofVec2f virtualSize = CogGetVirtualScreenSize();
		ofSetupScreenOrtho(virtualSize.x, virtualSize.y, -1000, 1000);
		ofBackground(0, 0, 0);
	}

	void Renderer::EndRender() {
		ofVec2f screenSize = CogGetScreenSize();
		ofVec2f virtualSize = CogGetVirtualScreenSize();

		ofLoadMatrix(ofMatrix4x4::newIdentityMatrix());
		ofSetColor(0);

		// draw black rectangles that will cover oversized viewport
		if (virtualSize.x != screenSize.x) {
			// draw left and right
			ofViewport(0, 0, (screenSize.x - virtualSize.x) / 2, virtualSize.y);
			ofDrawRectangle(0, 0, screenSize.x, screenSize.y);
			ofViewport(screenSize.x - (screenSize.x - virtualSize.x) / 2, 0, (screenSize.x - virtualSize.x) / 2, virtualSize.y);
			ofDrawRectangle(0, 0, screenSize.x, screenSize.y);

			// back to actual viewport
			ofViewport((screenSize.x - virtualSize.x) / 2, 0, virtualSize.x, virtualSize.y);
		}
		else if (virtualSize.y != screenSize.y) {
			// draw top and bottom
			ofViewport(0, 0,virtualSize.x,(screenSize.y - virtualSize.y) / 2);
			ofDrawRectangle(0, 0, screenSize.x, screenSize.y);
			ofViewport(0, screenSize.y-(screenSize.y-virtualSize.y)/2, virtualSize.x, (screenSize.y - virtualSize.y) / 2);
			ofDrawRectangle(0, 0, screenSize.x, screenSize.y);
			// back to actual viewport
			ofViewport((screenSize.x - virtualSize.x) / 2, 0, virtualSize.x, virtualSize.y);
		}
		
	}

	void Renderer::InitViewPort(Scene* sceneNode) {
		ofVec2f screenSize = CogGetScreenSize();
		ofVec2f virtualSize = CogGetVirtualScreenSize();

		// handle custom aspect ratio
		if (virtualSize.x != screenSize.x) {
			ofViewport(sceneNode->GetViewPortOffset().x + (screenSize.x - virtualSize.x) / 2, sceneNode->GetViewPortOffset().y, virtualSize.x, virtualSize.y);
		}
		else if (virtualSize.y != screenSize.y) {
			ofViewport(sceneNode->GetViewPortOffset().x, sceneNode->GetViewPortOffset().y + (screenSize.y - virtualSize.y) / 2, virtualSize.x, virtualSize.y);
		}
		else {
			// back to actual viewport
			ofViewport(sceneNode->GetViewPortOffset().x, sceneNode->GetViewPortOffset().y, virtualSize.x, virtualSize.y);
		}
	}

	void Renderer::Render() {
		
		for (auto it = rendererLayers.begin(); it != rendererLayers.end(); ++it) {
			renderer->clearCounters((*it));
		}

		for (auto it = zIndexes.begin(); it != zIndexes.end(); ++it) {

			vector<Node*>& arr = (*it).second;

			for (auto it2 = arr.begin(); it2 != arr.end(); ++it2) {
				Node* node = (*it2);

				switch (node->GetShape()->GetRenderType()) {
				case RenderType::IMAGE:
					RenderImage(node);
					break;
				case RenderType::POLYGON:
					RenderPolygon(node);
					break;
				case RenderType::RECTANGLE:
					RenderRectangle(node);
					break;
				case RenderType::TEXT:
					RenderText(node);
					break;
				case RenderType::SPRITE:
					RenderSprite(node);
					break;
				case RenderType::MULTISPRITE:
					RenderMultiSprite(node);
					break;
				}

			}
		}
		//ofLoadMatrix(ofMatrix4x4::newIdentityMatrix());
		//renderer->draw();
	}
}// namespace