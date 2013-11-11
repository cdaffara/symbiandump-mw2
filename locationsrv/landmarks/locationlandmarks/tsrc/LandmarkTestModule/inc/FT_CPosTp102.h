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


#ifndef CPOSTP102_H
#define CPOSTP102_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"


/**
*  Test procedure 102
*/
class CPosTp102 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        //CPosTp102() : CLandmarkTestProcedureBase(102) {};
        CPosTp102(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp102() {};

    public: // Functions from base classes

        
        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:

        /**
        * Verify the number of existing landmark databases
        */
        void VerifyNrOfDatabasesL(TInt aExpNrOfDatabases, const TDesC&  aDatabaseUri=KNullDesC);

        /**
        * Create a new landmark database using DBMS api (instead of the Landmarks API)
        */
        void CreateDatabaseL(const TDesC& aUri);
        
        void TestDifferentUrisL();

    };

#endif      // CPOSTP102_H
            
// End of File
