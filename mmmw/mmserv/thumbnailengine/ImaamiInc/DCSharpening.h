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




#ifndef __DCSharpening_H
#define __DCSharpening_H

#include <e32std.h> // for Fundamental Types
#include <e32base.h> // for Fundamental Types
#include "DCInit.h"

//Adjust to proper signal range (10 bits)
#define ADJUST_RANGE_TO_10BITS(value) ((value) < 0) ? 0 : ((value) > 1023) ? 1023 : (value)


//Class definition
class DCSharpening : public CBase
{
public:

	//parameters
	struct DCSharpeningParams
	{
		TUint SHARP_OVER;
		TUint SHARP_DZONE;
		TReal SHARP_GAIN;
	};

	DCSharpening();
	static DCSharpening* NewL();
	static DCSharpening* NewLC();
	~DCSharpening();
	
	// Second Phase contructor (may leave)
	void ConstructL();
	
	//Process original image and store result
	void ProcessL(CFbsBitmap* aBPtr);
	
	//Set processing parameters
	void SetParameters(DCSharpeningParams* params);

	//Get current processing parameters
	void GetParameters(DCSharpeningParams* params);

	
	//Limit integer value to byte [0,255]
	static inline TUint8 Limit255(TInt i) {
		return (TUint8)(i<0?0:(i>255?255:i));
	}
	
	
private:
	
	//Processing parameters
	DCSharpeningParams iParameters;

	//Line Buffer and pointer
	HBufC8*	iScanLine;
	HBufC8*	iPrevScanLine;
	HBufC8*	iPrevPrevScanLine;
	HBufC8*	iNextScanLine;
	HBufC8*	iNextNextScanLine;

	//Sharpening functions from IMAAMI
	TInt Peak(TInt aA, TInt aB, TInt aC, TInt aD, TInt aE, TInt aF, TInt aG, TInt aH, TInt aO);
	TInt Median3(TInt aA, TInt aB, TInt aC);
	void findMinMax4(TInt A, TInt B, TInt C, TInt D, TInt *min, TInt *max);

	//Processing parameters for stretch
	TInt iGain;
};

#endif // ifndef __DCSharpening_H
//----IMAAMI----
