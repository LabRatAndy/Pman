#pragma once
#include "Tile.h"
#include "Player.h"
#include "Ghost.h"
#include "Maths.h"
#include <vector>
#include <string>

#include <raylib.h>
namespace Pman
{
	
	static std::vector<std::string> s_DefaultLevel =
	{
		" ################### ",
		" #........#........# ",
		" #o##.###.#.###.##o# ",
		" #.................# ",
		" #.##.#.#####.#.##.# ",
		" #....#...#...#....# ",
		" ####.### # ###.#### ",
		"    #.#   0   #.#    ",
		"#####.# ##=## #.#####",
		"     .  #123#  .     ",
		"#####.# ##### #.#####",
		"    #.#       #.#    ",
		" ####.# ##### #.#### ",
		" #........#........# ",
		" #.##.###.#.###.##.# ",
		" #o.#.....P.....#.o# ",
		" ##.#.#.#####.#.#.## ",
		" #....#...#...#....# ",
		" #.######.#.######.# ",
		" #.................# ",
		" ################### "  
	};
	class Level
	{
	public:
		Level(const uint32_t tilesize);
		~Level();

		void LoadLevel(const std::vector<std::string>& leveldata = s_DefaultLevel);

		void OnUpdate(float ts);
		void OnRender();

		void StartGame();

		//void Reset();

		uint32_t GetLevelWidthInTiles() const { return m_LevelWidth; }
		uint32_t GetLevelHeightInTiles() const { return m_LevelHeight; }
		uint32_t GetLevelTileSize() const { return m_TileSize; }
		//Absolute size getters return size in pixels
		uint32_t GetAbsoluteWidth() const { return m_LevelWidth * m_TileSize; }
		uint32_t GetAbsoluteHeight() const { return m_LevelHeight * m_TileSize; }

		bool CollideWithWall(const Vec2<uint32_t>& position, const Vec2<int32_t>& direction, bool canusedoor);
		bool CollectGem(const Vec2<float>& position);
		bool CollectPowerPellet(const Vec2<float>& position);
		bool CollideWithWall(const uint32_t& tilex, const uint32_t& tiley, const Vec2<int32_t>& direction, const bool& canusedoor);
		bool IsTileAWall(const float& x, const float& y,const Vec2<int32_t>& direction, const bool canusedoor) const;

		const std::array<int32_t, 4>& GetAdjacentTileList(size_t tile) const { return m_Tiles[tile].GetAdjacentTile(); }
		const Tile& GetTile(const size_t index) const { return m_Tiles[index]; }

		Vec2<float> GetPacmanPosition() const
		{
			if (m_Player == nullptr)
			{
				return { 0.0f,0.0f };
			}
			else
				return m_Player->GetPosition();
		}
		Vec2<int32_t> GetPacmanDirection() const
		{
			if (m_Player == nullptr)
			{
				return { 0,0 };
			}
			else
				return m_Player->GetDirection();
		}
		Vec2<float> GetRedGhostPosition() const
		{
			if (m_RedGhost == nullptr)
			{
				return { 0.0f,0.0f };
			}
			else
				return m_RedGhost->GetPosition();
		}

	private:
		//height and width in tiles
		uint32_t m_LevelWidth = 0;
		uint32_t m_LevelHeight = 0;
		//tile size in pixels they are always square  ie 32 is a tile 32px * 32 px.
		uint32_t m_TileSize = 0;

		//tile vector
		std::vector<Tile> m_Tiles;

		//collison rectange vectors
		std::vector<Rectangle> m_WallsRectangles;
		std::vector<Rectangle> m_GemsRectangles;
		std::vector<Rectangle> m_PowerPelletRectangles;
		std::vector<Rectangle> m_DoorRectangles;

		void ProcessRectangles();
		void ProcessAdjacentTiles();

		//ghosts and player objects
		Player* m_Player = nullptr;
		Ghost* m_RedGhost = nullptr;
		Ghost* m_CyanGhost = nullptr;
		Ghost* m_OrangeGhost = nullptr;
		Ghost* m_PinkGhost = nullptr;

		//Sprites 
		Sprite* m_RedGhostSprite = nullptr;
		Sprite* m_CyanGhostSprite = nullptr;
		Sprite* m_OrangeGhostSprite = nullptr;
		Sprite* m_PinkGhostSprite = nullptr;
		Sprite* m_BlueGhostSprite = nullptr;
		Sprite* m_GhostEyesSprite = nullptr;
		Sprite* m_PlayerSprite = nullptr;
		Sprite* m_GemSprite = nullptr;
		Sprite* m_PowerPelletSprite = nullptr;



	};

}