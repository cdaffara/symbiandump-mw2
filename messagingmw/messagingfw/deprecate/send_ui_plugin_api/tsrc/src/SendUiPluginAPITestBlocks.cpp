/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Tests senduisingleton.h and CSendingService.h
*
*/




// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <coemain.h>
#include "SendUiPluginAPITest.h"
#include <CSendingService.h>
#include <SendUiConsts.h>
#include <senduisingleton.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendUiPluginAPITest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CSendUiPluginAPITest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CSendUiPluginAPITest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//

TInt CSendUiPluginAPITest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "CSendUiSingleton", CSendUiPluginAPITest::TestCSendUiSingletonNewL ), 
        ENTRY( "CMsvSessions", CSendUiPluginAPITest::TestMsvSessionL ),
        ENTRY( "CClientMtmRegistryL", CSendUiPluginAPITest::TestClientMtmRegistryL ),
        ENTRY( "CMtmUiRegistry", CSendUiPluginAPITest::TestMtmUiRegistryL ),
        ENTRY( "CMtmUiDataRegistry", CSendUiPluginAPITest::TestMtmUiDataRegistryL ),
        ENTRY( "CSendingService", CSendUiPluginAPITest::TestCSendingServiceL ),
                     
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CSendUiPluginAPITest::TestCSendUiSingletonNewL
// Tests NewL of CSendUISingleton::NewL
// -----------------------------------------------------------------------------
//
TInt CSendUiPluginAPITest::TestCSendUiSingletonNewL(CStifItemParser& /*aItem*/ )
   {
   CSendUiSingleton* singleton = CSendUiSingleton::NewL();
   delete singleton;
   return KErrNone;
   }

// -----------------------------------------------------------------------------
// CSendUiPluginAPITest::TestMsvSessionL
// Tests NewL of CSendUISingleton::MsvSessionL
// -----------------------------------------------------------------------------
//
TInt CSendUiPluginAPITest::TestMsvSessionL(CStifItemParser& /*aItem*/ )
   {
   CSendUiSingleton* singleton = CSendUiSingleton::NewL();
   CleanupStack::PushL( singleton );
   TRAPD( err, CMsvSession& msvSession = singleton->MsvSessionL());
   if( err != KErrNone )
       {
       _LIT( KError, "CSendUiSingleton::MsvSessionL leaves with %d error");
       iLog->Log( KError, err);
       }
   else
       {
       _LIT( KError, "CSendUiSingleton::MsvSessionL Passed");
       iLog->Log( KError );       	
       }
   CleanupStack::PopAndDestroy();//singleton
   return err;
   }


// -----------------------------------------------------------------------------
// CSendUiPluginAPITest::TestClientMtmRegistryL
// Tests NewL of CSendUISingleton::ClientMtmRegistryL
// -----------------------------------------------------------------------------
//
TInt CSendUiPluginAPITest::TestClientMtmRegistryL(CStifItemParser& /*aItem*/ )
    {
    CSendUiSingleton* singleton = CSendUiSingleton::NewL();
    CleanupStack::PushL( singleton );
    TRAPD( err, CClientMtmRegistry& clientMtmRegistry = singleton->ClientMtmRegistryL());
    if( err != KErrNone )
        {
        _LIT( KError, "CSendUiSingleton::ClientMtmRegistryL leaves with %d error");
        iLog->Log( KError, err);
        }
    else
        {
        _LIT( KError, "CSendUiSingleton::ClientMtmRegistryL Passed");
       iLog->Log( KError );
        }
    CleanupStack::PopAndDestroy();//singleton
    return err;
    }


// -----------------------------------------------------------------------------
// CSendUiPluginAPITest::TestMtmUiRegistryL
// Tests NewL of CSendUISingleton::MtmUiRegistryL
// -----------------------------------------------------------------------------
//
TInt CSendUiPluginAPITest::TestMtmUiRegistryL(CStifItemParser& /*aItem*/ )
    {
    CSendUiSingleton* singleton = CSendUiSingleton::NewL();
    CleanupStack::PushL( singleton );
    TRAPD( err, CMtmUiRegistry& MtmUiRegistry = singleton->MtmUiRegistryL());
    if( err != KErrNone )
        {
        _LIT( KError, "CSendUiSingleton::MtmUiRegistryL leaves with %d error");
        iLog->Log( KError, err);
        }
    else
        {
        _LIT( KError, "CSendUiSingleton::MtmUiRegistryL Passed");
       iLog->Log( KError );
        }
    CleanupStack::PopAndDestroy();//singleton
    return err;    
    }

// -----------------------------------------------------------------------------
// CSendUiPluginAPITest::TestMtmUiDataRegistryL
// Tests NewL of CSendUISingleton::MtmUiDataRegistryL
// -----------------------------------------------------------------------------
//
TInt CSendUiPluginAPITest::TestMtmUiDataRegistryL(CStifItemParser& /*aItem*/ )
    {
    CSendUiSingleton* singleton = CSendUiSingleton::NewL();
    CleanupStack::PushL( singleton );
    TRAPD( err, CMtmUiDataRegistry& MtmUiDataRegistry = singleton->MtmUiDataRegistryL());
    if( err != KErrNone )
        {
        _LIT( KError, "CSendUiSingleton::MtmUiDataRegistryL leaves with %d error");
        iLog->Log( KError, err);
        }
    else
        {
        _LIT( KError, "CSendUiSingleton::MtmUiDataRegistryL Passed");
       iLog->Log( KError );
        }
    CleanupStack::PopAndDestroy();//singleton
    return err;
    }
    
// -----------------------------------------------------------------------------
// CSendUiPluginAPITest::TestCSendingServiceL
// Tests CSendingService::NewL, CSendingService::~CSendingService(), CSendingService::CSendingService
// -----------------------------------------------------------------------------
//

TInt CSendUiPluginAPITest::TestCSendingServiceL(CStifItemParser& /*aItem*/ )
    {
    CSendUiSingleton* singleton = CSendUiSingleton::NewL();
    CSendingService* sendingservice = NULL;
    TRAPD( error, sendingservice =  CSendingService::NewL(KMeetingRequestSender,*(CCoeEnv::Static()),*singleton));
    if( error == KErrNone )
        {
        _LIT( KError, "CSendingService::NewL Passed");
        iLog->Log( KError);
        delete sendingservice;
        _LIT( KError1, "CSendingService::~CSendingService()Passed");
       iLog->Log( KError1 );
        }
    else
        {
        _LIT( KError, "CSendingService::NewL Failed");
        iLog->Log( KError);
        }
    delete singleton;
    REComSession::FinalClose();
    return error;
    }     
// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
