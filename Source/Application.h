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
#include <string>

namespace Pman
{
	struct ApplicationSpecification
	{
		uint32_t TileSize = 32;
		uint32_t TilesWide = 21;
		uint32_t TilesHigh = 21;
		uint32_t TextTilesHigh = 1; // number of extra tiles at the bottom for the score
		std::string WindowTitle = "LabRatMan";
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
		bool m_Started = false;
		bool m_Paused = false;

		void DrawGameOver() const;
		void DrawStartScreen() const;
		void DrawPausedScreen() const;
		
		void ResetLevel();

	};
}