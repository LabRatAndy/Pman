#include "Window.h"
#include <raylib.h>
namespace Pman
{
	Window::Window(const WindowSpecification& spec)
	{
		InitWindow(spec.Width, spec.Height, spec.WindowTitle.c_str());
	}
	Window::~Window()
	{
		CloseWindow();
	}
	void Window::ChangeWindowSize(uint32_t newwidth, uint32_t newheight) 
	{
		SetWindowSize(newwidth, newheight);
	}
	bool Window::ShouldWindowClose() const
	{
		return WindowShouldClose();
	}
	float Window::GetTimeDelta() const
	{
		return GetFrameTime();
	}
	double Window::GetAbsoluteTime() const
	{
		return GetTime();
	}
	bool Window::GetKeyDown(Key key) const
	{
		return IsKeyDown(static_cast<int>(key));
	}
	bool Window::GetKeyPressed(Key key) const
	{
		return IsKeyPressed(static_cast<int>(key));
	}
	bool Window::GetKeyPressedRepeat(Key key) const
	{
		return IsKeyPressedRepeat(static_cast<int>(key));
	}
	bool Window::GetKeyRelease(Key key) const
	{
		return IsKeyReleased(static_cast<int>(key));
	}
	bool Window::GetKeyUP(Key key) const
	{
		return IsKeyUp(static_cast<int>(key));
	}
}