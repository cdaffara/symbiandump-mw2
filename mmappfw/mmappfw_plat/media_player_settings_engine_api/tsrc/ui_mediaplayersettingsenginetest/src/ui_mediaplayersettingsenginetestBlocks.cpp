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
* Description: Cui_mediaplayersettingsenginetest block implementation for STIF Test Framework TestScripter
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "ui_mediaplayersettingsenginetest.h"

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

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
// Cui_mediaplayersettingsenginetest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cui_mediaplayersettingsenginetest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Cui_mediaplayersettingsenginetest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cui_mediaplayersettingsenginetest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", Cui_mediaplayersettingsenginetest::ExampleL ),
        ENTRY( "CreateCMPSettingsModel", Cui_mediaplayersettingsenginetest::CreateCMPSettingsModel ),
        ENTRY( "DestructCMPSettingsModel", Cui_mediaplayersettingsenginetest::DestructCMPSettingsModel ),
        ENTRY( "MPSListImplementations", Cui_mediaplayersettingsenginetest::MPSListImplementations ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Cui_mediaplayersettingsenginetest::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_mediaplayersettingsenginetest::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Kui_mediaplayersettingsenginetest, "ui_mediaplayersettingsenginetest" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Kui_mediaplayersettingsenginetest, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Kui_mediaplayersettingsenginetest, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Cui_mediaplayersettingsenginetest::CreateCMPSettingsModel
// CreateCMPSettingsModel test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_mediaplayersettingsenginetest::CreateCMPSettingsModel( CStifItemParser& /*aItem*/ )
    {
    TInt err = KErrNone;
 
    const TUid KSettingsModelForROPUid = {KMPSettEngImplUidROP};
    TRAP(err, iRopSettingsModel = CMPSettingsModel::NewL(KSettingsModelForROPUid));
  
    if (err==KErrNone)
    	{
    	iLog->Log( _L("Create CMPSettingsModel Success!"));
    	}
    else
    	{
    	iLog->Log( _L("Create CMPSettingsModel Faile! %d"), err );
    	}
    return  err ;
    }
// -----------------------------------------------------------------------------
// Cui_mediaplayersettingsenginetest::DestructCMPSettingsModel
// DestructCMPSettingsModel test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_mediaplayersettingsenginetest::DestructCMPSettingsModel( CStifItemParser& /*aItem*/ )
    {
    TInt err = KErrNone;
    if(iRopSettingsModel)
	   {
		//REComSession::FinalClose();
	   delete iRopSettingsModel;
	   iRopSettingsModel = NULL;
	   }
	iLog->Log( _L("Create CMPSettingsModel Success!"));
    return err;
    }

// -----------------------------------------------------------------------------
// Cui_mediaplayersettingsenginetest::MPSListImplementations
// MPSListImplementations test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_mediaplayersettingsenginetest::MPSListImplementations( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("Test ListImplementationsL,begins!") );
    TInt result( KErrNone );
    RImplInfoPtrArray impInfoArray;
    
    TRAP( result, CMPSettingsModel::ListImplementationsL(impInfoArray));
  
    if( result == KErrNone )
    	{
    	iLog->Log( _L("Succeeds!The details of the implementation array are listed here:") );
    	for( TInt i=0;i<impInfoArray.Count();i++ )
    		{
    		iLog->Log( _L("The class name of impInfoArray[%d] is:"),i );
    		iLog->Log( impInfoArray[i]->DisplayName() );
    		iLog->Log( _L("Its uid is %d."),impInfoArray[i]->ImplementationUid().iUid );
    		}
    	iLog->Log( _L("End!") );
    	}
    else
    	{
    	iLog->Log( _L("Fails,error %d returns!End!"),result );
    	}
    impInfoArray.Close();
    return result;
    }

// -----------------------------------------------------------------------------
// Cui_mediaplayersettingsenginetest::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt Cui_mediaplayersettingsenginetest::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
