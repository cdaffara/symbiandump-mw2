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
* Description:  Main for spsettings backuphelper
*
*/

//  Include Files  
#include <e32base.h>

#include "spsbackuphelpermonitor.h"

//  Constants

//  Local Functions

// ---------------------------------------------------------------------------
// DoStartL
// ---------------------------------------------------------------------------
//
void DoStartL()
    {
    // Create active scheduler (to run active objects)
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    CSpsBackupHelperMonitor* monitor = CSpsBackupHelperMonitor::NewL();
    
    scheduler->Start();
    delete monitor;
    
    // Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    }

//  Global Functions

// ---------------------------------------------------------------------------
// E32Main
// ---------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    // Create cleanup stack
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    
    TInt mainError;
    if( cleanup )
        {
        // Run application code inside TRAP harness
        TRAP(mainError, DoStartL());
        }

    delete cleanup;
    __UHEAP_MARKEND;
    return KErrNone;
    }


