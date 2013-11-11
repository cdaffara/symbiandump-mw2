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


#ifndef  __DCIetd_H__
#define  __DCIetd_H__

//  INCLUDES

#ifndef   __E32STD_H__
#include  <e32std.h>   // for Fundamental Types
#endif // __E32STD_H__

#ifndef   __E32BASE_H__
#include  <e32base.h>  // for CBase
#endif // __E32BASE_H__

//Default parameters
#include "DCInit.h"


//-------------------
//  CLASS DEFINITIONS 
// CDCIetd - The one and ony class to contain all required methods
// to implement DCIetd functionality.
class CDCIetd : public CBase
    {
    public:     

		//Parameter struct
		struct DCIetdParameters
		{
			TUint8 aWhitePixels; 	//Percentage of pixels stretched to maximum value
			TUint8 aBlackPixels;	//Percentage of pixels stretched to minimum value
			TUint8 aStretchLimit;	//Narrowest histogram to be stretched to full range
			TUint8 aSaturationGain;	//Saturation increase gain
			TUint8 aBitLimit;		//Minimum number of color values for full gain		
			TUint8 aWBC;			//White balance correction limit
			TUint8 aDBC;			//Dark balance correction limit
		};

        CDCIetd();				// Constructor
        static CDCIetd* NewL();	// Factory function NewL
        static CDCIetd* NewLC();// Factory function NewLC
        ~CDCIetd();				// Destructor
        void ConstructL();		// Second Phase contructor (may leave)
        
		// Analyze image referenced by aBPtr
		void Analyze(CFbsBitmap& aBPtr);

		// Process and store image referenced by aBPtr
		void ProcessL(CFbsBitmap& aBPtr);

		// Parameter exchange
		void SetParams(DCIetdParameters* parameters);
		void GetParams(DCIetdParameters* parameters);
        
    private:

		void GatherHistograms(const CFbsBitmap& aBPtr); // Construct the R, G, and B Histograms
        void MakeMappings();                            // Calculate the mapping LUTs

        // Limit integer value to byte [0,255]
        static inline TUint8 Limit255(TInt i) {return (TUint8)(i<0?0:(i>255?255:i));}

        TUint    iHistogram[3][256];  // Histogram data
        TUint8   iMap[3][256];        // Color component mapping funtions
        HBufC8*  iScanLineBuffer;     // Scan line buffer

        TUint8 iReducedStretchLimit[3]; //Stretch limits for each component

		DCIetdParameters iParameters; //Parameters
    };

#endif // __DCIetd_H__
            
// End of File
//----IMAAMI----
