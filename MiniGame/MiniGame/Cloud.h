#pragma once
#include <BitMap.h>
#include "Object.h"

class Cloud : public Object
{
private:
public:
	Cloud();
	~Cloud();

	virtual void MoveTo();
};

