#include "AnimContextStack.h"

namespace Cog {


	void AnimContextStack::SetRootNode(spt<CommonAnim> node) {
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

	void AnimContextStack::MoveToNext(const uint64 delta, int fps) {

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


	bool AnimContextStack::TryNextFrame(spt<CommonAnim> actualNode, const uint64 delta, int fps) {

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


	bool AnimContextStack::TryChildren(spt<CommonAnim> actualNode) {

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

	bool AnimContextStack::TryNextLoop(spt<CommonAnim> actualNode, const uint64 delta, int fps) {

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

	bool AnimContextStack::TrySibling() {

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

} // namespace