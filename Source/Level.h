//MIT License
//
//LabRatMan Copyright(c) 2025 Andrew Dawson
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#pragma once
#include "Tile.h"
#include "Player.h"
#include "Ghost.h"
#include "Maths.h"
#include <vector>
#include <string>

namespace Pman
{
	
	static std::vector<std::string> s_DefaultLevel =
	{
		"i###################i",
		"i#........#........#i",
		"i#o##.###.#.###.##o#i",
		"i#.................#i",
		"i#.##.#.#####.#.##.#i",
		"i#....#...#...#....#i",
		"i####.### # ###.####i",
		"iiii#.#   0   #.#iiii",
		"#####.# ##=## #.#####",
		"     .  #123#  .     ",
		"#####.# ##### #.#####",
		"iiii#.#       #.#iiii",
		"i####.# ##### #.####i",
		"i#........#........#i",
		"i#.##.###.#.###.##.#i",
		"i#o.#.....P.....#.o#i",
		"i##.#.#.#####.#.#.##i",
		"i#....#...#...#....#i",
		"i#.######.#.######.#i",
		"i#.................#i",
		"i###################i"  
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

		bool IsGameOver() const;

		int32_t GetLevelWidthInTiles() const { return m_LevelWidth; }
		int32_t GetLevelHeightInTiles() const { return m_LevelHeight; }
		int32_t GetLevelTileSize() const { return m_TileSize; }
		//Absolute size getters return size in pixels
		int32_t GetAbsoluteWidth() const { return m_LevelWidth * m_TileSize; }
		int32_t GetAbsoluteHeight() const { return m_LevelHeight * m_TileSize; }

		// these will likely need to be reviewed
		bool CollideWithWall(const Vec2<int32_t>& position, const Vec2<int32_t>& direction) const;
		void CollideWithGhost(const Vec2<int32_t>& position) const;
		bool CollectGem(const Vec2<int32_t>& position);
		bool CollectPowerPellet(const Vec2<int32_t>& position);
		

		const std::array<int32_t, 4>& GetAdjacentTileList(size_t tile) const { return m_Tiles[tile].GetAdjacentTile(); }
		const Tile& GetTile(const size_t index) const { return m_Tiles[index]; }

		Vec2<int32_t> GetPacmanPosition() const
		{
			if (m_Player == nullptr)
			{
				return { -1,-1 };
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
		Vec2<int32_t> GetRedGhostPosition() const
		{
			if (m_RedGhost == nullptr)
			{
				return { -1,-1 };
			}
			else
				return m_RedGhost->GetPosition();
		}

	private:
		//constants
		const size_t TOP_LEFT = 0;
		const size_t BOTTOM_LEFT = 1;
		const size_t TOP_RIGHT = 2;
		const size_t BOTTOM_RIGHT = 3;

		//height and width in tiles
		int32_t m_LevelWidth = 0;
		int32_t m_LevelHeight = 0;
		//tile size in pixels they are always square  ie 32 is a tile 32px * 32 px.
		int32_t m_TileSize = 0;

		//tile vector
		std::vector<Tile> m_Tiles;

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