#pragma once
#include "Assert.h"
#include <cmath>

namespace Pman
{
	template<typename T>
	struct Vec2
	{
		T X;
		T Y;

		Vec2(T x, T y): X(x), Y(y) 
		{
		}
		void operator=(const Vec2<T>& other)
		{
			X = other.X;
			Y = other.Y;
		}
		bool operator==(const Vec2<T>& other)
		{
			if (X != other.X)
				return false;
			if (Y != other.Y)
				return false;
			return true;
		}
		bool operator!=(const Vec2<T>& other)
		{
			return !(*this == other);
		}
		void ReverseDirection()
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
		bool IsZero() const
		{
			if (X == 0 && Y == 0)
			{
				return true;
			}
			return false;
		}
		float Length() const
		{
			return static_cast<float>(std::hypot(X, Y));
		}
		void Normalise()
		{
			float length = this->Length();
			X = static_cast<T>(X / length);
			Y = static_cast<T>(Y / length);
		}
		void operator*(const int32_t scalar)
		{
			X = X * scalar;
			Y = Y * scalar;
		}
		Vec2<T> operator-(const Vec2<T>& other)
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
		Vec3(T x, T y, T z) : X(x), Y(y), Z(z)
		{
		}
		void operator=(const Vec3<T>& other)
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
		}
		bool operator==(const Vec3<T>& other)
		{
			if (X != other.X)
				return false;
			if (Y != other.Y)
				return false;
			if (Z != other.Z)
				return false;
			return true;
		}
		bool operator!=(const Vec3<T>& other)
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
		Vec4(T x, T y, T z, T w) : X(x), Y(y), Z(z), W(w)
		{
		}
		void operator=(const Vec4<T>& other)
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			W = other.W;
		}
		bool operator==(const Vec4<T>& other)
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
		bool operator!=(const Vec4<T>& other)
		{
			return !(*this == other);
		}
	};
}