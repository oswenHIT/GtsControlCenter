//#include "stdafx.h"
#include "WRFilter.h"

using namespace _DSHOWLIB_NAMESPACE;


WRFilter::WRFilter() 
	: isHistEql(false), fType(wr::FilterType::None),
	  m_bEnabled(true), m_bLiveVideoRunning(false)
{
	// The parameter input and output methods must be members of the class
	addBoolParam("Hist EQL", &WRFilter::getHistEql, &WRFilter::enableHistEql);
	addBoolParam("Filter Enable", &WRFilter::getFilter, &WRFilter::enableFilter);
	addLongParam("Filter Mode", &WRFilter::getFilterMode, &WRFilter::setFilterMode);
}

//////////////////////////////////////////////////////////////////////////

_DSHOWLIB_NAMESPACE::FilterInfo WRFilter::getStaticFilterInfo()
{
	static _DSHOWLIB_NAMESPACE::FilterInfo f = { L"WRFilter", L"", eFC_GENERIC };
	return f;
}

//////////////////////////////////////////////////////////////////////////
// Implementation of the parameter input and output methods.
//////////////////////////////////////////////////////////////////////////

tFilterError WRFilter::enableHistEql(bool isHist)
{
	isHistEql = isHist;

	return tFilterError();
}

tFilterError WRFilter::getHistEql(bool & isHist)
{
	isHist = isHistEql;

	return tFilterError();
}

tFilterError WRFilter::enableFilter(bool isEnable)
{
	m_bEnabled = isEnable;

	return tFilterError();
}

tFilterError WRFilter::getFilter(bool & isEnable)
{
	isEnable = m_bEnabled;

	return tFilterError();
}

tFilterError WRFilter::setFilterMode(long type)
{
	fType = static_cast<int>(type);

	return tFilterError();
}

tFilterError WRFilter::getFilterMode(long & type)
{
	type = static_cast<long>(fType);

	return tFilterError();
}


//////////////////////////////////////////////////////////////////////////
// This method is called by the framework when the live video is started.
//////////////////////////////////////////////////////////////////////////

void WRFilter::notifyStart( const FrameTypeInfo& in_type )
{
	m_bLiveVideoRunning = true;

	img = new cv::Mat(960, 1280, CV_8U);
}
//////////////////////////////////////////////////////////////////////////
// This method is called by the framework when the live video is stopped.
//////////////////////////////////////////////////////////////////////////

void WRFilter::notifyStop()
{
	m_bLiveVideoRunning = false;
	
	delete img;
}


//////////////////////////////////////////////////////////////////////////
// The array "arr" is filled with the color formats that are supported by
// this filter. If the filter only handels monochrome images,
// then only "eY800" should be added to the array.
//////////////////////////////////////////////////////////////////////////

void WRFilter::getSupportedInputTypes( FrameTypeInfoArray& arr ) const 
{
	arr.addFrameType( eRGB8 );
	arr.addFrameType( eY800 );
	arr.addFrameType( eRGB24 );
	arr.addFrameType( eRGB32 );
}

//////////////////////////////////////////////////////////////////////////
// This is the update method. If m_bEnabled is true, the image data will be
// inverted. If m_bEnabled is false, the method returns immediately.
//////////////////////////////////////////////////////////////////////////

bool WRFilter::updateInPlace( IFrame& frame )
{
	if (m_bEnabled)
	{
		img->data = frame.getPtr();
		img->copyTo(tmpImg);

		switch (fType)
		{
		case wr::FilterType::Gauss:
			cv::GaussianBlur(tmpImg, *img, cv::Size(5, 5), 0.3, 0.3);
			break;
		case wr::FilterType::Homogeneous:
			break;
		case wr::FilterType::Median:
			cv::medianBlur(tmpImg, *img, 3);
			break;
		case wr::FilterType::Bilatrial:
			break;
		default:
			break;                       
		}
			
		if (isHistEql)
			cv::equalizeHist(tmpImg, *img);
	}
	else
	{
		return true;
	}

	return true;
}

