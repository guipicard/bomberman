#pragma once
#include "Explosion.h"

class Block
{

public:

	Block(const int posX, const int posY, SDL_Renderer* graphics);
	~Block();

	void Draw(SDL_Renderer* graphics);

	int GetPosX()const { return m_X; }
	int GetPosY()const { return m_Y; }

private:

	Sprite* m_Sprite;
	int m_X;
	int m_Y;
};

