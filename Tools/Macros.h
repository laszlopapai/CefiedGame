#pragma once

#include <iostream>

#ifndef InitCheck

#define InitCheck(x, msg)                \
{                                        \
	if (!(x))                            \
	{                                    \
		std::cerr << (msg) << std::endl; \
		return false;                    \
	}                                    \
}                                        \

#endif

#ifndef HRCheck

#define HRCheck(x, msg)                  \
{                                        \
	HRESULT hr = x;                      \
	if (FAILED(hr))                      \
	{                                    \
		std::cerr << msg << std::endl;   \
		return false;                    \
	}                                    \
}

#define HRCheck(x, msg, afterFail)       \
{                                        \
	HRESULT hr = x;                      \
	if (FAILED(hr))                      \
	{                                    \
		std::cerr << msg << std::endl;   \
		afterFail;                       \
		return false;                    \
	}                                    \
}                                        \

#endif

namespace Cefied
{
	namespace Game
	{
		template <class T> void SafeDelete(T& t)
		{
			if (t)
			{
				delete t;
				t = nullptr;
			}
		}

		template <class T> void SafeDeleteArr(T& t)
		{
			if (t)
			{
				delete[] t;
				t = nullptr;
			}
		}

		template <class T> void SafeRelease(T& t)
		{
			if (t)
			{
				t->Release();
				t = nullptr;
			}
		}
	}
}
