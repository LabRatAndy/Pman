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