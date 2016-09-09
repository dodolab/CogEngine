
#ifndef ENCOLLISION_H
#define ENCOLLISION_H

class EnCollision{
private:
	int _firstId;
	int _secondId;

public:
	EnCollision(int firstId, int secondId);
	
	int GetFirstId();

	void SetFirstId(int val);

	int GetSecondId();

	void SetSecondId(int val);
};

EnCollision::EnCollision(int firstId, int secondId) : _firstId(firstId), _secondId(secondId){

}


int EnCollision::GetFirstId(){
	return _firstId;
}

void EnCollision::SetFirstId(int val){
	_firstId = val;
}

int EnCollision::GetSecondId(){
	return _secondId;
}

void EnCollision::SetSecondId(int val){
	_secondId = val;
}

#endif

