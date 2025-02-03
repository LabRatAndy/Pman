#pragma once
#include "Assert.h"
#include "Maths.h"
#include <cinttypes>
namespace Pman
{

	size_t GetTileArrayIndexofTile(uint32_t x, uint32_t y, uint32_t levelwidth);

	uint32_t GetTileIndex(const Vec2<int32_t>& tile, uint32_t levelwidth);

	void SetTileCoordsFromOriginAndPixelPosition(Vec2<int32_t>& tile, const Vec2<int32_t>& pixelposition, const uint32_t tilesize);


}