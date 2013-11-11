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
* Description:  Helper application to stop servers for IAD upgrade
*
*/

#include <e32base.h>
#include <e32std.h>  
#include <e32property.h>

#include "mdscommoninternal.h"
#include "iadstop.h"

// Print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

LOCAL_C void MainL()
    {
    
    TInt res( KErrNone );
    RProcess process;
    TFullName name;
    
    // define P&S property types    
    res = RProperty::Define(KMdSPSShutdown,KShutdown,RProperty::EInt,KAllowAllPolicy,KPowerMgmtPolicy);
    
    PRINT((_L("IADStop - terminate Harvester server, res = %d"), res ));    
    
    // find and terminate Harvester Server
    TFindProcess findProcess2( KHarvesterServerProcess );
    if ( findProcess2.Next(name) == KErrNone )
        {        
                 
        res = process.Open(name);
         
        // logon to get termination signal
        TRequestStatus status;
        process.Logon(status);

        // shutdown using P&S key
        TInt error = RProperty::Set(KHarvesterPSShutdown, KShutdown, 1);
        
        PRINT((_L("IADStop - set property, error = %d"), error ));

        // blocks here until thread is terminated
        User::WaitForRequest(status); 

        PRINT(_L("IADStop - Harvester server terminated"));
          
        process.Close();
        }

    PRINT(_L("IADStop - terminate MdS server"));
            
    // find and terminate Mds Server
    TFindProcess findProcess( KMdSServerProcess );
    if ( findProcess.Next(name) == KErrNone )
        {
        PRINT(_L("IADStop - found MdS server"));
                 
        res = process.Open(name);
         
        // logon to get termination signal
        TRequestStatus status;
        process.Logon(status);

        // shutdown using P&S key
        RProperty::Set(KMdSPSShutdown, KShutdown, 1);               

        // blocks here until thread is terminated
        User::WaitForRequest(status); 

        PRINT(_L("IADStop - MdS server terminated"));
          
        process.Close();
        }    
    
    else
        {
        PRINT(_L("IADStop - terminate Thumb AG Daemon"));
        
        // Kill Thumb AG Daemon !!!
        // find and terminate Mds Server
        TFindProcess findProcess3( KTAGDaemonProcess );
        if ( findProcess3.Next(name) == KErrNone )
            {
            PRINT(_L("IADStop - found thumb daemon"));
                     
            res = process.Open(name);
             
            // logon to get termination signal
            TRequestStatus status;
            process.Logon(status);

            // shutdown using P&S key
            RProperty::Set(KMdSPSShutdown, KShutdown, 1);               

            // blocks here until thread is terminated
            User::WaitForRequest(status); 

            PRINT(_L("IADStop - thumb daemon terminated"));
              
            process.Close();
            }    
        }

    PRINT(_L("IADStop - terminate MdS watchdog"));    
    
    // find and terminate mds watchdog
    TFindProcess findProcess4( KWatchdogProcess );
    if ( findProcess4.Next(name) == KErrNone )
        {        
                 
        res = process.Open(name);
         
        // logon to get termination signal
        TRequestStatus status;
        process.Logon(status);

        // shutdown using P&S key
        TInt error = RProperty::Set(KWatchdogPSShutdown, KShutdown, 1);
        
        PRINT((_L("IADStop - set property, error = %d"), error ));

        // blocks here until thread is terminated
        User::WaitForRequest(status); 

        PRINT(_L("IADStop - MdS watchdog terminated"));
          
        process.Close();
        }
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
