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

#include "Window.h"

#include <raylib.h>
namespace Pman
{
	Window::Window(const WindowSpecification& spec_)
	{
		::InitWindow(spec_.Width, spec_.Height, spec_.WindowTitle.c_str());
	}
	Window::~Window()
	{
		::CloseWindow();
	}
	void Window::ChangeWindowSize(uint32_t new_width, uint32_t new_height) 
	{
		::SetWindowSize(new_width, new_height);
	}
	bool Window::ShouldWindowClose() const
	{
		return ::WindowShouldClose();
	}
	float Window::GetTimeDelta() const
	{
		return ::GetFrameTime();
	}
	double Window::GetAbsoluteTime() const
	{
		return ::GetTime();
	}
	bool Window::GetKeyDown(Key key_) const
	{
		return ::IsKeyDown(static_cast<int>(key_));
	}
	bool Window::GetKeyPressed(Key key_) const
	{
		return ::IsKeyPressed(static_cast<int>(key_));
	}
	bool Window::GetKeyPressedRepeat(Key key_) const
	{
		return ::IsKeyPressedRepeat(static_cast<int>(key_));
	}
	bool Window::GetKeyRelease(Key key_) const
	{
		return ::IsKeyReleased(static_cast<int>(key_));
	}
	bool Window::GetKeyUP(Key key_) const
	{
		return ::IsKeyUp(static_cast<int>(key_));
	}
}