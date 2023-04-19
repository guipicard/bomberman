#pragma once
#include "Bomb.h"

class Explosion
{
public:
	Explosion(SDL_Renderer* graphics, const int x, const int y, const int range, const int rotation, SDL_RendererFlip flip);
	~Explosion();

	void Update(float dt);
	void Draw(SDL_Renderer* graphics);

	int GetPosX() const { return m_X; }
	int GetPosY() const { return m_Y; }

	void SetPos(int x, int y);

	bool GetTimeUp() const { return m_TimeUp; }
private:
	SDL_Rect m_Rect;
	Sprite* m_Sprite;
	const int m_SpriteLength;
	const int m_SpriteHeight;

	const int m_Range;
	const int m_Rotation;
	const SDL_RendererFlip m_Flip;

	int m_X;
	int m_Y;

	float m_Timer;
	float m_Elapsed;

	bool m_TimeUp;
};

