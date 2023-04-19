#pragma once
#include "grid.h"
#include <set>

class Game
{
public:
	static const int SCREEN_WIDTH;
	static const int SCREEN_HEIGHT;
	static const int GRID_WIDTH;
	static const int GRID_HEIGHT;
	static const int SQUARE_SIZE;
	static const int BOARD_WIDTH;
	static const int BOARD_HEIGHT;
	static const int BOARD_OFFSET_X;
	static const int BOARD_OFFSET_Y;
	static const int ENEMY_COUNT;
	// Game states
	static const int STATE_START;
	static const int STATE_GAME;
	static const int STATE_PAUSE;
	static const int STATE_LOSE;
	static const int STATE_WIN;
	// Directions
	static const vector<vector<int>> DIRECTIONS;

	Game();

	~Game();

	void Run();

	void SetGameState(int state)
	{
		m_State = state;
	}

private:
	void Init();
	void Update(float dt);
	void Draw(SDL_Renderer* graphics);

	SDL_Window* m_Window;
	SDL_Renderer* m_Graphics;
	
	bool m_Running;
	
	int m_State;

	Player* m_Player;
	Grid* m_Grid;

	SDL_Rect m_ScreenRect;
	SDL_Rect m_GameRect;

	//Start Menu
	StartMenu* m_StartMenuWindow;

	//PAUSE
	float m_BlackOutElapsed;
	Sprite* m_PauseSprite;
	SDL_Texture* m_PauseTexture;
	SDL_Texture* m_BlackOutTexture;
	SDL_Texture* CreateTransparentTexture(SDL_Renderer* renderer, int width, int height, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	//WINMENU
	WinWindow* m_WinWindow;
	//LOSEMENU
	LoseWindow* m_LoseWindow;

	list<Enemy*> enemies;

	set<SDL_Keycode> keyDowns;
	void InputListener();
	void ProcessEvents();
	bool GetKeyDown(SDL_Keycode key);
	void ClearKeyDowns();

	void DestroyGame();

};
