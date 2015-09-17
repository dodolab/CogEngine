#pragma once

#include "GBehavior.h"
#include "GNode.h"

class AnimNodeContext{
public:
	AnimNodeContext(){

	}

	AnimNodeContext(vector<spt<EnAnim>> nodes, bool isInRevertedScope){
		
		this->actualRepeat = 0;
		this->nodes = nodes;
		this->isInRevertedScope = isInRevertedScope;

		if (isInRevertedScope){
			this->actualNodeIndex = nodes.size() - 1;
		}
		else{
			this->actualNodeIndex = 0;
		}

		RefreshFrame();
	}

	void RefreshFrame(){
		if (IsReverted()){
			this->actualFrame = nodes[actualNodeIndex]->GetTotalFrames() - nodes[actualNodeIndex]->GetSpeed();
		}
		else{
			this->actualFrame = 0;
		}
	}

	bool IsReverted(){
		return isInRevertedScope ^ nodes[actualNodeIndex]->GetIsRevert();
	}
	
	int actualRepeat;
	// must be floating point, because of variable speed
	double actualFrame;
	vector<spt<EnAnim>> nodes;
	int actualNodeIndex;
	bool isInRevertedScope;
};


/**x
* Behavior for common animations
*/
class BeAnim : public GBehavior{
private:
	spt<EnAnim> root;

	AnimNodeContext actualContext;
	vector<string> actualContextImages;
	stack<AnimNodeContext> nodeStack;
	

public:

	BeAnim(spt<EnAnim> anim) : GBehavior(ElemType::MODEL), root(anim){

	}

	void Init(){
		cout << "ANIM: initialization" << endl;
		vector<spt<EnAnim>> nodes;
		nodes.push_back(root);

		// the very root is never inverted, because method IsReverted() can handle parentnes
		actualContext = AnimNodeContext(nodes, false);
		// start with -1 so the first update will get the proper frame
		actualContext.actualFrame = root->GetIsRevert() ? (root->GetTotalFrames()-root->GetSpeed()) : -root->GetSpeed();
		actualContextImages = root->GetSheetPaths();
	}

	// tries to go to the next frame
	bool TryNextFrame(spt<EnAnim> actualNode){
		//cout << "ANIM: Trying next frame" << endl;
		// must by cast to int, because of rounding
		if (!actualContext.IsReverted() && ((int)(actualContext.actualFrame+actualNode->GetSpeed())) < actualNode->GetTotalFrames()){
			actualContext.actualFrame+=actualNode->GetSpeed();
			//	cout << "ANIM: success" << endl;
			return true;
		}// no cast to int, because we need exactly value higher than 0
		else if (actualContext.IsReverted() && (actualContext.actualFrame-actualNode->GetSpeed()) >= 0){
			actualContext.actualFrame-=actualNode->GetSpeed();
		//	cout << "ANIM: success" << endl;
			return true;
		}
		else return false;
	}

	// tries to go to the first child
	bool TryChildren(spt<EnAnim> actualNode){
		vector<spt<EnAnim>> children = actualNode->GetChildren();
		//cout << "ANIM: Trying children" << endl;
		if (children.size() != 0){
			// node has children -> process them
			nodeStack.push(actualContext);
			// XOR function -> two inverted parents give non-inverted animation for children
			bool invertedScope = actualContext.IsReverted();
			cout << "GOING TO THE " << actualNode->GetName() << " CHILDREN; INVERTED: " << (invertedScope ? "true" : "false") << endl;

			actualContext = AnimNodeContext(children, invertedScope);
		//	cout << "ANIM: success" << endl;
			return true;
		}
		else return false;
	}

	// tries to repeat the inner animation
	bool TryNextLoop(spt<EnAnim> actualNode){
	//	cout << "ANIM: Trying next loop" << endl;
		actualContext.actualRepeat++;
		if (actualContext.actualRepeat < actualNode->GetRepeat() || actualNode->GetRepeat() == 0){
			// repeat animation of this node
			if (!actualContext.IsReverted()) actualContext.actualFrame = 0;
			else{
				actualContext.actualFrame = actualNode->GetTotalFrames() - actualNode->GetSpeed();
			}
			
			//cout << "ANIM: success" << endl;
			return true;
		}
		else return false;
	}

	// tries to go to the next sibling
	bool TrySibling(){
		//cout << "ANIM: Trying sibling" << endl;
		// repetition has ended (if there was some) -> try to get to the sibling
		if (!actualContext.isInRevertedScope && actualContext.actualNodeIndex < (actualContext.nodes.size() - 1)){
			// go to the next element
			actualContext.actualNodeIndex++;
			actualContext.RefreshFrame();
			actualContext.actualRepeat = 0;

			//cout << "ANIM: success" << endl;
			return true;
		}
		else if (actualContext.isInRevertedScope && actualContext.actualNodeIndex > 0){
			// go to the previous element
			actualContext.actualNodeIndex--;
			actualContext.RefreshFrame();
			actualContext.actualRepeat = 0;
		}
		else return false;
	}

	spt<EnAnim> GetActualNode(){
		return actualContext.nodes[actualContext.actualNodeIndex];
	}

	void MoveToNext(){
		spt<EnAnim> tempNode = GetActualNode();

		spt<EnAnim> actualNode;

		do{

			// get actual node
			actualNode = GetActualNode();

			// try to go further
			if (!TryNextFrame(actualNode) && !TryChildren(actualNode) && !TryNextLoop(actualNode) && !TrySibling()){

				// can't go further -> pop until we find next node
				while (true){
					if (!nodeStack.empty()){
						//cout << "ANIM: popping context" << endl;

						actualContext = nodeStack.top();
						nodeStack.pop();
						
						actualNode = GetActualNode();
						cout << "---POP , scope inverted: " << (actualContext.isInRevertedScope ? "true" : "false") << endl;

						if (TryNextLoop(actualNode) || TrySibling()){
							break;
						}
					}
					else{
						//cout << "ANIM: stack empty -> ending" << endl;
						// stack is empty -> animation has ended
						ended = true;
						return;
					}
				}
			}
			else{

			}

		} while (!GetActualNode()->HasSheets());

		if (GetActualNode() != tempNode){
			//cout << "ANIM: opening node " << GetActualNode()->GetName().c_str() << endl;
			// node has been changed -> refresh image array
			actualContextImages = GetActualNode()->GetSheetPaths();
		}
	}
	


	virtual void Update(const uint64 delta, const uint64 absolute){
	
		MoveToNext();

		if (!Ended()){
			int actualIndex = (int)actualContext.actualFrame;
			spt<EnAnim> actualNode = GetActualNode();

			if (actualNode->GetFrames() > 1){
				// image is a spritesheet
				spt<ofImage> spriteSheet = COGGet2DImage(actualContextImages[0]);
				int frameRow = actualIndex / actualNode->GetFrames();
				int frameColumn = actualIndex % actualNode->GetFrames();
				int cellWidth = spriteSheet->getWidth()/actualNode->GetFrames();
				int cellHeight = spriteSheet->getHeight()/actualNode->GetLines();

				ofRectangle imageBound(frameColumn*cellWidth, frameRow*cellHeight, cellWidth,cellHeight);
				owner->ChangeAttr(Attrs::IMGBOUNDS, imageBound);
				owner->ChangeAttr(Attrs::IMGSOURCE, spriteSheet);
			}
			else{
				spt<ofImage> image = COGGet2DImage(actualContextImages[actualIndex]);
				owner->ChangeAttr(Attrs::IMGSOURCE, image);
			}			
		}
	}
};
