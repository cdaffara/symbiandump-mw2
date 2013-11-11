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




/*
-----------------------------------------------------------------------------

    DESCRIPTION

    Defines the dithering class.

-----------------------------------------------------------------------------
*/

#ifndef  __DCDithering_H__
#define  __DCDithering_H__



//  INCLUDES
#ifndef   __E32STD_H__
#include  <e32std.h>   // for Fundamental Types
#endif // __E32STD_H__
#ifndef   __E32BASE_H__
#include  <e32base.h>  // for CBase
#endif // __E32BASE_H__



//Class definition
class CDCDithering : public CBase
{
public:

	CDCDithering();					// Constructor
	static CDCDithering* NewL();	// Factory function
	static CDCDithering* NewLC();	// Factory function
	~CDCDithering();				// Destructor
	void ConstructL();				// Second Phase contructor (may leave)
	
	// Process and store image referenced by aBPtr
	void ProcessL(CFbsBitmap& aBPtr);
	
private:

	// Limit integer value to byte [0,255]
	static inline TUint8 Limit255(TInt i) {return (TUint8)(i<0?0:(i>255?255:i));}

	// Scan line buffer
	HBufC8*  iScanLineBuffer;
};

#endif // __DCDithering_H__
// End of File
//----IMAAMI----
