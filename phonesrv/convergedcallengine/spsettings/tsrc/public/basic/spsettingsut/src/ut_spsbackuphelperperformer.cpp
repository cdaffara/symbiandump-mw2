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

#include "ut_spsbackuphelperperformer.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

#include <spentry.h>
#include <spproperty.h>
#include "spsbackuphelperperformer.h"

// - Construction -----------------------------------------------------------

Ut_CSpsBackupHelperPerformer* Ut_CSpsBackupHelperPerformer::NewL()
    {
    
    Ut_CSpsBackupHelperPerformer* self = new (ELeave) Ut_CSpsBackupHelperPerformer;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

Ut_CSpsBackupHelperPerformer::~Ut_CSpsBackupHelperPerformer()
    {
    }

Ut_CSpsBackupHelperPerformer::Ut_CSpsBackupHelperPerformer()
    {
    }

void Ut_CSpsBackupHelperPerformer::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// - Test methods -----------------------------------------------------------



void Ut_CSpsBackupHelperPerformer::SetupL(  )
    {
    iPerformer = CSpsBackupHelperPerformer::NewL();
    }
    

void Ut_CSpsBackupHelperPerformer::Teardown(  )
    {
    delete iPerformer;
    iPerformer = NULL;
    }

void Ut_CSpsBackupHelperPerformer::Ut_ConstructL()
    {
    // Do nothing
    }

void Ut_CSpsBackupHelperPerformer::Ut_CSpsBackupHelperPerformer_PerformL()
    {
    iPerformer->PerformL();
    }

// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    Ut_CSpsBackupHelperPerformer,
    "Add test suite description here.",
    "UNIT" )
    
EUNIT_TEST(
    "Construct - destruct",
    "CSPSettingsEngine",
    "Construct - destruct",
    "FUNCTIONALITY",
    SetupL, Ut_ConstructL, Teardown)

EUNIT_TEST(
    "Test Perform",
    "CSPSettingsEngine",
    "Test Perform",
    "FUNCTIONALITY",
    SetupL, Ut_CSpsBackupHelperPerformer_PerformL, Teardown)

EUNIT_END_TEST_TABLE
