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
#include "Assert.h"
#include <cmath>
#include <format>



namespace Pman
{
	template<typename T>
	struct Vec2
	{
		T X;
		T Y;

		constexpr Vec2(T x_, T y_): X(x_), Y(y_) 
		{
		}
		constexpr void operator=(const Vec2<T>& other_)
		{
			X = other_.X;
			Y = other_.Y;
		}
		constexpr bool operator==(const Vec2<T>& other_)
		{
			if (X != other_.X)
				return false;
			if (Y != other_.Y)
				return false;
			return true;
		}
		constexpr bool operator!=(const Vec2<T>& other_)
		{
			return !(*this == other_);
		}
		constexpr void ReverseDirection()
		{
			if (X == 1)
			{
				X = -1;
			}
			else if(X ==-1)
			{
				X = 1;
			}
			else
			{
				ASSERT((X == 0), "This vector is not a direction!");
			}
			if (Y == 1)
			{
				Y = -1;
			}
			else if (Y == -1)
			{
				Y = 1;
			}
			else
			{
				ASSERT((Y == 0), "This vector is not a direction!");
			}
		}
		constexpr bool IsZero() const
		{
			if (X == 0 && Y == 0)
			{
				return true;
			}
			return false;
		}
		constexpr float Length() const
		{
			return static_cast<float>(std::hypot(X, Y));
		}
		constexpr void Normalise()
		{
			float length = this->Length();
			X = static_cast<T>(X / length);
			Y = static_cast<T>(Y / length);
		}
		constexpr void operator*(const int32_t scalar_)
		{
			X = X * scalar_;
			Y = Y * scalar_;
		}
		constexpr Vec2<T> operator-(const Vec2<T>& other_)
		{
			T x = X - other_.X;
			T y = Y - other_.Y;
			Vec2<T> retval{ x,y };
			return retval;
		}
	};


	template<typename T>
	struct Vec3
	{
		T X;
		T Y;
		T Z;
		constexpr Vec3(T x_, T y_, T z_) : X(x_), Y(y_), Z(z_)
		{
		}
		constexpr void operator=(const Vec3<T>& other_)
		{
			X = other_.X;
			Y = other_.Y;
			Z = other_.Z;
		}
		constexpr bool operator==(const Vec3<T>& other_)
		{
			if (X != other_.X)
				return false;
			if (Y != other_.Y)
				return false;
			if (Z != other_.Z)
				return false;
			return true;
		}
		constexpr bool operator!=(const Vec3<T>& other_)
		{
			return !(*this == other_);
		}
	};
	template<typename T>
	struct Vec4
	{
		T X;
		T Y;
		T Z;
		T W;
		constexpr Vec4(T x_, T y_, T z_, T w_) : X(x_), Y(y_), Z(z_), W(w_)
		{
		}
		constexpr void operator=(const Vec4<T>& other_)
		{
			X = other_.X;
			Y = other_.Y;
			Z = other_.Z;
			W = other_.W;
		}
		constexpr bool operator==(const Vec4<T>& other_)
		{
			if (X != other_.X)
				return false;
			if (Y != other_.Y)
				return false;
			if (Z != other_.Z)
				return false;
			if (W != other_.W)
				return false;
			return true;
		}
		constexpr bool operator!=(const Vec4<T>& other_)
		{
			return !(*this == other_);
		}
	};

	template<typename T>
	struct Rect
	{
		T Left;
		T Right;
		T Top;
		T Bottom;

		constexpr Rect(T left_, T right_, T top_, T bottom_) : Left(left_), Right(right_), Top(top_), Bottom(bottom_)
		{
		}
	};
}
template<>
struct std::formatter<Pman::Vec2<int32_t>>
{
	constexpr auto parse(std::format_parse_context& ctx_)
	{
		return std::begin(ctx_);
	}
	auto format(const Pman::Vec2<int32_t>& obj_, std::format_context& ctx_) const
	{
		return std::format_to(ctx_.out(), "{},{}", obj_.X, obj_.Y);
	}
};
template<>
struct std::formatter<Pman::Vec2<uint32_t>>
{
	constexpr auto parse(std::format_parse_context& ctx_)
	{
		return std::begin(ctx_);
	}
	auto format(const Pman::Vec2<uint32_t>& obj_, std::format_context& ctx_) const
	{
		return std::format_to(ctx_.out(), "{},{}", obj_.X, obj_.Y);
	}
};
template<>
struct std::formatter<Pman::Vec2<float>>
{
	constexpr auto parse(std::format_parse_context& ctx_)
	{
		return std::begin(ctx_);
	}
	auto format(const Pman::Vec2<float>& obj_, std::format_context& ctx_) const
	{
		return std::format_to(ctx_.out(), "{},{}", obj_.X, obj_.Y);
	}
};
template<>
struct std::formatter<Pman::Rect<int32_t>>
{
	constexpr auto parse(std::format_parse_context& ctx_)
	{
		return std::begin(ctx_);
	}
	auto format(const Pman::Rect<int32_t> obj_, std::format_context& ctx_) const
	{
		return std::format_to(ctx_.out(), "Left: {}, Right: {}, Top: {}, Bottom: {}", obj_.Left, obj_.Right, obj_.Top, obj_.Bottom);
	}

};