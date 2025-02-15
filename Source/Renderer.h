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
#include "Maths.h"

#include <filesystem>
#include <string>

namespace Pman
{
	//fwd dec
	struct SpriteList;
	class Tile;

	constexpr uint32_t MAX_SPTITES = 10;

	class Renderer
	{
	public:
		Renderer(uint32_t width_, uint32_t height_,uint32_t text_start_height);
		~Renderer();

		void ResetSprites();

		void Initialise(uint32_t FPS_);
		void Clear(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t alpha_);
		void BeginFrame();
		void EndFrame();

		Sprite* LoadSprite(const std::filesystem::path& filename_, SpriteType type_);

		void RenderTile(const Tile& tile_);
		void RenderSprite(const Sprite* sprite_, uint32_t x_pos, uint32_t y_pos, uint32_t tile_size);
		void RenderRectangle(uint32_t x_pos, uint32_t y_pos, uint32_t tile_size, Vec4<uint8_t> colour_);
		void RenderRectangle(uint32_t x_pos, uint32_t y_pos, uint32_t width_, uint32_t height_, Vec4<uint8_t> colour_);
		void RenderGridLines(uint32_t tile_size);
		void RenderScore(const uint32_t score_);
		void RenderPlayerLives(const uint32_t player_lives);
		void RenderText(const std::string& text_, uint32_t x_pos, uint32_t y_pos, float size_, float spacing_, Vec4<uint8_t> colour_);

		Vec2<int32_t> MeasureText(const std::string& text_, float size_, float spacing_) const;

	private:
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_TextStartHeight = 0;

		SpriteList* m_Sprites = nullptr;
		uint32_t m_SpriteCount = 0;
	};
}