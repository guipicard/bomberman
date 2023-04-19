#include "Game.h"

const int Game::SCREEN_WIDTH = 640;
const int Game::SCREEN_HEIGHT = 640;
const int Game::GRID_WIDTH = 15;
const int Game::GRID_HEIGHT = 13;
const int Game::SQUARE_SIZE = 32;
const int Game::BOARD_WIDTH = SQUARE_SIZE * Game::GRID_WIDTH;
const int Game::BOARD_HEIGHT = SQUARE_SIZE * Game::GRID_HEIGHT;
const int Game::BOARD_OFFSET_X = (Game::SCREEN_WIDTH - BOARD_WIDTH) / 2;
const int Game::BOARD_OFFSET_Y = (Game::SCREEN_HEIGHT - BOARD_HEIGHT) / 2;
const int Game::ENEMY_COUNT = 6;
// Game States
const int Game::STATE_START = 0;
const int Game::STATE_GAME = 1;
const int Game::STATE_PAUSE = 2;
const int Game::STATE_LOSE = 3;
const int Game::STATE_WIN = 4;
// Directions
const vector<vector<int>> Game::DIRECTIONS = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };

Game::Game()
{
	Init();
}

Game::~Game()
{
	if (m_StartMenuWindow != nullptr)
	{
		delete m_StartMenuWindow;
		m_StartMenuWindow = nullptr;
	}
	for (auto iter = enemies.begin(); iter != enemies.end(); iter++)
	{
		if ((*iter) != nullptr)
		{
			delete (*iter);
			(*iter) = nullptr;
		}
	}
	if (m_WinWindow != nullptr)
	{
		delete m_WinWindow;
		m_WinWindow = nullptr;
	}
	if (m_LoseWindow != nullptr)
	{
		delete m_LoseWindow;
		m_LoseWindow = nullptr;
	}
	if (m_Player != nullptr)
	{
		delete m_Player;
		m_Player = nullptr;
	}
	if (m_Grid != nullptr)
	{
		delete m_Grid;
		m_Grid = nullptr;
	}
	if (m_Graphics != nullptr)
	{
		SDL_DestroyRenderer(m_Graphics);
		m_Graphics = nullptr;
	}
	if (m_Window != nullptr)
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}

	SDL_Quit();
	SDL_Log("PROGRAM END\n");
}

void Game::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_Log("Error : %s\n", SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Game App",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_UTILITY
	);

	m_Graphics = SDL_CreateRenderer(m_Window, 0, SDL_RENDERER_ACCELERATED);

	keyDowns = set<SDL_Keycode>();

	m_Player = new Player(m_Graphics);

	float timeOffset = 0.0f;
	float currentTimeOffset = 1.0f / ENEMY_COUNT;

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		enemies.push_back(new Enemy(m_Graphics, timeOffset));
		timeOffset += currentTimeOffset;
	}

	m_Grid = nullptr;

	m_ScreenRect.x = 0;
	m_ScreenRect.y = 0;
	m_ScreenRect.w = SCREEN_WIDTH;
	m_ScreenRect.h = SCREEN_HEIGHT;

	m_GameRect.x = BOARD_OFFSET_X;
	m_GameRect.y = BOARD_OFFSET_Y;
	m_GameRect.w = BOARD_WIDTH;
	m_GameRect.h = BOARD_HEIGHT;

	m_StartMenuWindow = new StartMenu(m_Graphics);

	m_WinWindow = new WinWindow(m_Graphics);

	m_LoseWindow = new LoseWindow(m_Graphics);

	m_PauseSprite = new Sprite((SCREEN_WIDTH - 153) / 2, (SCREEN_HEIGHT - 46) / 2, 153, 46);
	m_PauseSprite->LoadTexture(m_Graphics, "assets/pause.png");

	m_State = STATE_START;

	m_PauseTexture = CreateTransparentTexture(m_Graphics, SCREEN_WIDTH, SCREEN_HEIGHT, 60, 60, 60, 128);
	m_BlackOutTexture = CreateTransparentTexture(m_Graphics, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 0);

	m_BlackOutElapsed = 0.0f;
}

