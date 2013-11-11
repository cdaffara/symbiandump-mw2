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


#ifndef CPOSTP146_H
#define CPOSTP146_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <StifLogger.h>
#include "lcfsbucommondefinitions.h"



/**
*  Test procedure 146
*/
class CPosTp146 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp146(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp146() {};

    public: // Functions from base classes

        
        void StartL();

    private:

		
         // check  serialization invalid
         void CheckLmCatSerializationInvalidL();
        CPosLandmark* CreateDummyLandmarkLC();
        CPosLandmarkCategory* CreateDummyCategoryLC();
        
        
    private:
        // common variables used in tests
        TInt                    iErrorCounter;


    };

#endif      // CPOSTP146_H
            
// End of File
