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
*	CDCDCDigitalZoom 
*		Scaling of image to display size & zooming.
*		Includes support for different scaling and crop sizes with pan&scan.
*		Pan can use previously computed scaled data when changing.
*		Based on IMAAMI scaling algorithm.
*
*/




#include <fbs.h>
#include "DCDigitalZoom.h"



/*
-----------------------------------------------------------------------------

    CDCDigitalZoom::CDCDigitalZoom

	C++ constructor.

	Initialises set of member parameters.

-----------------------------------------------------------------------------
*/
CDCDigitalZoom::CDCDigitalZoom()
{

	iParams.sizeX      = 176; //640;	// Size of VGA image for X
	iParams.sizeY      = 144; //480;	// Size of VGA image for Y
	iParams.scaleX     = 1.0f;	// No Scaling for X
	iParams.scaleY     = 1.0f;	// No scaling for Y
	iParams.allShiftX  = 0;		// No pan
	iParams.allShiftY  = 0;		// 
	iParams.newShiftX  = 0;		// No pan
	iParams.newShiftY  = 0;		//

}



/*
-----------------------------------------------------------------------------

    CDCDigitalZoom::ConstructL

	Second phase constructor.

	Construct the object. (not used, may leave)

-----------------------------------------------------------------------------
*/

void CDCDigitalZoom::ConstructL()
{
	// This function is intentionally left blank.
}



/*
-----------------------------------------------------------------------------

    CDCDigitalZoom::NewLC

	Allocate the memory and construct the object.
	Pushs pointer to a new instance to Cleanupstack.

    Return value:
		CDCDigitalZoom* self		Pointer to a new instance

-----------------------------------------------------------------------------
*/

CDCDigitalZoom* CDCDigitalZoom::NewLC()
{
	CDCDigitalZoom* self = new (ELeave) CDCDigitalZoom();
	CleanupStack::PushL(self);
	self->ConstructL();

	return self;
}



/*
-----------------------------------------------------------------------------

    CDCDigitalZoom::NewL

	Allocate the memory and construct the object.

    Return value:
		CDCDigitalZoom* self		Pointer to a new instance

-----------------------------------------------------------------------------
*/

CDCDigitalZoom* CDCDigitalZoom::NewL()
{
	CDCDigitalZoom* self = CDCDigitalZoom::NewLC();
	CleanupStack::Pop();

	return self;
}



/*
-----------------------------------------------------------------------------

    CDCDigitalZoom::~CDCDigitalZoom

	C++ destructor.

-----------------------------------------------------------------------------
*/

CDCDigitalZoom::~CDCDigitalZoom()
{
	// This function is intentionally left blank.
}



/*
-----------------------------------------------------------------------------

    CDCDigitalZoom::ProcessL

	Main function of digital zoom. (public)
	Calls processing function (zoomImage).

	NOTE:
		ImageZoomParams iParams have to be set before calling
		this function, so that the wanted processing is done.

    Parameters in:
		CFbsBitmap* aOriPtr		Pointer to source image bitmap

	Parameters out:
		CFbsBitmap* aOutPtr		Pointer to destination image bitmap

    Return value: None

-----------------------------------------------------------------------------
*/

void CDCDigitalZoom::ProcessL(const CFbsBitmap *aOriPtr, CFbsBitmap *aOutPtr)
{
	//EColor16M image is needed
	if(aOutPtr->DisplayMode() != EColor16M)
	{
		return;
	}

	if(aOriPtr->DisplayMode() != EColor16M)
	{
		return;
	}

	// Find size of original image
	TInt oriSizeX = aOriPtr->SizeInPixels().iWidth;
	TInt oriSizeY = aOriPtr->SizeInPixels().iHeight;

	//Do scaling
	DecimateL(aOriPtr, aOutPtr,
			iParams.sizeX, iParams.sizeY,
			oriSizeX, oriSizeY,
			iParams.scaleX, iParams.scaleY,
			iParams.allShiftX, iParams.allShiftY,
			iParams.newShiftX, iParams.newShiftY);
}




