#pragma once
#include <iostream>
#include <intrin.h>

#ifdef DEBUG
#define ASSERT(x, msg) if(!x) { std::cerr << msg<<std::endl; __debugbreak();}
#define LOG(msg) std::cerr<<"[Log message]: "<<msg<<std::endl;
#else
#define ASSERT(x, msg)
#define LOG(msg)
#endif

#include <print>
#include <format>
#include <source_location>

namespace Pman
{
	enum class ErrorLevel
	{
		TRACE,
		INFO,
		WARN,
		ERROR
	};
	template<ErrorLevel L, typename ... Args>
	struct Log {};
	template<ErrorLevel L, typename ... Args>
	struct Log<L ,const char*, Args ...>
	{
		Log(const char* msg_, Args ... args_, std::source_location loc_ = std::source_location::current())
		{
			if constexpr (L == ErrorLevel::TRACE)
			{
				std::println("[TRACE] {}:{} {}", loc_.file_name(), loc_.line(), std::vformat(msg_, std::make_format_args(args_ ...)));
			}
			else if constexpr (L == ErrorLevel::INFO)
			{
				std::println("[INFO] {}:{} {}", loc_.file_name(), loc_.line(), std::vformat(msg_, std::make_format_args(args_ ...)));
			}
			else if constexpr (L == ErrorLevel::WARN)
			{
				std::println("[WARNING] {}:{} {}", loc_.file_name(), loc_.line(), std::vformat(msg_, std::make_format_args(args_ ...)));
			}
			else if constexpr (L == ErrorLevel::ERROR)
			{
				std::println("[ERROR] {}:{} {}", loc_.file_name(), loc_.line(), std::vformat(msg_, std::make_format_args(args_ ...)));
			}
		}
	};
	//Template deduction guide
	template<ErrorLevel L, typename ...Args >
	Log(Args ...) -> Log<L, Args ...>;

	template<typename ...Args>
	using TRACE = Log<ErrorLevel::TRACE, Args ...>;
	template<typename ...Args>
	using INFO = Log<ErrorLevel::INFO, Args ...>;
	template<typename ...Args>
	using WARN = Log<ErrorLevel::WARN, Args ...>;
	template<typename ...Args>
	using ERROR = Log<ErrorLevel::ERROR, Args ...>;
}