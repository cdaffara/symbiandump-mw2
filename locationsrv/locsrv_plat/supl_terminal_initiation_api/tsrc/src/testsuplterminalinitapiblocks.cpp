/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CTestSuplTerminalInitApi
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "testsuplterminalinitapi.h"
#include "testsuplterminalqop.h"
#include "testrsuplterminalserver.h"
#include "ctestrsuplterminalsubsession.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTestSuplTerminalInitApi::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {
        // TSuplTerminalQop Class  
        ENTRY( "Set", CTestSuplTerminalInitApi::SetL ),
        ENTRY( "SetHorAccuracy", CTestSuplTerminalInitApi::SetHorAccuracyL ),
        ENTRY( "SetVerAccuracy", CTestSuplTerminalInitApi::SetVerAccuracyL ),
        ENTRY( "SetMaxLocationAge", CTestSuplTerminalInitApi::SetMaxLocationAgeL ),
        ENTRY( "SetDelay", CTestSuplTerminalInitApi::SetDelayL ),
        ENTRY( "Get", CTestSuplTerminalInitApi::GetL ),
        ENTRY( "GetHorAccuracy", CTestSuplTerminalInitApi::GetHorAccuracyL ),
        ENTRY( "GetVerAccuracy", CTestSuplTerminalInitApi::GetVerAccuracyL ),
        ENTRY( "GetMaxLocationAge", CTestSuplTerminalInitApi::GetMaxLocationAgeL ),
        ENTRY( "GetDelay", CTestSuplTerminalInitApi::GetDelayL ),
        // RSuplTerminalServer Class
        ENTRY( "ConnectClose", CTestSuplTerminalInitApi::ConnectCloseL ),
        ENTRY( "Version", CTestSuplTerminalInitApi::VersionL ),
        // RSuplTerminalSubSession Class
        ENTRY( "OpenClose", CTestSuplTerminalInitApi::OpenCloseL ),
        ENTRY( "GetPosition", CTestSuplTerminalInitApi::GetPositionL ),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );
	
	TInt ret = KErrNone;
	TRAPD( error, ret = RunInternalL( KFunctions, count, aItem ));
	if ( error != KErrNone )
		{
		return error;
		}
	return ret;	
   

    }


// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::SetL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::SetL( CStifItemParser& aItem )
    {
    TestSuplTerminalQop testSuplTermQop( iLog );
    TInt err = testSuplTermQop.SetL( aItem );
    return err;
    }

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::SetHorAccuracyL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::SetHorAccuracyL( CStifItemParser& aItem )
    {
    TestSuplTerminalQop testSuplTermQop( iLog );
    TInt err = testSuplTermQop.SetHorAccuracyL( aItem );
    return err;
    }

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::SetVerAccuracyL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::SetVerAccuracyL( CStifItemParser& aItem )
    {
    TestSuplTerminalQop testSuplTermQop( iLog );
    TInt err = testSuplTermQop.SetVerAccuracyL( aItem );
    return err;
    }
    
// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::SetMaxLocationAgeL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::SetMaxLocationAgeL( CStifItemParser& aItem )
    {
    TestSuplTerminalQop testSuplTermQop( iLog );
    TInt err = testSuplTermQop.SetMaxLocationAgeL( aItem );
    return err;
    }    

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::SetDelayL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::SetDelayL( CStifItemParser& aItem )
    {
    TestSuplTerminalQop testSuplTermQop( iLog );
    TInt err = testSuplTermQop.SetDelayL( aItem );
    return err;
    }    

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::GetL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::GetL( CStifItemParser& aItem )
    {
    TestSuplTerminalQop testSuplTermQop( iLog );
    TInt err = testSuplTermQop.GetL( aItem );
    return err;
    }    

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::GetHorAccuracyL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::GetHorAccuracyL( CStifItemParser& aItem )
    {
    TestSuplTerminalQop testSuplTermQop( iLog );
    TInt err = testSuplTermQop.GetHorAccuracyL( aItem );
    return err;
    }    

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::GetVerAccuracyL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::GetVerAccuracyL( CStifItemParser& aItem )
    {
    TestSuplTerminalQop testSuplTermQop( iLog );
    TInt err = testSuplTermQop.GetVerAccuracyL( aItem );
    return err;
    }    

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::GetMaxLocationAgeL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::GetMaxLocationAgeL( CStifItemParser& aItem )
    {
    TestSuplTerminalQop testSuplTermQop( iLog );
    TInt err = testSuplTermQop.GetMaxLocationAgeL( aItem );
    return err;
    }    

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::GetDelayL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::GetDelayL( CStifItemParser& aItem )
    {
    TestSuplTerminalQop testSuplTermQop( iLog );
    TInt err = testSuplTermQop.GetDelayL( aItem );
    return err;
    }
    
// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::ConnectCloseL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::ConnectCloseL( CStifItemParser& aItem )
    {
    TestRSuplTerminalServer testRSuplTermServer( iLog );
    TInt err = testRSuplTermServer.ConnectCloseL( aItem );
    return err;
    }    
    
// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::VersionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::VersionL( CStifItemParser& aItem )
    {
    TestRSuplTerminalServer testRSuplTermServer( iLog );
    TInt err = testRSuplTermServer.VersionL( aItem );
    return err;
    }    

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::OpenCloseL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::OpenCloseL( CStifItemParser& aItem )
    {
    CTestRSuplTerminalSubSession* testRSuplTermSubSession = CTestRSuplTerminalSubSession::NewL( iLog );
    CleanupStack::PushL( testRSuplTermSubSession );
    TInt err = testRSuplTermSubSession->OpenCloseL( aItem );
    CleanupStack::PopAndDestroy( testRSuplTermSubSession );
    return err;
    }    

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::GetPositionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestSuplTerminalInitApi::GetPositionL( CStifItemParser& aItem )
    {
    CTestRSuplTerminalSubSession* testRSuplTermSubSession = CTestRSuplTerminalSubSession::NewL( iLog );
    CleanupStack::PushL( testRSuplTermSubSession );
    TInt err = testRSuplTermSubSession->GetPositionL( aItem );
    CleanupStack::PopAndDestroy( testRSuplTermSubSession );
    return err;
    }    


//  End of File
