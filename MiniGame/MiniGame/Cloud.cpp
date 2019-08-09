#include "Cloud.h"

Cloud::Cloud()
{
}


Cloud::~Cloud()
{
}


void Cloud::MoveTo()
{
	if (Object::GetPos().y > 800)
		Object::SetPos(NULL, -600);
}