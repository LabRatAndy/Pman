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
		Renderer(uint32_t width, uint32_t height,uint32_t textstartheight);
		~Renderer();

		void ResetSprites();

		void Initialise(uint32_t FPS);
		void Clear(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
		void BeginFrame();
		void EndFrame();

		Sprite* LoadSprite(const std::filesystem::path& filename, SpriteType type);

		void RenderTile(const Tile& tile);
		void RenderSprite(const Sprite* sprite, uint32_t xpos, uint32_t ypos, uint32_t tilesize);
		void RenderRectangle(uint32_t xpos, uint32_t ypos, uint32_t tilesize, Vec4<uint8_t> colour);
		void RenderRectangle(uint32_t xpos, uint32_t ypos, uint32_t width, uint32_t height, Vec4<uint8_t> colour);
		void RenderTest();
		void RenderGridLines(uint32_t tilesize);
		void RenderScore(const uint32_t score);
		void RenderPlayerLives(const uint32_t playerlives);
		void RenderText(const std::string& text, uint32_t xpos, uint32_t ypos, float size, float spacing, Vec4<uint8_t> colour);

		Vec2<int32_t> MeasureText(const std::string& text, float size, float spacing) const;

	private:
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_TextStartHeight = 0;

		SpriteList* m_Sprites = nullptr;
		uint32_t m_SpriteCount = 0;
	};
}