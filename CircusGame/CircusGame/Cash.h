#pragma once
#include "Enemy.h"

#define CASH_X 25
#define CASH_Y 26

class Cash : public Enemy
{
private:
	bool m_bCheckGet;
public:
	Cash();
	~Cash();

	virtual void CheckGetCash();

	virtual inline bool GetCashItem()
	{
		return m_bCheckGet;
	}
};

