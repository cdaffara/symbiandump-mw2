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

#include "ut_callinfos.h"
#include "ut_remotepartyinfos.h"
#include "ut_remotepartyinfo.h"
#include <CEUnitTestSuite.h>



EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    _LIT( KSuiteName, "Mediator API tests" );
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( KSuiteName );
    // Note that NewLC leaves the object in the cleanupstack.
    rootSuite->AddL( ut_callinfos::NewLC() );
    CleanupStack::Pop(); 
    rootSuite->AddL( ut_remotepartyinfos::NewLC() );
    CleanupStack::Pop();
    rootSuite->AddL( ut_remotepartyinfo::NewLC() );
    CleanupStack::Pop();
    CleanupStack::Pop( rootSuite );
    return rootSuite;
    }
