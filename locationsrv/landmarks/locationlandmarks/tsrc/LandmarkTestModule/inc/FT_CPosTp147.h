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


#ifndef CPOSTP147_H
#define CPOSTP147_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <StifLogger.h>
#include "lcfsbucommondefinitions.h"



/**
*  Test procedure 147
*/
class CPosTp147 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp147(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp147() {};

    public: // Functions from base classes

        
        void StartL();

    private:

		
         // check  serialization memory
         void CheckSerializationMemoryL();
         CPosLandmark* CreateDummyLandmarkLC();
        CPosLandmarkCategory* CreateDummyCategoryLC();
            void CheckLmSerialization_MemoryL();
            void CheckLmCatSerialization_MemoryL();
            
        
        
    private:
        // common variables used in tests
        TInt                    iErrorCounter;


    };

#endif      // CPOSTP147_H
            
// End of File
