#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}


int Player::MoveTo(float ElapseTime)
{
	int iKeyState;

	if (GetKeyState(VK_LEFT) & 0x8000)
		iKeyState = VK_LEFT;
	else if (GetKeyState(VK_RIGHT) & 0x8000)
		iKeyState = VK_RIGHT;
	else if (GetKeyState(VK_UP) & 0x8000)
		iKeyState = VK_UP;
	else if (GetKeyState(VK_DOWN) & 0x8000)
		iKeyState = VK_DOWN;
	else
		iKeyState = NULL;

	Tank::Move(ElapseTime, iKeyState);

	return iKeyState;
}