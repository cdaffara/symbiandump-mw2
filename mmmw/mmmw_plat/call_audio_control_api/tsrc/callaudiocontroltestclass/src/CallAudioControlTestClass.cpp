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
* Description: Call Audio Control
*
*/



// INCLUDE FILES
#include <StifTestInterface.h>
#include "CallAudioControlTestClass.h"

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
// CCallAudioControlTestClass::CCallAudioControlTestClass
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCallAudioControlTestClass::CCallAudioControlTestClass(
    CTestModuleIf& aTestModuleIf ):
    CScriptBase( aTestModuleIf ),
    iTestModuleIf(aTestModuleIf)
  //  iPlayBufPtr(0,0,0),
   // iRecBufPtr(0,0,0)
{
	//iDnLinkCodec = ENULL;
	//iUpLinkCodec = ENULL;
	//iDnLinkStatus = ENotReady;
	//iUpLinkStatus = ENotReady;
}

// -----------------------------------------------------------------------------
// CCallAudioControlTestClass::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCallAudioControlTestClass::ConstructL()
{
    iLog = CStifLogger::NewL(KCallAudioControlTestClassLogPath,
                          	 KCallAudioControlTestClassLogFile,
                             CStifLogger::ETxt,
                             CStifLogger::EFile,
                             EFalse );

    iTimeoutController = CSimpleTimeout::NewL (this, iLog);
}

// -----------------------------------------------------------------------------
// CCallAudioControlTestClass::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCallAudioControlTestClass* CCallAudioControlTestClass::NewL(
    CTestModuleIf& aTestModuleIf )
{
    CCallAudioControlTestClass* self = new (ELeave) CCallAudioControlTestClass( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

}

// Destructor
CCallAudioControlTestClass::~CCallAudioControlTestClass()
{

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog;
    delete iTimeoutController;
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

    return ( CScriptBase* ) CCallAudioControlTestClass::NewL( aTestModuleIf );

}


//  End of File
