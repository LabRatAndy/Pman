#include  "utills.h"
#include "Maths.h"
#include "Assert.h"

#include <cinttypes>
#include <algorithm>

namespace Pman
{
	size_t GetTileArrayIndexofTile(uint32_t x, uint32_t y, uint32_t levelwidth)
	{
		return (y * levelwidth) + x;
	}
	uint32_t GetTileIndex(const Vec2<int32_t>& tile, uint32_t levelwidth)
	{
		ASSERT((tile.X >= 0), "Invalid tile position");
		ASSERT((tile.Y >= 0), "Invalid tile position");
		return (tile.Y * levelwidth) + tile.X;
	}
	void SetTileCoordsFromOriginAndPixelPosition(Vec2<int32_t>& tile, const Vec2<int32_t>& pixelposition, const uint32_t tilesize)
	{
		int32_t x = static_cast<int32_t>(std::floor((pixelposition.X + tilesize / 2) / tilesize));
		int32_t y = static_cast<int32_t>(std::floor((pixelposition.Y + tilesize / 2) / tilesize));
		x = std::clamp(x, 0, 20);	//ensure that we don't go out of bounds 
		y = std::clamp(y, 0, 20);
		tile.X = x;
		tile.Y = y;
	}
}