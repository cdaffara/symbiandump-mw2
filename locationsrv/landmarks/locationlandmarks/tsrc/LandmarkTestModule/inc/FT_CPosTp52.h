/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef CPOSTP52_H
#define CPOSTP52_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION
class CPosLandmarkParser;

/**
*  Test procedure 48
*/
class CPosTp52 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp52(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iErrorsFound=KErrNone;};
        /**
        * Destructor.
        */
        ~CPosTp52() {};

    public: // Functions from base classes

       
        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:

        /**
        * Do the import of landmarks
        */
        TInt ImportLandmarksL(TBool aSync, TUint aTransferOption);

        /**
        * Prepare landmarkdb for test
        */
        void ResetLandmarksDbL();

        /**
        * Get nr of landmarks in lm db
        */
        TInt GetNrOfLandmarksL();

        /**
        * Import landmarks from test, both sync and async
        */
        void DoTestL(
            const TDesC&  aTestFile, 
            const TDesC8& aMimeType,
            TInt aExpectedErrorCode, 
            TInt aExpNrImportedLandmarks=0);

        /**
        * Check landmark in lm db
        */
        void CheckLongLandmarkNameL();

        /**
        * Check landmark category in lm db
        */
        void CheckLongCategoryNameL(TInt aNumExpectedCategories);

    private:
        CPosLandmarkParser* iLandmarkParser;
        TInt iErrorsFound;

    };

#endif      // CPOSTp52_H
            
// End of File
