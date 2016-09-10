#pragma once

#include "Behavior.h"

namespace Cog {

	/**
	* XML animation loader
	*/
	class AnimationLoader {
	public:

		/**
		* Loads animation from xml
		*/
		void LoadAnimations(spt<ofxXmlSettings> xml, vector<spt<Anim>>& rootAnims) {

			if (xml->tagExists("animations")) {
				xml->pushTag("animations");

				int numberOfAnims = xml->getNumTags("anim");
				// referenced anims will be processed at second phase
				map<string, Anim*> referencedAnims;

				// phase 1: load not-referenced animations
				for (int i = 0; i < numberOfAnims; i++) {

					xml->pushTag("anim", i);
					// load
					spt<Anim> anim = spt<Anim>(CreateAnimationFromXml(xml, referencedAnims));
					if (anim->GetName().length() == 0) throw ConfigErrorException("All root animations must have a name!");

					rootAnims.push_back(anim);
					xml->popTag();
				}

				// phase 2: load referenced animations
				for (int i = 0; i < numberOfAnims; i++) {

					xml->pushTag("anim", i);
					ProcessRefAnimationFromXml(xml, referencedAnims, i, rootAnims);
					xml->popTag();
				}

				xml->popTag();
			}
		}

	private:

		/**
		* Creates animation from XML
		* @param xml xml to load from
		* @param referencedAnims array that will be filled with animations that couldn't be load yet becase
		* they reference to another animation
		*/
		Anim* CreateAnimationFromXml(spt<ofxXmlSettings> xml, map<string, Anim*>& referencedAnims) {

			int innerAnimations = xml->getNumTags("anim");

			Anim* anim = new Anim();

			// fill ref and name attribute
			FillBaseAttributes(xml, anim);

			if (anim->GetRef().length() != 0) {
				// animation is referenced -> push it to the referencedAnims and return
				referencedAnims[anim->GetName()] = anim;
				if (innerAnimations != 0) throw ConfigErrorException("Referenced animations mustn't have inner animations!");

				return anim;
			}
			else {
				// children will be taken from referenced animation
				for (int i = 0; i < innerAnimations; i++) {
					xml->pushTag("anim", i);
					spt<Anim> newAnim = spt<Anim>(CreateAnimationFromXml(xml, referencedAnims));
					anim->GetChildren().push_back(newAnim);
					xml->popTag();
				}
			}

			// set other attributes
			FillOtherAttributes(xml, anim);

			return anim;
		}

		/**
		* Fills base attributes (ref and name)
		*/
		void FillBaseAttributes(spt<ofxXmlSettings> xml, Anim* anim) {
			anim->SetRef(xml->getAttribute(":", "ref", anim->GetRef()));
			anim->SetName(xml->getAttribute(":", "name", anim->GetName()));
		}

		/**
		* Fills other attributes
		*/
		void FillOtherAttributes(spt<ofxXmlSettings> xml, Anim* anim) {
			anim->SetSheetPath(xml->getAttribute(":", "sheet", anim->GetSheetPath()));
			anim->SetFrames(xml->getAttribute(":", "frames", anim->GetFrames()));
			if (anim->GetFrames() < 0) throw IllegalArgumentException(string_format("Error in animation %s; frames bust be greater or equal to 0", anim->GetName().c_str()));
			anim->SetLines(xml->getAttribute(":", "lines", anim->GetLines()));
			if (anim->GetLines() < 0) throw IllegalArgumentException(string_format("Error in animation %s; lines bust be greater or equal to 0", anim->GetName().c_str()));
			anim->SetStart(xml->getAttribute(":", "start", anim->GetStart()));
			if (anim->GetStart() < 0) throw IllegalArgumentException(string_format("Error in animation %s; start bust be greater or equal to 0", anim->GetName().c_str()));
			anim->SetEnd(xml->getAttribute(":", "end", anim->GetEnd()));
			if (anim->GetEnd() < 0) throw IllegalArgumentException(string_format("Error in animation %s; end bust be greater or equal to 0", anim->GetName().c_str()));
			if (anim->GetStart() > anim->GetEnd()) throw IllegalArgumentException(string_format("Error in animation %s; start frame must be lower or equal to end frame", anim->GetName().c_str()));
			anim->SetIncrement(xml->getAttribute(":", "increment", anim->GetIncrement()));
			if (anim->GetIncrement() <= 0) throw IllegalArgumentException(string_format("Error in animation %s; increment must be greater than 0", anim->GetName().c_str()));
			anim->SetSpeed(xml->getAttribute(":", "speed", anim->GetSpeed()));
			if (anim->GetSpeed() < 0) throw IllegalArgumentException(string_format("Error in animation %s; speed bust be greater than 0", anim->GetName().c_str()));
			anim->SetRepeat(xml->getAttribute(":", "repeat", anim->GetRepeat()));
			if (anim->GetRepeat() < 0) throw IllegalArgumentException(string_format("Error in animation %s; number of repetitions must be greater or equal to 0", anim->GetName().c_str()));
			anim->SetIsRevert(xml->getBoolAttribute(":", "revert", anim->GetIsRevert()));

		}

