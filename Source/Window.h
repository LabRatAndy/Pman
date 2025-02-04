#pragma once
#include "Keys.h"

#include <string>
namespace Pman
{
	struct WindowSpecification
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		std::string WindowTitle = "";
	};

	class Window
	{
	public:
		Window(const WindowSpecification& spec);
		~Window();

		bool ShouldWindowClose() const;
		void ChangeWindowSize(uint32_t newwidth, uint32_t newheight);
		//Gets delta time between frames ie time last frame took to draw
		float GetTimeDelta() const;
		//Gets Time since window was created in seconds
		double GetAbsoluteTime() const;

		bool GetKeyDown(Key key) const;
		bool GetKeyPressed(Key key) const;
		bool GetKeyPressedRepeat(Key key) const;
		bool GetKeyRelease(Key key) const;
		bool GetKeyUP(Key key) const;
	};
}