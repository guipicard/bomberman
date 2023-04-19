#include "Explosion.h"
#include "Game.h"


Explosion::Explosion(SDL_Renderer* graphics, const int x, const int y, const int range, const int rotation, SDL_RendererFlip flip)
	: m_X(x)
	, m_Y(y)
	, m_SpriteLength(7)
	, m_SpriteHeight(3)
	, m_Range(range)
	, m_Rotation(rotation)
	, m_Timer(1.0f)
	, m_Elapsed(0.0f)
	, m_TimeUp(false)
	, m_Flip(flip)
	
{
	m_Sprite = new Sprite(0, 0, Game::SQUARE_SIZE, Game::SQUARE_SIZE);
	m_Sprite->LoadTexture(graphics, "assets/flames.png");
	m_Rect.x = 3 * Game::SQUARE_SIZE;
	m_Rect.y = m_Range * Game::SQUARE_SIZE;
	m_Rect.w = Game::SQUARE_SIZE;
	m_Rect.h = Game::SQUARE_SIZE;
	SetPos(m_X, m_Y);

}

Explosion::~Explosion()
{
	if (m_Sprite != nullptr)
	{
		delete m_Sprite;
		m_Sprite = nullptr;
	}
}

void Explosion::Update(float dt)
{
	m_Elapsed += dt;
	if (m_Elapsed > m_Timer)
	{
		m_TimeUp = true;
	}
}

void Explosion::Draw(SDL_Renderer* graphics)
{
	m_Sprite->Draw(graphics, &m_Rect, m_Rotation, m_Flip);
}

void Explosion::SetPos(int x, int y)
{
	m_X = x;
	m_Y = y;
	m_Sprite->SetPos((m_X * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_X, (m_Y * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_Y);
}
