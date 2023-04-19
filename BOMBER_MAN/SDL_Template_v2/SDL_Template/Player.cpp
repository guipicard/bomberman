#include "Player.h"
#include "Game.h"

Player::Player(SDL_Renderer* graphics)
	: m_SpriteSheetWidth(4)
	, m_SpriteSheetHeight(4)
	, m_FrameTimer(0.2f)
	, m_X(1)
	, m_Y(1)
	, m_Dead(false)
	, m_Win(false)
{
	m_Rect.x = 0;
	m_Rect.y = 0;
	m_Rect.w = Game::SQUARE_SIZE;
	m_Rect.h = Game::SQUARE_SIZE;

	m_SpriteSheet = new Sprite(0, 0, Game::SQUARE_SIZE, Game::SQUARE_SIZE);
	m_SpriteSheet->LoadTexture(graphics, "assets/Bonberman_MVMT_SS.png");
	SetPos(m_X, m_Y);
}

Player::~Player()
{
	if (m_SpriteSheet != nullptr)
	{
		delete m_SpriteSheet;
		m_SpriteSheet = nullptr;
	}
}

void Player::Update(float dt)
{

}

void Player::Draw(SDL_Renderer* graphics)
{
	if (!m_Dead)
	{
		m_SpriteSheet->Draw(graphics, &m_Rect, 0, SDL_FLIP_NONE);
	}
}

void Player::SetPos(int x, int y)
{
	m_X = x;
	m_Y = y;
	m_SpriteSheet->SetPos((m_X * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_X, (m_Y * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_Y);
}

void Player::MovePos(int x, int y)
{
	m_X += x;
	m_Y += y;
	m_SpriteSheet->SetPos((m_X * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_X, (m_Y * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_Y);
}

void Player::SetIsDead()
{
	m_Dead = true;
}

void Player::SetAlive()
{
	SetPos(1, 1);
	m_Win = false;
	m_Dead = false;
}

void Player::SetHasWon()
{
	m_Win = true;
}
