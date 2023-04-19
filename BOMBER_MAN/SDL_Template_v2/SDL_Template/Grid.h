#pragma once
#include "Player.h"




class Grid
{
public:

	Grid(SDL_Renderer* graphics, Player* player, list<Enemy*> enemiesList);
	~Grid();

	void Draw(SDL_Renderer* graphics);

	void Update(float dt);

	char GetGridAt(int x, int y);

	void SetGridAt(int x, int y, char val);
	void SetGridAt(int x, int y, char val, bool print);

	void CreateBomb(int x, int y);

	vector<vector<int>> GetEnemyPos() const { return enemiesPos; }

	void SetDrawDoor();

	void MovePlayer(vector<int> move);

	void MoveEnemy(Enemy* enemy, vector<int> move);

	void SetPlayerDead();

private:

	SDL_Renderer* m_Graphics;

	Player* m_Player;

	list<Enemy*> enemies;

	list<Block*> blocks;

	list<SoftWall*> softWalls;

	vector<std::vector<char>> m_Grid;

	list<Bomb*> bombs;

	list<Explosion*> explosions;

	vector<vector<int>> enemiesPos;

	void DeleteBomb(int x, int y);

	void CreateExplosion(int x, int y);

	void DeleteExplosions();

	Sprite* m_Door;

	bool m_DrawDoor;

};
