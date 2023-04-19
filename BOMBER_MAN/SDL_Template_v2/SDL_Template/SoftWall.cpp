#include "SoftWall.h"
#include "Game.h"

SoftWall::SoftWall(SDL_Renderer* graphics)
	: m_X(0)
	, m_Y(0)
	, m_Broken(false)
	, m_Door(false)
{
	m_Sprite = new Sprite(0, 0, Game::SQUARE_SIZE, Game::SQUARE_SIZE);
	m_Sprite->LoadTexture(graphics, "assets/SoftWall.png");
}

SoftWall::~SoftWall()
{
	if (m_Sprite != nullptr)
	{
		delete m_Sprite;
		m_Sprite = nullptr;
	}
}

void SoftWall::Draw(SDL_Renderer* graphics)
{
	m_Sprite->Draw(graphics);
}

void SoftWall::SetBroken()
{
	m_Broken = true;
}

void SoftWall::SetDoor()
{
	m_Door = true;
}

void SoftWall::SetPos(int x, int y)
{
	m_X = x;
	m_Y = y;
	m_Sprite->SetPos((x * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_X, (y * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_Y);
}
