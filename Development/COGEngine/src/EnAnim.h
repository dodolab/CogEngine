#pragma once


/**
* Entity for animations
*/
class EnAnim{
protected:
	// link to root animation
	spt<EnAnim> rootAnim;
	// name of animation
	string name;
	// path to image
	string imgPath;
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
	// number of repetition
	int repeat;
	// if true, animation will be reverted
	bool isRevert;
	// list of children
	vector<spt<EnAnim>> children;

	void FillFromXml(spt<ofxXmlSettings> xml){
		int innerAnimations = xml->getNumTags("anim");

		this->name = xml->getAttribute(":", "name", "");
		cout << "LOADING ANIMATION " << this->name << endl;
		cout << "  INNER ANIMATIONS: " << innerAnimations << endl;

		this->imgPath = xml->getAttribute(":", "img", "");
		cout << "  --img: " << this->imgPath << endl;
		this->ref = xml->getAttribute(":", "ref", "");
		cout << "  --ref: " << this->ref << endl;
		this->frames = xml->getAttribute(":", "frames", 1);
		cout << "  --frames: " << ofToString(this->frames) << endl;
		this->lines = xml->getAttribute(":", "lines", 1);
		cout << "  --lines: " << ofToString(this->lines) << endl;
		this->start = xml->getAttribute(":", "start", 0);
		cout << "  --start: " << ofToString(this->start) << endl;
		this->end = xml->getAttribute(":", "end", 0);
		cout << "  --end: " << ofToString(this->end) << endl;
		this->increment = xml->getAttribute(":", "increment", 1);
		cout << "  --increment: " << ofToString(this->increment) << endl;
		this->repeat = xml->getAttribute(":", "repeat", 1);
		cout << "  --repeat: " << ofToString(this->repeat) << endl;
		this->isRevert = xml->getBoolValue(":", false);
		cout << "  --isRevert: " << ofToString(this->isRevert) << endl;

		for (int i = 0; i < innerAnimations; i++){
			xml->pushTag("anim", i);
			spt<EnAnim> anim = spt<EnAnim>(new EnAnim(xml));
			anim->SetRoot(spt<EnAnim>(this));
			children.push_back(anim);
			xml->popTag();
		}
	}

public:
		
	EnAnim(spt<ofxXmlSettings> construct) : name(""), imgPath(""), ref(""), frames(0), lines(0), start(0), end(0), increment(0), repeat(0), isRevert(false){
		FillFromXml(construct);
	}

	EnAnim(string name, string imgPath, string ref, int frames, int lines, int start, 
		int end, int increment, int repeat, bool isRevert) : name(name), imgPath(imgPath), ref(ref),
		frames(frames), lines(lines), start(start), end(end), increment(increment), repeat(repeat), isRevert(isRevert){
		this->rootAnim = spt<EnAnim>(nullptr);
	}

	EnAnim() : name(""), imgPath(""), ref(""), frames(0), lines(0), start(0), end(0), increment(0), repeat(0), isRevert(false){
		this->rootAnim = spt<EnAnim>(nullptr);
	}

	~EnAnim(){
		for (auto it = children.begin(); it != children.end(); ++it){
			delete (*it);
		}
	}

	/**
	* Finds recursively child by name
	* @param name according  to find the child
	*/
	spt<EnAnim> FindChild(string name){
		if (this->GetName().compare(name) == 0) return spt<EnAnim>(this);

		for (auto it = children.begin(); it != children.end(); ++it){
			if ((*it)->GetName().compare(name) == 0) return (*it);
			else{
				auto childFound = (*it)->FindChild(name);
				if (childFound != spt<EnAnim>()) return childFound;
			}
		}

		return spt<EnAnim>();
	}

	/**
	* Adds a new animation child
	* @param child child to add
	* @return true, if child has been added (it didn't exist in the collection)
	*/
	bool AddChild(spt<EnAnim> child){
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
	bool RemoveChild(spt<EnAnim> child){
		auto found = find(children.begin(), children.end(), child);
		if (found != children.end()){
			children.erase(found);
			return true;
		}
		else return false;
	}

	/**
	* Gets root animation
	*/
	spt<EnAnim> GetRoot(){
		return rootAnim;
	}

	/**
	* Sets root animation
	*/
	void SetRoot(spt<EnAnim> root){
		this->rootAnim = root;
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
	* Gets path to the animation image
	*/
	const string GetImgPath() const{
		return imgPath;
	}

	/**
	* Sets path to the animation image
	*/
	void SetImgPath(string imgPath){
		this->imgPath = imgPath;
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
	vector<spt<EnAnim>>& GetChildren(){
		return children;
	}

};



