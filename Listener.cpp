// Listener.cpp: implementation of the CListener class.
//
//////////////////////////////////////////////////////////////////////

#define _WIN32_WINNT 0x0500

#include <iostream>

#include "Listener.h"

using namespace DShowLib;

CListener::CListener()
{
	centerRect.top = 476;
	centerRect.bottom = 484;
	centerRect.left = 635;
	centerRect.right = 643;
}

//////////////////////////////////////////////////////////////////////////
/*! The overlayCallback() method draws the number of the current frame. The
	frame count is a member of the tsMediaSampleDesc structure that is passed
	to overlayCallback() by the Grabber.
*/
void	CListener::overlayCallback( Grabber& caller, smart_ptr<OverlayBitmap> pBitmap, 
								   const tsMediaSampleDesc& MediaSampleDesc)
{
	static int a = 1;
	static bool isStarted = false;	

	if( pBitmap->getEnable() == true ) // Draw only, if the overlay bitmap is enabled.
	{
		if (roi.isGrabMode && !roi.isRelease)
			pBitmap->fill(pBitmap->getDropOutColor());

		if (roi.isPress && roi.isGrabMode)
			pBitmap->drawFrameRect(RGB(234, 199, 135), roi.rect);
		if (!roi.isGrabMode)
			pBitmap->fill(pBitmap->getDropOutColor());

		char szText[25];
		sprintf(szText, "Frames: %04d ", a++);
		pBitmap->drawText(RGB(0, 255, 0), 4, 4, szText);

		pBitmap->drawLine(RGB(255, 0, 0), 0, 479, 1279, 479);
		pBitmap->drawLine(RGB(255, 0, 0), 639, 0, 639, 1279);
		pBitmap->drawSolidEllipse(RGB(0, 0, 255), centerRect);
	}

	if (a == 10000)
		a = 0;
}

//////////////////////////////////////////////////////////////////////////
/*! The frameReady() method calls the saveImage method to save the image buffer to disk.
*/
void	CListener::frameReady( Grabber& caller, smart_ptr<MemBuffer> pBuffer, DWORD currFrame)
{
	//saveImage( pBuffer, currFrame ); // Do the buffer processing.

	//::Sleep(250); // Simulate a time expensive processing.
}

//////////////////////////////////////////////////////////////////////////
/*! Initialize the array of bools that is used to memorize, which buffers were processed in 
	the frameReady() method. The size of the array is specified by the parameter NumBuffers.
	It should be equal to the number of buffers in the FrameHandlerSink.
	All members of m_BufferWritten are initialized to false.
	This means that no buffers have been processed.
*/
void	CListener::setBufferSize( unsigned long NumBuffers )
{
	m_BufferWritten.resize( NumBuffers, false );
}

void CListener::setROI(wr::ROI & region)
{
	roi = region;
}

//////////////////////////////////////////////////////////////////////////
/*! The image passed by the MemBuffer pointer is saved to a BMP file.
*/
void	CListener::saveImage( smart_ptr<MemBuffer> pBuffer, DWORD currFrame)
{
	if( currFrame < m_BufferWritten.size() )
	{
		char filename[MAX_PATH];
		sprintf( filename, "image%02i.bmp", currFrame );

		saveToFileBMP( *pBuffer, filename );

		m_BufferWritten.at( currFrame ) = true;
	}
}
//>>