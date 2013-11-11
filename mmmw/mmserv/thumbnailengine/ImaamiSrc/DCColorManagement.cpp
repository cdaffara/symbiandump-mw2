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
* Description:   Implementation for TNE processor
*
*/



#include <fbs.h>
#include <s32file.h>
#include <stdio.h>
#include "DCColorManagement.h"



/*
-----------------------------------------------------------------------------

  CDCColorManagement
  
  Constructor
	
  Default constructor, initializes member variables to initial values
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
CDCColorManagement::CDCColorManagement()
{
	//Set default values for parameters
	//Set pointer to internal data (LUTs and matrix memory locations)
	iParams.GammaR = iGammaR;
	iParams.GammaG = iGammaG;
	iParams.GammaB = iGammaB;
	iParams.Matrix = iMatrix;
	iParams.TRCR = iTRCR;
	iParams.TRCG = iTRCG;
	iParams.TRCB = iTRCB;
}



/*
-----------------------------------------------------------------------------
	
  NewLC
	
  Factory function to instantiate the class.
  This function leaves the class pointer to the cleanup stack
  May leave with KErrNoMemory if no memory available
  
  Return Values:  CDCColorManagement* self:  pointer to the class instance

-----------------------------------------------------------------------------
*/
CDCColorManagement* CDCColorManagement::NewLC()
{
	CDCColorManagement* self = new (ELeave) CDCColorManagement();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}



/*
-----------------------------------------------------------------------------

  NewL
	
  Factory function to instantiate the class.
  May leave with KErrNoMemory if no memory available
	  
  Return Values:  CDCColorManagement* self:  pointer to the class instance
	
-----------------------------------------------------------------------------
*/
CDCColorManagement* CDCColorManagement::NewL()
{
	CDCColorManagement* self = CDCColorManagement::NewLC();
	CleanupStack::Pop();
	return self;
}




/*
-----------------------------------------------------------------------------

  ConstructL
	
  Second phase constructor. Does nothing at the moment
	  
  Return Values:  none
		
 -----------------------------------------------------------------------------
*/
void CDCColorManagement::ConstructL()
{
	// This function is intentionally left blank.
}



/*
-----------------------------------------------------------------------------

  Destructor
	
  Deletes the scanline buffer
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
CDCColorManagement::~CDCColorManagement()
{
	// This function is intentionally left blank.
}




/*
-----------------------------------------------------------------------------

  ProcessL
	
  Process image referenced by aBPtr (modify aBPtr).
  May leave with KErrNoMemory if no memory available
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDCColorManagement::ProcessL(CFbsBitmap* aBPtr)
{

	TInt	r, g, b, rr, gg, bb;	// Color components
	TUint8* DataPtr;	// Pointer to image data
	
	//Check image mode
	if (aBPtr->DisplayMode() != EColor16M) 
		return;
	
	// Line Buffer and pointer to the data
	TUint LineLength = aBPtr->SizeInPixels().iWidth;
	iScanLine = HBufC8::NewMaxL(LineLength*3);
	TPtr8 LinePtr((TUint8*)iScanLine->Des().Ptr(), LineLength*3);
	
	// Step through image lines
	for (TInt LineNo=0; LineNo<aBPtr->SizeInPixels().iHeight; ++LineNo)
	{
		//Get line from image and set data pointer
		aBPtr->GetScanLine(LinePtr, TPoint(0, LineNo), aBPtr->SizeInPixels().iWidth, aBPtr->DisplayMode());
		DataPtr = (TUint8 *)iScanLine->Des().Ptr();

		//Step through image pixels
		for (TUint x=aBPtr->SizeInPixels().iWidth; x != 0; --x)
		{
			//Get the color components and map them using linearization LUTs
			b=iParams.GammaR[*DataPtr];
			g=iParams.GammaG[*(DataPtr+1)];
			r=iParams.GammaB[*(DataPtr+2)];

			//Process with 3x3 matrix
			rr=(r*iParams.Matrix[0] + g*iParams.Matrix[3*0+1] + b*iParams.Matrix[3*0+2])>>16;
			gg=(r*iParams.Matrix[3*1+0] + g*iParams.Matrix[3*1+1] + b*iParams.Matrix[3*1+2])>>16;
			bb=(r*iParams.Matrix[3*2+0] + g*iParams.Matrix[3*2+1] + b*iParams.Matrix[3*2+2])>>16;

			//Limit between 0...255
			if (rr<0) rr=0; if (rr>255) rr=255;
			if (gg<0) gg=0; if (gg>255) gg=255;
			if (bb<0) bb=0; if (bb>255) bb=255;

			//Process with TRC LUTs
			rr=iParams.TRCR[rr];
			gg=iParams.TRCG[gg];
			bb=iParams.TRCB[bb];

			//Set color components
			*DataPtr = (TUint8)bb;
			++DataPtr;
			*DataPtr = (TUint8)gg;
			++DataPtr;
			*DataPtr = (TUint8)rr;
			++DataPtr;
		}

		//Set processed line
		aBPtr->SetScanLine(LinePtr, LineNo);
	}

	//Free memory
	delete(iScanLine);
	iScanLine = 0;
}




/*
-----------------------------------------------------------------------------

  SetParams
	
  Set processing parameters
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDCColorManagement::SetParameters(DCColorManagementParams* params)
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
void CDCColorManagement::GetParameters(DCColorManagementParams* params)
{
	*params = iParams;
}
//----IMAAMI----
