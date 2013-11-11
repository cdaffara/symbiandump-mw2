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


#ifndef CPOSTp114_H
#define CPOSTp114_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <e32base.h>
#include <e32property.h>

// CLASS DECLARATION
class CPosTp114BackupListener;
class MUtfwLog;

/**
*  Test procedure 114
*/
class CPosTp114 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp114() : CLandmarkTestProcedureBase(114) {};
        
        /**
        * Destructor.
        */
        ~CPosTp114() {};

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
        * Always called, even if test leaves
        */
        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:
        TBool FileExistL();

        void PrepareForRestoreTestL();

        void InitBackupTestL();

        TBool CheckDbAfterRestoreL();
        
    private:
        //CPosTp114BackupListener* iBackupListener;

    };

/**
*  CPosTp114BackupListener
*/
class CPosTp114BackupListener : public CActive
    {
    public:  // Constructors and destructor

        // Two-phased constructor.
        static CPosTp114BackupListener* NewL(MUtfwLog* aLog);

        /**
        * C++ Destructor
        */
        ~CPosTp114BackupListener();
        
        void StartCheckingL();
     
    protected:  // Functions from base classes

        /**
        * From CActive.
        * Active Object event handler.
        */
        void RunL();

        /**
        * From CActive.
        * Active Object cancel handler.
        */
        void DoCancel();
 
    private:
    
        /**
        * C++ Constructor
        */
        CPosTp114BackupListener();
        
        void ConstructL(MUtfwLog* aLog);
            
        TBool IsBackupRunning();
        
    private:
        
        MUtfwLog* iLog;
        RProperty iProperty;
        TBool iBackupIsRunning;
};

#endif      // CPOSTp114_H
            
// End of File
