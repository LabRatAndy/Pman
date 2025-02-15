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

#include "Tile.h"

namespace Pman
{
	Tile::Tile(const TileSpecification& spec_) : m_TileSize(spec_.TileSize), m_Type(spec_.Type), m_XPosition(spec_.XPosition), m_YPosition(spec_.YPosition), m_Sprite(spec_.Texture)
	{

	}

	void Tile::CollectGem()
	{
		if (m_Type != TileType::Gem)
		{
			return;
		}
		m_Type = TileType::Empty;
	}
	void Tile::CollectPowerPellet()
	{
		if (m_Type != TileType::PowerPellet)
			return;
		m_Type = TileType::Empty;
	}
	void Tile::SetAdjacentTile(const int32_t first_, const int32_t second_, const int32_t third_, const int32_t fourth_)
	{
		m_AdjacentTiles[0] = first_;
		m_AdjacentTiles[1] = second_;
		m_AdjacentTiles[2] = third_;
		m_AdjacentTiles[3] = fourth_;
	}
}