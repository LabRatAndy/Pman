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

		constexpr Vec2(T x, T y): X(x), Y(y) 
		{
		}
		constexpr void operator=(const Vec2<T>& other)
		{
			X = other.X;
			Y = other.Y;
		}
		constexpr bool operator==(const Vec2<T>& other)
		{
			if (X != other.X)
				return false;
			if (Y != other.Y)
				return false;
			return true;
		}
		constexpr bool operator!=(const Vec2<T>& other)
		{
			return !(*this == other);
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
		constexpr void operator*(const int32_t scalar)
		{
			X = X * scalar;
			Y = Y * scalar;
		}
		constexpr Vec2<T> operator-(const Vec2<T>& other)
		{
			T x = X - other.X;
			T y = Y - other.Y;
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
		constexpr Vec3(T x, T y, T z) : X(x), Y(y), Z(z)
		{
		}
		constexpr void operator=(const Vec3<T>& other)
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
		}
		constexpr bool operator==(const Vec3<T>& other)
		{
			if (X != other.X)
				return false;
			if (Y != other.Y)
				return false;
			if (Z != other.Z)
				return false;
			return true;
		}
		constexpr bool operator!=(const Vec3<T>& other)
		{
			return !(*this == other);
		}
	};
	template<typename T>
	struct Vec4
	{
		T X;
		T Y;
		T Z;
		T W;
		constexpr Vec4(T x, T y, T z, T w) : X(x), Y(y), Z(z), W(w)
		{
		}
		constexpr void operator=(const Vec4<T>& other)
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			W = other.W;
		}
		constexpr bool operator==(const Vec4<T>& other)
		{
			if (X != other.X)
				return false;
			if (Y != other.Y)
				return false;
			if (Z != other.Z)
				return false;
			if (W != other.W)
				return false;
			return true;
		}
		constexpr bool operator!=(const Vec4<T>& other)
		{
			return !(*this == other);
		}
	};

	template<typename T>
	struct Rect
	{
		T Left;
		T Right;
		T Top;
		T Bottom;

		constexpr Rect(T left, T right, T top, T bottom) : Left(left), Right(right), Top(top), Bottom(bottom)
		{
		}
	};
}
template<>
struct std::formatter<Pman::Vec2<int32_t>>
{
	constexpr auto parse(std::format_parse_context& ctx)
	{
		return std::begin(ctx);
	}
	auto format(const Pman::Vec2<int32_t>& obj, std::format_context& ctx) const
	{
		return std::format_to(ctx.out(), "{},{}", obj.X, obj.Y);
	}
};
template<>
struct std::formatter<Pman::Vec2<uint32_t>>
{
	constexpr auto parse(std::format_parse_context& ctx)
	{
		return std::begin(ctx);
	}
	auto format(const Pman::Vec2<uint32_t>& obj, std::format_context& ctx) const
	{
		return std::format_to(ctx.out(), "{},{}", obj.X, obj.Y);
	}
};
template<>
struct std::formatter<Pman::Vec2<float>>
{
	constexpr auto parse(std::format_parse_context& ctx)
	{
		return std::begin(ctx);
	}
	auto format(const Pman::Vec2<float>& obj, std::format_context& ctx) const
	{
		return std::format_to(ctx.out(), "{},{}", obj.X, obj.Y);
	}
};
template<>
struct std::formatter<Pman::Rect<int32_t>>
{
	constexpr auto parse(std::format_parse_context& ctx)
	{
		return std::begin(ctx);
	}
	auto format(const Pman::Rect<int32_t> obj, std::format_context& ctx) const
	{
		return std::format_to(ctx.out(), "Left: {}, Right: {}, Top: {}, Bottom: {}", obj.Left, obj.Right, obj.Top, obj.Bottom);
	}

};