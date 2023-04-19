#include "Grid.h"
#include "Game.h"

Grid::Grid(SDL_Renderer* graphics, Player* player, list<Enemy*> enemiesList)
{
	m_Graphics = graphics;
	m_Player = player;
	enemies = enemiesList;
	m_DrawDoor = false;

	m_Door = new Sprite(0, 0, Game::SQUARE_SIZE, Game::SQUARE_SIZE);
	m_Door->LoadTexture(m_Graphics, "assets/door.png");

	vector<vector<int>> freeSquares;
	bombs = std::list<Bomb*>();
	explosions = std::list<Explosion*>();

	srand(time(NULL));
	for (int i = 0; i < Game::GRID_HEIGHT; i++)
	{
		m_Grid.push_back(std::vector<char>());

		for (int j = 0; j < Game::GRID_WIDTH; j++)
		{
			if (j % 2 == 0 && i % 2 == 0 || i == 0 || j == 0 || i == Game::GRID_HEIGHT - 1 || j == Game::GRID_WIDTH - 1)
			{
				m_Grid[i].push_back('x');
				blocks.push_back(new Block((j * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_X, (i * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_Y, m_Graphics));
			}
			else
			{
				m_Grid[i].push_back(' ');
				if (i > 3 || j > 3)
				{
					freeSquares.push_back({ j, i });
				}
			}
		}
	}

	for (auto iter = enemies.begin(); iter != enemies.end(); iter++)
	{
		int randIndex = rand() % freeSquares.size();
		vector<int> randPos = freeSquares[randIndex];
		(*iter)->SetPos(randPos[0], randPos[1]);
		vector<vector<int>>::iterator it;
		it = remove_if(freeSquares.begin(), freeSquares.end(),
			[=](vector<int> k) {
				return (k[0] == freeSquares[randIndex][0] && k[1] == freeSquares[randIndex][1]);
			});
		freeSquares.erase(it, freeSquares.end());
	}

	for (int i = 0; i < freeSquares.size() * 30 / 100; i++)
	{
		softWalls.push_back(new SoftWall(m_Graphics));
	}
	(*softWalls.begin())->SetDoor();

	for (auto iter = softWalls.begin(); iter != softWalls.end(); iter++)
	{
		int freeSquaresSize = freeSquares.size();
		int randIndex = rand() % freeSquaresSize;
		int posX = freeSquares[randIndex][0];
		int posY = freeSquares[randIndex][1];
		if ((*iter)->IsDoor())
		{
			m_Door->SetPos((posX * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_X, (posY * Game::SQUARE_SIZE) + Game::BOARD_OFFSET_Y);
		}
		(*iter)->SetPos(posX, posY);
		SetGridAt(posX, posY, 'o', false);
		vector<vector<int>>::iterator it;
		it = remove_if(freeSquares.begin(), freeSquares.end(),
			[=](vector<int> k) {
				return (k[0] == freeSquares[randIndex][0] && k[1] == freeSquares[randIndex][1]);
			});
		freeSquares.erase(it, freeSquares.end());
	}
}

Grid::~Grid()
{
	if (m_Door != nullptr)
	{
		delete m_Door;
		m_Door = nullptr;
	}
	for (auto iter = blocks.begin(); iter != blocks.end(); iter++)
	{
		if ((*iter) != nullptr)
		{
			delete (*iter);
			(*iter) = nullptr;
		}
	}
	for (auto iter = softWalls.begin(); iter != softWalls.end(); iter++)
	{
		if ((*iter) != nullptr)
		{
			delete (*iter);
			(*iter) = nullptr;
		}
	}
}

void Grid::Draw(SDL_Renderer* graphics)
{
	for (auto iter = blocks.begin(); iter != blocks.end(); iter++)
	{
		(*iter)->Draw(graphics);
	}
	for (auto iter = softWalls.begin(); iter != softWalls.end(); iter++)
	{
		const int currentX = (*iter)->GetPosX();
		const int currentY = (*iter)->GetPosY();
		if (!(*iter)->IsBroken())
		{
			if (GetGridAt(currentX, currentY) != 'o')
			{
				(*iter)->SetBroken();

			}
			(*iter)->Draw(graphics);
		}
		else if ((*iter)->IsDoor() && !m_DrawDoor)
		{
			m_DrawDoor = true;
		}
		else if ((*iter)->IsDoor() && m_DrawDoor && GetGridAt(currentX, currentY) != 'd')
		{
			SetGridAt(currentX, currentY, 'd');
		}
	}
	if (explosions.size() != 0)
	{
		for (auto iter = explosions.begin(); iter != explosions.end(); iter++)
		{
			if (!(*iter)->GetTimeUp())
			{
				(*iter)->Draw(graphics);
			}
		}
	}
	if (bombs.size() != 0)
	{
		for (auto iter = bombs.begin(); iter != bombs.end(); iter++)
		{
			(*iter)->Draw(graphics);
			if (GetGridAt((*iter)->GetPosX(), (*iter)->GetPosY()) == ' ')
			{
				SetGridAt((*iter)->GetPosX(), (*iter)->GetPosY(), 'b');
			}
		}
	}
	if (m_DrawDoor)
	{
		m_Door->Draw(m_Graphics);
	}
	for (auto iter = enemies.begin(); iter != enemies.end(); iter++)
	{
		if (!(*iter)->IsDead())
		{
			(*iter)->Draw(m_Graphics);
		}
	}
}

void Grid::Update(float dt)
{
	if (explosions.size() != 0)
	{
		for (auto iter = explosions.begin(); iter != explosions.end(); iter++)
		{
			(*iter)->Update(dt);
		}
		DeleteExplosions();
	}
	if (bombs.size() != 0)
	{
		for (auto iter = bombs.begin(); iter != bombs.end(); iter++)
		{
			(*iter)->Update(dt);
		}
		DeleteBomb((*bombs.begin())->GetPosX(), (*bombs.begin())->GetPosY());
	}
	int enemyAlive = 0;
	for (auto iter = enemies.begin(); iter != enemies.end(); iter++)
	{
		if (!(*iter)->IsDead())
		{
			enemyAlive++;
			vector<int> enemyPos = (*iter)->GetPos();
			if (GetGridAt(enemyPos[0], enemyPos[1]) == 'e')
			{
				(*iter)->SetIsDead();
			}

			(*iter)->Update(dt);

			if ((*iter)->GetCanMove())
			{
				vector<vector<int>> availableDirs;
				for (int i = 0; i < Game::DIRECTIONS.size(); i++)
				{
					char currentVal = GetGridAt((*iter)->GetPos()[0] + Game::DIRECTIONS[i][0], (*iter)->GetPos()[1] + Game::DIRECTIONS[i][1]);
					if (currentVal != 'x' && currentVal != 'o' && currentVal != 'g' && currentVal != 'b')
					{
						availableDirs.push_back(Game::DIRECTIONS[i]);
					}
				}
				if (availableDirs.size() != 0)
				{
					int dirIndex = rand() % availableDirs.size();
					vector<int> currentdir = availableDirs[rand() % availableDirs.size()];
					(*iter)->SetLastDirection(currentdir);
					if (GetGridAt((*iter)->GetPos()[0] + currentdir[0], (*iter)->GetPos()[1] + currentdir[1]) == 'p')
					{
						SetPlayerDead();
					}
					MoveEnemy((*iter), currentdir);
				}

			}
		}
	}
	if (enemyAlive == 0)
	{
		m_DrawDoor = true;
	}
}

char Grid::GetGridAt(int x, int y)
{
	return m_Grid[y][x];
}

void Grid::SetGridAt(int x, int y, char val)
{
	SetGridAt(x, y, val, true);
}

void Grid::SetGridAt(int x, int y, char val, bool print)
{
	m_Grid[y][x] = val;
	//if (print)
	//{
	//	system("CLS");
	//	for (int i = 0; i < m_Grid.size(); i++)
	//	{
	//		for (int j = 0; j < m_Grid[i].size(); j++)
	//		{
	//			std::cout << m_Grid[i][j];
	//		}
	//		std::cout << std::endl;
	//	}
	//}
}

void Grid::CreateBomb(int x, int y)
{
	bombs.push_back(new Bomb(m_Graphics, x, y));
}

void Grid::DeleteBomb(int x, int y)
{
	auto firstBomb = bombs.begin();
	if ((*firstBomb)->GetTimeUp())
	{
		CreateExplosion(x, y);
		bombs.pop_front();
	}

}

void Grid::CreateExplosion(int x, int y)
{
	explosions.push_back(new Explosion(m_Graphics, x, y, 0, 0, SDL_FLIP_NONE));
	if (GetGridAt(x, y) == 'p')
	{
		SetPlayerDead();
	}
	SetGridAt(x, y, 'e');
	vector<vector<vector<int>>> ranges = { {{ x - 1, y }, {x - 2, y}}, {{x, y - 1}, {x, y - 2}}, {{ x + 1, y }, {x + 2, y}}, {{x, y + 1}, {x, y + 2}} };

	for (int i = 0; i < ranges.size(); i++)
	{
		for (int j = 0; j < ranges[i].size(); j++)
		{
			const int currentX = ranges[i][j][0];
			const int currentY = ranges[i][j][1];

			if (currentX >= 0 && currentX < Game::GRID_WIDTH && currentY >= 0 && currentY < Game::GRID_HEIGHT)
			{
				// ---------- BOUNDS
				const char currentValue = GetGridAt(currentX, currentY);
				if (currentValue == 'x')
				{
					j = ranges[i].size();
				}
				else
				{
					int currentRange;
					double currentRotation;
					SDL_RendererFlip currentFlip;

					currentRange = j == 0 ? 1 : 2; // Range
					currentFlip = i > 1 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL; // Flip
					currentRotation = i % 2 == 0 ? 0 : 90.0; // Rotation

					if (currentValue == 'p')
					{
						SetPlayerDead();
					}
					else if (currentValue == 'o')
					{
						currentRange = 2;
						j = ranges[i].size();
					}

					explosions.push_back(new Explosion(m_Graphics, currentX, currentY, currentRange, currentRotation, currentFlip));
					SetGridAt(currentX, currentY, 'e');
				}
			}
		}
	}
}

void Grid::DeleteExplosions()
{
	auto firstExplosion = explosions.begin();
	while (explosions.size() != 0)
	{
		if ((*firstExplosion)->GetTimeUp())
		{
			SetGridAt((*firstExplosion)->GetPosX(), (*firstExplosion)->GetPosY(), ' ');
			explosions.pop_front();
			firstExplosion = explosions.begin();
		}
		else
		{
			break;
		}
	}
}

void Grid::SetDrawDoor()
{
	m_DrawDoor = true;
}

void Grid::MovePlayer(vector<int> move)
{
	int playerCurrentX = m_Player->GetPosX();
	int playerCurrentY = m_Player->GetPosY();
	vector<int> playerCurrentPos = { playerCurrentX , playerCurrentY };
	vector<int> playerNextPos = { playerCurrentPos[0] + move[0], playerCurrentPos[1] + move[1] };
	char currentPlaceHolder = GetGridAt(playerNextPos[0], playerNextPos[1]);
	if (currentPlaceHolder != 'x' && currentPlaceHolder != 'o')
	{

		if (currentPlaceHolder == 'e' || currentPlaceHolder == 'g')
		{
			SetPlayerDead();
		}
		if (currentPlaceHolder == 'd')
		{
			m_Player->SetHasWon();
		}
		SetGridAt(playerCurrentX, playerCurrentY, ' ');

		SetGridAt(playerNextPos[0], playerNextPos[1], 'p');

		m_Player->SetPos(playerNextPos[0], playerNextPos[1]);
	}
}

void Grid::MoveEnemy(Enemy* enemy, vector<int> move)
{
	vector<int> enemyCurrentPos = enemy->GetPos();
	vector<int> enemyNextPos = { enemyCurrentPos[0] + move[0], enemyCurrentPos[1] + move[1] };
	const char currentPlaceHolder = GetGridAt(enemyCurrentPos[0], enemyCurrentPos[1]);
	const char nextPlaceHolder = GetGridAt(enemyNextPos[0], enemyNextPos[1]);
	if (nextPlaceHolder != 'x' && nextPlaceHolder != 'o')
	{
		if (nextPlaceHolder == 'e')
		{
			enemy->SetIsDead();
		}
		if (currentPlaceHolder != 'b')
		{
			SetGridAt(enemyCurrentPos[0], enemyCurrentPos[1], ' ');
		}
		SetGridAt(enemyNextPos[0], enemyNextPos[1], 'g');
		enemy->SetPos(enemyNextPos[0], enemyNextPos[1]);
	}
}

void Grid::SetPlayerDead()
{
	m_Player->SetIsDead();
}
