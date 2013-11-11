/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*	CDCDithering 
*		Dithering by H263 algorithm developed in NMP/Oulu.
*
*/



//  Include Files  
#include <e32std.h>   // The basic definitions
#include <fbs.h>      // For FBS bitmap
#include "DCDithering.h"     // The DCDithering class



//  MEMBER FUNCTIONS
//=============================================================================



/*
-----------------------------------------------------------------------------

    CDCDithering

    Constructor

    Default constructor

    Return Values:  none

-----------------------------------------------------------------------------
*/
CDCDithering::CDCDithering()
{
	// This function is intentionally left blank.
}




/*
-----------------------------------------------------------------------------

    CDCDithering

    NewLC

    Factory function to instantiate the class.
    This function leaves the class pointer to the cleanup stack
    May leave with KErrNoMemory if no memory available

    Return Values:  CDCDithering* self:  pointer to the class instance

-----------------------------------------------------------------------------
*/
CDCDithering* CDCDithering::NewLC()
{
    CDCDithering* self = new (ELeave) CDCDithering();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}




/*
-----------------------------------------------------------------------------

    CDCDithering

    NewL

    Factory function to instantiate the class.
    May leave with KErrNoMemory if no memory available

    Return Values:  CDCDithering* self:  pointer to the class instance

-----------------------------------------------------------------------------
*/
CDCDithering* CDCDithering::NewL()
{
    CDCDithering* self = CDCDithering::NewLC();
    CleanupStack::Pop();
    return self;
}



/*
-----------------------------------------------------------------------------

    CDCDithering

    ConstructL

    Second phase constructor. Does nothing at the moment

    Return Values:  none

-----------------------------------------------------------------------------
*/
void CDCDithering::ConstructL()
{
    // This function is intentionally left blank.
}




/*
-----------------------------------------------------------------------------

    CDCDithering

    Destructor

    Return Values:  none

-----------------------------------------------------------------------------
*/
CDCDithering::~CDCDithering()
{
	// This function is intentionally left blank.
}



/*
-----------------------------------------------------------------------------

  ProcessL
	
  Process image referenced by aImage (modify aImage).
  May leave with KErrNoMemory if no memory available
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDCDithering::ProcessL(CFbsBitmap& aImage)
{
	TUint	r, g, b;	// Color components
	TUint8*	dataPtr;	// Pointer to data

	//Dithering variables, init to 0
	TInt	count=0;	
	TInt16	dither=0;
	
	//EColor16M image is needed
	if (aImage.DisplayMode() != EColor16M || aImage.DisplayMode() != EColor16M)
		return;
	
	// Line Buffer and pointer to the data
	TUint imageWidth = aImage.SizeInPixels().iWidth;
	TUint scanLineLengthInBytes = aImage.ScanLineLength(imageWidth, aImage.DisplayMode());
	
	//Allocate buffer for scanline
	iScanLineBuffer = HBufC8::NewMaxL(scanLineLengthInBytes);
	//Pointer to scanline
	TPtr8 linePtr = iScanLineBuffer->Des();
	
	//Step through image lines
	for (TInt lineNo=0; lineNo<aImage.SizeInPixels().iHeight; ++lineNo)
	{
		//Get line
		aImage.GetScanLine(linePtr, TPoint(0, lineNo), imageWidth, aImage.DisplayMode());
		//CHECK! CONST_CAST not used in every algorithm which way is better?
		dataPtr = CONST_CAST(TUint8*, linePtr.Ptr());

		//Step through image pixels
		for (TUint x=0; x < imageWidth; ++x)
		{
			// Get original values
			b = *dataPtr++;
			g = *dataPtr++;
			r = *dataPtr++;
			
			//Compute DCDithering factor from base count
			switch (count&1)
			{
			case 0:
				dither = (TInt16)(dither*0x7ffd);
				break;
			case 1:
				dither = (TInt16)(dither+0x7f21);
				break;
			}
			
			//Add DCDithering factor, adjust gain according to quantization factors.
			r = Limit255((TInt)r + (dither>>13));
			g = Limit255((TInt)g - (dither>>14));
			b = Limit255((TInt)b + (dither>>13));
			
			//Move to the previous pixel
			dataPtr -= 3;
			
			/* Set the result */
			*dataPtr++ = (TUint8)b;
			*dataPtr++ = (TUint8)g;
			*dataPtr++ = (TUint8)r;
		
			//Increase bae count
			count++;
		}
		
		//Set scan line
		aImage.SetScanLine(linePtr, lineNo);
	}

	//Free allocated memory
	delete(iScanLineBuffer);
	iScanLineBuffer = 0;
}
//  End of File  
//----IMAAMI----
