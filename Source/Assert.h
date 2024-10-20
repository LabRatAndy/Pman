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
