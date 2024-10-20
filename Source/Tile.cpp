#include "Tile.h"

namespace Pman
{
	Tile::Tile(const TileSpecification& spec) : m_TileSize(spec.TileSize), m_Type(spec.Type), m_XPosition(spec.XPosition), m_YPosition(spec.YPosition), m_Sprite(spec.Texture)
	{

	}
	Tile::~Tile()
	{
		//spites are cleaned up by the renderer!
	}

	void Tile::OnRender()
	{
		//TODO
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
	void Tile::SetAdjacentTile(const int32_t first, const int32_t second, const int32_t third, const int32_t fourth)
	{
		m_AdjacentTiles[0] = first;
		m_AdjacentTiles[1] = second;
		m_AdjacentTiles[2] = third;
		m_AdjacentTiles[3] = fourth;
	}
}