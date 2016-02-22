#pragma once

#include "ofxCogCommon.h"
#include "CommonAnim.h"

namespace Cog {


	/**
	* Class that holds context of
	* animation tree as it goes through
	*/
	class AnimContext {
	private:
		// index of actual loop, starting from 0
		int actualLoop;
		// actual frame/position in animation units (frames or ms) (float is used because of customizable speed)
		float actualProgress;
		// actual node, whose children are being iterated
		spt<CommonAnim> node;
		// index of child being iterated
		int actualChildIndex;
		// indicator, if the actual node has inverted animation
		// two inverted scopes give uninverted scope, that's the reason of using this variable
		bool isScopeReverted;
		// indicator, if the actual node is a root node
		bool isRootNode;
		// speed of the scope (multiplied by the speed of each tree level)
		float scopeSpeed;


		friend class AnimContextStack;
	public:

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
		* Gets true, if this child has inverted animation
		*/
		bool IsChildReverted() {
			return isScopeReverted ^ GetActualChild()->GetIsRevert();
		}

		/*
		* Gets actual child being iterated
		*/
		spt<CommonAnim> GetActualChild() {
			if (isRootNode) return node;
			else return node->GetChildren()[actualChildIndex];
		}

	private:

		/**
		* Refreshes actual progress
		*/
		void RefreshProgress() {
			if (IsChildReverted()) {
				if (GetActualChild()->IsContinous()) {
					// start from the end
					this->actualProgress = GetActualChild()->GetDuration();
				}
				else {
					// start from the beginning
					this->actualProgress = GetActualChild()->GetDuration() - GetActualChild()->GetSpeed();
				}
			}
			else {
				this->actualProgress = 0;
			}
		}
	};

	/**
	* Class that holds the whole animation tree and iterates over animatable nodes
	* May be considered a kind of a complex tree iterator
	*/
	class AnimContextStack {
	private:
		// animation root node
		spt<CommonAnim> root = spt<CommonAnim>();
		// actual tree context
		AnimContext context;
		// stack of higher levels
		stack<AnimContext> nodeStack;
		// indicator, if there is no node to animate
		bool ended = false;

	public:
		AnimContextStack() {

		}

		AnimContextStack(spt<CommonAnim> root)  {
			SetRootNode(root);
		}

		/**
		* Gets true, if there is no node to animate
		*/
		bool Ended() {
			return ended;
		}

		/**
		* Gets the root node
		*/
		spt<CommonAnim> GetRootNode() {
			return root;
		}

		/**
		* Gets the actual tree contxt
		*/
		AnimContext& GetContext() {
			return context;
		}

		/**
		* Gets the progress of actual node
		*/
		float GetActualProgress() {
			return context.actualProgress;
		}

		/**
		* Sets the root node
		*/
		void SetRootNode(spt<CommonAnim> node) {
			this->root = node;
			// set context
			context = AnimContext(root, false, 1, true);

			// set the first progress so that the first iteration fits into the correct interval
			if (root->GetIsRevert()) {
				context.actualProgress = root->IsContinous() ? root->GetDuration() : root->GetDuration() - GetScopeSpeed();
			}
			else {
				context.actualProgress = root->IsContinous() ? 0 : -GetScopeSpeed();
			}
		}

		/**
		* Calculates speed of actual node (it is constantly multiplied as it goes deeper)
		*/
		float GetScopeSpeed() {
			return context.scopeSpeed*context.GetActualChild()->GetSpeed();
		}

		/**
		* Goes to the next animation frame or scans the animation tree until an animatable
		* node is found (or there is no such node)
		*/
		void MoveToNext(const uint64 delta, int fps) {

			spt<CommonAnim> actualNode;

			do {

				// get actual node
				actualNode = context.GetActualChild();

				// 1) scan frames of actual node
				// 2) scan children of actual node
				// 3) check next loop of actual node
				// 4) scan siblings
				if (!TryNextFrame(actualNode, delta, fps) && !TryChildren(actualNode) && !TryNextLoop(actualNode, delta, fps) && !TrySibling()) {

					// nothing found -> pop context and go to the higher level
					bool foundNode = false;

					while (!nodeStack.empty()) {
						// pop context
						context = nodeStack.top();
						nodeStack.pop();
						actualNode = context.GetActualChild();

						// check next loop and scan siblings
						if (TryNextLoop(actualNode, delta, fps) || TrySibling()) {
							foundNode = true;
							break;
						}
					}

					if (!foundNode && nodeStack.empty()) {
						// there is no node to animate
						ended = true;
						return;
					}
				}
				// do it until an animatable node is found
			} while (!context.GetActualChild()->IsAnimatable());
		}

		private:
			/**
			* Tries to go to the next frame/position
			* @return true, if there is a frame/position to go to
			*/
			bool TryNextFrame(spt<CommonAnim> actualNode, const uint64 delta, int fps) {

				float timeMultiplier = context.GetActualChild()->IsContinous() ? delta : delta / ((float)(1000 / fps));

				// must by cast to int, because of rounding
				if (!context.IsChildReverted() && ((int)(context.actualProgress + GetScopeSpeed()*timeMultiplier)) < actualNode->GetDuration()) {
					// there is still something to animate
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
					// node has children -> push context and go deeper
					nodeStack.push(context);
					// XOR function -> two inverted parents give non-inverted animation for children
					bool invertedScope = context.IsChildReverted();
					// multiply speed by actual nodes'
					context = AnimContext(actualNode, invertedScope, actualNode->GetSpeed()*context.scopeSpeed, false);
					return true;
				}
				else return false;
			}


			/**
			* Tries to start next loop
			* @return true if there is a loop to start
			*/
			bool TryNextLoop(spt<CommonAnim> actualNode, const uint64 delta, int fps) {

				float timeMultiplier = context.GetActualChild()->IsContinous() ? delta : delta / ((float)(1000 / fps));

				context.actualLoop++;
				if (context.actualLoop < actualNode->GetRepeat() || actualNode->GetRepeat() == 0) {

					// repeat animation
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

				// root node has no siblings
				if (context.isRootNode) return false;

				if (!context.isScopeReverted && context.actualChildIndex < (int)(context.node->GetChildren().size() - 1)) {
					// go to the next element
					context.actualChildIndex++;
					context.RefreshProgress();
					context.actualLoop = 0;

					return true;
				}
				else if (context.isScopeReverted && context.actualChildIndex > 0) {
					// for inverted animation: go to the previous element
					context.actualChildIndex--;
					context.RefreshProgress();
					context.actualLoop = 0;
					return true;
				}
				else return false;
			}

	};
} // namespace