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

#include "Application.h"
#include"Window.h"
#include"Renderer.h"
#include "Level.h"
#include "Assert.h"

#include <algorithm>

namespace Pman
{
	constexpr float FONTSIZE = 40.0f;
	constexpr float FONTSPACING = 2.5f;
	constexpr uint32_t TEXTBOXPADDING = 5;
	constexpr Vec4<uint8_t> WHITE = { 255,255,255,255 };
	constexpr Vec4<uint8_t> RED = { 250,50,50,255 };
	constexpr Vec4<uint8_t> REDTRANSPARENT = { 250,50,50,175 };

	Application::Application(const ApplicationSpecification& spec_) : m_Specification(spec_), m_Renderer(new Renderer(spec_.TileSize* spec_.TilesWide, spec_.TileSize* (spec_.TilesHigh + spec_.TextTilesHigh), spec_.TileSize* spec_.TilesHigh))
	{
		s_Instance = this;
	}
	Application::~Application()
	{
		delete m_Level;
		delete m_Renderer;
		delete m_Window;
	}
	/// <summary>
	/// contains the main game loop code
	/// </summary>
	void Application::Run()
	{
		WindowSpecification winspec{};
		winspec.Height = (m_Specification.TilesHigh + m_Specification.TextTilesHigh) * m_Specification.TileSize;
		winspec.Width = m_Specification.TilesWide * m_Specification.TileSize;
		winspec.WindowTitle = m_Specification.WindowTitle;
		m_Window = new Window(winspec);
		m_Renderer->Initialise(60);
		ResetLevel();
		//main game loop
		float timedelta = 0.0f;
		while (!m_Window->ShouldWindowClose())
		{
			//temporary needs to be finessed!
			timedelta = m_Window->GetTimeDelta();
			if (m_Level->IsGameOver())
			{
				DrawGameOver();
				if (m_Window->GetKeyPressed(Key::ENTER))
				{
					ResetLevel();
				}
			}
			else
			{
				//update game/level
				if (!m_Paused)
				{
					m_Level->OnUpdate(timedelta);
				}

				m_Renderer->BeginFrame();
				m_Renderer->Clear(0, 0, 0, 0);
				//render game/level
				m_Level->OnRender();
				if (!m_Started)
				{
					DrawStartScreen();
					if (m_Window->GetKeyPressed(Key::SPACE))
					{
						m_Level->StartGame();
						m_Started = true;
					}
				}
				else
				{
					if (m_Paused && m_Window->GetKeyPressed(Key::P))
					{
						m_Paused = false;
					}
					else if (!m_Paused && m_Window->GetKeyPressed(Key::P))
					{
						m_Paused = true;
					}
				}
				if (m_Paused)
				{
					DrawPausedScreen();
				}

				m_Renderer->EndFrame();
			}
		}
	}
	void Application::DrawGameOver() const
	{
		Vec2<int32_t> gameoversize = m_Renderer->MeasureText("Game over!", FONTSIZE, FONTSPACING);
		Vec2<int32_t> pressspacesize = m_Renderer->MeasureText("Press Enter to play again!", FONTSIZE, FONTSPACING);
		uint32_t height = gameoversize.Y + pressspacesize.Y + (TEXTBOXPADDING * 2);
		uint32_t width = (gameoversize.X > pressspacesize.X ? gameoversize.X : pressspacesize.X) + (TEXTBOXPADDING * 2);
		uint32_t xpos = (m_Level->GetAbsoluteWidth() - width) / 2;
		uint32_t ypos = (m_Level->GetAbsoluteHeight() - height) / 2;
		m_Renderer->BeginFrame();
		m_Renderer->Clear(0, 0, 0, 0);
		m_Renderer->RenderRectangle(xpos, ypos, width, height, RED);
		xpos = xpos + TEXTBOXPADDING;
		ypos = ypos + TEXTBOXPADDING;
		m_Renderer->RenderText("Game over!", xpos, ypos, FONTSIZE, FONTSPACING, WHITE);
		ypos = ypos + gameoversize.Y;
		m_Renderer->RenderText("Press Enter to play again!", xpos, ypos, FONTSIZE, FONTSPACING, WHITE);
		m_Renderer->EndFrame();
	}
	void Application::ResetLevel()
	{
		m_Renderer->ResetSprites();
		delete m_Level;
		m_Level = new Level(m_Specification.TileSize);
		m_Level->LoadLevel();
		uint32_t winheight = m_Level->GetAbsoluteHeight() + (m_Specification.TextTilesHigh * m_Specification.TileSize);
		m_Window->ChangeWindowSize(m_Level->GetAbsoluteWidth(), winheight);
		m_Started = false;
		m_Paused = false;
	}
	void Application::DrawStartScreen() const
	{
		Vec2<int32_t> textsizeln1 = m_Renderer->MeasureText("Welcome to LabRatMan", FONTSIZE, FONTSPACING);
		Vec2<int32_t> textsizeln2 = m_Renderer->MeasureText("Press space to start game", FONTSIZE, FONTSPACING);
		Vec2<int32_t> textsizeln3 = m_Renderer->MeasureText("Press P to pause, Esc to quit", FONTSIZE, FONTSPACING);
		uint32_t height = textsizeln1.Y + textsizeln2.Y + textsizeln3.Y + (TEXTBOXPADDING * 2);
		uint32_t width = std::max({ textsizeln1.X,textsizeln2.X,textsizeln3.X });
		width = width + (TEXTBOXPADDING * 2);
		uint32_t xpos = (m_Level->GetAbsoluteWidth() - width) / 2;
		uint32_t ypos = (m_Level->GetAbsoluteHeight() - height) / 2;
		m_Renderer->RenderRectangle(xpos, ypos, width, height, REDTRANSPARENT);
		xpos = xpos + TEXTBOXPADDING;
		ypos = ypos + TEXTBOXPADDING;
		m_Renderer->RenderText("Welcome to LabRatMan", xpos, ypos, FONTSIZE, FONTSPACING, WHITE);
		ypos = ypos + textsizeln1.Y;
		m_Renderer->RenderText("Press space to start game", xpos, ypos, FONTSIZE, FONTSPACING, WHITE);
		ypos = ypos + textsizeln2.Y;
		m_Renderer->RenderText("Press P to pause, Esc to quit", xpos, ypos, FONTSIZE, FONTSPACING, WHITE);
	}
	void Application::DrawPausedScreen() const
	{
		Vec2<int32_t> line1size = m_Renderer->MeasureText("Paused", FONTSIZE, FONTSPACING);
		Vec2<int32_t> line2size = m_Renderer->MeasureText("Press P to continue", FONTSIZE, FONTSPACING);
		uint32_t height = line1size.Y + line2size.Y + (TEXTBOXPADDING * 2);
		uint32_t width = std::max({ line1size.X,line2size.X }) + (TEXTBOXPADDING * 2);
		uint32_t xpos = (m_Level->GetAbsoluteWidth() - width) / 2;
		uint32_t ypos = (m_Level->GetAbsoluteHeight() - height) / 2;
		m_Renderer->RenderRectangle(xpos, ypos, width, height, REDTRANSPARENT);
		xpos = xpos + TEXTBOXPADDING;
		ypos = ypos + TEXTBOXPADDING;
		m_Renderer->RenderText("Paused", xpos, ypos, FONTSIZE, FONTSPACING, WHITE);
		ypos = ypos + line1size.Y;
		m_Renderer->RenderText("Press P to continue", xpos, ypos, FONTSIZE, FONTSPACING, WHITE);
	}
}