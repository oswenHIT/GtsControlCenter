#pragma  once

#include <assert.h>
#include <tisudshl.h>

#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

#include "assisstclass.h"

using namespace _DSHOWLIB_NAMESPACE;

/* 
 * Using the filter
 * 
 *	1.)	In your main form, create a variable of this class type:
 * 
 *		smart_com<WRFilter>	m_pFilter;
 *
 *	2.)	Instantiate the class at the beginning of the program:
 * 
 *		m_pFilter.attach( new WRFilter );
 * 
 *	3.)	Insert the filter in IC Imaging Control, e.g. as a device filter:
 * 
 *		m_cGrabber.setDeviceFrameFilters( m_pFilter.get() );
 *
 *		Important:	The smart_com variable that holds the instance of the filter
 *					must exist until the filter is removed from the grabber.
 * 
 * 
 *	4.)	Important:	This header file must be included, where the filter is used.
 */

class WRFilter: 
	public _DSHOWLIB_NAMESPACE::FrameUpdateFilterImpl<WRFilter>
{
public:
	WRFilter();
	static _DSHOWLIB_NAMESPACE::FilterInfo getStaticFilterInfo();

public:// Input and output methods for the parameters of this filter.

	tFilterError enableHistEql(bool isHist);
	tFilterError getHistEql(bool & isHist);

	tFilterError enableFilter(bool isEnable);
	tFilterError getFilter(bool & isEnable);

	tFilterError setFilterMode(long type);
	tFilterError getFilterMode(long & type);
		
protected:
	virtual void    getSupportedInputTypes( FrameTypeInfoArray& arr ) const ;
	virtual bool    updateInPlace( IFrame& src ) override;

	// These methods are called, when the live video starts and stops.
	virtual void    notifyStart( const FrameTypeInfo& in_type ) override;
	virtual void    notifyStop();

private:
	bool m_bEnabled;			
	bool m_bLiveVideoRunning;	
	bool isHistEql;
	int fType;

private:
	cv::Mat * img;
	cv::Mat tmpImg;
};

