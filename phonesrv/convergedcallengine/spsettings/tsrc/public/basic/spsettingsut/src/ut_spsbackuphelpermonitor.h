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

#ifndef __UT_SPSBACKUPHELPERMONITOR_H__
#define __UT_SPSBACKUPHELPERMONITOR_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>

// FORWARD DECLARATIONS
class CSpsBackupHelperMonitor;

// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( Ut_CSpsBackupHelperMonitor )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static Ut_CSpsBackupHelperMonitor* NewL();

        ~Ut_CSpsBackupHelperMonitor();

    private: // Constructors

        Ut_CSpsBackupHelperMonitor();
        void ConstructL();

    private: // New methods

         void SetupL();
        
         void Teardown();

         void Ut_ConstructL();
         
         void Ut_FullBackupOngoingSetupL();

         void Ut_PartialBackupOngoingSetupL();

         void Ut_FullRestoreOngoingSetupL();

         void Ut_PartialRestoreOngoingSetupL();
    private: // Data

        CSpsBackupHelperMonitor* iMonitor;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_SPSBACKUPHELPERMONITOR_H__
