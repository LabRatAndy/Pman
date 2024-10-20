#pragma once
#include "Sprite.h"
#include "Maths.h"
#include <filesystem>

namespace Pman
{
	//fwd dec
	struct SpriteList;
	class Tile;

	constexpr uint32_t MAX_SPTITES = 10;

	class Renderer
	{
	public:
		Renderer(uint32_t width, uint32_t height);
		~Renderer();

		void Initialise(uint32_t FPS);
		void Clear(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
		void BeginFrame();
		void EndFrame();

		Sprite* LoadSprite(const std::filesystem::path& filename, SpriteType type);

		void RenderTile(const Tile& tile);
		void RenderSprite(const Sprite* sprite, uint32_t xpos, uint32_t ypos, uint32_t tilesize);
		void RenderRectangle(uint32_t xpos, uint32_t ypos, uint32_t tilesize, Vec4<uint8_t> colour);
		void RenderTest();
		void RenderGridLines(uint32_t tilesize);

	private:
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		SpriteList* m_Sprites = nullptr;
		uint32_t m_SpriteCount = 0;
	};
}