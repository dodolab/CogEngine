#pragma once

#include "Behavior.h"
#include "AnimationLoader.h"

namespace Cog {



	/**
	* Context entity used in animation behavior as
	* it goes through the animation tree
	*/
	class AnimSceneContext {
	public:
		// index of actual loop
		int actualLoop;
		// must be floating point, because of variable speed
		double actualFrameIndex;
		// actual node whose children are just processing
		spt<Anim> node;
		// index of actual child being processed
		int actualChildIndex;
		// indicator, if scope of actual node is reverted
		// two reverted scopes give uninverted scope !!
		bool isScopeReverted;
		// indicator, if the node entity is a root node
		bool isRootNode;

		AnimSceneContext() {

		}

		AnimSceneContext(spt<Anim> node, bool isScopeReverted, bool isRootNode) {

			this->isRootNode = isRootNode;
			this->actualLoop = 0;
			this->node = node;
			this->isScopeReverted = isScopeReverted;

			if (isScopeReverted) {
				// start at end
				this->actualChildIndex = node->GetChildren().size() - 1;
			}
			else {
				// start at the beginning
				this->actualChildIndex = 0;
			}

			RefreshFrameIndex();
		}

		/**
		* Refreshes actual frame index
		*/
		void RefreshFrameIndex() {
			if (IsChildReverted()) {
				this->actualFrameIndex = GetActualChild()->GetTotalFrames()
					- GetActualChild()->GetSpeed();
			}
			else {
				this->actualFrameIndex = 0;
			}
		}

		/**
		* Gets true, if this child is reverted; depends on actual scope
		* and settings of actual child
		*/
		bool IsChildReverted() {
			return isScopeReverted ^ GetActualChild()->GetIsRevert();
		}

		/*
		* Gets actual child being processed
		*/
		spt<Anim> GetActualChild() {
			if (isRootNode) return node;
			else return node->GetChildren()[actualChildIndex];
		}
	};


	/**x
	* Behavior for common animations
	*/
	class SheetAnim : public Behavior {
		OBJECT_PROTOTYPE_INIT(SheetAnim)
	private:
		// animation root
		spt<Anim> root = spt<Anim>();
		// actual tree context
		AnimSceneContext context;
		// stack of processing tree
		stack<AnimSceneContext> nodeStack;
	public:

		SheetAnim(spt<Anim> anim) : root(anim) {

		}

		SheetAnim(Setting& setting) {
			string animation = setting.GetItemVal("animation");
			auto resCache = GETCOMPONENT(ResourceCache);
			this->root = resCache->GetAnimation(animation);
		}

		void Init() {
			if (root == spt<Anim>()) {
				CogLogError("Anim", "Animation cant' run, entity is null");
				ended = true;
				return;
			}

			//int gridWidth = settings.GetSettingValInt("transform", "grid_width");
			//int gridHeight = settings.GetSettingValInt("transform", "grid_height");

			// the root is not in inverted scope (but it can be inverted itself)
			context = AnimSceneContext(root, false, true);
			// start with -SPEED so the first update will get the proper frame
			context.actualFrameIndex = root->GetIsRevert() ? (root->GetTotalFrames() - root->GetSpeed()) : -root->GetSpeed();
		}

		// debug only, will be deleted
		string actualNodeName;

		virtual void Update(const uint64 delta, const uint64 absolute) {

			MoveToNext();

			if (!Ended()) {
				int actualIndex = (int)context.actualFrameIndex;
				spt<Anim> actualNode = context.GetActualChild();

				if (actualNodeName != actualNode->GetName()) {
					actualNodeName = actualNode->GetName();
				}

				if (actualNode->GetFrames() > 1 || actualNode->GetLines() > 1) {
					// image is a spritesheet
					string imagePath = actualNode->GetSheet(0);
					spt<ofImage> spriteSheet = CogGet2DImage(imagePath);

					// calculate image offset
					int frameIndex = actualIndex + actualNode->GetStart();

					int frameRow = frameIndex / actualNode->GetFrames();
					int frameColumn = frameIndex % actualNode->GetFrames();
					int cellWidth = (int) (spriteSheet->getWidth() / actualNode->GetFrames());
					int cellHeight = (int) (spriteSheet->getHeight() / actualNode->GetLines());

					ofRectangle imageBound((float)(frameColumn*cellWidth), (float)(frameRow*cellHeight), (float)cellWidth, (float)cellHeight);
					owner->ChangeAttr(ATTR_IMGBOUNDS, imageBound);
					owner->GetShape<spt<Image>>()->SetImage(spriteSheet);

					if (owner->HasRenderType(RenderType::IMAGE)) {
						owner->GetShape<spt<Image>>()->SetImage(spriteSheet);
					}
					else {
						owner->SetShape(spt<Image>(new Image(spriteSheet)));
					}

				}
				else {
					// image is only a common image
					if (owner->HasAttr(ATTR_IMGBOUNDS)) owner->RemoveAttr(ATTR_IMGBOUNDS, true);

					string imagePath = actualNode->GetSheet(actualIndex);
					spt<ofImage> image = CogGet2DImage(imagePath);
					if (owner->HasRenderType(RenderType::IMAGE)) {
						owner->GetShape<spt<Image>>()->SetImage(image);
					}
					else {
						owner->SetShape(spt<Image>(new Image(image)));
					}


				}
			}
		}

