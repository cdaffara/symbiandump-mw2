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
* Description:  ui_mpxViewUtilityTest block implemetation for STIF Test Framework TestScripter
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>

#include "ui_mpxViewUtilityTest.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cui_mpxViewUtilityTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cui_mpxViewUtilityTest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Cui_mpxViewUtilityTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cui_mpxViewUtilityTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
        		
        ENTRY( "TestMPXViewUtility", Cui_mpxViewUtilityTest::TestMPXViewUtility ),
        
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }
// -----------------------------------------------------------------------------
// Cui_mpxViewUtilityTest::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_mpxViewUtilityTest::TestMPXViewUtility( CStifItemParser& )
	{
	TInt err = KErrNone;
	TRAP( err, iMPXViewUtility = MMPXViewUtility::UtilityL());


	iMPXViewUtility->Close();

	
	if(err!=KErrNone)
		{
		iLog->Log(_L("Cui_mpxViewUtilityTest::TestMPXViewUtility: %d"), err);
		}
	
	return err;
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