void Game::Run()
{

	if (m_Graphics == nullptr)
	{
		SDL_Log("Error : %s\n", SDL_GetError());
	}

	m_Running = true;
	Uint32 lastFrameTime = SDL_GetTicks();
	SDL_Event events;

	while (m_Running)
	{
		ProcessEvents();
		Uint32 currentFrameTime = SDL_GetTicks();
		float deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f;
		while (SDL_PollEvent(&events))
		{
			switch (events.type)
			{
			case SDL_QUIT:
				m_Running = false;
				break;
			}
		}
		Update(deltaTime);
		Draw(m_Graphics);

		// Push back buffer to the front buffer
		SDL_RenderPresent(m_Graphics);

		lastFrameTime = currentFrameTime;
		ClearKeyDowns();
	}
}

void Game::Update(float dt)
{

	switch (m_State)
	{
	case STATE_START:
		m_StartMenuWindow->Update(dt);
		InputListener();
		break;
	case STATE_GAME:
		if (m_Grid != nullptr)
		{
			if (m_Player->GetHasWon())
			{
				m_BlackOutElapsed += dt;
				if (m_BlackOutElapsed > 2.0f)
				{
					m_State = STATE_WIN;
				}
				break;
			}
			if (m_Player->GetIsDead())
			{
				m_BlackOutElapsed += dt;
				if (m_BlackOutElapsed > 2.0f)
				{
					m_State = STATE_LOSE;
				}
				break;
			}
			m_Grid->Update(dt);
			InputListener();
		}
		else
		{
			for (auto iter = enemies.begin(); iter != enemies.end(); iter++)
			{
				(*iter)->SetIsAlive();
			}
			m_Grid = new Grid(m_Graphics, m_Player, enemies);
			m_Player->SetAlive();
			m_BlackOutElapsed = 0.0f;
			m_WinWindow->ResetWindow();
			m_LoseWindow->ResetWindow();
		}
		break;
	case STATE_PAUSE:
		InputListener();
		break;
	case STATE_LOSE:
		m_LoseWindow->Update(dt);
		DestroyGame();
		InputListener();
		break;
	case STATE_WIN:
		m_WinWindow->Update(dt);
		DestroyGame();
		InputListener();
		break;
	}
}

void Game::Draw(SDL_Renderer* graphics)
{
	switch (m_State)
	{
	case STATE_START:
		SDL_SetRenderDrawColor(m_Graphics, 60, 60, 60, 1);
		SDL_RenderClear(m_Graphics);
		SDL_SetRenderDrawColor(m_Graphics, 0, 0, 0, 1);
		SDL_RenderFillRect(m_Graphics, &m_GameRect);
		m_StartMenuWindow->Draw(m_Graphics);
		break;
	case STATE_GAME:
		if (m_Grid != nullptr)
		{
			SDL_SetRenderDrawColor(m_Graphics, 60, 60, 60, 1);
			SDL_RenderClear(m_Graphics);
			SDL_SetRenderDrawColor(m_Graphics, 0, 255, 0, 1);
			SDL_RenderFillRect(m_Graphics, &m_GameRect);
			m_Grid->Draw(graphics);
			if (m_BlackOutElapsed > 1.0f)
			{
				m_BlackOutTexture = CreateTransparentTexture(m_Graphics, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 255 * (m_BlackOutElapsed - 1.0f));
				SDL_RenderCopy(m_Graphics, m_BlackOutTexture, nullptr, &m_GameRect);
			}
			else
			{
				m_Player->Draw(graphics);
			}
		}
		break;
	case STATE_PAUSE:
		if (m_Grid != nullptr)
		{
			SDL_SetRenderDrawColor(m_Graphics, 60, 60, 60, 1);
			SDL_RenderClear(m_Graphics);
			SDL_SetRenderDrawColor(m_Graphics, 0, 255, 0, 1);
			SDL_RenderFillRect(m_Graphics, &m_GameRect);
			m_Grid->Draw(graphics);
			m_Player->Draw(graphics);
		}

		SDL_RenderCopy(m_Graphics, m_PauseTexture, nullptr, &m_GameRect);
		m_PauseSprite->Draw(m_Graphics);
		break;
	case STATE_LOSE:
		SDL_SetRenderDrawColor(m_Graphics, 60, 60, 60, 1);
		SDL_RenderClear(m_Graphics);
		SDL_SetRenderDrawColor(m_Graphics, 0, 0, 0, 1);
		SDL_RenderFillRect(m_Graphics, &m_GameRect);
		m_LoseWindow->Draw(m_Graphics);
		break;
	case STATE_WIN:
		SDL_SetRenderDrawColor(m_Graphics, 60, 60, 60, 1);
		SDL_RenderClear(m_Graphics);
		SDL_SetRenderDrawColor(m_Graphics, 0, 0, 0, 1);
		SDL_RenderFillRect(m_Graphics, &m_GameRect);
		m_WinWindow->Draw(m_Graphics);
		break;
	}
}

