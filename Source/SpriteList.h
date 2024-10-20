#pragma once
#include <raylib.h>
namespace Pman
{
	struct SpriteList
	{
		Texture2D* Sprites = nullptr;

		SpriteList(size_t maxsprites)
		{
			Sprites = new Texture2D[maxsprites];
		}
		~SpriteList()
		{
			delete[] Sprites;
		}
		Texture2D& operator[](const size_t index)
		{
			return Sprites[index];
		}
		void InitialiseSprite(const uint32_t width, const uint32_t height, const size_t index)
		{
			Image image = GenImageColor(width, height, WHITE);
			Sprites[index] = LoadTextureFromImage(image);
		}
	};
}
