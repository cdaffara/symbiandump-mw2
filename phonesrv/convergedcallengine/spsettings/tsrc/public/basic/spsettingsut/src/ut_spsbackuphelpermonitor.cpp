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

#include "ut_spsbackuphelpermonitor.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

#include <spentry.h>
#include <spproperty.h>
#include "spsbackuphelpermonitor.h"

// - Construction -----------------------------------------------------------

Ut_CSpsBackupHelperMonitor* Ut_CSpsBackupHelperMonitor::NewL()
    {
    Ut_CSpsBackupHelperMonitor* self = new (ELeave) Ut_CSpsBackupHelperMonitor;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

Ut_CSpsBackupHelperMonitor::~Ut_CSpsBackupHelperMonitor()
    {
    }

Ut_CSpsBackupHelperMonitor::Ut_CSpsBackupHelperMonitor()
    {
    }

void Ut_CSpsBackupHelperMonitor::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// - Test methods -----------------------------------------------------------



void Ut_CSpsBackupHelperMonitor::SetupL(  )
    {
    iMonitor = CSpsBackupHelperMonitor::NewL();
    }
    

void Ut_CSpsBackupHelperMonitor::Teardown(  )
    {
    delete iMonitor;
    iMonitor = NULL;
    }


void Ut_CSpsBackupHelperMonitor::Ut_ConstructL()
    {
    // Do nothing
    }

void Ut_CSpsBackupHelperMonitor::Ut_FullBackupOngoingSetupL()
    {
    RProperty prop;
    prop.Attach( KUidSystemCategory, conn::KUidBackupRestoreKey );
    prop.Set( EBURBackupFull );
    iMonitor = CSpsBackupHelperMonitor::NewL();
    prop.Close();
    }

void Ut_CSpsBackupHelperMonitor::Ut_PartialBackupOngoingSetupL()
    {
    RProperty prop;
    prop.Attach( KUidSystemCategory, conn::KUidBackupRestoreKey );
    prop.Set( EBURBackupPartial );
    iMonitor = CSpsBackupHelperMonitor::NewL();
    prop.Close();
    }

void Ut_CSpsBackupHelperMonitor::Ut_FullRestoreOngoingSetupL()
    {
    RProperty prop;
    prop.Attach( KUidSystemCategory, conn::KUidBackupRestoreKey );
    prop.Set( EBURRestoreFull );
    iMonitor = CSpsBackupHelperMonitor::NewL();
    prop.Close();
    }

void Ut_CSpsBackupHelperMonitor::Ut_PartialRestoreOngoingSetupL()
    {
    RProperty prop;
    prop.Attach( KUidSystemCategory, conn::KUidBackupRestoreKey );
    prop.Set( EBURRestorePartial );
    iMonitor = CSpsBackupHelperMonitor::NewL();
    prop.Close();
    }

// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    Ut_CSpsBackupHelperMonitor,
    "Add test suite description here.",
    "UNIT" )
    
EUNIT_TEST(
    "Construct - destruct",
    "CSPSettingsEngine",
    "Construct - destruct",
    "FUNCTIONALITY",
    SetupL, Ut_ConstructL, Teardown)
    
EUNIT_TEST(
    "FullBackup Construct - destruct",
    "CSPSettingsEngine",
    "Construct - destruct",
    "FUNCTIONALITY",
    Ut_FullBackupOngoingSetupL, Ut_ConstructL, Teardown)
    
EUNIT_TEST(
    "PartialBackup Construct - destruct",
    "CSPSettingsEngine",
    "Construct - destruct",
    "FUNCTIONALITY",
    Ut_PartialBackupOngoingSetupL, Ut_ConstructL, Teardown)
    
EUNIT_TEST(
    "FullRestore Construct - destruct",
    "CSPSettingsEngine",
    "Construct - destruct",
    "FUNCTIONALITY",
    Ut_FullRestoreOngoingSetupL, Ut_ConstructL, Teardown)
    
EUNIT_TEST(
    "PartialRestore Construct - destruct",
    "CSPSettingsEngine",
    "Construct - destruct",
    "FUNCTIONALITY",
    Ut_PartialRestoreOngoingSetupL, Ut_ConstructL, Teardown)

EUNIT_END_TEST_TABLE
