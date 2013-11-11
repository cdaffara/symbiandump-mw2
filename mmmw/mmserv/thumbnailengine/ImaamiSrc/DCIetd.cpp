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
*	CDCIETD 
*		Display specific color contrast enhancement, 
*		Image Enhancement for Transflective Displays version 2,
*		IETD 2.
*
*/



//  Include Files  
#include <e32std.h>	// The basic definitions
#include <fbs.h>	// For FBS bitmap
#include "DCIetd.h"	// The DCIetd class


//  MEMBER FUNCTIONS
//=============================================================================




/*
-----------------------------------------------------------------------------

  CDCIetd
  
  Constructor
	
  Default constructor, initializes member variables to initial values
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
CDCIetd::CDCIetd()
{
	// Set default values for parameters (from init file)
	iParameters.aWhitePixels = WhitePixels;
	iParameters.aBlackPixels = BlackPixels;
	iParameters.aStretchLimit = StretchLimit;
	iParameters.aSaturationGain = SaturationGain;
	iParameters.aBitLimit = BitLimit;
	iParameters.aWBC = WBC;
	iParameters.aDBC = DBC;
}




/*
-----------------------------------------------------------------------------

  CDCIetd
  
  NewLC
	
  Factory function to instantiate the class.
  This function leaves the class pointer to the cleanup stack
  May leave with KErrNoMemory if no memory available
  
  Return Values:  CDCIetd* self:  pointer to the class instance

-----------------------------------------------------------------------------
*/
CDCIetd* CDCIetd::NewLC()
{
    CDCIetd* self = new (ELeave) CDCIetd();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}




/*
-----------------------------------------------------------------------------

  CDCIetd
  
  NewL
	
  Factory function to instantiate the class.
  May leave with KErrNoMemory if no memory available
	  
  Return Values:  CDCIetd* self:  pointer to the class instance
	
-----------------------------------------------------------------------------
*/
CDCIetd* CDCIetd::NewL()
{
    CDCIetd* self = CDCIetd::NewLC();
    CleanupStack::Pop();
    return self;
}




/*
-----------------------------------------------------------------------------

  CDCIetd
  
  ConstructL
	
  Second phase constructor. Does nothing at the moment
	  
  Return Values:  none
		
 -----------------------------------------------------------------------------
*/
void CDCIetd::ConstructL()
{
    // This function is intentionally left blank.
}




