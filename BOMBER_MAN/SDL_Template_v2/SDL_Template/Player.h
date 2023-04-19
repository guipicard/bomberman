#pragma once
#include "Enemy.h"

class Player
{
public:
	Player(SDL_Renderer* graphics);
	~Player();

	void Update(float dt);
	void Draw(SDL_Renderer* graphics);

	void SetPos(int x, int y);
	void MovePos(int x, int y);

	int GetPosX() const { return m_X; }
	int GetPosY() const { return m_Y; }

	bool GetIsDead() const { return m_Dead; }
	void SetIsDead();
	void SetAlive();

	bool GetHasWon() const { return m_Win; }
	void SetHasWon();


private:

	Sprite* m_SpriteSheet;
	SDL_Rect m_Rect;
	int m_X;
	int m_Y;

	bool m_Dead;
	bool m_Win;

	const int m_SpriteSheetWidth;
	const int m_SpriteSheetHeight;
	const float m_FrameTimer;
};