/*
-----------------------------------------------------------------------------

  DecimateL

  IMAAMI scaling core function
		
-----------------------------------------------------------------------------
*/
void CDCDigitalZoom::DecimateL(const CFbsBitmap* aOriPtr, CFbsBitmap* aOutPtr,
									 TInt aOutSizeX, TInt aOutSizeY,
									 TInt aOriSizeX, TInt aOriSizeY,
									 TReal aZoomX, TReal aZoomY,
									 TInt allShiftX, TInt allShiftY,
									 TInt newShiftX, TInt newShiftY)
{
	// We don't waste time if request ZOOM 1x without Pan
    if(aZoomX==1 && aZoomY==1)
    {
		if(allShiftX==0 && allShiftY==0 && newShiftX ==0 && newShiftX == 0)
  	  	{
  	  		TInt handle = aOriPtr->Handle();
  	  	        aOutPtr->Duplicate(handle);
  	  	        return;
  	  	}
  	}
	TInt32
		divider,
		xPos, yPos, tmpline,
		xAver, yAver,
		xStep, yStep,
		tmpEnd, tmpSta,
		sumB, sumG, sumR,
		tmpB, tmpG, tmpR;

	TInt32
		x, y,
		i, j,
		LastLine,
		xInt, yInt,
		xStaInt, yStaInt,
		xEndInt, yEndInt,
		xFirstInt, yFirstInt;

	TUint32
		LineNum,
		outFlag,
		xRem, yRem,
		xStaRem, yStaRem,
		xEndRem, yEndRem,
		xStaWei, yStaWei,
		xEndWei, yEndWei,
		xAllWei, yAllWei,
		xMaxWei, yMaxWei,
		xLoopSta, yLoopSta,
		xLoopEnd, yLoopEnd,
		xFirstRem, yFirstRem;

	TUint32
		PIX_BITS	= 13,						// 13
		PIXEL		= (TUint32)(1 << PIX_BITS),
		HALF_PIX	= (TUint32)(1 << (PIX_BITS - 1)),
		REMAINDER	= (TUint32)(PIXEL - 1),
		WEI_BITS	= 4,						// 4
		HALF_WEI	= (TUint32)(1 << (WEI_BITS - 1)),
		DIF1_BITS	= (TUint32)(PIX_BITS - WEI_BITS),
		HALF_DIF1	= (TUint32)(1 << (DIF1_BITS - 1)),
		REM_HDIF1	= (TUint32)(HALF_DIF1 - 1),
		RED_BITS	= 4,						// 4
		HALF_RED	= (TUint32)(1 << (RED_BITS - 1));


	if(aZoomX < 0.20 || aZoomY < 0.20)
	{
		RED_BITS = 5;
		HALF_RED = (TUint32)(1 << (RED_BITS - 1));
	}

	// Allocate local temporal input0 line buffer and push its pointer to CleanupStack
	HBufC8* oriLine0 = HBufC8::NewMaxL(3 * aOriSizeX);	// BGRBGR...
	CleanupStack::PushL(oriLine0);
	
	// Allocate local temporal input1 line buffer and push its pointer to CleanupStack
	HBufC8* oriLine1 = HBufC8::NewMaxL(3 * aOriSizeX);	// BGRBGR...
	CleanupStack::PushL(oriLine1);

	// Allocate local temporal input2 line buffer and push its pointer to CleanupStack
	HBufC8* oriLine2 = HBufC8::NewMaxL(3 * aOriSizeX);	// BGRBGR...
	CleanupStack::PushL(oriLine2);

	// Allocate local temporal input3 line buffer and push its pointer to CleanupStack
	HBufC8* oriLine3 = HBufC8::NewMaxL(3 * aOriSizeX);	// BGRBGR...
	CleanupStack::PushL(oriLine3);

	// Allocate local temporal input4 line buffer and push its pointer to CleanupStack
	HBufC8* oriLine4 = HBufC8::NewMaxL(3 * aOriSizeX);	// BGRBGR...
	CleanupStack::PushL(oriLine4);

	// Allocate local temporal input5 line buffer and push its pointer to CleanupStack
	HBufC8* oriLine5 = HBufC8::NewMaxL(3 * aOriSizeX);	// BGRBGR...
	CleanupStack::PushL(oriLine5);
	
	// Allocate local temporal input6 line buffer and push its pointer to CleanupStack
	HBufC8* oriLine6 = HBufC8::NewMaxL(3 * aOriSizeX);	// BGRBGR...
	CleanupStack::PushL(oriLine6);

	// Allocate local temporal input7 line buffer and push its pointer to CleanupStack
	HBufC8* oriLine7 = HBufC8::NewMaxL(3 * aOriSizeX);	// BGRBGR...
	CleanupStack::PushL(oriLine7);

	// Allocate local temporal input8 line buffer and push its pointer to CleanupStack
	HBufC8* oriLine8 = HBufC8::NewMaxL(3 * aOriSizeX);	// BGRBGR...
	CleanupStack::PushL(oriLine8);

	// Allocate local temporal input9 line buffer and push its pointer to CleanupStack
	HBufC8* oriLine9 = HBufC8::NewMaxL(3 * aOriSizeX);	// BGRBGR...
	CleanupStack::PushL(oriLine9);

	// Allocate local temporal input10 line buffer and push its pointer to CleanupStack
	HBufC8* oriLine10 = HBufC8::NewMaxL(3 * aOriSizeX);	// BGRBGR...
	CleanupStack::PushL(oriLine10);
	
	// Set pointers of input lines
	TUint8* line0Ptr = (TUint8*)oriLine0->Des().Ptr();
	TUint8* line1Ptr = (TUint8*)oriLine1->Des().Ptr();
	TUint8* line2Ptr = (TUint8*)oriLine2->Des().Ptr();
	TUint8* line3Ptr = (TUint8*)oriLine3->Des().Ptr();
	TUint8* line4Ptr = (TUint8*)oriLine4->Des().Ptr();
	TUint8* line5Ptr = (TUint8*)oriLine5->Des().Ptr();
	TUint8* line6Ptr = (TUint8*)oriLine6->Des().Ptr();
	TUint8* line7Ptr = (TUint8*)oriLine7->Des().Ptr();
	TUint8* line8Ptr = (TUint8*)oriLine8->Des().Ptr();
	TUint8* line9Ptr = (TUint8*)oriLine9->Des().Ptr();
	TUint8* line10Ptr = (TUint8*)oriLine10->Des().Ptr();
	TUint8* linePtrs[11] = {line0Ptr, line1Ptr, line2Ptr, line3Ptr, line4Ptr,
							line5Ptr, line6Ptr, line7Ptr, line8Ptr, line9Ptr, line10Ptr};

	TUint8* tmpPtr;
	TUint8* tempPtr;
	
	// Set TPtr8s of input lines
	TPtr8 Ptr0(line0Ptr, 3 * aOriSizeX, 3 * aOriSizeX);
	TPtr8 Ptr1(line1Ptr, 3 * aOriSizeX, 3 * aOriSizeX);
	TPtr8 Ptr2(line2Ptr, 3 * aOriSizeX, 3 * aOriSizeX);
	TPtr8 Ptr3(line3Ptr, 3 * aOriSizeX, 3 * aOriSizeX);
	TPtr8 Ptr4(line4Ptr, 3 * aOriSizeX, 3 * aOriSizeX);
	TPtr8 Ptr5(line5Ptr, 3 * aOriSizeX, 3 * aOriSizeX);
	TPtr8 Ptr6(line6Ptr, 3 * aOriSizeX, 3 * aOriSizeX);
	TPtr8 Ptr7(line7Ptr, 3 * aOriSizeX, 3 * aOriSizeX);
	TPtr8 Ptr8(line8Ptr, 3 * aOriSizeX, 3 * aOriSizeX);
	TPtr8 Ptr9(line9Ptr, 3 * aOriSizeX, 3 * aOriSizeX);
	TPtr8 Ptr10(line10Ptr, 3 * aOriSizeX, 3 * aOriSizeX);
	TPtr8 Ptrs[11] = {Ptr0, Ptr1, Ptr2, Ptr3, Ptr4, Ptr5, Ptr6, Ptr7, Ptr8, Ptr9, Ptr10};
	
	// Set indicator for order of input lines
	TInt lines[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	
	// Allocate local temporal output line buffer and push its pointer to CleanupStack
	HBufC8* scanLine = HBufC8::NewMaxL(3 * aOutSizeX);
	CleanupStack::PushL(scanLine);
	
	// Set pointer of output line
	TUint8* DataPtr = (TUint8*)scanLine->Des().Ptr();
	
	// Set TPtr8 of output line
	TPtr8 linePtr(DataPtr, 3 * aOutSizeX, 3 * aOutSizeX);

	
	// Calculate step between output pixels in original image
	xStep = (TInt)(PIXEL / aZoomX + 0.5);
	yStep = (TInt)(PIXEL / aZoomY + 0.5);
	
	// Calculate First output pixel position in original image
	xPos = ((aOriSizeX - 1) << (PIX_BITS - 1)) - (((aOutSizeX - 1 - (allShiftX << 1)) * xStep) >> 1);
	yPos = ((aOriSizeY - 1) << (PIX_BITS - 1)) - (((aOutSizeY - 1 - (allShiftY << 1)) * yStep) >> 1);
	
	xFirstInt = (TInt32)(xPos >> PIX_BITS);
	if(xPos < 0) xFirstRem = (TUint32)((xPos + ((-xFirstInt) << PIX_BITS)) & REMAINDER);
	else		 xFirstRem = (TUint32)(xPos & REMAINDER);
	
	yFirstInt = (TInt32)(yPos >> PIX_BITS);
	if(yPos < 0) yFirstRem = (TUint32)((yPos + ((-yFirstInt) << PIX_BITS)) & REMAINDER);
	else		 yFirstRem = (TUint32)(yPos & REMAINDER);
	
	// Calculate averaging area around the original pixel position
	xAver = (TInt)(xStep >> 1);
	yAver = (TInt)(yStep >> 1);
	
	// For bilinear interpolation at least 1 pixel have to be used
	if(aZoomX > 1 && xAver < (TInt32)(HALF_PIX)) xAver = HALF_PIX;
	if(aZoomY > 1 && yAver < (TInt32)(HALF_PIX)) yAver = HALF_PIX;
	
	// Calculate maximum weight sum
	yMaxWei = (TUint32)(((yAver << 1) + HALF_DIF1) >> DIF1_BITS);
	xMaxWei = (TUint32)(((xAver << 1) + HALF_DIF1) >> DIF1_BITS);
	
	// Calculate filter divider for filter window
	divider = (TInt)((xMaxWei * yMaxWei + HALF_RED) >> RED_BITS);
	
	while(divider <= 256 && WEI_BITS < PIX_BITS)
	{
		WEI_BITS++;
		HALF_WEI  = (TUint32)(1 << (WEI_BITS - 1));
		DIF1_BITS = (TUint32)(PIX_BITS - WEI_BITS);
		HALF_DIF1 = (TUint32)(1 << (DIF1_BITS - 1));
		REM_HDIF1 = (TUint32)(HALF_DIF1 - 1);
		
		// Calculate maximum weight sum
		yMaxWei = (TUint32)(((yAver << 1) + HALF_DIF1) >> DIF1_BITS);
		xMaxWei = (TUint32)(((xAver << 1) + HALF_DIF1) >> DIF1_BITS);
		
		// Calculate filter divider for filter window
		divider = (TInt)((xMaxWei * yMaxWei + HALF_RED) >> RED_BITS);
	}
	
	if(divider > 1024)
	{
		WEI_BITS--;
		HALF_WEI  = (TUint32)(1 << (WEI_BITS - 1));
		DIF1_BITS = (TUint32)(PIX_BITS - WEI_BITS);
		HALF_DIF1 = (TUint32)(1 << (DIF1_BITS - 1));
		REM_HDIF1 = (TUint32)(HALF_DIF1 - 1);
		
		// Calculate maximum weight sum
		yMaxWei = (TUint32)(((yAver << 1) + HALF_DIF1) >> DIF1_BITS);
		xMaxWei = (TUint32)(((xAver << 1) + HALF_DIF1) >> DIF1_BITS);
		
		// Calculate filter divider for filter window
		divider = (TInt)((xMaxWei * yMaxWei + HALF_RED) >> RED_BITS);
	}
	
	while(divider <= 512 && RED_BITS > 0)
	{
		RED_BITS--;
		HALF_RED = (TUint32)(1 << (RED_BITS - 1));
		
		divider = (TInt)((xMaxWei * yMaxWei + HALF_RED) >> RED_BITS);
	}
	
	if(divider > 1024)
	{
		RED_BITS++;
		HALF_RED = (TUint32)(1 << (RED_BITS - 1));
	}
	
	// Initialise y loop limiters
	yLoopSta = 0;
	yLoopEnd = (TUint32)(aOutSizeY);
	
	// Initialise x loop limiters
	xLoopSta = 0;
	xLoopEnd = (TUint32)(aOutSizeX);

	// Calculate only the panned image
		if(newShiftY > 0)
		{
			// Update y loop start
			yLoopSta = (TUint32)(aOutSizeY - newShiftY);

			// Initialise y position
			yInt = yFirstInt;
			yRem = yFirstRem;

			// Copy available image and change y position
			for(y = 0; y < (TInt32)yLoopSta; y++)
			{
				// Read output line from source image
				aOriPtr->GetScanLine(Ptrs[lines[0]], TPoint(0, y+newShiftY), aOriSizeX, aOriPtr->DisplayMode());

				//Set the line to destination image
				aOutPtr->SetScanLine(linePtr, y);

				// Update y position
				tmpEnd = (TInt)(yRem + yStep);
				yInt = (TInt32)(yInt + (tmpEnd >> PIX_BITS));
				yRem = (TUint32)(tmpEnd & REMAINDER);
			}

			// Update y position of first pixel
			yFirstInt = yInt;
			yFirstRem = yRem;
		}
		else if(newShiftY < 0)
		{
			// Update y loop end
			yLoopEnd = (TUint32)(-newShiftY);

			// Copy available image
			for(y = (TInt32)(aOutSizeY - 1); y >= (TInt32)yLoopEnd; y--)
			{
				// Read output line from source image
				aOriPtr->GetScanLine(Ptrs[lines[0]], TPoint(0, y+newShiftY), aOriSizeX, aOriPtr->DisplayMode());

				//Set the line to destination image
				aOutPtr->SetScanLine(linePtr, y);
			}
		}

		// Calculate only the panned image
		if(newShiftX > 0)
		{
			// Update x loop start
			xLoopSta = (TUint32)(aOutSizeX - newShiftX);

			// Initialise x position
			xInt = xFirstInt;
			xRem = xFirstRem;

			// Change x position
			for(x = 0; x < (TInt32)xLoopSta; x++)
			{
				// Update x position
				tmpSta = (TInt)(xRem + xStep);
				xInt = (TInt32)(xInt + (tmpSta >> PIX_BITS));
				xRem = (TUint32)(tmpSta & REMAINDER);
			}

			// Update x position of first pixel
			xFirstInt = xInt;
			xFirstRem = xRem;
		}
		else if(newShiftX < 0)
		{
			// Update loop end
			xLoopEnd = (TUint32)(-newShiftX);
		}

		// Initialise y position
		yInt = yFirstInt;
		yRem = yFirstRem;

		tmpEnd = (TInt)(yRem + yAver + HALF_PIX + HALF_WEI);
		yEndInt = (TInt32)((tmpEnd >> PIX_BITS) + yInt);
		yEndRem = (TUint32)(tmpEnd & REMAINDER);

		//Read 11 lines from the source image
		if (yEndInt >= 10)
		{
			aOriPtr->GetScanLine(Ptrs[lines[0]], TPoint(0, yEndInt-10), aOriSizeX, aOriPtr->DisplayMode());
			aOriPtr->GetScanLine(Ptrs[lines[1]], TPoint(0, yEndInt-9), aOriSizeX, aOriPtr->DisplayMode());
			aOriPtr->GetScanLine(Ptrs[lines[2]], TPoint(0, yEndInt-8), aOriSizeX, aOriPtr->DisplayMode());
			aOriPtr->GetScanLine(Ptrs[lines[3]], TPoint(0, yEndInt-7), aOriSizeX, aOriPtr->DisplayMode());
			aOriPtr->GetScanLine(Ptrs[lines[4]], TPoint(0, yEndInt-6), aOriSizeX, aOriPtr->DisplayMode());
			aOriPtr->GetScanLine(Ptrs[lines[5]], TPoint(0, yEndInt-5), aOriSizeX, aOriPtr->DisplayMode());
			aOriPtr->GetScanLine(Ptrs[lines[6]], TPoint(0, yEndInt-4), aOriSizeX, aOriPtr->DisplayMode());
			aOriPtr->GetScanLine(Ptrs[lines[7]], TPoint(0, yEndInt-3), aOriSizeX, aOriPtr->DisplayMode());
			aOriPtr->GetScanLine(Ptrs[lines[8]], TPoint(0, yEndInt-2), aOriSizeX, aOriPtr->DisplayMode());
			aOriPtr->GetScanLine(Ptrs[lines[9]], TPoint(0, yEndInt-1), aOriSizeX, aOriPtr->DisplayMode());
			aOriPtr->GetScanLine(Ptrs[lines[10]], TPoint(0, yEndInt  ), aOriSizeX, aOriPtr->DisplayMode());
		}
		else
		{
			if(yEndInt >= 0)
				aOriPtr->GetScanLine(Ptrs[lines[10]], TPoint(0, yEndInt  ), aOriSizeX, aOriPtr->DisplayMode());
			if(yEndInt >= 1)
				aOriPtr->GetScanLine(Ptrs[lines[9]], TPoint(0, yEndInt-1), aOriSizeX, aOriPtr->DisplayMode());
			if(yEndInt >= 2)
				aOriPtr->GetScanLine(Ptrs[lines[8]], TPoint(0, yEndInt-2), aOriSizeX, aOriPtr->DisplayMode());
			if(yEndInt >= 3)
				aOriPtr->GetScanLine(Ptrs[lines[7]], TPoint(0, yEndInt-3), aOriSizeX, aOriPtr->DisplayMode());
			if(yEndInt >= 4)
				aOriPtr->GetScanLine(Ptrs[lines[6]], TPoint(0, yEndInt-4), aOriSizeX, aOriPtr->DisplayMode());
			if(yEndInt >= 5)
				aOriPtr->GetScanLine(Ptrs[lines[5]], TPoint(0, yEndInt-5), aOriSizeX, aOriPtr->DisplayMode());
			if(yEndInt >= 6)
				aOriPtr->GetScanLine(Ptrs[lines[4]], TPoint(0, yEndInt-6), aOriSizeX, aOriPtr->DisplayMode());
			if(yEndInt >= 7)
				aOriPtr->GetScanLine(Ptrs[lines[3]], TPoint(0, yEndInt-7), aOriSizeX, aOriPtr->DisplayMode());
			if(yEndInt >= 8)
				aOriPtr->GetScanLine(Ptrs[lines[2]], TPoint(0, yEndInt-8), aOriSizeX, aOriPtr->DisplayMode());
			if(yEndInt >= 9)
				aOriPtr->GetScanLine(Ptrs[lines[1]], TPoint(0, yEndInt-9), aOriSizeX, aOriPtr->DisplayMode());
			if(yEndInt >= 10)
				aOriPtr->GetScanLine(Ptrs[lines[0]], TPoint(0, yEndInt-10), aOriSizeX, aOriPtr->DisplayMode());
		}
		LastLine = (TInt32)(yEndInt);

		// Loop y for result image
		for(y = (TInt32)yLoopSta; y < (TInt32)yLoopEnd; y++)
		{
			// Calculate used y pixels
			tmpSta = (TInt)(yRem - yAver + HALF_PIX + HALF_WEI);
			yStaInt = (TInt32)((tmpSta >> PIX_BITS) + yInt);
			yStaRem = (TUint32)(tmpSta & REMAINDER);

			tmpEnd = (TInt)(yRem + yAver + HALF_PIX + HALF_WEI);
			yEndInt = (TInt32)((tmpEnd >> PIX_BITS) + yInt);
			yEndRem = (TUint32)(tmpEnd & REMAINDER);

			//Read a new line from the source image if needed
			while (yEndInt > LastLine && LastLine < aOriSizeY)
			{
				LastLine++;
				tmpline = lines[0];
				lines[0] = lines[1];
				lines[1] = lines[2];
				lines[2] = lines[3];
				lines[3] = lines[4];
				lines[4] = lines[5];
				lines[5] = lines[6];
				lines[6] = lines[7];
				lines[7] = lines[8];
				lines[8] = lines[9];
				lines[9] = lines[10];
				lines[10] = tmpline;

				if(LastLine >= 0)
					aOriPtr->GetScanLine(Ptrs[lines[10]], TPoint(0, LastLine), aOriSizeX, aOriPtr->DisplayMode());
			}

			//Set pixel pointer to beginning of destination line
			DataPtr = (TUint8*)scanLine->Des().Ptr();

			// Calculate column weights and weight sum
			yStaWei = (TUint32)((PIXEL - yStaRem - 1) >> DIF1_BITS);
			yEndWei = (TUint32)((yEndRem) >> DIF1_BITS);
			yAllWei = (TUint32)(yStaWei + ((yEndInt - yStaInt - 1) << WEI_BITS) + yEndWei);

			// Check that the weight sum is not too big
			if(yAllWei > yMaxWei)
			{
				if(((yEndRem) & REM_HDIF1) > ((PIXEL - yStaRem) & REM_HDIF1))
				{
					yStaWei -= 1;
				}
				else
				{
					yEndWei -= 1;
				}
				yAllWei -= 1;
			}

			// Initialise x position
			xInt = xFirstInt;
			xRem = xFirstRem;

			// Calculate only the panned image
			if(newShiftX > 0)
			{
				tmpPtr = DataPtr;	
				tempPtr = tmpPtr + newShiftX * 3;

				// Copy available image to the beginning of line
				for(x = 0; x < (TInt32)xLoopSta; x++)
				{
					*tmpPtr++ = *tempPtr++;
					*tmpPtr++ = *tempPtr++;
					*tmpPtr++ = *tempPtr++;	
				}
			}
			else if(newShiftX < 0)
			{
				tmpPtr = DataPtr + 3 * aOutSizeX - 1;	
				tempPtr = tmpPtr + newShiftX * 3;

				// Copy available image to the end of line
				for(x = (TInt32)(aOutSizeX - 1); x >= (TInt32)xLoopEnd; x--)
				{
					*tmpPtr-- = *tempPtr--;
					*tmpPtr-- = *tempPtr--;
					*tmpPtr-- = *tempPtr--;	
				}
			}

				LineNum = (TUint32)((yStaInt - LastLine + 10) % 11);

			// Loop x for result image
			for(x = (TInt32)xLoopSta; x < (TInt32)xLoopEnd; x++)
			{
				// Calculate used x pixels
				tmpSta = (TInt)(xRem - xAver + HALF_PIX + HALF_WEI);
				xStaInt = (TInt32)((tmpSta >> PIX_BITS) + xInt);
				xStaRem = (TUint32)((tmpSta & REMAINDER));

				tmpEnd = (TInt)(xRem + xAver + HALF_PIX + HALF_WEI);
				xEndInt = (TInt32)((tmpEnd >> PIX_BITS) + xInt);
				xEndRem = (TUint32)(tmpEnd & REMAINDER);

				// Calculate line weights and weight sum
				xStaWei = (TUint32)((PIXEL - xStaRem - 1) >> DIF1_BITS);
				xEndWei = (TUint32)((xEndRem) >> DIF1_BITS);
				xAllWei = (TUint32)(xStaWei + ((xEndInt - xStaInt - 1) << WEI_BITS) + xEndWei);

				// Check that the weight sum is not too big
				if(xAllWei > xMaxWei)
				{
					if(((xEndRem) & REM_HDIF1) > ((PIXEL - xStaRem) & REM_HDIF1))
					{
						xStaWei -= 1;
					}
					else
					{
						xEndWei -= 1;
					}
					xAllWei -= 1;
				}

				// Calculate filter divider for filter window
				divider = (TInt)((xAllWei * yAllWei + HALF_RED) >> RED_BITS);

				// Calculate pixel values
				outFlag = 0;

				// Initialise block result
				sumB = 0;
				sumG = 0;
				sumR = 0;

				LineNum = (TUint32)((yStaInt - LastLine + 10) % 11);

				// Accumulate first line
				if(yStaWei != 0)
				{
					// Line number
					if(yStaInt < 0)		   		 outFlag = 1;
					else if(yStaInt > aOriSizeY) outFlag = 1;
					else
					{
						// Initialise line result
						tmpB = 0;
						tmpG = 0;
						tmpR = 0;

						// First pixel in first line
						if(xStaWei != 0)
						{
							// Column number
							if(xStaInt < 0)				 outFlag = 1;
							else if(xStaInt >= aOriSizeX) outFlag = 1;
							else
							{
								// Pixel weighting to line result
								tmpB = (TInt)(tmpB + (*(linePtrs[lines[LineNum]] + 3 * xStaInt))     * xStaWei);
								tmpG = (TInt)(tmpG + (*(linePtrs[lines[LineNum]] + 3 * xStaInt + 1)) * xStaWei);
								tmpR = (TInt)(tmpR + (*(linePtrs[lines[LineNum]] + 3 * xStaInt + 2)) * xStaWei);
							}
						}

						// Middle pixels in first line
						for(i = (TInt32)(xStaInt + 1); i < xEndInt; i++)
						{
							// Column number
							if(i < 0)			   outFlag = 1;
							else if(i >= aOriSizeX) outFlag = 1;
							else
							{
								// Pixel weighting to line result
								tmpB = (TInt)(tmpB + ((*(linePtrs[lines[LineNum]] + 3 * i    )) << WEI_BITS));
								tmpG = (TInt)(tmpG + ((*(linePtrs[lines[LineNum]] + 3 * i + 1)) << WEI_BITS));
								tmpR = (TInt)(tmpR + ((*(linePtrs[lines[LineNum]] + 3 * i + 2)) << WEI_BITS));
							}
						}

						// Last pixel in first line
						if(xEndWei != 0 && xEndInt < aOriSizeX)
						{
							// Column number
							if(xEndInt < 0)				 outFlag = 1;
							else
							{
								// Pixel weighting to line result
								tmpB = (TInt)(tmpB + (*(linePtrs[lines[LineNum]] + 3 * xEndInt    )) * xEndWei);
								tmpG = (TInt)(tmpG + (*(linePtrs[lines[LineNum]] + 3 * xEndInt + 1)) * xEndWei);
								tmpR = (TInt)(tmpR + (*(linePtrs[lines[LineNum]] + 3 * xEndInt + 2)) * xEndWei);
							}
						}

						// Pixel weighting to block result
						sumB = (TInt)(sumB + ((yStaWei * tmpB + HALF_RED) >> RED_BITS));
						sumG = (TInt)(sumG + ((yStaWei * tmpG + HALF_RED) >> RED_BITS));
						sumR = (TInt)(sumR + ((yStaWei * tmpR + HALF_RED) >> RED_BITS));
					}
				}
				LineNum++;

				// Accumulate middle lines
				for(j = (TInt32)(yStaInt + 1); j < yEndInt; j++)
				{
					// Line number 
					if(j < 0)			   outFlag = 1;
					else if(j > aOriSizeY) outFlag = 1;
					else
					{
						// Initialise line result
						tmpB = 0;
						tmpG = 0;
						tmpR = 0;

						// First pixel in middle lines
						if(xStaWei != 0)
						{
							// Column number
							if(xStaInt < 0)				 outFlag = 1;
							else if(xStaInt >= aOriSizeX) outFlag = 1;
							else
							{
								// Pixel weighting to line result
								tmpB = (TInt)(tmpB + (*(linePtrs[lines[LineNum]] + 3 * xStaInt    )) * xStaWei);
								tmpG = (TInt)(tmpG + (*(linePtrs[lines[LineNum]] + 3 * xStaInt + 1)) * xStaWei);
								tmpR = (TInt)(tmpR + (*(linePtrs[lines[LineNum]] + 3 * xStaInt + 2)) * xStaWei);
							}
						}

						// Middle pixels in middle lines
						for(i = (TInt32)(xStaInt + 1); i < xEndInt; i++)
						{
							// Column number
							if(i < 0)			   outFlag = 1;
							else if(i >= aOriSizeX) outFlag = 1;
							else
							{
								// Pixel weighting to line result
								tmpB = (TInt)(tmpB + ((*(linePtrs[lines[LineNum]] + 3 * i    )) << WEI_BITS));
								tmpG = (TInt)(tmpG + ((*(linePtrs[lines[LineNum]] + 3 * i + 1)) << WEI_BITS));
								tmpR = (TInt)(tmpR + ((*(linePtrs[lines[LineNum]] + 3 * i + 2)) << WEI_BITS));
							}
						}

						// Last pixel in middle lines
						if(xEndWei != 0 && xEndInt < aOriSizeX)
						{
							// Column number
							if(xEndInt < 0)				 outFlag = 1;
							else
							{
								// Pixel weighting to line result
								tmpB = (TInt)(tmpB + (*(linePtrs[lines[LineNum]] + 3 * xEndInt    )) * xEndWei);
								tmpG = (TInt)(tmpG + (*(linePtrs[lines[LineNum]] + 3 * xEndInt + 1)) * xEndWei);
								tmpR = (TInt)(tmpR + (*(linePtrs[lines[LineNum]] + 3 * xEndInt + 2)) * xEndWei);
							}
						}

						// Pixel weighting to block result
						sumB = (TInt)(sumB + (((tmpB << WEI_BITS) + HALF_RED) >> RED_BITS));
						sumG = (TInt)(sumG + (((tmpG << WEI_BITS) + HALF_RED) >> RED_BITS));
						sumR = (TInt)(sumR + (((tmpR << WEI_BITS) + HALF_RED) >> RED_BITS));
					}
					LineNum++;
				}


				// Accumulate last line
				if(yEndWei != 0)
				{
					// Line number 
					if(yEndInt < 0)				 outFlag = 1;
					else if(yEndInt > aOriSizeY) outFlag = 1;
					else
					{
						// Initialise line result
						tmpB = 0;
						tmpG = 0;
						tmpR = 0;

						// First pixel in last line
						if(xStaWei != 0)
						{
							// Column number
							if(xStaInt < 0)				 outFlag = 1;
							else if(xStaInt >= aOriSizeX) outFlag = 1;
							else
							{
								tmpB = (TInt)(tmpB + (*(linePtrs[lines[LineNum]] + 3 * xStaInt    )) * xStaWei);
								tmpG = (TInt)(tmpG + (*(linePtrs[lines[LineNum]] + 3 * xStaInt + 1)) * xStaWei);
								tmpR = (TInt)(tmpR + (*(linePtrs[lines[LineNum]] + 3 * xStaInt + 2)) * xStaWei);
							}
						}

						// Middle pixels in last line
						for(i = (TInt32)(xStaInt + 1); i < xEndInt; i++)
						{
							// Column number
							if(i < 0)			   outFlag = 1;
							else if(i >= aOriSizeX) outFlag = 1;
							else
							{
								tmpB = (TInt)(tmpB + ((*(linePtrs[lines[LineNum]] + 3 * i    )) << WEI_BITS));
								tmpG = (TInt)(tmpG + ((*(linePtrs[lines[LineNum]] + 3 * i + 1)) << WEI_BITS));
								tmpR = (TInt)(tmpR + ((*(linePtrs[lines[LineNum]] + 3 * i + 2)) << WEI_BITS));
							}
						}

						// Last pixel in last line
						if(xEndWei != 0 && xEndInt < aOriSizeX)
						{
							// Column number
							if(xEndInt < 0)				 outFlag = 1;
							else
							{
								tmpB = (TInt)(tmpB + (*(linePtrs[lines[LineNum]] + 3 * xEndInt    )) * xEndWei);
								tmpG = (TInt)(tmpG + (*(linePtrs[lines[LineNum]] + 3 * xEndInt + 1)) * xEndWei);
								tmpR = (TInt)(tmpR + (*(linePtrs[lines[LineNum]] + 3 * xEndInt + 2)) * xEndWei);
							}
						}

						// Pixel weighting to block result
						sumB = (TInt)(sumB + ((yEndWei * tmpB + HALF_RED) >> RED_BITS));
						sumG = (TInt)(sumG + ((yEndWei * tmpG + HALF_RED) >> RED_BITS));
						sumR = (TInt)(sumR + ((yEndWei * tmpR + HALF_RED) >> RED_BITS));
					}
				}
				LineNum++;


				// Pixels outside the original image are needed
				if(outFlag > 0 || divider == 0)
				{
					// Save output values
					*(DataPtr + x * 3    ) = 255;
					*(DataPtr + x * 3 + 1) = 255;
					*(DataPtr + x * 3 + 2) = 255;
				}

				// Pixels are inside the original image
				else
				{
					if(divider == 1)
					{
						tmpB = sumB;
						tmpG = sumG;
						tmpR = sumR;
					}
					else if(divider == 2)
					{
						tmpB = (TInt)((sumB + 1) >> 1);
						tmpG = (TInt)((sumG + 1) >> 1);
						tmpR = (TInt)((sumR + 1) >> 1);
					}
					else if(divider == 4)
					{
						tmpB = (TInt)((sumB + 2) >> 2);
						tmpG = (TInt)((sumG + 2) >> 2);
						tmpR = (TInt)((sumR + 2) >> 2);
					}
					else if(divider == 8)
					{
						tmpB = (TInt)((sumB + 4) >> 3);
						tmpG = (TInt)((sumG + 4) >> 3);
						tmpR = (TInt)((sumR + 4) >> 3);
					}
					else if(divider == 16)
					{
						tmpB = (TInt)((sumB + 8) >> 4);
						tmpG = (TInt)((sumG + 8) >> 4);
						tmpR = (TInt)((sumR + 8) >> 4);
					}
					else if(divider == 32)
					{
						tmpB = (TInt)((sumB + 16) >> 5);
						tmpG = (TInt)((sumG + 16) >> 5);
						tmpR = (TInt)((sumR + 16) >> 5);
					}
					else if(divider == 64)
					{
						tmpB = (TInt)((sumB + 32) >> 6);
						tmpG = (TInt)((sumG + 32) >> 6);
						tmpR = (TInt)((sumR + 32) >> 6);
					}
					else if(divider == 128)
					{
						tmpB = (TInt)((sumB + 64) >> 7);
						tmpG = (TInt)((sumG + 64) >> 7);
						tmpR = (TInt)((sumR + 64) >> 7);
					}
					else if(divider == 256)
					{
						tmpB = (TInt)((sumB + 128) >> 8);
						tmpG = (TInt)((sumG + 128) >> 8);
						tmpR = (TInt)((sumR + 128) >> 8);
					}
					else if(divider == 512)
					{
						tmpB = (TInt)((sumB + 256) >> 9);
						tmpG = (TInt)((sumG + 256) >> 9);
						tmpR = (TInt)((sumR + 256) >> 9);
					}
					else if(divider == 1024)
					{
						tmpB = (TInt)((sumB + 512) >> 10);
						tmpG = (TInt)((sumG + 512) >> 10);
						tmpR = (TInt)((sumR + 512) >> 10);
					}
					else
					{
						tmpB = (TInt)(((sumB * KDivTable[divider - 2]) + 32768) >> 16);
						tmpG = (TInt)(((sumG * KDivTable[divider - 2]) + 32768) >> 16);
						tmpR = (TInt)(((sumR * KDivTable[divider - 2]) + 32768) >> 16);
					}

					// Save output values
					if(tmpB > 255) 
						*(DataPtr + x * 3    ) = 255;
					else           
						*(DataPtr + x * 3    ) = (TUint8)(tmpB);
					if(tmpG > 255) 
						*(DataPtr + x * 3 + 1) = 255;
					else           
						*(DataPtr + x * 3 + 1) = (TUint8)(tmpG);
					if(tmpR > 255) 
						*(DataPtr + x * 3 + 2) = 255;
					else           
						*(DataPtr + x * 3 + 2) = (TUint8)(tmpR);
				}

				// Update x position
				tmpEnd = (TInt)(xRem + xStep);
				xInt = (TInt32)(xInt + (tmpEnd >> PIX_BITS));
				xRem = (TUint32)(tmpEnd & REMAINDER);
			}

			//Set processed line
			aOutPtr->SetScanLine(linePtr, y);

			// Update y position
			tmpEnd = (TInt)(yRem + yStep);
			yInt = (TInt32)(yInt + (tmpEnd >> PIX_BITS));
			yRem = (TUint32)(tmpEnd & REMAINDER);
		}
		
	// Delete local temporal line buffers (pop from CleanupStack)
	CleanupStack::PopAndDestroy(12);	// scanLine, oriLine0 and oriLine1
}




/*
-----------------------------------------------------------------------------

  SetParams
	
  Set processing parameters
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDCDigitalZoom::SetParameters(DCDigitalZoomParams* params)
{
	iParams = *params;
}



/*
-----------------------------------------------------------------------------

  GetParams
	
  Get current processing parameters
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDCDigitalZoom::GetParameters(DCDigitalZoomParams* params)
{
	*params = iParams;
}
//----IMAAMI----
