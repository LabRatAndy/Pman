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
	size_t GetTileArrayIndexofTile(uint32_t x_, uint32_t y_, uint32_t level_width)
	{
		return (y_ * level_width) + x_;
	}
	uint32_t GetTileIndex(const Vec2<int32_t>& tile_, uint32_t level_width)
	{
		ASSERT((tile_.X >= 0), "Invalid tile position");
		ASSERT((tile_.Y >= 0), "Invalid tile position");
		return (tile_.Y * level_width) + tile_.X;
	}
	void SetTileCoordsFromOriginAndPixelPosition(Vec2<int32_t>& tile_, const Vec2<int32_t>& pixel_position, const uint32_t tile_size)
	{
		int32_t x = static_cast<int32_t>(std::floor((pixel_position.X + tile_size / 2) / tile_size));
		int32_t y = static_cast<int32_t>(std::floor((pixel_position.Y + tile_size / 2) / tile_size));
		x = std::clamp(x, 0, 20);	//ensure that we don't go out of bounds 
		y = std::clamp(y, 0, 20);
		tile_.X = x;
		tile_.Y = y;
	}
}