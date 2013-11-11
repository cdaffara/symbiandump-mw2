/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Implementation of CBCBlidTest class     
*
*/









// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "BCBlidTest.h"

#include <CSatelliteInfoUi.h>


// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
_LIT(KSvrName,"Blid App");

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CBCBlidTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CBCBlidTest::Delete() 
    {

    }


// -----------------------------------------------------------------------------
// CBCBlidTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CBCBlidTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", CBCBlidTest::ExampleL ),
        ENTRY( "TestNewL", CBCBlidTest::TestNewL ),
        ENTRY( "TestExecuteLD", CBCBlidTest::TestExecuteLD ),
        ENTRY( "TestHandleForegroundEventL", CBCBlidTest::TestHandleForegroundEventL ),
        ENTRY( "TestSetLaunchView", CBCBlidTest::TestSetLaunchViewL ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CBCBlidTest::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CBCBlidTest::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KBCBlidTest, "BCBlidTest" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KBCBlidTest, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KBCBlidTest, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }


TInt CBCBlidTest::TestNewL( CStifItemParser& aItem )
    {

			CSatelliteInfoUI* iSatInfoUi = CSatelliteInfoUI::NewL();
			
			delete iSatInfoUi;
			iSatInfoUi = NULL;

    return KErrNone;

    }

TInt CBCBlidTest::TestExecuteLD( CStifItemParser& aItem )
    {

			CSatelliteInfoUI* iSatInfoUi = CSatelliteInfoUI::NewL();
			TRAPD(result, iSatInfoUi->ExecuteLD(KSvrName));

    return KErrNone;

    }


TInt CBCBlidTest::TestHandleForegroundEventL( CStifItemParser& aItem )
    {

			CSatelliteInfoUI* iSatInfoUi = CSatelliteInfoUI::NewL();
			TRAPD(result, iSatInfoUi->HandleForegroundEventL(ETrue));
		
			delete iSatInfoUi;
			iSatInfoUi = NULL;

    return KErrNone;

    }

TInt CBCBlidTest::TestSetLaunchViewL(CStifItemParser& aItem)
{
#ifdef __S60_32__                    
 
			CSatelliteInfoUI* iSatInfoUi = CSatelliteInfoUI::NewL();
			iSatInfoUi->SetLaunchView(CSatelliteInfoUI::ESatelliteFirmamentView);
			TRAPD(result, iSatInfoUi->ExecuteLD(KSvrName));

 #endif        

	return KErrNone;
}

// -----------------------------------------------------------------------------
// CBCBlidTest::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CBCBlidTest::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
