#include "Enemy.h"
#include "Game.h"


Enemy::Enemy(SDL_Renderer* graphics, float timeOffset)
	: m_X(0)
	, m_Y(0)
	, m_Dead(false)
	, m_MoveTimer(1.0f)
	, m_MoveElapsed(timeOffset)
	, m_AnimElapsed(0.0f)
	, m_AnimX(0.0f)
	, m_AnimY(0.0f)
	, m_CanMove(false)
{
	m_LastDirection = vector<int>();

	m_Sprite = new Sprite(0, 0, Game::SQUARE_SIZE, Game::SQUARE_SIZE);
	m_Sprite->LoadTexture(graphics, "assets/enemy.png");
}

Enemy::~Enemy()
{
	if (m_Sprite != nullptr)
	{
		delete m_Sprite;
		m_Sprite = nullptr;
	}
}

void Enemy::Update(float dt)
{
	m_AnimElapsed += dt;
	m_MoveElapsed += dt;
	if (m_MoveElapsed > m_MoveTimer)
	{
		if (rand() % 2 == 1)
		{
			m_CanMove = true;
		}
	}
}

void Enemy::Draw(SDL_Renderer* graphics)
{
	if (m_AnimElapsed > 0.3f)
	{
		m_AnimX = m_AnimX == 0.0f ? 32.0f : 0.0f;
		m_AnimElapsed = 0.0f;
	}
	if (m_LastDirection == Game::DIRECTIONS[3])
	{
		m_AnimY = 32.0f;
	}
	else if (m_LastDirection == Game::DIRECTIONS[1])
	{
		m_AnimY = 64.0f;
	}
	else
	{
		m_AnimY = 0.0f;
	}
	SDL_Rect src = SDL_Rect();
	src.x = m_AnimX;
	src.y = m_AnimY;
	src.w = Game::SQUARE_SIZE;
	src.h = Game::SQUARE_SIZE;

	m_Sprite->Draw(graphics, &src, 0.0, m_LastDirection == Game::DIRECTIONS[0] ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

void Enemy::SetPos(int x, int y)
{
	m_X = x;
	m_Y = y;
	m_Sprite->SetPos((m_X * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_X, (m_Y * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_Y);
	SetHasMoved();
}

void Enemy::SetIsDead()
{
	m_Dead = true;
}

void Enemy::SetHasMoved()
{
	m_CanMove = false;
	m_MoveElapsed = 0.0f;
}

void Enemy::SetLastDirection(vector<int> direction)
{
	m_LastDirection = direction;
}

void Enemy::SetIsAlive()
{
	m_Dead = false;
	m_CanMove = false;
	m_MoveElapsed = 0.0f;
	m_AnimX = 0.0f;
	m_AnimY = 0.0f;
}
