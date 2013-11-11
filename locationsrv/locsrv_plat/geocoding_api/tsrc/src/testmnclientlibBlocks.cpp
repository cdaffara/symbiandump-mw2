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
* Description:  Test Module DLL to Geocoding API
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <mnerrors.h>

#include "testmnclientlib.h"
#include "testmngeocoder.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestMNClientLib::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTestMNClientLib::Delete() 
    {
	if(iTestMnGeocoder)
		{
		delete iTestMnGeocoder;
		iTestMnGeocoder = NULL;
		}
    }

// -----------------------------------------------------------------------------
// CTestMNClientLib::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestMNClientLib::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "TestNewL", CTestMNClientLib::TestNewL ),
        ENTRY( "TestNewChainedL", CTestMNClientLib::TestNewChainedL ),
        ENTRY( "TestGetSetOptionsL", CTestMNClientLib::TestGetSetOptionsL ),
        ENTRY( "TestFindAddressByCoordinateL", CTestMNClientLib::TestFindAddressByCoordinateL ),
       	ENTRY( "TestFindCoordinateByAddressL", CTestMNClientLib::TestFindCoordinateByAddressL ),
        ENTRY( "TestFindCoordinateByAddressDescL", CTestMNClientLib::TestFindCoordinateByAddressDescL ),
        ENTRY( "TestSetExitObserverL", CTestMNClientLib::TestSetExitObserverL ),
        ENTRY( "TestRemoveExitObserverL", CTestMNClientLib::TestRemoveExitObserverL ),
        ENTRY( "TestCancelRequestL", CTestMNClientLib::TestCancelRequestL ),
        ENTRY( "TestRequestPanicL", CTestMNClientLib::TestRequestPanicL ),
        
         
        //ADD NEW ENTRY HERE

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// ---------------------------------------------------------
// CTestMNClientLib::TestNewL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMNClientLib::TestNewL( CStifItemParser& /*aItem*/ )
    {
    return iTestMnGeocoder->TestNewL();

    }

// ---------------------------------------------------------
// CTestMNClientLib::TestNewChainedL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMNClientLib::TestNewChainedL( CStifItemParser& /*aItem*/ )
    {
    return iTestMnGeocoder->TestNewChainedL();

    }

// ---------------------------------------------------------
// CTestMNClientLib::TestGetSetOptionsL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMNClientLib::TestGetSetOptionsL( CStifItemParser& aItem )
    {
    return iTestMnGeocoder->TestGetSetOptionsL( aItem );

    }

// ---------------------------------------------------------
// CTestMNClientLib::TestFindAddressByCoordinateL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMNClientLib::TestFindAddressByCoordinateL( CStifItemParser& aItem )
    {
    return iTestMnGeocoder->TestFindAddressByCoordinateL( aItem );

    }

// ---------------------------------------------------------
// CTestMNClientLib::TestGetSetOptionsL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMNClientLib::TestFindCoordinateByAddressL( CStifItemParser& aItem )
    {
    return iTestMnGeocoder->TestFindCoordinateByAddressL( aItem );

    }

// ---------------------------------------------------------
// CTestMNClientLib::TestGetSetOptionsL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMNClientLib::TestFindCoordinateByAddressDescL( CStifItemParser& aItem )
    {
    return iTestMnGeocoder->TestFindCoordinateByAddressDescL( aItem );

    }

// ---------------------------------------------------------
// CTestMNClientLib::TestSetExitObserverL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMNClientLib::TestSetExitObserverL( CStifItemParser& /*aItem*/ )
    {
    return iTestMnGeocoder->TestSetExitObserverL( );

    }
    
// ---------------------------------------------------------
// CTestMNClientLib::TestRemoveExitObserverL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMNClientLib::TestRemoveExitObserverL( CStifItemParser& /*aItem*/ )
    {
    return iTestMnGeocoder->TestRemoveExitObserverL( );

    }

// ---------------------------------------------------------
// CTestMNClientLib::TestCancelRequestL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMNClientLib::TestCancelRequestL( CStifItemParser& aItem )
    {
    return iTestMnGeocoder->TestCancelRequestL( aItem );

    }
    
// ---------------------------------------------------------
// CTestMNClientLib::TestRequestPanicL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMNClientLib::TestRequestPanicL( CStifItemParser& aItem )
    {
    TestModuleIf().SetExitReason(CTestModuleIf::EPanic , KMnPanicDuplicateRequest );
    return iTestMnGeocoder->TestRequestPanicL( aItem );

    }

//  End of File
