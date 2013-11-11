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


#ifndef CPOSTP127_H
#define CPOSTP127_H

// INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <ss_std.h>
#include <e32std.h>

// CLASS DECLARATION
class CPosLandmarkParser;
class CPosTp127;

// Data types
struct TThreadParam
    {
    void (*iTestFunction)(TDes*);
    TDes* iMessage;
    RThread* iThread;
    };

class CThreadMonitor : public CActive
    {
    public:

        static CThreadMonitor* NewL(RThread* aThread);

        ~CThreadMonitor();

    protected:

        void DoCancel();

        void RunL();

    private:

        CThreadMonitor(RThread* aThread);

        void ConstructL();

    private:

        RThread*    iThread;

    };

/**
*  Test procedure 127
*/
class CPosTp127 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp127() : CLandmarkTestProcedureBase(127) {};
        
        /**
        * Destructor.
        */
        ~CPosTp127() {};

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
        * Always called before StartL()
        */
        void InitTestL();

        /**
        * From CUtfwTestBase
        * Always called, even if test leaves
        */
        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    public: // New functions
       
        static void MultipleDbCategorySearchL(TDes* aMessage);

        static void MultipleDbLandmarkSearchL(TDes* aMessage);

        static void MultipleDbLandmarkWithoutCategorySearchL(TDes* aMessage);

        static void LockDatabaseL(TDes* aMessage);
        
        static void DbManagementL(TDes* aMessage);

        static void DbAccessL(TDes* aMessage);

        static void SingleDbSearchL(TDes* aMessage);
        
        static void MultipleDbSearchL(TDes* aMessage);        

        static void CopyLockedDatabaseL(TDes* aMessage);        

        static void CopyDatabaseL(TDes* aMessage);        

    private:

        TBool ResumeThreadsAndVerifyExitL(const RArray<TThreadParam>& aThreadParams);
        
        TBool MultipleDbSearchClientsL(TBool aLockDb);
        
        TBool MultipleLmClientsL();
        
        TBool VerifyInUseWhenCopyL();

        TBool VerifyCopyStressL();

    private:

        TBuf<100>               iBuf;

    };

#endif      // CPOSTP127_H
            
// End of File
