#pragma once
#include "SoftWall.h"

class StartMenu
{
public:
	StartMenu(SDL_Renderer* graphics);
	~StartMenu();

	void Draw(SDL_Renderer* graphics);
	void Update(float dt);
private:
	Sprite* m_Sprite;

	int m_X;
	int m_Y;
	int m_W;
	int m_H;

	bool m_DrawImg;
	void ToggleDrawImg();
	float m_BlinkTime;
	float m_BlinkElasped;
};
