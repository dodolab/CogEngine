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
		void SetRootNode(spt<CommonAnim> node);

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
		void MoveToNext(const uint64 delta, int fps);

		private:
			/**
			* Tries to go to the next frame/position
			* @return true, if there is a frame/position to go to
			*/
			bool TryNextFrame(spt<CommonAnim> actualNode, const uint64 delta, int fps);

			/**
			* Tries to go to the nearest child
			* @return true, if there is a child to go to
			*/
			bool TryChildren(spt<CommonAnim> actualNode);


			/**
			* Tries to start next loop
			* @return true if there is a loop to start
			*/
			bool TryNextLoop(spt<CommonAnim> actualNode, const uint64 delta, int fps);

			/**
			* Tries to go to the next sibling
			* @return true if there is a sibling to go to
			*/
			bool TrySibling();

	};
} // namespace