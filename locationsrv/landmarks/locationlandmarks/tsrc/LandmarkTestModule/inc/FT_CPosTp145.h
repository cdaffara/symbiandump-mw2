/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*   ?description_line
*
*/


#ifndef CPOSTP145_H
#define CPOSTP145_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <StifLogger.h>
#include "lcfsbucommondefinitions.h"



/**
*  Test procedure 145
*/
class CPosTp145 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp145(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp145() {};

    public: // Functions from base classes

        
        void StartL();

    private:

		//void DumpBuffer(const TDesC8& aBuffer);
         // check category serialization
        void CheckLmCatSerializationL();
        void CheckLmCatSerialization_SubL(CPosLandmarkCategory* aTest);
        void CompareBuffers(const TDesC8& aBuf1, const TDesC8& aBuf2);
        CPosLandmarkCategory* CreateDummyCategoryLC();
        void CompareCategoriesL(CPosLandmarkCategory& aCat1, CPosLandmarkCategory& aCat2);
        
    private:
        // common variables used in tests
        TInt                    iErrorCounter;


    };

#endif      // CPOSTP145_H
            
// End of File
