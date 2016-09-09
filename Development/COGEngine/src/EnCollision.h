#ifndef ENCOLLISION_H
#define ENCOLLISION_H

/**
* EnCollision - Entity for collisions
*/
class EnCollision{
private:
	int _firstId;
	int _secondId;

public:
	EnCollision(int firstId, int secondId);
	
	int GetFirstId() const;

	void SetFirstId(int val);

	int GetSecondId() const;

	void SetSecondId(int val);
};


#endif

