#pragma once

#include "ofxCogMain.h"

/**
* Entity for animations
*/
class Anim{
protected:
	// name of animation
	string name;
	// path to sheet(s)
	string sheetPath;
	// reference to another animation
	string ref;
	// number of frames per one line in spritesheet
	int frames;
	// number of lines in spritesheet
	int lines;
	// start frame
	int start;
	// end frame
	int end;
	// sequence increment
	int increment;
	// speed multiplier
	double speed;
	// number of repetition
	int repeat;
	// if true, animation will be reverted
	bool isRevert;
	// list of children
	vector<spt<Anim>> children;

	// list of paths to sheets this animation holds
	vector<string> sheets;
	// indicator, if sheet array has been initialized
	bool sheetInit;


public:
		

	Anim(string name, string sheetPath, string ref, int frames, int lines, int start,
		int end, int increment, double speed, int repeat, bool isRevert) : name(name), sheetPath(sheetPath), ref(ref),
		frames(frames), lines(lines), start(start), end(end), increment(increment), speed(speed), repeat(repeat), isRevert(isRevert){
	
		sheetInit = false;
		//	this->rootAnim = spt<Anim>();
	}

	Anim() : name(""), sheetPath(""), ref(""), frames(1), lines(1), start(1), end(1), increment(1), speed(1), repeat(1), isRevert(false){
		//this->rootAnim = spt<Anim>();
		sheetInit = false;
	}

	~Anim(){
		// todo: some trash here...
		/*for (auto it = children.begin(); it != children.end(); ++it){
			delete (*it);
		}*/
	}

	/**
	* Copies all parameters from other animation
	*/
	void GetParametersFromReference(Anim* reference){
		this->SetSheetPath(reference->GetSheetPath());
		this->SetFrames(reference->GetFrames());
		this->SetLines(reference->GetLines());
		this->SetStart(reference->GetStart());
		this->SetEnd(reference->GetEnd());
		this->SetIncrement(reference->GetIncrement());
		this->SetSpeed(reference->GetSpeed());
		this->SetRepeat(reference->GetRepeat());
		this->SetIsRevert(reference->GetIsRevert());

		// insert children
		vector<spt<Anim>>& refChildren = reference->children;

		for (auto it = refChildren.begin(); it != refChildren.end(); ++it){
			spt<Anim> child = (*it);
			AddChild(child);
		}
	}

	/**
	* Adds itself and all children to the output array
	*/
	void GetAllNodes(vector<Anim*> &output){
		output.push_back(this);

		for (auto it = children.begin(); it != children.end(); ++it){
			spt<Anim> an = (*it);
			an->GetAllNodes(output);
		}
	}

	/**
	* Finds recursively child by name
	* @param name according  to find the child
	*/
	spt<Anim> FindChild(string name){
		if (this->GetName().compare(name) == 0) return spt<Anim>(this);

		for (auto it = children.begin(); it != children.end(); ++it){
			if ((*it)->GetName().compare(name) == 0) return (*it);
			else{
				auto childFound = (*it)->FindChild(name);
				if (childFound != spt<Anim>()) return childFound;
			}
		}

		return spt<Anim>();
	}

	/**
	* Initializes list of paths to all sheets this animation holds
	*/
	void InitSheets(){

		sheets.clear();

		if (!HasSheets()){
			sheetInit = true;
			return;
		}

		int firstBracket = sheetPath.find("{");
		int secondBracket = sheetPath.find("}");


		if (firstBracket != string::npos && secondBracket != string::npos){
			string sequencePrefix = sheetPath.substr(0, firstBracket);
			string sequenceSuffix = sheetPath.substr(secondBracket + 1, sheetPath.length() - (secondBracket + 1));

			// string is in form {XXX}, it means that this is a file sequence
			int numberOfDigits = secondBracket - firstBracket - 1;

			for (int i = start; i <= end; i += increment){
				// todo: danger increment value !!
				string file = sequencePrefix + ofToString(i, 1, numberOfDigits, '0') + sequenceSuffix;
				sheets.push_back(file);
			}
		}
		else{
			sheets.push_back(sheetPath);
		}

		sheetInit = true;
	}

