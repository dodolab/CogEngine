#pragma once

#include "ofxCogCommon.h"
#include "CommonAnim.h"

namespace Cog {


	/**
	* Context entity used in animation behavior as
	* it goes through the animation tree
	*/
	class AnimContext {
	public:
		// index of actual loop
		int actualLoop;
		// must be floating point, because of variable speed
		float actualProgress;
		// actual node whose children are just processing
		spt<CommonAnim> node;
		// index of actual child being processed
		int actualChildIndex;
		// indicator, if scope of actual node is reverted
		// two reverted scopes give uninverted scope !!
		bool isScopeReverted;
		// indicator, if the node entity is a root node
		bool isRootNode;
		// speed of the scope
		float scopeSpeed;

		AnimContext() {

		}

		AnimContext(spt<CommonAnim> node, bool isScopeReverted, float scopeSpeed, bool isRootNode) {

			this->isRootNode = isRootNode;
			this->actualLoop = 0;
			this->node = node;
			this->isScopeReverted = isScopeReverted;
			this->scopeSpeed = scopeSpeed;

			if (isScopeReverted) {
				// start at end
				this->actualChildIndex = node->GetChildren().size() - 1;
			}
			else {
				// start at the beginning
				this->actualChildIndex = 0;
			}

			RefreshProgress();
		}

		/**
		* Refreshes actual frame index
		*/
		void RefreshProgress() {
			if (IsChildReverted()) {
				if (GetActualChild()->IsMeasurable()) {
					this->actualProgress = GetActualChild()->GetDuration();
				}
				else {
					this->actualProgress = GetActualChild()->GetDuration() - GetActualChild()->GetSpeed();
				}
			}
			else {
				this->actualProgress = 0;
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
		spt<CommonAnim> GetActualChild() {
			if (isRootNode) return node;
			else return node->GetChildren()[actualChildIndex];
		}
	};

	class AnimContextStack {
	private:
		// animation root
		spt<CommonAnim> root = spt<CommonAnim>();
		// actual tree context
		AnimContext context;
		// stack of processing tree
		stack<AnimContext> nodeStack;
		bool ended = false;

	public:
		AnimContextStack() {

		}

		AnimContextStack(spt<CommonAnim> root)  {
			SetRootNode(root);
		}

		bool Ended() {
			return ended;
		}

		spt<CommonAnim> GetRootNode() {
			return root;
		}

		AnimContext& GetContext() {
			return context;
		}

		void SetRootNode(spt<CommonAnim> node) {
			this->root = node;
			context = AnimContext(root, false, 1, true);

			if (root->GetIsRevert()) {
				context.actualProgress = root->IsMeasurable() ? root->GetDuration() : root->GetDuration() - GetScopeSpeed();
			}
			else {
				context.actualProgress = root->IsMeasurable() ? 0 : -GetScopeSpeed();
			}
		}

		float GetScopeSpeed() {
			return context.scopeSpeed*context.GetActualChild()->GetSpeed();
		}

		/**
		* Tries to go to the next frame
		* @return true, if there is a frame to go to
		*/
		bool TryNextFrame(spt<CommonAnim> actualNode, const uint64 delta) {

			uint64 timeMultiplier = context.GetActualChild()->IsMeasurable() ? delta : 1;

			// must by cast to int, because of rounding
			if (!context.IsChildReverted() && ((int)(context.actualProgress + GetScopeSpeed()*timeMultiplier)) < actualNode->GetDuration()) {
				context.actualProgress += GetScopeSpeed()*timeMultiplier;
				return true;
			}// no cast to int, because we need exactly value higher than 0
			else if (context.IsChildReverted() && (context.actualProgress - GetScopeSpeed()*timeMultiplier) >= 0) {
				context.actualProgress -= GetScopeSpeed()*timeMultiplier;
				return true;
			}
			else return false;
		}

		/**
		* Tries to go to the nearest child
		* @return true, if there is a child to go to
		*/
		bool TryChildren(spt<CommonAnim> actualNode) {


			if (actualNode->GetChildren().size() != 0) {
				// node has children -> process them
				nodeStack.push(context);
				// XOR function -> two inverted parents give non-inverted animation for children
				bool invertedScope = context.IsChildReverted();

				context = AnimContext(actualNode, invertedScope, actualNode->GetSpeed()*context.scopeSpeed, false);
				return true;
			}
			else return false;
		}


		/**
		* Tries to start next loop
		* @return true if there is a loop to go to
		*/
		bool TryNextLoop(spt<CommonAnim> actualNode, const uint64 delta) {
			uint64 timeMultiplier = context.GetActualChild()->IsMeasurable() ? delta : 1;

			context.actualLoop++;
			if (context.actualLoop < actualNode->GetRepeat() || actualNode->GetRepeat() == 0) {

				// repeat animation of this node
				if (!context.IsChildReverted()) {
					context.actualProgress = 0;
				}
				else {
					context.actualProgress = actualNode->GetDuration() - GetScopeSpeed()*timeMultiplier;
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

			if (context.isRootNode) return false;

			if (!context.isScopeReverted && context.actualChildIndex < (int)(context.node->GetChildren().size() - 1)) {
				// go to the next element
				context.actualChildIndex++;
				context.RefreshProgress();
				context.actualLoop = 0;

				return true;
			}
			else if (context.isScopeReverted && context.actualChildIndex > 0) {
				// go to the previous element (animation is inverted)
				context.actualChildIndex--;
				context.RefreshProgress();
				context.actualLoop = 0;
				return true;
			}
			else return false;
		}

		/**
		* Goes to the next animation frame or traverses through animation tree until
		* an available frame is found
		*/
		void MoveToNext(const uint64 delta) {

			spt<CommonAnim> actualNode;

			do {

				// get actual node
				actualNode = context.GetActualChild();

				// 1) loop through frames of actual node
				// 2) loop through children of actual node
				// 3) loop through all loops of actual node
				// 4) go to the next node (sibling)
				if (!TryNextFrame(actualNode, delta) && !TryChildren(actualNode) && !TryNextLoop(actualNode, delta) && !TrySibling()) {

					bool foundNode = false;

					// can't go further -> pop until we find next node
					while (!nodeStack.empty()) {
						// pop context
						context = nodeStack.top();
						nodeStack.pop();
						// get actual node of the popped context
						actualNode = context.GetActualChild();

						// try to start next loop or go to the next sibling
						if (TryNextLoop(actualNode, delta) || TrySibling()) {
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
			} while (!context.GetActualChild()->IsAnimatable());
		}
	};
} // namespace