	private:

		/**
		* Tries to go to the next frame
		* @return true, if there is a frame to go to
		*/
		bool TryNextFrame(spt<Anim> actualNode) {

			// must by cast to int, because of rounding
			if (!context.IsChildReverted() && ((int)(context.actualFrameIndex + actualNode->GetSpeed())) < actualNode->GetTotalFrames()) {
				context.actualFrameIndex += actualNode->GetSpeed();
				return true;
			}// no cast to int, because we need exactly value higher than 0
			else if (context.IsChildReverted() && (context.actualFrameIndex - actualNode->GetSpeed()) >= 0) {
				context.actualFrameIndex -= actualNode->GetSpeed();
				return true;
			}
			else return false;
		}

		/**
		* Tries to go to the nearest child
		* @return true, if there is a child to go to
		*/
		bool TryChildren(spt<Anim> actualNode) {

			if (actualNode->GetChildren().size() != 0) {
				// node has children -> process them
				nodeStack.push(context);
				// XOR function -> two inverted parents give non-inverted animation for children
				bool invertedScope = context.IsChildReverted();
				context = AnimSceneContext(actualNode, invertedScope, false);
				return true;
			}
			else return false;
		}

		/**
		* Tries to start next loop
		* @return true if there is a loop to go to
		*/
		bool TryNextLoop(spt<Anim> actualNode) {
			context.actualLoop++;
			if (context.actualLoop < actualNode->GetRepeat() || actualNode->GetRepeat() == 0) {
				// repeat animation of this node
				if (!context.IsChildReverted()) {
					context.actualFrameIndex = 0;
				}
				else {
					context.actualFrameIndex = actualNode->GetTotalFrames() - actualNode->GetSpeed();
				}
				return true;
			}
			else return false;
		}

		/**
		* Tries to go to the next sibling
		* @return true if there is a sibling to go to
		*/
		bool TrySibling() {

			if (!context.isScopeReverted && context.actualChildIndex < (int)(context.node->GetChildren().size() - 1)) {
				// go to the next element
				context.actualChildIndex++;
				context.RefreshFrameIndex();
				context.actualLoop = 0;

				return true;
			}
			else if (context.isScopeReverted && context.actualChildIndex > 0) {
				// go to the previous element (animation is inverted)
				context.actualChildIndex--;
				context.RefreshFrameIndex();
				context.actualLoop = 0;
				return true;
			}
			else return false;
		}

		/**
		* Goes to the next animation frame or traverses through animation tree until
		* an available frame is found
		*/
		void MoveToNext() {

			spt<Anim> actualNode;

			do {

				// get actual node
				actualNode = context.GetActualChild();

				// 1) loop through frames of actual node
				// 2) loop through children of actual node
				// 3) loop through all loops of actual node
				// 4) go to the next node (sibling)
				if (!TryNextFrame(actualNode) && !TryChildren(actualNode) && !TryNextLoop(actualNode) && !TrySibling()) {

					bool foundNode = false;

					// can't go further -> pop until we find next node
					while (!nodeStack.empty()) {
						// pop context
						context = nodeStack.top();
						nodeStack.pop();
						// get actual node of the popped context
						actualNode = context.GetActualChild();

						// try to start next loop or go to the next sibling
						if (TryNextLoop(actualNode) || TrySibling()) {
							foundNode = true;
							break;
						}
					}

					if (!foundNode && nodeStack.empty()) {
						ended = true;
						return;
					}
				}
				// do it until a node with animation is found
			} while (!context.GetActualChild()->HasSheets());
		}

	};

}// namespace