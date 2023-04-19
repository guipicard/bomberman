#include "WinWindow.h"
#include "Game.h"

WinWindow::WinWindow(SDL_Renderer* graphics)
{
	m_BlinkElasped = 0.0f;
	m_BlinkTime = 1.0f;
	m_DrawImg = false;
	m_W = 440; // 552W, 67H
	m_H = 320; // 8.24
	m_X = (Game::SCREEN_WIDTH - m_W) / 2;
	m_Y = (Game::SCREEN_HEIGHT - m_H) / 2;

	m_Sprite = new Sprite(m_X, m_Y, m_W, m_H);
	m_Sprite->LoadTexture(graphics, "assets/youwin.png");

	m_ContinueSprite = new Sprite((Game::SCREEN_WIDTH - 528) / 2, (Game::SCREEN_HEIGHT - 28) / 2 + 150, 528, 28);
	m_ContinueSprite->LoadTexture(graphics, "assets/continue.png");
}

WinWindow::~WinWindow()
{
	if (m_Sprite != nullptr)
	{
		delete m_Sprite;
		m_Sprite = nullptr;
	}
	if (m_ContinueSprite != nullptr)
	{
		delete m_ContinueSprite;
		m_ContinueSprite = nullptr;
	}
}

void WinWindow::Draw(SDL_Renderer* graphics)
{
		m_Sprite->Draw(graphics);
	if (m_DrawImg)
	{
		m_ContinueSprite->Draw(graphics);
	}
}

void WinWindow::Update(float dt)
{
	m_BlinkElasped += dt;
	if (m_BlinkElasped > m_BlinkTime + 1.0f)
	{
		ToggleDrawImg();
		m_BlinkElasped = 1.0f;
	}
}

void WinWindow::ToggleDrawImg()
{
	m_DrawImg = !m_DrawImg;
}

void WinWindow::ResetWindow()
{
	m_BlinkElasped = 0.0f;
	m_DrawImg = false;
}