		/**
		* Finds animation by name, in selected collection
		*/
		spt<Anim> FindAnimByName(string name, vector<spt<Anim>> anims) {
			for (auto it = anims.begin(); it != anims.end(); ++it) {
				spt<Anim> anim = (*it);
				if (anim->GetName() == name) return anim;
			}

			return spt<Anim>();
		}

		/**
		* Processes referenced animations from XML
		* @xml xml to load from
		* @referencedAnims list of all referenced animations
		* @rootAnimIndex index of root for actual XML scope
		* @rootAnims list of all root animations
		*/
		void ProcessRefAnimationFromXml(spt<ofxXmlSettings> xml, map<string, Anim*>& referencedAnims,
			int rootAnimIndex, vector<spt<Anim>>& rootAnims) {

			int innerAnimations = xml->getNumTags("anim");

			string ref = (xml->getAttribute(":", "ref", ""));
			string name = (xml->getAttribute(":", "name", ""));

			if (ref.length() != 0) {
				// got referenced animation

				Anim* refAnim = referencedAnims[name];

				if (ref.find(".") == -1) {
					// reference doesn't contain dot - it means that we can find its reference in THIS scope
					spt<Anim> reference = rootAnims[rootAnimIndex]->FindChild(ref);

					if (reference != spt<Anim>()) {
						// found reference
						refAnim->GetParametersFromReference(reference);
					}
					else {
						// reference anim doesn't contain dot, but it isn't in this scope... that means it must refer to some other root animation
						spt<Anim> rootReference = FindAnimByName(ref, rootAnims);
						if (rootReference == spt<Anim>()) throw ConfigErrorException(string_format("Referenced animation %s not found", ref.c_str()));
						refAnim->GetParametersFromReference(rootReference);
					}
				}
				else {
					// reference contains dot -> referenced animation should be in another scope
					string rootAnimName = ref.substr(0, ref.find("."));
					string subAnim = ref.substr(ref.find(".") + 1);
					spt<Anim> root = FindAnimByName(rootAnimName, rootAnims);
					spt<Anim> scopeAnim = root->FindChild(subAnim);
					if (root == spt<Anim>() || scopeAnim == spt<Anim>()) throw ConfigErrorException(string_format("Referenced animation %s not found", ref.c_str()));

					refAnim->GetParametersFromReference(scopeAnim);
				}

				// fill remaining attributes from XML
				FillOtherAttributes(xml, refAnim);

			}
			else {
				// this animation doesn't have ref attribute -> keep searching over its children

				for (int i = 0; i < innerAnimations; i++) {
					// use recursion
					xml->pushTag("anim", i);
					ProcessRefAnimationFromXml(xml, referencedAnims, rootAnimIndex, rootAnims);
					xml->popTag();
				}
			}
		}

	};



	/**
	* Context entity used in animation behavior as
	* it goes through the animation tree
	*/
	class AnimNodeContext {
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

		AnimNodeContext() {

		}

		AnimNodeContext(spt<Anim> node, bool isScopeReverted, bool isRootNode) {

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
	class Animator : public Behavior {
		OBJECT_PROTOTYPE(Animator)
	private:
		// animation root
		spt<Anim> root = spt<Anim>();
		// actual tree context
		AnimNodeContext context;
		// stack of processing tree
		stack<AnimNodeContext> nodeStack;
	public:

		Animator(spt<Anim> anim) : root(anim) {

		}

		void Init() {
			if (root == spt<Anim>()) {
				CogLogError("Anim", "Animation cant' run, entity is null");
				ended = true;
				return;
			}

			// the root is not in inverted scope (but it can be inverted itself)
			context = AnimNodeContext(root, false, true);
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
					cout << "OPENING " << actualNodeName << endl;
				}

				if (actualNode->GetFrames() > 1 || actualNode->GetLines() > 1) {
					// image is a spritesheet
					string imagePath = actualNode->GetSheet(0);
					spt<ofImage> spriteSheet = CogGet2DImage(imagePath);

					// calculate image offset
					int frameIndex = actualIndex + actualNode->GetStart() - 1;

					int frameRow = frameIndex / actualNode->GetFrames();
					int frameColumn = frameIndex % actualNode->GetFrames();
					int cellWidth = spriteSheet->getWidth() / actualNode->GetFrames();
					int cellHeight = spriteSheet->getHeight() / actualNode->GetLines();

					ofRectangle imageBound(frameColumn*cellWidth, frameRow*cellHeight, cellWidth, cellHeight);
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
				context = AnimNodeContext(actualNode, invertedScope, false);
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

			if (!context.isScopeReverted && context.actualChildIndex < (context.node->GetChildren().size() - 1)) {
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