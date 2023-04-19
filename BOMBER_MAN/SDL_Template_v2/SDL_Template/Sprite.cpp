#include "Sprite.h"

Sprite::Sprite()
	: Sprite(0.0f, 0.0f, 1.0f, 1.0f)
{

}

Sprite::Sprite(float x, float y, float w, float h)
	: m_Texture(nullptr)
	, m_X(x)
	, m_Y(y)
	, m_W(w)
	, m_H(h)
{

}

Sprite::~Sprite()
{
	if (m_Texture != nullptr)
	{
		SDL_DestroyTexture(m_Texture);
		m_Texture = nullptr;
	}
}

bool Sprite::LoadTexture(SDL_Renderer* graphics, const std::string& filename)
{
	m_Texture = IMG_LoadTexture(graphics, filename.c_str());
	if (m_Texture == nullptr)
	{
		SDL_Log("Error loading texture %s", filename.c_str());
		return false;
	}

	return true;
}

void Sprite::Draw(SDL_Renderer* graphics)
{
	Draw(graphics, nullptr, m_X, m_Y, m_W, m_H, 0, SDL_FLIP_NONE);
}

void Sprite::Draw(SDL_Renderer* graphics, SDL_Rect* src, double rotation, SDL_RendererFlip flip)
{
	Draw(graphics, src, m_X, m_Y, m_W, m_H, rotation, flip);
}

void Sprite::Draw(SDL_Renderer* graphics, SDL_Rect* src, int x, int y, int w, int h, double rotation, SDL_RendererFlip flip)
{
	SDL_Rect dest = SDL_Rect();
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;

	SDL_RenderCopyEx(graphics, m_Texture, src, &dest, rotation, nullptr, flip);


}

void Sprite::SetPos(int x, int y)
{
	m_X = x;
	m_Y = y;
}
