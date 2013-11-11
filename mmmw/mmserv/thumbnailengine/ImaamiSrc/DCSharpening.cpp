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
*	DCSharpening 
*		Sharpening using IMAAMI algorithm.
*
*/




#include <fbs.h>
#include <e32math.h> 
#include "DCSharpening.h"



/*
-----------------------------------------------------------------------------

  Constructor
	
  Default constructor, initializes member variables to initial values
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
DCSharpening::DCSharpening()
{
	// Set default values for parameters
	iParameters.SHARP_OVER = SharpOver;
	iParameters.SHARP_DZONE = SharpDZone;
	iParameters.SHARP_GAIN = SharpGain;
}



/*
-----------------------------------------------------------------------------

  NewLC
	
  Factory function to instantiate the class.
  This function leaves the class pointer to the cleanup stack
  May leave with KErrNoMemory if no memory available
  
  Return Values:  DCSharpening* self:  pointer to the class instance

-----------------------------------------------------------------------------
*/
DCSharpening* DCSharpening::NewLC()
{
	DCSharpening* self = new (ELeave) DCSharpening();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}




/*
-----------------------------------------------------------------------------

  NewL
	
  Factory function to instantiate the class.
  May leave with KErrNoMemory if no memory available
	  
  Return Values:  DCSharpening* self:  pointer to the class instance
	
-----------------------------------------------------------------------------
*/
DCSharpening* DCSharpening::NewL()
{
	DCSharpening* self = DCSharpening::NewLC();
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
void DCSharpening::ConstructL()
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
DCSharpening::~DCSharpening()
{
	// This function is intentionally left blank.
}




/*
-----------------------------------------------------------------------------

  ProcessL
	
  Process image referenced by aSrcBmpPtr (modify aSrcBmpPtr).
  May leave with KErrNoMemory if no memory available
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void DCSharpening::ProcessL(CFbsBitmap* aSrcBmpPtr)
{
	
	HBufC8*	iSrcDataBuffer; //Local memory for source data
	HBufC8*	iDstDataBuffer;	//Local memory for destination data
	TUint8* SrcDataPtr[3];	//Pointers to source image pixels in 3 successive lines  
	TUint8* DstDataPtr;		//Pointer to destianation pixels
	TInt y,u,v, out;		//Color components
	TUint x;				//Pixel indes
	TInt LineNo;			//Line index
	TUint LineLength;		//Image width
	TUint8 TmpLineIdx;		//Temporary line index used for line swapping
	TInt a, b, c, d, e, f, g, h, o;
	
	//Get image width
	LineLength=aSrcBmpPtr->SizeInPixels().iWidth;
	
	//Allocate local memory for 3 source lines.
	//Each line contains RGB triplets in BGRBGRBGR... format
	//(VUYVUYVUY... after conversion).
	//Descriptor HBufC8 is used.
	iSrcDataBuffer=HBufC8::NewMaxL(3*LineLength*3);
	CleanupStack::PushL(iSrcDataBuffer);
	
	//Set line pointers to point into beginnings of lines in allocated memory.
	//Lines are stored consecutively.
	//SrcLinePtrs[0] points to 1st line, [1] to 2nd, etc...
	//Use descriptor TPtr8 instead of c++ pointer, because
	//Set- and GetScanLine functions require it.
	//Set also descriptor lengths.
	TPtr8 SrcLinePtrs[3]={
		TPtr8((TUint8*)iSrcDataBuffer->Des().Ptr(),LineLength*3,LineLength*3),
			TPtr8((TUint8*)iSrcDataBuffer->Des().Ptr()+LineLength*3,LineLength*3,LineLength*3),
			TPtr8((TUint8*)iSrcDataBuffer->Des().Ptr()+2*LineLength*3,LineLength*3,LineLength*3)
	};
	
	//Set source line indexes. 
	//Line indexes are used to select proper line pointer.
	//SrcLineIndex[0] selects first line under filter window, [1] second, etc...
	//Indexes are needed because lines are swapped so that only one new line is
	//read from bitmap when filtering window moves down.
	//TPtr8 cannot be swapped (operator = copies data). Therefore swapping is
	//done through indexing.
	TUint8 SrcLineIndex[3]={0,1,2};
	
	//Allocate local memory for destination data.
	//Descriptor HBufC8 is used.
	iDstDataBuffer=HBufC8::NewMaxL(LineLength*3);
	CleanupStack::PushL(iDstDataBuffer);
	
	//Set destination line pointer to beginning of allocated memory.
	//Use descriptor TPtr8 instead of c++ pointer, because
	//Set- and GetScanline require it.
	//Set also descriptor length.
	TPtr8 DstLinePtr((TUint8*)iDstDataBuffer->Des().Ptr(),LineLength*3,LineLength*3);
	
	
	//Get 1st line from source image to 1st line in local memory buffer.
	LineNo=0;
	aSrcBmpPtr->GetScanLine(SrcLinePtrs[SrcLineIndex[0]],TPoint(0,LineNo),LineLength,aSrcBmpPtr->DisplayMode());
	
	//Get 2nd line from source image to 2nd line in local memory buffer.
	LineNo=1;
	aSrcBmpPtr->GetScanLine(SrcLinePtrs[SrcLineIndex[1]],TPoint(0,LineNo),LineLength,aSrcBmpPtr->DisplayMode());
	
	// Step through image lines, do not process 1st and last line (3x3 filter mask).
	for (LineNo=1;LineNo<aSrcBmpPtr->SizeInPixels().iHeight-1;++LineNo)
	{
		//Get next line from image to 3rd line of the filtering window
		aSrcBmpPtr->GetScanLine(SrcLinePtrs[SrcLineIndex[2]],TPoint(0,LineNo+1),LineLength,aSrcBmpPtr->DisplayMode());
		
		//Set destination pixel pointer to 2nd pixel of line (1st pixel is not processed (3x3 filter))
		DstDataPtr=(TUint8*)iDstDataBuffer->Des().Ptr()+3;
		
		//Set source pixel pointers to beginnings of lines in filtering window
		SrcDataPtr[0]=(TUint8*)iSrcDataBuffer->Des().Ptr()+SrcLineIndex[0]*LineLength*3;
		SrcDataPtr[1]=(TUint8*)iSrcDataBuffer->Des().Ptr()+SrcLineIndex[1]*LineLength*3;
		SrcDataPtr[2]=(TUint8*)iSrcDataBuffer->Des().Ptr()+SrcLineIndex[2]*LineLength*3;
		
		//Copy the first and the last pixels from the original image
		*(DstDataPtr-3)=*(SrcDataPtr[1]);
		*(DstDataPtr+1-3)=*(SrcDataPtr[1]+1);
		*(DstDataPtr+2-3)=*(SrcDataPtr[1]+2);
		*(DstDataPtr+3*(LineLength-1)-3)=*(SrcDataPtr[1]+3*(LineLength-1));
		*(DstDataPtr+1+3*(LineLength-1)-3)=*(SrcDataPtr[1]+1+3*(LineLength-1));
		*(DstDataPtr+2+3*(LineLength-1)-3)=*(SrcDataPtr[1]+2+3*(LineLength-1));
		
		//Step through image pixels, do not process 1st and last pixel (3x3 filter).
		for (x=LineLength-2;x!=0;--x)
		{
			
			//Compute filter output
			//Get input values from luminance component
			//.....abc.... o=processed pixel
			//.....doe.... 
			//.....fgh.... 
			//a,b,c,d come from saved previous values
			//pppppPPPpppp
			//.....Po..... 
			//............ 
			a = *(SrcDataPtr[0]+1);
			b = *(SrcDataPtr[0]+1+3);
			c = *(SrcDataPtr[0]+1+6);
			d = *(SrcDataPtr[1]+1);
			o = *(SrcDataPtr[1]+1+3);
			e = *(SrcDataPtr[1]+1+6);
			f = *(SrcDataPtr[2]+1);
			g = *(SrcDataPtr[2]+1+3);
			h = *(SrcDataPtr[2]+1+6);
			
			//Sharpen green component with IMAAMI sharpening algorithm
			out=Peak(a, b, c, d, e, f, g, h, o);
			
			//Scale result and limit to available dynamic range.
			y=Limit255(out);
			
			//Get B and R components
			v=*(SrcDataPtr[1]+3);	//blue
			u=*(SrcDataPtr[1]+2+3);	//red
			
			//Compute difference in green component due to sharpening and add it to B and R 
			v=Limit255(v+(out-o));			//blue
			u=Limit255(u+(out-o));			//red
			
			//Set destination color components
			*DstDataPtr=(TUint8)v ;		//blue
			*(DstDataPtr+1)=(TUint8)y;  //green
			*(DstDataPtr+2)=(TUint8)u;	//red	
			
			//Move to next VUY/RGB triplet in line
			DstDataPtr+=3;
			SrcDataPtr[0]+=3;
			SrcDataPtr[1]+=3;
			SrcDataPtr[2]+=3;
		}
		
		//Set processed line
		aSrcBmpPtr->SetScanLine(DstLinePtr,LineNo);
		
		//Swap source line indexes: 0<-1<-2<-0.
		//Now [0] is indexing the previous [1] data, [1] previous [2], etc...
		//When filtering window is moved down the new line is read to index [2],
		//so previous [0] data (i.e. 1st line in filtering window) is discarded.
		TmpLineIdx=SrcLineIndex[0];
		SrcLineIndex[0]=SrcLineIndex[1];
		SrcLineIndex[1]=SrcLineIndex[2];
		SrcLineIndex[2]=TmpLineIdx;
	}
	
	//Free memory
	CleanupStack::PopAndDestroy(2); //free iSrcDataBuffer and iDstDataBuffer
} 



/*
-----------------------------------------------------------------------------

  Peak
	
  IMAAMI sharpening function
	  
  Return Values:  TInt pixel sharpening data
		
-----------------------------------------------------------------------------
*/
TInt DCSharpening::Peak(TInt aA, TInt aB, TInt aC, TInt aD, TInt aE, TInt aF, TInt aG, TInt aH, TInt aO)
{
	TInt out, tmp;
	TInt lim  = 0;
	TInt sign = 1;
	TInt over = (iParameters.SHARP_OVER << 2);
	TInt gain = (TInt)(iParameters.SHARP_GAIN * (TReal)(1 << 16) + 0.5);
	TInt gradHor, gradVer;
	TInt gradDip, gradDin;
	TInt max, min;
	TInt add, weig;
	TInt tmp1, tmp2;
	TInt SHARP_LIM2 = ((1 << 15) + (1 << 14));


	gradHor = (aO << 1) - aD - aE;
	gradVer = (aO << 1) - aB - aG;
	gradDip = ((((aO << 1) - aA - aH) * 3) >> 2);
	gradDin = ((((aO << 1) - aC - aF) * 3) >> 2);

	findMinMax4(gradHor, gradVer, gradDip, gradDin, &min, &max);

	if(min < 0)
	{
		tmp = -min;

		if(tmp > max)
		{
			sign = -1;
			lim  = tmp;
			tmp2 = tmp;
			if(max < 0) tmp1 = -max;
			else        tmp1 =  max;
		}
		else
		{
			lim  = max;
			tmp2 = max;
			tmp1 = tmp;
		}
	}
	else if(max == 0)
	{
		tmp2 = 1;
		tmp1 = 1;
	}
	else
	{
		lim  = max;
		tmp2 = max;
		tmp1 = min;
	}

	if((tmp1 << 2) > 3 * tmp2)
	{
		out = aO;
	}
	else if((tmp1 << 2) < tmp2)
	{
		add = sign * ((lim * gain) >> 16);
		if(lim < (TInt)(iParameters.SHARP_DZONE)) out = aO;
		else				  out = ADJUST_RANGE_TO_10BITS(aO + add);
	}
	else
	{
		tmp = (tmp1 << 16) / tmp2;
		weig = (SHARP_LIM2 - tmp);
		if(lim < (TInt)(iParameters.SHARP_DZONE)) out = aO;
		else
		{
			add = sign * ((((weig * lim) >> 16) * gain) >> 16);
			out = ADJUST_RANGE_TO_10BITS(aO + add);
		}
	}

	if(sign < 0)
	{
		tmp = aO - over;

		if(out < tmp)
		{
			lim = -lim;

			if(gradHor == lim)
			{
				if(aD < aE)
				{
					if(aD < aO)
					{
						tmp = aD - over;
					}
				}
				else
				{
					if(aE < aO)
					{
						tmp = aE - over;
					}
				}
			}
			else if(gradVer == lim)
			{
				if(aB < aG)
				{
					if(aB < aO)
					{
						tmp = aB - over;
					}
				}
				else
				{
					if(aG < aO)
					{
						tmp = aG - over;
					}
				}
			}
			else if(gradDip == lim)
			{
				if(aA < aH)
				{
					if(aA < aO)
					{
						tmp = aA - over;
					}
				}
				else
				{
					if(aH < aO)
					{
						tmp = aH - over;
					}
				}
			}
			else
			{
				if(aC < aF)
				{
					if(aC < aO)
					{
						tmp = aC - over;
					}
				}
				else
				{
					if(aF < aO)
					{
						tmp = aF - over;
					}
				}
			}

			if(out < tmp)
			{
				out = tmp;
			}
		}
	}
	else
	{
		tmp = aO + over;
		if(out > tmp)
		{
			if(gradHor == lim)
			{
				if(aD > aE)
				{
					if(aD > aO)
					{
						tmp = aD + over;
					}
				}
				else
				{
					if(aE > aO)
					{
						tmp = aE + over;
					}
				}
			}
			else if(gradVer == lim)
			{
				if(aB > aG)
				{
					if(aB > aO)
					{
						tmp = aB + over;
					}
				}
				else
				{
					if(aG > aO)
					{
						tmp = aG + over;
					}
				}
			}
			else if(gradDip == lim)
			{
				if(aA > aH)
				{
					if(aA > aO)
					{
						tmp = aA + over;
					}
				}
				else
				{
					if(aH > aO)
					{
						tmp = aH + over;
					}
				}
			}
			else
			{
				if(aC > aF)
				{
					if(aC > aO)
					{
						tmp = aC + over;
					}
				}
				else
				{
					if(aF > aO)
					{
						tmp = aF + over;
					}
				}
			}

			if(out > tmp)
			{
				out = tmp;
			}
		}
	}

	return(out);
}



/*
-----------------------------------------------------------------------------

  Median3
	
  IMAAMI sharpening help function
	  
  Return Values:  TInt median of input values
		
-----------------------------------------------------------------------------
*/
TInt DCSharpening::Median3(TInt aA, TInt aB, TInt aC)
{
	if(aA < aB)
	{
		if(aA > aC)		 return aA;
		else if(aB < aC) return aB;
		else             return aC;
	}
	else
	{
		if(aA < aC)		 return aA;
		else if(aB > aC) return aB;
		else             return aC;
	}
}



/*
-----------------------------------------------------------------------------

  findMinMax4
	
  IMAAMI sharpening help function

  Finds minimum and maximum of A,B,C & D. Modifies min & max arguments
	  
  Return Values:  None
		
-----------------------------------------------------------------------------
*/
void DCSharpening::findMinMax4(TInt A, TInt B, TInt C, TInt D, TInt *min, TInt *max)
{
	if(A < B)
	{
		if(C < D)
		{
			if(A < C) *min = A;
			else      *min = C;
			if(B > D) *max = B;
			else      *max = D;
		}
		else
		{
			if(A < D) *min = A;
			else      *min = D;
			if(B > C) *max = B;
			else      *max = C;
		}
	}
	else
	{
		if(C < D)
		{
			if(B < C) *min = B;
			else      *min = C;
			if(A > D) *max = A;
			else      *max = D;
		}
		else
		{
			if(B < D) *min = B;
			else      *min = D;
			if(A > C) *max = A;
			else      *max = C;
		}
	}
}




/*
-----------------------------------------------------------------------------

  SetParams
	
  Set processing parameters
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void DCSharpening::SetParameters(DCSharpeningParams* params)
{
	iParameters = *params;
}



/*
-----------------------------------------------------------------------------

  GetParams
	
  Get current processing parameters
	  
  Return Values:  none
		
-----------------------------------------------------------------------------
*/
void DCSharpening::GetParameters(DCSharpeningParams* params)
{
	*params = iParameters;
}
//----IMAAMI----

