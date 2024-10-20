#pragma once
#include <string>

namespace Pman
{
	struct ApplicationSpecification
	{
		uint32_t TileSize = 32;
		uint32_t TilesWide = 21;
		uint32_t TilesHigh = 21;
		std::string WindowTitle = "Pacman";
	};
	//fwd decs
	class Renderer;
	class Window;
	class Level;

	class Application
	{
	public:
		Application(const ApplicationSpecification& spec = ApplicationSpecification());
		~Application();

		void Run();

		Renderer& GetRenderer() { return *m_Renderer; }
		Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }

	private:
		static inline Application* s_Instance = nullptr;
		Window* m_Window = nullptr;
		Renderer* m_Renderer = nullptr;
		Level* m_Level = nullptr;
		ApplicationSpecification m_Specification;
	};
}