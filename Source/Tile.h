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
		uint32_t XPosition = 0; // in pixels
		uint32_t YPosition = 0;	//in pixels
		uint32_t TileSize = 0;
		Sprite* Texture = nullptr;
	};

	class Tile
	{
	public:
		Tile(const TileSpecification& spec);
		~Tile();

		void OnRender();

		void CollectGem();
		void CollectPowerPellet();

		uint32_t GetTileXPosition() const { return m_XPosition / m_TileSize; }
		uint32_t GetTileYPosition() const { return m_YPosition / m_TileSize; }

		uint32_t GetAbsoluteXPosition() const { return m_XPosition; }
		uint32_t GetAbsoluteYPosition() const { return m_YPosition; }

		uint32_t GetTileSize() const { return m_TileSize; }

		TileType GetTileType() const { return m_Type; }
		Sprite* GetSprite() const { return m_Sprite; }

		const std::array<int32_t, 4>& GetAdjacentTile() const { return m_AdjacentTiles; }
		void SetAdjacentTile(const int32_t first, const int32_t second, const int32_t third, const int32_t fourth);

	private:
		TileType m_Type = TileType::Invaild;
		uint32_t m_XPosition = 0;
		uint32_t m_YPosition = 0;
		uint32_t m_TileSize = 0;
		Sprite* m_Sprite = nullptr;
		std::array<int32_t, 4> m_AdjacentTiles = { -1,-1,-1,-1 };

	};
}