#pragma once
#include <cinttypes>
#include <memory>

namespace Pman
{
	struct Buffer
	{
		uint8_t* Data = nullptr;
		uint64_t Size = 0;

		Buffer() = default;
		Buffer(uint64_t size)
		{
			Allocate(size);
		}
		Buffer(const Buffer&) = default;

		static Buffer Copy(Buffer other)
		{
			Buffer retval = Buffer(other.Size);
			memcpy(retval.Data, other.Data, other.Size);
			return retval;
		}

		void Allocate(uint64_t size)
		{
			Release();
			Data = new uint8_t[size];
			Size - size;
		}
		void Release()
		{
			delete[] Data;
			Data = nullptr;
			Size = 0;
		}
		template<typename T>
		T* As()
		{
			return (T*)Data;
		}
	};
	struct ScopedBuffer
	{
		ScopedBuffer(Buffer buffer) : m_Buffer(buffer)
		{

		}
		ScopedBuffer(uint64_t size) : m_Buffer(size)
		{

		}
		~ScopedBuffer()
		{
			m_Buffer.Release();
		}
		void Zero()
		{
			memset(m_Buffer.Data, 0, m_Buffer.Size);
		}
		uint8_t* Data() { return m_Buffer.Data; }
		uint64_t Size() { return m_Buffer.Size; }
		template<typename T>
		T* As()
		{
			return m_Buffer.As<T>();
		}
	private:
		Buffer m_Buffer;
	};
}