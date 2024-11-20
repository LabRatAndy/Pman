#pragma once
#include "Assert.h"
#include <cinttypes>
namespace Pman
{
	static size_t GetTileArrayIndexofTile(uint32_t x, uint32_t y, uint32_t levelwidth)
	{
		return (y * levelwidth) + x;
	}
	static Vec2<int32_t> RoundFloatPostionToUint32(const Vec2<float>& position, uint32_t tilesize)
	{
		int32_t x = static_cast<int32_t>(std::floor(position.X / tilesize));
		int32_t y = static_cast<int32_t>(std::floor(position.Y / tilesize));
		return { x,y };
	}
	static uint32_t ConvertPixelPosToTilePosition(float position, uint32_t tilesize)
	{
		return std::floor(position / tilesize);
	}
	static uint32_t InTileIndex(float position, uint32_t tilesize, float& leftover)
	{
		leftover = std::fmod(position, (float)tilesize);
		position = position - leftover;
		return (uint32_t)(position / tilesize);
	}
	static float PortionInTile(float position, uint32_t tilesize)
	{
		return std::fmod(position, (float)tilesize);
	}
	static uint32_t GetTileIndex(const Vec2<int32_t>& tile, uint32_t levelwidth)
	{
		ASSERT((tile.X >= 0), "Invalid tile position");
		ASSERT((tile.Y >= 0), "Invalid tile position");
		return (tile.Y * levelwidth) + tile.X;
	}
	static uint32_t GetTileIndexFromPosition(const Vec2<int32_t>& position, const uint32_t& tilesize, const uint32_t& levelwidth)
	{
		/*float leftoverx = 0.0f;
		float leftovery = 0.0f;
		uint32_t tilex = InTileIndex(position.X, tilesize, leftoverx);
		uint32_t tiley = InTileIndex(position.Y, tilesize, leftovery);*/
		uint32_t tilex = ConvertPixelPosToTilePosition(position.X, tilesize);
		uint32_t tiley = ConvertPixelPosToTilePosition(position.Y, tilesize);
		return (tiley * levelwidth) + tilex;
	}
}