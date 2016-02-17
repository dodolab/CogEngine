#pragma once


#include "Behavior.h"
#include "Node.h"
#include "AttribAnim.h"

namespace Cog {


	/**x
	* Behavior for per-attribute animation
	*/
	class AttribsAnim : public Behavior {
		OBJECT_PROTOTYPE_INIT(AttribsAnim)
	private:
		vector<AttribAnim*> attribAnims;

		// animation root
		spt<AttrAnimNode> root = spt<AttrAnimNode>();
		// actual tree context
		AttrAnimContext context;
		// stack of processing tree
		stack<AttrAnimContext> nodeStack;
	public:

		AttribsAnim(Setting setting) {
			auto anims = setting.GetItemVals("animations");
			auto resCache = GETCOMPONENT(ResourceCache);
			
			for (string anim : anims) {
				auto entity = resCache->GetEntityC<AttrAnimEnt>(anim);
				if (!entity) throw IllegalArgumentException(string_format("Error while loading attribute animation: %s not found",anim.c_str()));
				AttribAnim* anim = new AttribAnim(entity);
				attribAnims.push_back(anim);
			}
		}

		~AttribsAnim() {
			for (AttribAnim* anim : attribAnims) {
				delete anim;
			}
		}

		void Init() {

			// the root is not in inverted scope(but it can be inverted itself)
			context = AttrAnimContext(root, false, true);

			for (AttribAnim* anim : attribAnims) {
				SetOwner(anim, owner);
				anim->Init();
			}
		}

		void Update(const uint64 delta, const uint64 absolute) {
			int animsEnded = 0;
			for (AttribAnim* anim : attribAnims) {
				if (!anim->Ended()) anim->Update(delta, absolute);
				else animsEnded++;
			}

			if (animsEnded == attribAnims.size()) {
				MoveToNext();

				if (!ended) {

				}
			}
		}

	private:


		/**
		* Tries to go to the nearest child
		* @return true, if there is a child to go to
		*/
		bool TryChildren(spt<AttrAnimNode> actualNode) {

			if (actualNode->children.size() != 0) {
				// node has children -> process them
				nodeStack.push(context);
				// XOR function -> two inverted parents give non-inverted animation for children
				bool invertedScope = context.IsChildReverted();
				context = AttrAnimContext(actualNode, invertedScope, false);
				return true;
			}
			else return false;
		}

		/**
		* Tries to start next loop
		* @return true if there is a loop to go to
		*/
		bool TryNextLoop(spt<AttrAnimNode> actualNode) {
			context.actualLoop++;
			if (context.actualLoop < actualNode->entity->repeat || actualNode->entity->isInfinite == 0) {
				return true;
			}
			else return false;
		}

		/**
		* Tries to go to the next sibling
		* @return true if there is a sibling to go to
		*/
		bool TrySibling() {

			if (!context.isScopeReverted && context.actualChildIndex < (int)(context.node->children.size() - 1)) {
				// go to the next element
				context.actualChildIndex++;
				context.actualLoop = 0;

				return true;
			}
			else if (context.isScopeReverted && context.actualChildIndex > 0) {
				// go to the previous element (animation is inverted)
				context.actualChildIndex--;
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

			spt<AttrAnimNode> actualNode;

			do {

				// get actual node
				actualNode = context.GetActualChild();

				// 1) loop through children of actual node
				// 2) loop through all loops of actual node
				// 3) go to the next node (sibling)
				if (!TryChildren(actualNode) && !TryNextLoop(actualNode) && !TrySibling()) {

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
			} while (!context.GetActualChild()->entity->hasValues);
		}
	};

}// namespace