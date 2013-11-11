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


#ifndef CPOSTp27_H
#define CPOSTp27_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 27
*/
class CPosTp27 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp27(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) 
            {iGlobalErr = KErrNone;iNrSearchRounds=0;iBreakSearch=-1;
             iCancelOrDelete = EFalse;iHandleOperationDisabled=EFalse;
             iDebug=ETrue;iProgress=100;};
        
        /**
        * Destructor.
        */
        ~CPosTp27() {};

    public: // Functions from base classes

        /**
        * From CUtfwTestBase
        * Retrieves the test module name
        * @param aName The name of the test module.
        */
        void GetName(
            TDes&   aName
            ) const;

        /**
        * From CUtfwTestBase
        */
        void CloseTest();

        /**
        * From CPosTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        /**
        * From MPosLmAsyncObserver
        * Called to report progress of an asynchronous landmarko peration.
        * @param aOperation The object which handles the asyncronous operation.
        */
        void HandleOperationProgress(CPosLmOperation*  aOperation);


        static TInt StopActiveSchedulerWait(TAny* aSelf);

    private:    // New functions

        /**
        * Test to cancel a landmark category search
        * 
        */
        void CancelLandmarkCategorySearch();

        /**
        * Test to cancel a landmark search
        * 
        */
        void CancelLandmarkSearch();

    private:    // Data

        RFs     iFileServer;
        RFile   iFile;
        TInt    iGlobalErr;
        TInt    iNrSearchRounds;
        TInt    iBreakSearch;
        TBool   iCancelOrDelete;
        TBool   iHandleOperationDisabled;
        TBool   iDebug;
        CPeriodic* iPeriodicTimer;
        TReal32 iProgress;

    };

#endif      // CPOSTp27_H
            
// End of File
