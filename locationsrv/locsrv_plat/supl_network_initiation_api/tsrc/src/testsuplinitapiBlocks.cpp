/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Ctestsuplinitapi class 
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "testsuplinitapi.h"
#include "testsuplnetworkserver.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ctestsuplinitapi::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ctestsuplinitapi::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ctestsuplinitapi::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ctestsuplinitapi::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        ENTRY( "RSuplNetworkServer_Connect",Ctestsuplinitapi::RSuplNetworkServer_ConnectL),
		ENTRY( "RSuplNetworkServer_ForwardMessage",Ctestsuplinitapi::RSuplNetworkServer_ForwardMessageL),
        ENTRY( "RSuplNetworkServer_Version",Ctestsuplinitapi::RSuplNetworkServer_VersionL),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    TInt ret = KErrNone;
	TRAPD( error, ret = RunInternalL( KFunctions, count, aItem ));
	if ( error )
		{
		return error;
		}
    return ret;

    }


// -----------------------------------------------------------------------------
// Ctestsuplinitapi::RSuplNetworkServer_ConnectL
// Test case will test Connect function of RSuplNetworkServer class
// -----------------------------------------------------------------------------
//
TInt Ctestsuplinitapi::RSuplNetworkServer_ConnectL(CStifItemParser& aItem)
   {
   CTestSuplNetworkServer* testSuplNetworkServer=CTestSuplNetworkServer::NewLC();
   TInt value;
   TInt error=aItem.GetNextInt(value);
   if(error==KErrNone)
       {
   	   TestModuleIf().SetExitReason( CTestModuleIf::EPanic,3);
   	   testSuplNetworkServer->TestConnectL(value);
   	   }
   else
   	   {
   	   testSuplNetworkServer->TestConnectL();	
   	   }
   CleanupStack::PopAndDestroy(testSuplNetworkServer);
   return KErrNone;
   }

// -----------------------------------------------------------------------------
// Ctestsuplinitapi::RSuplNetworkServer_ForwardMessageL
// Test case will test ForwardMessage function of RSuplNetworkServer class
// -----------------------------------------------------------------------------
//
TInt Ctestsuplinitapi::RSuplNetworkServer_ForwardMessageL(CStifItemParser& /* aItem*/)
   {
   CTestSuplNetworkServer* testSuplNetworkServer=CTestSuplNetworkServer::NewLC();
   TestModuleIf().SetExitReason( CTestModuleIf::EPanic,0);
   testSuplNetworkServer->TestForwardMessageL();	
   CleanupStack::PopAndDestroy(testSuplNetworkServer);
   return KErrNone;
   }	

// -----------------------------------------------------------------------------
// Ctestsuplinitapi::RSuplNetworkServer_VersionL
// Test case will test Version function of RSuplNetworkServer class
// -----------------------------------------------------------------------------
//
TInt Ctestsuplinitapi::RSuplNetworkServer_VersionL(CStifItemParser& /* aItem*/)
   {
   CTestSuplNetworkServer* testSuplNetworkServer=CTestSuplNetworkServer::NewLC();
   testSuplNetworkServer->TestVersionL();	
   CleanupStack::PopAndDestroy(testSuplNetworkServer);
   return KErrNone;
   }
//  End of File
