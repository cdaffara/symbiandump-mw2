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


#ifndef CPOSTP144_H
#define CPOSTP144_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <StifLogger.h>
#include "lcfsbucommondefinitions.h"



/**
*  Test procedure 144
*/
class CPosTp144 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp144(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp144() {};

    public: // Functions from base classes

        
        void StartL();

    private:

		void DumpBuffer(const TDesC8& aBuffer);
         // check serialization
        void CheckLmSerializationL();
        
        CPosLandmark* CreateDummyLandmarkLC();
        void CompareLandmarks(CPosLandmark& aLm1, CPosLandmark& aLm2);
        void CompareBuffers(const TDesC8& aBuf1, const TDesC8& aBuf2);
        
        void CheckLmSerialization_SubL(CPosLandmark* lm);
        
    private:
        // common variables used in tests
        TInt                    iErrorCounter;


    };

#endif      // CPOSTP144_H
            
// End of File
