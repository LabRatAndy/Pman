#include "Application.h"
#include"Window.h"
#include"Renderer.h"
#include "Level.h"
#include "Assert.h"

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
		m_Level = new Level(m_Specification.TileSize);
		m_Level->LoadLevel();
		uint32_t winheight = m_Level->GetAbsoluteHeight() + (m_Specification.TextTilesHigh * m_Specification.TileSize);
		m_Window->ChangeWindowSize(m_Level->GetAbsoluteWidth(), winheight);
		//main game loop
		float timedelta = 0.0f;
		while (!m_Window->ShouldWindowClose())
		{
			//temporary needs to be finessed!
			static bool started = false;
			if (!started)
			{
				m_Level->StartGame();
				started = true;
			}
			timedelta = m_Window->GetTimeDelta();
			//update game/level
			m_Level->OnUpdate(timedelta);

			m_Renderer->BeginFrame();
			m_Renderer->Clear(0, 0, 0, 0);

			//render game/level
			m_Level->OnRender();

			m_Renderer->EndFrame();
		}
	}
}