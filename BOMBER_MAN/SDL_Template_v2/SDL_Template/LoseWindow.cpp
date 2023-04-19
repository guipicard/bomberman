#include "LoseWindow.h"
#include "Game.h"

LoseWindow::LoseWindow(SDL_Renderer* graphics)
{
	m_BlinkElasped = 0.0f;
	m_BlinkTime = 1.0f;
	m_DrawImg = false;
	m_W = 204;
	m_H = 210;
	m_X = (Game::SCREEN_WIDTH - m_W) / 2;
	m_Y = (Game::SCREEN_HEIGHT - m_H) / 2;

	m_Sprite = new Sprite(m_X, m_Y, m_W, m_H);
	m_Sprite->LoadTexture(graphics, "assets/youlose.png");

	m_ContinueSprite = new Sprite((Game::SCREEN_WIDTH - 528) / 2, (Game::SCREEN_HEIGHT - 28) / 2 + 150, 528, 28);
	m_ContinueSprite->LoadTexture(graphics, "assets/continue.png");

}

LoseWindow::~LoseWindow()
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

void LoseWindow::Draw(SDL_Renderer* graphics)
{
	m_Sprite->Draw(graphics);
	if (m_DrawImg)
	{
		m_ContinueSprite->Draw(graphics);
	}
}

void LoseWindow::Update(float dt)
{
	m_BlinkElasped += dt;
	if (m_BlinkElasped > m_BlinkTime)
	{
		ToggleDrawImg();
		m_BlinkElasped = 0.0f;
	}
}

void LoseWindow::ToggleDrawImg()
{
	m_DrawImg = !m_DrawImg;
}

void LoseWindow::ResetWindow()
{
	m_BlinkElasped = 0.0f;
	m_DrawImg = false;
}
