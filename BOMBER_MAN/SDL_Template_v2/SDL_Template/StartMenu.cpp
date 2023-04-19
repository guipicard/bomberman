#include "StartMenu.h"
#include "Game.h"


StartMenu::StartMenu(SDL_Renderer* graphics)
{
	m_BlinkElasped = 0.0f;
	m_BlinkTime = 0.5f;
	m_DrawImg = true;
	m_W = 450; // 552W, 67H
	m_H = 55; 
	m_X = (Game::SCREEN_WIDTH - m_W) / 2;
	m_Y = (Game::SCREEN_HEIGHT - m_H) / 2;

	m_Sprite = new Sprite(m_X, m_Y, m_W, m_H);
	m_Sprite->LoadTexture(graphics, "assets/start.png");

}

StartMenu::~StartMenu()
{
	if (m_Sprite != nullptr)
	{
		delete m_Sprite;
		m_Sprite = nullptr;
	}
}

void StartMenu::Draw(SDL_Renderer* graphics)
{
	if (m_DrawImg)
	{
		m_Sprite->Draw(graphics);
	}
}

void StartMenu::Update(float dt)
{
	m_BlinkElasped += dt;
	if (m_BlinkElasped > m_BlinkTime)
	{
		ToggleDrawImg();
		m_BlinkElasped = 0.0f;
	}
}

void StartMenu::ToggleDrawImg()
{
	m_DrawImg = !m_DrawImg;
}
