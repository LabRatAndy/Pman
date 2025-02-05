#include "Application.h"
#include"Window.h"
#include"Renderer.h"
#include "Level.h"
#include "Assert.h"

#include <algorithm>

namespace Pman
{
	Application::Application(const ApplicationSpecification& spec) : m_Specification(spec), m_Renderer(new Renderer(spec.TileSize* spec.TilesWide, spec.TileSize* (spec.TilesHigh + spec.TextTilesHigh), spec.TileSize* spec.TilesHigh))
	{
		s_Instance = this;
	}
	Application::~Application()
	{
		delete m_Level;
		delete m_Renderer;
		delete m_Window;
	}
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
			static bool started = false;
			static bool paused = false;
			timedelta = m_Window->GetTimeDelta();
			if (m_Level->IsGameOver())
			{
				DrawGameOver();
				if (m_Window->GetKeyPressed(Key::SPACE))
				{
					ResetLevel();
				}
			}
			else
			{
				//update game/level
				if (!paused)
				{
					m_Level->OnUpdate(timedelta);
				}

				m_Renderer->BeginFrame();
				m_Renderer->Clear(0, 0, 0, 0);
				//render game/level
				m_Level->OnRender();
				if (!started)
				{
					DrawStartScreen();
					if (m_Window->GetKeyPressed(Key::SPACE))
					{
						m_Level->StartGame();
						started = true;
					}
				}
				else
				{
					if (paused && m_Window->GetKeyPressed(Key::P))
					{
						paused = false;
					}
					else if (!paused && m_Window->GetKeyPressed(Key::P))
					{
						paused = true;
					}
				}

				m_Renderer->EndFrame();
			}
		}
	}
	void Application::DrawGameOver()
	{
		Vec2<int32_t> gameoversize = m_Renderer->MeasureText("Game over!", 40.0f, 2.5f);
		Vec2<int32_t> pressspacesize = m_Renderer->MeasureText("Press space to play again!", 40.0f, 2.5f);
		uint32_t height = gameoversize.Y + pressspacesize.Y + 4;
		uint32_t width = (gameoversize.X > pressspacesize.X ? gameoversize.X : pressspacesize.X) + 4;
		uint32_t xpos = (m_Level->GetAbsoluteWidth() - width) / 2;
		uint32_t ypos = (m_Level->GetAbsoluteHeight() - height) / 2;
		m_Renderer->BeginFrame();
		m_Renderer->Clear(0, 0, 0, 0);
		m_Renderer->RenderRectangle(xpos, ypos, width, height, { 250,50,50,255 });
		xpos = xpos + 2;
		ypos = ypos + 2;
		m_Renderer->RenderText("Game over!", xpos, ypos, 40.0f, 2.5f, { 255,255,255,255 });
		ypos = ypos + gameoversize.Y;
		m_Renderer->RenderText("Press space to play again!", xpos, ypos, 40.0f, 2.5f, { 255,255,255,255 });
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
	}
	void Application::DrawStartScreen()
	{
		Vec2<int32_t> textsizeln1 = m_Renderer->MeasureText("Welcome to LabRatMan", 40.f, 2.5f);
		Vec2<int32_t> textsizeln2 = m_Renderer->MeasureText("Press space to start game", 40.0f, 2.5f);
		Vec2<int32_t> textsizeln3 = m_Renderer->MeasureText("Press P to pause, Esc to quit", 40.0f, 2.5f);
		uint32_t height = textsizeln1.Y + textsizeln2.Y + textsizeln3.Y + 10;
		uint32_t width = std::max({ textsizeln1.X,textsizeln2.X,textsizeln3.X });
		width = width + 10;
		uint32_t xpos = (m_Level->GetAbsoluteWidth() - width) / 2;
		uint32_t ypos = (m_Level->GetAbsoluteHeight() - height) / 2;
		m_Renderer->RenderRectangle(xpos, ypos, width, height, { 250,50,50,175 });
		xpos = xpos + 5;
		ypos = ypos + 5;
		m_Renderer->RenderText("Welcome to LabRatMan", xpos, ypos, 40.0f, 2.5f, { 255,255,255,255 });
		ypos = ypos + textsizeln1.Y;
		m_Renderer->RenderText("Press space to start game", xpos, ypos, 40.0f, 2.5f, { 255,255,255,255 });
		ypos = ypos + textsizeln2.Y;
		m_Renderer->RenderText("Press P to pause, Esc to quit", xpos, ypos, 40.0f, 2.5f, { 255,255,255,255 });
	}
}