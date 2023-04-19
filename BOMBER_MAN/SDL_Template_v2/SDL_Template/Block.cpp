#include "Block.h"
#include "Game.h"


Block::Block(const int posX, const int posY, SDL_Renderer* graphics)
	: m_X(posX)
	, m_Y(posY)
{
	m_Sprite = new Sprite(posX, posY, Game::SQUARE_SIZE, Game::SQUARE_SIZE);
	m_Sprite->LoadTexture(graphics, "assets/block.png");
	
}

Block::~Block()
{
	if (m_Sprite != nullptr)
	{
		delete m_Sprite;
		m_Sprite = nullptr;
	}
}

void Block::Draw(SDL_Renderer* graphics)
{
	m_Sprite->Draw(graphics);
}
