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
* Description:  Implementation of settings api classes
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include "settingsapi.h"

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
// CSettingsApi::CSettingsApi
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSettingsApi::CSettingsApi( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CSettingsApi::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSettingsApi::ConstructL()
    {
    iLog = CStifLogger::NewL( KsuplserverLogPath, 
                          KsuplserverLogFile,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
                          iErrCode=100;
    }

// -----------------------------------------------------------------------------
// CSettingsApi::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSettingsApi* CSettingsApi::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CSettingsApi* self = new (ELeave) CSettingsApi( aTestModuleIf );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    
    }
    
// Destructor
CSettingsApi::~CSettingsApi()
    { 
    
    // Delete resources allocated from test methods
    Delete();
    
    // Delete logger
    delete iLog; 
    delete iScheduler; 
    
    }
void CSettingsApi::HandleSuplTriggerStatusChangeL(
               CSuplSettings::TSuplTriggerStatus /* aSuplTriggerStatus */ ) 
    {
    iLog->Log(_L("Callback function HandleSuplTriggerStatusChange(); ")) ;
   // CActiveScheduler::Stop();
    }
// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {
    
    return ( CScriptBase* ) CSettingsApi::NewL( aTestModuleIf );
        
    }

// -----------------------------------------------------------------------------
// E32Dll is a DLL entry point function.
// Returns: KErrNone
// -----------------------------------------------------------------------------
//
#ifndef EKA2 // Hide Dll entry point to EKA2
GLDEF_C TInt E32Dll(
    TDllReason /*aReason*/) // Reason code
    {
    return(KErrNone);
    
    }
#endif // EKA2
    
//  End of File
