/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/

#ifndef __UT_SPSBACKUPHELPERPERFORMER_H__
#define __UT_SPSBACKUPHELPERPERFORMER_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>

// FORWARD DECLARATIONS
class CSpsBackupHelperPerformer;

// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( Ut_CSpsBackupHelperPerformer )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static Ut_CSpsBackupHelperPerformer* NewL();

        ~Ut_CSpsBackupHelperPerformer();

    private: // Constructors

        Ut_CSpsBackupHelperPerformer();
        void ConstructL();

    private: // New methods

         void SetupL();
        
         void Teardown();
         
         void Ut_ConstructL();
         
         void Ut_CSpsBackupHelperPerformer_PerformL();
        
    private: // Data

        CSpsBackupHelperPerformer* iPerformer;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_SPSBACKUPHELPERPERFORMER_H__
