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


#ifndef CPOSTP24_H
#define CPOSTP24_H

//  INCLUDES
#include <EPos_CPosLmAreaCriteria.h>

#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 24
*/
class CPosTp24 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp24(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp24() {};

    public: // Functions from base classes

        
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        void CloseTest();

    private:

        void SearchL(TExecutionMode aExecutionMode);

        void SearchWithSortOrderL();

        void SearchAndCancelL(const TBool& aInCallback=EFalse);

        static CPosLmAreaCriteria* NewLmAreaCriteriaL(const TReal64 &aSouthLat, 
                                               const TReal64 &aNorthLat,
                                               const TReal64 &aWestLong, 
                                               const TReal64 &aEastLong);
    
    };

#endif      // CPOSTP24_H
            
// End of File
