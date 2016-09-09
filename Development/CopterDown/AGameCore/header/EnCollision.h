
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


#endif
