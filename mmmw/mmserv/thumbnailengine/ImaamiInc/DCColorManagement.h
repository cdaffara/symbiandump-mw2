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



#ifndef __DCColorManagement_H
#define __DCColorManagement_H

#include <e32std.h> // for Fundamental Types
#include <e32base.h> // for Fundamental Types
#include <e32math.h> //for the calculation of exponents

//Class definition
class CDCColorManagement : public CBase
{

public:

	//Parameter struct definition
	struct DCColorManagementParams
	{
		//Gamma de-correction (linearization) LUTs
		//Pointers to actual data
		TInt *GammaR;
		TInt *GammaG;
		TInt *GammaB;
		
		//3x3 color space modification matrix
		//Pointer to actual data
		TInt *Matrix;
		
		//Tone rendering curves = display compensation LUTs
		//Pointers to actual data
		TInt *TRCR;
		TInt *TRCG;
		TInt *TRCB;
	};

	//Gamma de-correction (linearization) LUTs
	TInt iGammaR[256];
	TInt iGammaG[256];
	TInt iGammaB[256];
	
	//3x3 color space modification matrix
	TInt iMatrix[9];

	//Tone rendering curves = display compensation LUTs
	TInt iTRCR[256];
	TInt iTRCG[256];
	TInt iTRCB[256];

	//Standard class member functions
	CDCColorManagement();
	static CDCColorManagement* NewL();
	static CDCColorManagement* NewLC();
	~CDCColorManagement();
	void ConstructL();
	
	//Process original image and store result
	void ProcessL(CFbsBitmap* aBPtr);

	//Set processing parameters
	void SetParameters(DCColorManagementParams* params);
	
	//Get current processing parameters
	void GetParameters(DCColorManagementParams* params);

	//Limit integer value to byte [0,255]
	static inline TUint8 Limit255(TInt i) {return (TUint8)(i<0?0:(i>255?255:i));}
	
private:
	
	// Line Buffer and pointer
	HBufC8*	iScanLine;
	
	//Processing parameters
	DCColorManagementParams iParams;

};

#endif // ifndef __DCColorManagement_H
//----IMAAMI----