/*
-----------------------------------------------------------------------------

  CDCIetd
  
  Destructor
	
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
CDCIetd::~CDCIetd()
{
    // This function is intentionally left blank.
}




/*
-----------------------------------------------------------------------------

  CDCIetd
  
  Analyze
	
  Analyze image referenced by aBPtr 
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
// Analyze image referenced by aBPtr
void CDCIetd::Analyze(CFbsBitmap& aBPtr)
{

	//EColor16M image is needed
	if (aBPtr.DisplayMode() != EColor16M) return;
	
	//Do analysis
	GatherHistograms(aBPtr);
	MakeMappings();
}




/*
-----------------------------------------------------------------------------

  CDCIetd
  
  ProcessL
	
  Process image referenced by aImage (modify aImage).
  May leave with KErrNoMemory if no memory available
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDCIetd::ProcessL (CFbsBitmap& aImage)  // image reference
{
	TUint	r, g, b;	// Color components
	TUint	lum;		// Brightness estimate
	TInt	dr, dg, db;	// Differences to brightness
	TUint8*	dataPtr;	// Pointer to data
	
	//EColor16M image is needed
	if (aImage.DisplayMode() != EColor16M) return;
	
	//Line Buffer and pointer to the data
	TUint imageWidth = aImage.SizeInPixels().iWidth;
	TUint scanLineLengthInBytes = aImage.ScanLineLength(imageWidth, aImage.DisplayMode());

	//Allocate buffer for scanline
	iScanLineBuffer = HBufC8::NewMaxL(scanLineLengthInBytes);
	//Pointer to scanline
	TPtr8 linePtr = iScanLineBuffer->Des();
	
	//Step through image pixels and do stretching
	//and saturation increase
	//---------------------------------------------

	//Read all lines
	for (TInt lineNo=0; lineNo<aImage.SizeInPixels().iHeight; ++lineNo)
	{
		//Get line
		aImage.GetScanLine(linePtr, TPoint(0, lineNo), imageWidth, aImage.DisplayMode());
		//CHECK! CONST_CAST not used in every algorithm which way is better?
		dataPtr = CONST_CAST(TUint8*, linePtr.Ptr());
	
		//Step through pixels in line
		for (TUint x=0; x < imageWidth; ++x)
		{
			// Map color componets according to mapping LUTs
			b = iMap[2][*dataPtr++];
			g = iMap[1][*dataPtr++];
			r = iMap[0][*dataPtr++];
			
			//Compute brightness estimate
			//lum=0.299r+0.587g+0.114b; //true Y
			//lum=(32768+19595*r+38470*g+7471*b)>>16; //Y
			//lum = (r+g+b)/3; //Simple approximation
			lum=(r+(g<<1)+b)>>2; //More effective simple approximation
			
			//Compute componentwise differences to luminance
			dr = r-lum;
			dg = g-lum;
			db = b-lum;
			
			//Increase differences => saturation increases.
			//Use gain parameter for adjusting the strength of the effect.
			b += iParameters.aSaturationGain*db/32;
			g += iParameters.aSaturationGain*dg/32;
			r += iParameters.aSaturationGain*dr/32;
			
			//Save data to same image & same pixels
			dataPtr -= 3;
						
			//Limit to available dynamic range [0,255].
			*dataPtr++ = Limit255(b);
			*dataPtr++ = Limit255(g);
			*dataPtr++ = Limit255(r);
		}
		
		//Save line
		aImage.SetScanLine(linePtr, lineNo);
	}

	//Free memory
	delete(iScanLineBuffer);
	iScanLineBuffer = 0;
}




/*
-----------------------------------------------------------------------------

  CDCIetd
  
  GatherHistograms
	
  Gather histograms and make cumulative histogram.
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDCIetd::GatherHistograms (const CFbsBitmap& aImage)  // Pointer to the image bitmap
{
	const TUint8* dataPtr; //Pointer to data
	TInt lineNo; //Line number
	TUint x;	 //Pixel index	
	TUint color; //Color index
	TUint count; // Number of colors in each component

	//Compute image width & allocate scan line memory
	TUint imageWidth = aImage.SizeInPixels().iWidth;
	TUint histScanLineLengthInBytes = aImage.ScanLineLength(imageWidth, aImage.DisplayMode());
	iScanLineBuffer = HBufC8::NewMaxL(histScanLineLengthInBytes);
	
	//Pointer to line
	TPtr8 linePtr = iScanLineBuffer->Des();

	//Clear histograms
	Mem::FillZ(iHistogram, sizeof(iHistogram));

	// Read all lines and gather histograms
	for (lineNo=0; lineNo<aImage.SizeInPixels().iHeight; lineNo++)
	{
		//Get line
		aImage.GetScanLine(linePtr, TPoint(0, lineNo), imageWidth, aImage.DisplayMode());
		dataPtr = linePtr.Ptr();

		//Step through pixels
		for (x=imageWidth; x != 0; --x)
		{
			++iHistogram[2][*dataPtr++]; // Increase Blue bin value
			++iHistogram[1][*dataPtr++]; // Increase Green bin value
			++iHistogram[0][*dataPtr++]; // Increase Red bin value
		}
	}
	
	//Make cumulative histograms & count colors in each histogram
	for (color=0; color<3; ++color)
	{
		 // Count used colors
		count=0;
		for (x=0; x<256; ++x)
		{
			if (iHistogram[color][x]>0) count++;
		}
		
		// Compute increased stretch limit if a color component has less colors than iBitLimit.
		// Otherwise use predetermined stretch limit.
		if (count<iParameters.aBitLimit)
			iReducedStretchLimit[color] = (TUint8)(iParameters.aStretchLimit*count/iParameters.aBitLimit+255-255*count/iParameters.aBitLimit);
		else
			iReducedStretchLimit[color] = iParameters.aStretchLimit;
		
		//Make cumulative histogram
		for (x=1; x<256; ++x)
			iHistogram[color][x] += iHistogram[color][x-1];
		
	}
	
	//Free memory
	delete(iScanLineBuffer);
	iScanLineBuffer = 0;
}




/*
-----------------------------------------------------------------------------

  CDCIetd
  
  MakeMappings
	
  Make mapping function look-up table (LUT).
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDCIetd::MakeMappings()
{
	TUint    MinBins[3];// Smallest existing values in histogram
	TUint    MaxBins[3];// Largest existing values in histogram
	TUint    minBin;    // Minimum of smallest existing values
	TUint    maxBin;    // Maximum of largest existing values
	TUint    x;         // Index
	
	// Stretching limit variables
	TUint    minShift;
	TUint    maxShift;
	TUint    totalShift;
	
	TUint    color; //Color index
	
	//Step through colors
	for (color=0; color<3; ++color)
	{
		// Find smallest existing values in histograms, discard darkest pixels
		// according to blackpixels parameter
		x = 0; // Start from fist bin
		MinBins[color] = iParameters.aBlackPixels * iHistogram[color][255]/1000; // Compute value to be found
		while (x < 255 && (TUint)iHistogram[color][x] < MinBins[color])
			++x; // Find from histogram
		
		MinBins[color] = x; // Save bin index = start of stretching part of LUT
		
		// Find largest existing values in histograms, discard brightest pixels
		// according to whitepixels parameter
		x = 255;  // Start from last bin
		//Compute value to be found
		MaxBins[color] = iHistogram[color][255] - iParameters.aWhitePixels * iHistogram[color][255]/1000;
		while (x > 0 && (TUint)iHistogram[color][x] > MaxBins[color])
			--x; // Find from histogram
		
		MaxBins[color] = x; // Save bin index = end of stretching part of LUT
	}
	
	//Find minimum of all colors
	minBin=255;
	for (color=0; color<3; color++)
	{
		if (minBin>MinBins[color]) minBin=MinBins[color];
	}
	
	//Find maximum of all colors
	maxBin=0;
	for (color=0; color<3; color++)
	{
		if (maxBin<MaxBins[color]) maxBin=MaxBins[color];
	}
	
	//Adjust white and dark balance within limits given in parameters (maximum correction).
	//0 means that largest(or smallest) of all values is used => no WBC(or DBC).
	for (color=0; color<3; color++)
	{
		if(maxBin-MaxBins[color]>iParameters.aWBC) MaxBins[color]=maxBin-iParameters.aWBC;
		if((MinBins[color]-minBin) > iParameters.aDBC) MinBins[color]=minBin+iParameters.aDBC;
	}
	
	//Step through color components
	for (color=0; color<3; color++)
	{
		// If histogram has only one nonzero bin maxBin can be less than minBin.
		// In that case change maxBin value to minBin.
		if(MaxBins[color]<MinBins[color]) MaxBins[color]=MinBins[color];
		
		// Limit stretching to narrovest histogram that can be stretched
		if (MaxBins[color]-MinBins[color] < iReducedStretchLimit[color])
		{
			// Compute limiting shifts to measured values.
			// Compute shifts for dark and bright end in relation
			// to coresponding available space in dynamic range.
			totalShift = iReducedStretchLimit[color]-(MaxBins[color]-MinBins[color]);
			maxShift = totalShift*(255-MaxBins[color])/(255-(MaxBins[color]-MinBins[color]));
			minShift = totalShift*MinBins[color]/(255-(MaxBins[color]-MinBins[color]));
			
			// Check that dynamic range is not exceeded
			// (Should happen only with faulty parameter values)
			if(minShift > MinBins[color])
				MinBins[color]=0;
			else
			// Shift measured values, so that stretching is limited 
			MinBins[color] -= minShift;
			// Shift measured values, so that stretching is limited 
			MaxBins[color] += maxShift;
			
			// Check that dynamic range is not exceeded
			// (Should happen only with faulty parameter values)
			if (MaxBins[color]>255)
				MaxBins[color]=255;
		}
		
		// Set 0 mapping part of the LUT
		for (x=0; x<=MinBins[color]; ++x)
			iMap[color][x] = 0;
		
		// Set 255 mapping part of the LUT
		for (x=MaxBins[color]; x<=255; ++x)
			iMap[color][x] = 255;
		
		// Compute linear stretching part of the LUT
		for (x=MinBins[color]+1; x<MaxBins[color]; x++)
			iMap[color][x] = (TUint8)(255*(x-MinBins[color])/(MaxBins[color]-MinBins[color]));
	}
}



/*
-----------------------------------------------------------------------------

  CDCIetd
  
  SetParams
	
  Set processing parameters
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDCIetd::SetParams(DCIetdParameters* params)
{
	iParameters = *params;
}




/*
-----------------------------------------------------------------------------

  CDCIetd
  
  GetParams
	
  Get current processing parameters
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDCIetd::GetParams(DCIetdParameters* params)
{
	*params = iParameters;
}
//----IMAAMI----
