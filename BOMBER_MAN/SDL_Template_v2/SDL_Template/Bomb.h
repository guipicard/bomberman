#pragma once
#include "Sprite.h"

class Bomb
{
public:
	Bomb(SDL_Renderer* graphics, int x, int y);
	~Bomb();

	void Draw(SDL_Renderer* graphics);
	void Update(float dt);

	int GetPosX() const { return m_X; }
	int GetPosY() const { return m_Y; }

	bool GetTimeUp() const { return m_TimeUp; }

private:
	Sprite* m_Sprite;

	int m_X;
	int m_Y;

	float m_Timer;
	float m_Elapsed;

	bool m_TimeUp;
};

