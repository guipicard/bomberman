#pragma once
#include "WinWindow.h"
class LoseWindow
{
public:
	LoseWindow(SDL_Renderer* graphics);
	~LoseWindow();

	void Draw(SDL_Renderer* graphics);
	void Update(float dt);
	void ResetWindow();
private:
	Sprite* m_Sprite;
	Sprite* m_ContinueSprite;

	int m_X;
	int m_Y;
	int m_W;
	int m_H;

	bool m_DrawImg;
	void ToggleDrawImg();
	float m_BlinkTime;
	float m_BlinkElasped;
};

