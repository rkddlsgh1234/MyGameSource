#include "Cash.h"



Cash::Cash()
{
	m_bCheckGet = false;
}


Cash::~Cash()
{
}

void Cash::CheckGetCash()
{
	m_bCheckGet = true;
}