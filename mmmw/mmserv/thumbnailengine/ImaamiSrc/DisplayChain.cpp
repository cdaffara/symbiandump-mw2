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
*        CDisplayChain 
*		Image processing class implementing
*		display specific image processing:
*			- Scaling to display size with pan support
*			- IETD - display specific color contrast enhancement
*			- Sharpening 
*			- Display feature compensation by simple color management
*			- Dithering 
*
*/



#include <fbs.h>
#include "DisplayChain.h"


/*
-----------------------------------------------------------------------------

  Constructor
	
  Default constructor, initializes member variables to initial values
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
CDisplayChain::CDisplayChain()
{
	// This function is intentionally left blank.
}




/*
-----------------------------------------------------------------------------

  NewLC
	
  Factory function to instantiate the class.
  This function leaves the class pointer to the cleanup stack
  May leave with KErrNoMemory if no memory available
  
  Return Values:  CDisplayChain* self:  pointer to the class instance

-----------------------------------------------------------------------------
*/
CDisplayChain* CDisplayChain::NewLC()
{
	CDisplayChain* self = new (ELeave) CDisplayChain();
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
	  
  Return Values:  CDisplayChain* self:  pointer to the class instance
	
-----------------------------------------------------------------------------
*/
CDisplayChain* CDisplayChain::NewL()
{
	CDisplayChain* self = CDisplayChain::NewLC();
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
void CDisplayChain::ConstructL()
{
	// Create objects for ImaamiImageChain
	DigitalZoomPtr = CDCDigitalZoom::NewL();
	CleanupStack::PushL(DigitalZoomPtr);

	SharpeningPtr = DCSharpening::NewL();
	CleanupStack::PushL(SharpeningPtr);

	DitheringPtr = CDCDithering::NewL();
	CleanupStack::PushL(DitheringPtr);

	IetdPtr = CDCIetd::NewL();
	CleanupStack::PushL(IetdPtr);

	ColorManagementPtr = CDCColorManagement::NewL();
	CleanupStack::PushL(ColorManagementPtr);


	TInt i; //Index

	//As default image is just opened to be processed
	Parameters.FirstOpening = ETrue;

	//Get current processing parameters from processing objects
	DigitalZoomPtr->GetParameters(&Parameters.DZParameters);
	IetdPtr->GetParams(&Parameters.IETDParameters);
	SharpeningPtr->GetParameters(&Parameters.SharpeningParameters);
	ColorManagementPtr->GetParameters(&Parameters.ColorManagementParameters);

	//Here the parameters can be read for example from a text file or from the registers, etc.
	//if (!ReadFileL())
	{			
		//Set default parameter values
		//(Defined in DCInit.h)
		//----------------------------
		
		//Set each processing ON/OFF
		Parameters.DigitalZoomON = DZ;
		Parameters.DitheringON = DITHER;
		Parameters.ColorManagementON = ETrue; //CM;
		Parameters.IetdON = IETD;
		Parameters.SharpeningON = SHARP;
		
		//DigitalZoom
		DigitalZoomPtr->GetParameters(&Parameters.DZParameters);
		Parameters.DZParameters.sizeX = DZsizeX;
		Parameters.DZParameters.sizeY = DZsizeY;
		Parameters.DZParameters.scaleX = DZscaleX;
		Parameters.DZParameters.scaleY = DZscaleY;
		Parameters.DZParameters.allShiftX = DZallX;
		Parameters.DZParameters.allShiftY = DZallY;
		Parameters.DZParameters.newShiftX = DZnewX;
		Parameters.DZParameters.newShiftY = DZnewY;
		
		//IETD
		Parameters.IETDParameters.aStretchLimit = StretchLimit;
		Parameters.IETDParameters.aBlackPixels = BlackPixels;
		Parameters.IETDParameters.aWhitePixels = WhitePixels;
		Parameters.IETDParameters.aSaturationGain = SaturationGain;
		Parameters.IETDParameters.aBitLimit = BitLimit;
		Parameters.IETDParameters.aWBC = WBC;
		Parameters.IETDParameters.aDBC = DBC;
		
		//Sharpening
		Parameters.SharpeningParameters.SHARP_GAIN = SharpGain;
		Parameters.SharpeningParameters.SHARP_DZONE = SharpDZone;
		Parameters.SharpeningParameters.SHARP_OVER = SharpOver;
		
		//Color Management (fill LUTs and matrix)
		for(i=0;i<256;i++)
		{
			Parameters.ColorManagementParameters.GammaR[i] = CMGammaR[i];
		}
		
		for(i=0;i<256;i++)
		{
			Parameters.ColorManagementParameters.GammaG[i] = CMGammaG[i];
		}
		
		for(i=0;i<256;i++)
		{
			Parameters.ColorManagementParameters.GammaB[i] = CMGammaB[i];
		}
		
		for(i=0;i<256;i++)
		{
			Parameters.ColorManagementParameters.TRCR[i] = CMTRCR[i];
		}
		
		for(i=0;i<256;i++)
		{
			Parameters.ColorManagementParameters.TRCG[i] = CMTRCG[i];
		}
		
		for(i=0;i<256;i++)
		{
			Parameters.ColorManagementParameters.TRCB[i] = CMTRCB[i];
		}

		for(i=0;i<9;i++)
		{
			Parameters.ColorManagementParameters.Matrix[i] = CMMatrix[i];
		}
		
	}

	//Set the default parameters
	IetdPtr->SetParams(&Parameters.IETDParameters);
	SharpeningPtr->SetParameters(&Parameters.SharpeningParameters);
	ColorManagementPtr->SetParameters(&Parameters.ColorManagementParameters);

	CleanupStack::Pop(5); //Processing objects
}




/*
-----------------------------------------------------------------------------

  CDCIetd
  
  Destructor
	
  Deletes the allocated memory
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
CDisplayChain::~CDisplayChain()
{
	//Delete processing objects
	delete DigitalZoomPtr;
	delete SharpeningPtr;
	delete DitheringPtr;
	delete IetdPtr;
	delete ColorManagementPtr;
}



/*
-----------------------------------------------------------------------------

  ProcessL
	
  Process image referenced by InBitmap (modify aTargetBitmap).
  If scaling is not used aTargetBitmap is processed directly.
 
  May leave with KErrNoMemory if no memory available
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDisplayChain::ProcessL(const CFbsBitmap* InBitmap, CFbsBitmap* aTargetBitmap)
{

	//CHECK! pan check needed only when DZ is ON
	//Scaling
	if (Parameters.DigitalZoomON)
	{
		// refine the zooming parameters based on input data
		TSize inSize = InBitmap->SizeInPixels();
		TSize outSize = aTargetBitmap->SizeInPixels();
		Parameters.DZParameters.sizeX = outSize.iWidth;
		Parameters.DZParameters.sizeY = outSize.iHeight;
		Parameters.DZParameters.scaleX = (TReal)outSize.iWidth/(TReal)inSize.iWidth;
		Parameters.DZParameters.scaleY = (TReal)outSize.iHeight/(TReal)inSize.iHeight;

		//If the image is tried to pan over the image borders, bound to the borders
		if (Parameters.DZParameters.allShiftX < (-InBitmap->SizeInPixels().iWidth/2 * Parameters.DZParameters.scaleX + aTargetBitmap->SizeInPixels().iWidth/2))
		{
			Parameters.DZParameters.allShiftX = (TInt)((-InBitmap->SizeInPixels().iWidth/2)*Parameters.DZParameters.scaleX + aTargetBitmap->SizeInPixels().iWidth/2);
		}
		
		if (Parameters.DZParameters.allShiftX > (InBitmap->SizeInPixels().iWidth/2 * Parameters.DZParameters.scaleX - aTargetBitmap->SizeInPixels().iWidth/2))
		{
			Parameters.DZParameters.allShiftX = (TInt)((InBitmap->SizeInPixels().iWidth/2)*Parameters.DZParameters.scaleX - aTargetBitmap->SizeInPixels().iWidth/2);
		}
		
		if (Parameters.DZParameters.allShiftY < (-InBitmap->SizeInPixels().iHeight/2 * Parameters.DZParameters.scaleY + aTargetBitmap->SizeInPixels().iHeight/2))
		{
			Parameters.DZParameters.allShiftY = (TInt)((-InBitmap->SizeInPixels().iHeight/2)*Parameters.DZParameters.scaleY + aTargetBitmap->SizeInPixels().iHeight/2);
		}
		
		if (Parameters.DZParameters.allShiftY > (InBitmap->SizeInPixels().iHeight/2 * Parameters.DZParameters.scaleY - aTargetBitmap->SizeInPixels().iHeight/2))
		{
			Parameters.DZParameters.allShiftY = (TInt)((InBitmap->SizeInPixels().iHeight/2)*Parameters.DZParameters.scaleY - aTargetBitmap->SizeInPixels().iHeight/2);
		}
			
		DigitalZoomPtr->SetParameters(&Parameters.DZParameters);
		DigitalZoomPtr->ProcessL(InBitmap, aTargetBitmap);
	}
	
	//IETD 
	if (Parameters.IetdON)
	{
 		if (Parameters.FirstOpening)
		{
			IetdPtr->Analyze(*aTargetBitmap);
			Parameters.FirstOpening = EFalse;
		}
		//Set parameters is needed if default values can change (read from file etc.)
		//IetdPtr->SetParams(&Parameters.IETDParameters);
		IetdPtr->ProcessL(*aTargetBitmap);
	}
	
	//Sharpening
	if (Parameters.SharpeningON)
	{
		//Set parameters is needed if default values can change (read from file etc.)
		//SharpeningPtr->SetParameters(&Parameters.SharpeningParameters);
		SharpeningPtr->ProcessL(aTargetBitmap);
	}
	
	//ColorManagement
	if (Parameters.ColorManagementON)
	{
		//Set parameters is needed if default values can change (read from file etc.)
		//ColorManagementPtr->SetParameters(&Parameters.ColorManagementParameters);
		ColorManagementPtr->ProcessL(aTargetBitmap);
	}

	//Dithering
	if (Parameters.DitheringON)
	{
		DitheringPtr->ProcessL(*aTargetBitmap);
	}
}




/*
-----------------------------------------------------------------------------

  SetParams
	
  Set processing parameters
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDisplayChain::SetParameters(DisplayChainParams* params)
{
	//Copy parameter struct
	Parameters = *params;
}



/*
-----------------------------------------------------------------------------

  GetParams
	
  Get current processing parameters
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void CDisplayChain::GetParameters(DisplayChainParams* params)
{
	//Copy parameter struct
	*params = Parameters;
}


/*
-----------------------------------------------------------------------------

  ReadFileL
	
  Example function for reading processing parameters from file
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
TBool CDisplayChain::ReadFileL()
{
	
	FILE *fp;
	TInt i,j,data;
	TUint8 data_uchar;
	TReal data_real;
	
	// Open for read (will fail if file does not exist) 
	if((fp = fopen("e:\\DisplayChain.txt","r"))==NULL )
	{
		if((fp = fopen("c:\\DisplayChain.txt","r"))==NULL )
		{
			//AfxMessageBox("CMData.txt was not opened");
			return EFalse;
		}
	}
	
	//Digital Zoom
	fscanf(fp,"%d",&data);
	Parameters.DigitalZoomON = data;

	//Ietd
	fscanf(fp,"%d",&data);
	Parameters.IetdON = data;
	
	fscanf(fp,"%d",&data_uchar);
	Parameters.IETDParameters.aWhitePixels = data_uchar;
	
	fscanf(fp,"%d",&data_uchar);
	Parameters.IETDParameters.aBlackPixels = data_uchar;
	
	fscanf(fp,"%d",&data_uchar);
	Parameters.IETDParameters.aStretchLimit = data_uchar;
	
	fscanf(fp,"%d",&data_uchar);
	Parameters.IETDParameters.aSaturationGain = data_uchar;

	fscanf(fp,"%d",&data_uchar);
	Parameters.IETDParameters.aBitLimit = data_uchar;

	fscanf(fp,"%d",&data_uchar);
	Parameters.IETDParameters.aWBC = data_uchar;

	fscanf(fp,"%d",&data_uchar);
	Parameters.IETDParameters.aDBC = data_uchar;
	
	//Sharpening
	fscanf(fp,"%d",&data);
	Parameters.SharpeningON = data;
	
	fscanf(fp,"%d",&data);
	Parameters.SharpeningParameters.SHARP_OVER = data;
	
	fscanf(fp,"%d",&data);
	Parameters.SharpeningParameters.SHARP_DZONE = data;
	
	//fscanf(fp,"%f",&data_real);
	fscanf(fp,"%d",&data);
	data_real = ((TReal)data)/65536;
	Parameters.SharpeningParameters.SHARP_GAIN = data_real;

	//Dithering
	fscanf(fp,"%d",&data);
	Parameters.DitheringON = data;

	//ColorManagement
	fscanf(fp,"%d",&data);
	Parameters.ColorManagementON = data;
	
	for(i=0;i<256;i++)
	{
		fscanf(fp,"%d",&data);
		Parameters.ColorManagementParameters.GammaR[i]=data;
	}
	
	for(i=0;i<256;i++)
	{
		fscanf(fp,"%d",&data);
		Parameters.ColorManagementParameters.GammaG[i]=data;
	}
	
	for(i=0;i<256;i++)
	{
		fscanf(fp,"%d",&data);
		Parameters.ColorManagementParameters.GammaB[i]=data;
	}
	
	for(i=0;i<256;i++)
	{
		fscanf(fp,"%d",&data);
		Parameters.ColorManagementParameters.TRCR[i]=data;
	}
	
	for(i=0;i<256;i++)
	{
		fscanf(fp,"%d",&data);
		Parameters.ColorManagementParameters.TRCG[i]=data;
	}
	
	for(i=0;i<256;i++)
	{
		fscanf(fp,"%d",&data);
		Parameters.ColorManagementParameters.TRCB[i]=data;
	}
	
	
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			fscanf(fp,"%d",&data);
			Parameters.ColorManagementParameters.Matrix [3*i+j]=data;
		}
	}

	
	// Close stream 
	fclose(fp);

	return ETrue;	
}
*/
//----IMAAMI----
