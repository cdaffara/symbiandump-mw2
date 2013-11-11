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


#ifndef CPOSTp54_H
#define CPOSTp54_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 54
*/
class CPosTp54 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp54() : CLandmarkTestProcedureBase(54) {};
        
        /**
        * Destructor.
        */
        ~CPosTp54() {};

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
        * From CUtfwTestBase
        */
        void InitTestL();

        /**
        * From CPosTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:    // New functions

        /**
        * Returns the execution time in milliseconds
        */
        TInt64 ExecutionTimeSynchL(TBool aIsSorted);

        /**
        * Returns the longest execution time of a RunL call in milliseconds
        */
        TInt64 ExecutionTimeAsynchL(TBool aIsSorted);

        TBool VerifyResult(
            TInt64 aExecTime, 
            TInt aNrOfLandmarks, 
            TBool aSorted, 
            TInt64 aLimit, 
            TBool aSynch);

        TInt64 TestIteratorSynchL(const TDesC& aDatabase, TBool aIsSorted);

        TInt64 TestIteratorAsynchL(const TDesC& aDatabase, TBool aIsSorted);

        void RecreateDatabaseL(const TDesC& aDatabase);

        TInt64 ExecuteLmOp(CPosLmOperation* aOp, TInt& aResult);
        
        void CreateLargeDbs(TInt aNrOfItems);

    private:    // Data

    };

#endif      // CPOSTp54_H
            
// End of File
