#pragma once
#include <Windows.h>
#include "SDL_image.h"
#include <SDL.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <list>
#include <vector>

using namespace std;

class Sprite
{
public:
	Sprite();
	Sprite(float x, float y, float w, float h);
	~Sprite();

	bool LoadTexture(SDL_Renderer* graphics, const std::string& filename);

	void Draw(SDL_Renderer* graphics);
	void Draw(SDL_Renderer* graphics, SDL_Rect* src, double rotation, SDL_RendererFlip flip);

	void SetPos(int x, int y);
private:
	void Draw(SDL_Renderer* graphics, SDL_Rect* src, int x, int y, int w, int h, double rotation, SDL_RendererFlip flip);

	SDL_Texture* m_Texture;
	float m_X;
	float m_Y;
	float m_W;
	float m_H;
};

