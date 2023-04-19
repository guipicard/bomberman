#pragma once
#include "LoseWindow.h"

class Enemy
{
public:
	Enemy(SDL_Renderer* graphics, float timeOffset);
	~Enemy();

	void Update(float dt);
	void Draw(SDL_Renderer* graphics);

	vector<int> GetPos() const { return { m_X, m_Y }; }
	
	void SetPos(int x, int y);

	bool IsDead() const { return m_Dead; }

	void SetIsDead();

	bool GetCanMove() const { return m_CanMove; }
	void SetHasMoved();

	void SetLastDirection(vector<int> direction);

	void SetIsAlive();

private:

	Sprite* m_Sprite;

	int m_X;
	int m_Y;

	bool m_Dead;

	float m_MoveElapsed;
	const float m_MoveTimer;
	bool m_CanMove;
	vector<int> m_LastDirection;

	// Animations
	float m_AnimElapsed;
	float m_AnimX;
	float m_AnimY;
};
