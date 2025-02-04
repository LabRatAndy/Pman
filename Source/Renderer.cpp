#include "Renderer.h"
#include "SpriteList.h"
#include "Tile.h"
#include "Assert.h"
#include <raylib.h>
#include <iostream>
#include <string>
#include <format>

namespace Pman
{
	Renderer::Renderer(uint32_t width, uint32_t height, uint32_t textstartheight) : m_Width(width), m_Height(height), m_TextStartHeight(textstartheight)
	{
		m_Sprites = new SpriteList(MAX_SPTITES);
		m_SpriteCount = 0;
	}
	Renderer::~Renderer()
	{
		delete m_Sprites;
	}
	void Renderer::Initialise(uint32_t FPS)
	{
		SetTargetFPS(FPS);
	}
	void Renderer::RenderTile(const Tile& tile)
	{
		switch (tile.GetTileType())
		{
		case TileType::Door:
			RenderRectangle(tile.GetAbsoluteXPosition(), tile.GetAbsoluteYPosition(), tile.GetTileSize(), { 255,0,255,255 });
			break;
		case TileType::Wall:
			RenderRectangle(tile.GetAbsoluteXPosition(), tile.GetAbsoluteYPosition(), tile.GetTileSize(), { 0,121,241,255 });
			break;
		case TileType::Gem:
		case TileType::PowerPellet:
			RenderSprite(tile.GetSprite(), tile.GetAbsoluteXPosition(), tile.GetAbsoluteYPosition(), tile.GetTileSize());
			break;
		}
	}
	void Renderer::RenderSprite(const Sprite* sprite, uint32_t xpos, uint32_t ypos, uint32_t tilesize)
	{
		ASSERT(sprite, "Sprite cannot be nullptr");
		Texture2D& texture = m_Sprites->Sprites[sprite->SpriteID];
		Rectangle source{ 0.0f,0.0f,static_cast<float>(texture.width),static_cast<float>(texture.height) };
		Rectangle dest{ static_cast<float>(xpos + tilesize / 2),static_cast<float>(ypos + tilesize / 2),static_cast<float>(texture.width),static_cast<float>(texture.height) };
		Vector2 origin{ dest.width / 2,dest.height / 2 };
		DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
		
	}
	void Renderer::RenderRectangle(uint32_t xpos, uint32_t ypos, uint32_t tilesize, Vec4<uint8_t> colour)
	{
		DrawRectangle(xpos, ypos, tilesize, tilesize, { colour.X,colour.Y,colour.Z,colour.W });
	}
	void Renderer::RenderRectangle(uint32_t xpos, uint32_t ypos, uint32_t width, uint32_t height, Vec4<uint8_t> colour)
	{
		DrawRectangle(xpos, ypos, width, height, { colour.X,colour.Y,colour.Z,colour.W });
	}
	void Renderer::RenderTest()
	{
		DrawText("Hello", 10, 10, 20, RED);
	}
	void Renderer::Clear(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{
		ClearBackground({ red,green,blue,alpha });
	}
	void Renderer::BeginFrame()
	{
		BeginDrawing();
	}
	void Renderer::EndFrame()
	{
		EndDrawing();
	}
	Sprite* Renderer::LoadSprite(const std::filesystem::path& filename, SpriteType type)
	{
		if (!std::filesystem::exists(filename))
		{
			std::string strfilename = filename.generic_string();
			WARN("The sprite file {} does not exist", strfilename.c_str());;
			return nullptr;
		}
		if (m_SpriteCount > MAX_SPTITES)
		{
			std::string strfilename = filename.generic_string();
			WARN("Maximum number of sprites, was exceeded by sprite file {}", strfilename.c_str());
			return nullptr;
		}
		if (type == SpriteType::None)
		{
			WARN("Invalid sprite type given!");
			return nullptr;
		}
		std::string pathstring = filename.string();
		Image image = LoadImage(pathstring.c_str());
		size_t buffersize = image.width * image.height;
		uint32_t* imagebuffer = new uint32_t[buffersize];
		uint8_t* imagedata = (uint8_t*)image.data;
		if (image.format == PIXELFORMAT_UNCOMPRESSED_R8G8B8A8)
		{
			//RGBA
			for (size_t i = 0; i < buffersize; i++)
			{
				uint8_t r = imagedata[i * 4 + 0];
				uint8_t g = imagedata[i * 4 + 1];
				uint8_t b = imagedata[i * 4 + 2];
				uint8_t a = imagedata[i * 4 + 3];
				if (r == 255 && g == 0 && b == 255)
				{
					a = 0;
				}
				imagebuffer[i] = a << 24 | b << 16 | g << 8 | r;
			}
		}
		else if (image.format == PIXELFORMAT_UNCOMPRESSED_R8G8B8)
		{
			//RGB
			for (size_t i = 0; i < buffersize; i++)
			{
				uint8_t r = imagedata[i * 3 + 0];
				uint8_t g = imagedata[i * 3 + 1];
				uint8_t b = imagedata[i * 3 + 2];
				uint8_t a = 255;
				if (r == 255 && g == 0 && b == 255)
				{
					a = 0;
				}
				imagebuffer[i] = a << 24 | b << 16 | g << 8 | r;
			}
		}
		else
		{
			// unsupported format
			WARN("unsupported sprite texture file format");
			UnloadImage(image);
			delete[] imagebuffer;
			return nullptr;
		}
		Sprite* sprite = new Sprite;
		//need to initialise the texture in sprite list before we update the texture to have sprite in it
		m_Sprites->InitialiseSprite(image.width, image.height, m_SpriteCount);
		UpdateTexture(m_Sprites->Sprites[m_SpriteCount], imagebuffer);
		sprite->SpriteID = m_SpriteCount;
		sprite->Type = type;
		sprite->Width = image.width;
		sprite->Height = image.height;
		m_SpriteCount++;
		//clean up
		UnloadImage(image);
		delete[]imagebuffer;
		return sprite;
	}
	void Renderer::RenderGridLines(uint32_t tilesize)
	{
		uint32_t width = m_Width / tilesize;
		uint32_t height = m_Height / tilesize;
		for (uint32_t y = 0; y < height; y++)
		{
			for (uint32_t x = 0; x < width; x++)
			{
				Rectangle rec{ static_cast<float>(x),static_cast<float>(y),static_cast<float>(tilesize),static_cast<float>(tilesize) };
				DrawRectangleLinesEx(rec, 1.0f, WHITE);
			}
		}
	}
	void Renderer::RenderScore(const uint32_t score)
	{
		Font font = GetFontDefault();
		std::string scoretext = std::format("Score: {}", score);
		float xpos = m_Width * 0.33f;
		Vector2 textsize = MeasureTextEx(font, scoretext.c_str(), 30.0f, 2.5f);
		Vector2 position{ xpos,static_cast<float>(m_TextStartHeight) };
		Vector2 origin{ (textsize.x / 2),0.0f };
		DrawTextPro(font, scoretext.c_str(), position, origin, 0.0f, 30.0f, 2.5f, WHITE);
	}
	void Renderer::RenderPlayerLives(const uint32_t playerlives)
	{
		Font font = GetFontDefault();
		std::string scoretext = std::format("Lives: {}", playerlives);
		float xpos = m_Width * 0.66f;
		Vector2 textsize = MeasureTextEx(font, scoretext.c_str(), 30.0f, 2.5f);
		Vector2 position{ xpos, static_cast<float>(m_TextStartHeight) };
		Vector2 origin{ (textsize.x / 2),0.0f };
		DrawTextPro(font, scoretext.c_str(), position, origin, 0.0f, 30.0f, 2.5f, WHITE);
	}
	void Renderer::RenderText(const std::string& text, uint32_t xpos, uint32_t ypos, float size, float spacing, Vec4<uint8_t> colour)
	{
		Font font = GetFontDefault();
		Vector2 position{ static_cast<float>(xpos), static_cast<float>(ypos) };
		Vector2 textsize = MeasureTextEx(font, text.c_str(), size, spacing);
		Vector2 origin{ textsize.x / 2,textsize.y / 2 };
		DrawTextPro(font, text.c_str(), position, origin, 0.0f, size, spacing, { colour.X,colour.Y,colour.Z, colour.W });
	}
}