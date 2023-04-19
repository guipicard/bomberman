#include "Bomb.h"
#include "Game.h"

Bomb::Bomb(SDL_Renderer* graphics, int x, int y)
	: m_X(x)
	, m_Y(y)
	, m_Timer(2.0f)
	, m_Elapsed(0)
	, m_TimeUp(false)
{
	m_Sprite = new Sprite(0, 0, Game::SQUARE_SIZE, Game::SQUARE_SIZE);
	m_Sprite->LoadTexture(graphics, "assets/bomb.png");
	m_Sprite->SetPos((m_X * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_X, (m_Y * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_Y);
}

Bomb::~Bomb()
{
	if (m_Sprite != nullptr)
	{
		delete m_Sprite;
		m_Sprite = nullptr;
	}
}

void Bomb::Draw(SDL_Renderer* graphics)
{
	m_Sprite->Draw(graphics);
}

void Bomb::Update(float dt)
{
	m_Elapsed += dt;
	if (m_Elapsed > m_Timer)
	{
		m_TimeUp = true;
	}
}
