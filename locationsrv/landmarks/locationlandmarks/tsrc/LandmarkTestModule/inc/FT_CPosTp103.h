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


#ifndef CPOSTP103_H
#define CPOSTP103_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 103
*/
class CPosTp103 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp103(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp103() {};

    public: // Functions from base classes

        
        void StartL();
        
        void CloseTest();

    private:

        /**
        * Verify the number of existing landmark databases
        */
        void VerifyNrOfDatabasesL(TInt aExpNrOfDatabases, const TDesC&  aDatabaseUri=KNullDesC);

        /**
        *
        */
        void TestPartCopyDatabasesL();

    };

#endif      // CPOSTP103_H
            
// End of File
