#pragma once

#include "Definitions.h"
#include "GeneralAnim.h"



namespace Cog {

	/**
	* Container that holds the whole animation tree and iterates over animatable nodes up and down
	*/
	class AnimContextStack {
	private:

		/**
		* Context of actual scope of the animation tree
		*/
		class AnimContext {
		public:
			// index of actual loop, starting from 0
			int actualLoop;
			// actual frame/position in animation units (frames or ms) (float is used because of customizable speed)
			float actualProgress;
			// actual node, whose children are being iterated
			spt<GeneralAnim> node;
			// index of child being iterated
			int actualChildIndex;
			// indicator, if the actual node has inverted animation
			// two inverted scopes give uninverted scope, that's the reason of using this variable
			bool isScopeReverted;
			// indicator, if the actual node is a root node
			bool isRootNode;
			// speed of the scope (multiplied by the speed of each tree level)
			float scopeSpeed;

			AnimContext() {

			}

			/**
			* Creates a new animation context
			* @param node context node
			* @param isScopeReverted indicator, if the current animation scope is reverted
			* @param scopeSpeed speed of the animation scope
			* @param isRootNode indicator, if the node is the root node
			*/
			AnimContext(spt<GeneralAnim> node, bool isScopeReverted, float scopeSpeed, bool isRootNode);

			/**
			* Indicator whether this child has inverted animation
			*/
			bool IsChildReverted() const {
				return isScopeReverted ^ GetActualChild()->GetIsRevert();
			}

			/*
			* Gets actual child being iterated
			*/
			spt<GeneralAnim> GetActualChild() const {
				if (isRootNode) return node;
				else return node->GetChildren()[actualChildIndex];
			}

			/**
			* Refreshes actual progress
			*/
			void RefreshProgress();
		};



		// animation root node
		spt<GeneralAnim> root = spt<GeneralAnim>();
		// actual tree context
		AnimContext context;
		// stack of higher levels
		stack<AnimContext> nodeStack;
		// indicator, if there is no node to animate
		bool ended = false;

	public:
		AnimContextStack() {

		}

		AnimContextStack(spt<GeneralAnim> root) {
			SetRootNode(root);
		}

		/**
		* Gets true, if there is no node to animate
		*/
		bool Ended() const {
			return ended;
		}

		/**
		* Gets the root node
		*/
		spt<GeneralAnim> GetRootNode() {
			return root;
		}

		/**
		* Gets the actual tree context
		*/
		AnimContext& GetContext() {
			return context;
		}

		/**
		* Gets the progress of actual node
		*/
		float GetActualProgress() const {
			return context.actualProgress;
		}

		/**
		* Sets the root node
		*/
		void SetRootNode(spt<GeneralAnim> node);

		/**
		* Calculates speed of actual scope (it is constantly multiplied as it goes deeper)
		*/
		float GetScopeSpeed() const {
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
		bool TryNextFrame(spt<GeneralAnim> actualNode, const uint64 delta, int fps);

		/**
		* Tries to go to the nearest child
		* @return true, if there is a child to go to
		*/
		bool TryChildren(spt<GeneralAnim> actualNode);


		/**
		* Tries to start next loop
		* @return true if there is a loop to start
		*/
		bool TryNextLoop(spt<GeneralAnim> actualNode, const uint64 delta, int fps);

		/**
		* Tries to go to the next sibling
		* @return true if there is a sibling to go to
		*/
		bool TrySibling();

	};
} // namespace