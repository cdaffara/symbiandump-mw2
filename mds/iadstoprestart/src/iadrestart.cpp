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
* Description:  Helper application to restart servers for IAD upgrade
*
*/

#include <e32base.h>
#include <e32std.h>  
#include <e32property.h>

#include "mdscommoninternal.h"
#include "iadrestart.h"

// Print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

LOCAL_C void MainL()
    {
    
    PRINT(_L("IADRestart - Reset key"));

	// reset key
    RProperty::Set(KWatchdogPSShutdown,KShutdown,0);
    
    // reset key
    RProperty::Set(KMdSPSShutdown,KShutdown,0);
    RProperty::Set(KHarvesterPSShutdown,KShutdown,0);

    }

GLDEF_C TInt E32Main()
    {
    // Create cleanup stack
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();

    // Run application code inside TRAP harness
    TInt err = KErrNone;
    TRAP(err, MainL());

    delete cleanup;
    cleanup = NULL;
    __UHEAP_MARKEND;
    return err;
    }

// End of file
