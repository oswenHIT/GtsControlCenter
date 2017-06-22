#pragma once
#ifndef ASSISSTCLASS_H
#define ASSISSTCLASS_H

#include <tisudshl.h>

using namespace _DSHOWLIB_NAMESPACE;

namespace wr {
	struct ROI 
	{
		ROI() : isPress(false), isRelease(false), isGrabMode(false)
		{ 
			rect.top = 0; rect.bottom = 0; 
			rect.left = 0; rect.right = 0;
		}
		ROI(ROI& region) = default;
		ROI& operator= (ROI& region) = default;
		auto operator= (std::initializer_list<long> il) -> ROI&
		{
			if (il.size() >= 4)
			{
				auto beg = il.begin();
				rect.left = *beg++;
				rect.top = *beg++;
				rect.right = *beg++;
				rect.bottom = *beg;
			}
		}

		RECT rect;
		bool isPress;
		bool isRelease;
		bool isGrabMode;
	};
}

namespace wr {
	enum FilterType { None, Gauss, Homogeneous, Median, Bilatrial };
}

#endif // !ASSISSTCLASS_H

