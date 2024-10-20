#include "Application.h"
#include"Window.h"
#include"Renderer.h"
#include "Level.h"

namespace Pman
{
	Application::Application(const ApplicationSpecification& spec) : m_Specification(spec), m_Renderer(new Renderer(spec.TileSize* spec.TilesWide, spec.TileSize* spec.TilesHigh))
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
		m_Window = new Window({ m_Specification.TileSize * m_Specification.TilesWide, m_Specification.TileSize * m_Specification.TilesHigh, m_Specification.WindowTitle });
		m_Renderer->Initialise(60);
		m_Level = new Level(m_Specification.TileSize);
		m_Level->LoadLevel();
		m_Window->ChangeWindowSize(m_Level->GetAbsoluteWidth(), m_Level->GetAbsoluteHeight());
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
			//TODO only do 10 frames to test ghost pathfinding
			/*static uint32_t framecount = 0;
			framecount++;
			if (framecount > 130)
			{
				break;
			}*/
		}
	}
}