	/**
	* Get sheets at selected index
	*/
	string GetSheet(int frameIndex){
		if (!sheetInit) InitSheets();

		return sheets[frameIndex];
	}


	/**
	* Adds a new animation child
	* @param child child to add
	* @return true, if child has been added (it didn't exist in the collection)
	*/
	bool AddChild(spt<Anim> child){
		auto found = find(children.begin(), children.end(), child);
		if (found != children.end()){
			return false;
		}
		else{
			children.push_back(child);
			return true;
		}
	}

	/**
	* Removes animation child
	* @param child child to remove
	* @return true, if child has been removed (it did exist in the collection)
	*/
	bool RemoveChild(spt<Anim> child){
		auto found = find(children.begin(), children.end(), child);
		if (found != children.end()){
			children.erase(found);
			return true;
		}
		else return false;
	}

	/**
	* Returns true, if this animation has sheet(s)
	*/
	bool HasSheets(){
		return sheetPath.length() != 0;
	}


	/**
	* Gets name of this animation
	*/
	const string GetName() const{
		return name;
	}

	/**
	* Sets name of this animation
	*/
	void SetName(string name){
		this->name = name;
	}

	/**
	* Gets path to the animation sheet
	*/
	const string GetSheetPath() const{
		return sheetPath;
	}

	/**
	* Sets path to the animation sheet
	*/
	void SetSheetPath(string sheetPath){
		this->sheetPath = sheetPath;
	}

	/**
	* Gets name of referenced animation
	*/
	const string GetRef() const{
		return ref;
	}

	/**
	* Sets name of referenced animation
	*/
	void SetRef(string ref){
		this->ref = ref;
	}

	/**
	* Gets number of frames per one row in spritesheet
	*/
	const int GetFrames() const{
		return frames;
	}

	/**
	* Gets total number of frames
	*/
	const int GetTotalFrames() const {
		return (end - start+1+(increment-1))/increment;
	}

	/**
	* Sets number of frames per one row in spritesheet
	*/
	void SetFrames(int frames){
		this->frames = frames;
	}

	/**
	* Gets number of lines in spritesheet
	*/
	const int GetLines() const{
		return lines;
	}

	/**
	* Sets number of lines in spritesheet
	*/
	void SetLines(int lines){
		this->lines = lines;
	}

	/**
	* Gets starting frame
	*/
	const int GetStart() const{
		return start;
	}

	/**
	* Sets starting frame
	*/
	void SetStart(int start){
		this->start = start;
	}

	/**
	* Gets ending frame
	*/
	const int GetEnd() const{
		return end;
	}

	/**
	* Sets ending frame
	*/
	void SetEnd(int end){
		this->end = end;
	}

	/**
	* Gets increment value
	*/
	const int GetIncrement() const{
		return increment;
	}

	/**
	* Sets increment value
	*/
	void SetIncrement(int increment){
		this->increment = increment;
	}

	/**
	* Gets speed
	*/
	const double GetSpeed() const{
		return speed;
	}

	/**
	* Sets speed
	*/
	void SetSpeed(double speed){
		this->speed = speed;
	}

	/**
	* Gets number of repetitions
	*/
	const int GetRepeat() const{
		return repeat;
	}

	/**
	* Sets number of repetitions
	*/
	void SetRepeat(int repeat){
		this->repeat = repeat;
	}

	/**
	* Gets indicator, if animation is inverted
	*/
	const bool GetIsRevert() const{
		return isRevert;
	}

	/**
	* Sets indicator, if animation is inverted
	*/
	void SetIsRevert(bool isRevert){
		this->isRevert = isRevert;
	}

	/**
	* Gets collection of children
	*/
	vector<spt<Anim>>& GetChildren(){
		return children;
	}

	/**
	* Gets collection of paths to all sheets
	*/
	vector<string>& GetSheetPaths(){
		if (!sheetInit) InitSheets();
		return sheets;
	}
};



