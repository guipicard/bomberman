#pragma once
#include "Block.h"
class SoftWall
{
public:
	SoftWall(SDL_Renderer* graphics);
	~SoftWall();

	void Draw(SDL_Renderer* graphics);

	int GetPosX()const { return m_X; }
	int GetPosY()const { return m_Y; }

	void SetPos(int x, int y);

	bool IsBroken()const { return m_Broken; }

	void SetBroken();

	bool IsDoor()const { return m_Door; }

	void SetDoor();

private:
	Sprite* m_Sprite;

	int m_X;
	int m_Y;

	bool m_Broken;

	bool m_Door;
};
