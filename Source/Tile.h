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
#include "Sprite.h"
#include <cinttypes>
#include <array>

namespace Pman
{
	enum class TileType
	{
		Invaild = 0,
		Empty,
		Door,
		Wall,
		Gem,
		PowerPellet
	};
	struct TileSpecification
	{
		TileType Type = TileType::Invaild;
		int32_t XPosition = 0; // Grid position where 0,0 is top left corner tile
		int32_t YPosition = 0;	// Grid position where 0,0 is top left corner tile
		int32_t TileSize = 0;
		Sprite* Texture = nullptr;
	};

	class Tile
	{
	public:
		Tile(const TileSpecification& spec_);
		~Tile() = default; //spites are cleaned up by the renderer!

		void CollectGem();
		void CollectPowerPellet();

		int32_t GetTileXPosition() const { return m_XPosition; }
		int32_t GetTileYPosition() const { return m_YPosition; }

		int32_t GetAbsoluteXPosition() const { return m_XPosition * m_TileSize; }
		int32_t GetAbsoluteYPosition() const { return m_YPosition * m_TileSize; }

		int32_t GetTileSize() const { return m_TileSize; }

		TileType GetTileType() const { return m_Type; }
		Sprite* GetSprite() const { return m_Sprite; }

		const std::array<int32_t, 4>& GetAdjacentTile() const { return m_AdjacentTiles; }
		void SetAdjacentTile(const int32_t first_, const int32_t second_, const int32_t third_, const int32_t fourth_);

	private:
		TileType m_Type = TileType::Invaild;
		int32_t m_XPosition = 0;
		int32_t m_YPosition = 0;
		int32_t m_TileSize = 0;
		Sprite* m_Sprite = nullptr;
		std::array<int32_t, 4> m_AdjacentTiles = { { -1,-1, -1,-1 } };

	};
}