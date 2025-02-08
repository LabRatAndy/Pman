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