void Game::InputListener()
{
	switch (m_State)
	{
	case STATE_START:
		if (GetKeyDown(SDLK_ESCAPE))
		{
			m_Running = false;
		}
		if (GetKeyDown(SDLK_SPACE))
		{
			m_State = STATE_GAME;
		}
		break;
	case STATE_GAME:
		if (GetKeyDown(SDLK_ESCAPE))
		{
			m_State = STATE_PAUSE;
		}
		if (GetKeyDown(SDLK_SPACE))
		{
			m_Grid->CreateBomb(m_Player->GetPosX(), m_Player->GetPosY());
		}
		if (GetKeyDown(SDLK_LEFT))
		{
			m_Grid->MovePlayer({ -1, 0 });
		}
		if (GetKeyDown(SDLK_RIGHT))
		{
			m_Grid->MovePlayer({ 1, 0 });
		}
		if (GetKeyDown(SDLK_UP))
		{
			m_Grid->MovePlayer({ 0, -1 });
		}
		if (GetKeyDown(SDLK_DOWN))
		{
			m_Grid->MovePlayer({ 0, 1 });
		}
		break;
	case STATE_PAUSE:
		if (GetKeyDown(SDLK_ESCAPE))
		{
			m_State = STATE_GAME;
		}
		break;
	case STATE_WIN:
		if (GetKeyDown(SDLK_ESCAPE))
		{
			m_Running = false;
		}
		if (GetKeyDown(SDLK_SPACE))
		{
			m_State = STATE_START;
		}
		break;
	case STATE_LOSE:
		if (GetKeyDown(SDLK_ESCAPE))
		{
			m_Running = false;
		}
		if (GetKeyDown(SDLK_SPACE))
		{
			m_State = STATE_START;
		}
		break;
	}
}

void Game::ProcessEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			m_Running = false;
		}
		else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_UP:
			case SDLK_DOWN:
			case SDLK_LEFT:
			case SDLK_RIGHT:
			case SDLK_SPACE:
			case SDLK_ESCAPE:
				if (e.key.repeat == 0) {
					keyDowns.insert(e.key.keysym.sym);
				}
				break;
			default:
				break;
			}
		}
	}
}

bool Game::GetKeyDown(SDL_Keycode key)
{
	return keyDowns.find(key) != keyDowns.end();
}

void Game::ClearKeyDowns()
{
	keyDowns.clear();
}

void Game::DestroyGame()
{
	if (m_Grid != nullptr)
	{
		ClearKeyDowns();
		delete m_Grid;
		m_Grid = nullptr;
	}
}

SDL_Texture* Game::CreateTransparentTexture(SDL_Renderer* renderer, int width, int height, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, r, g, b, a));

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}
