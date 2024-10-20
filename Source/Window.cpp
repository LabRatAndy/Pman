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
}