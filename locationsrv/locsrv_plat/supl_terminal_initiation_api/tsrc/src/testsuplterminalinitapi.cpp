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
#include <Stiftestinterface.h>
#include "testsuplterminalinitapi.h"




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::CTestSuplTerminalInitApi
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestSuplTerminalInitApi::CTestSuplTerminalInitApi( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTestSuplTerminalInitApi::ConstructL()
    {
    iLog = CStifLogger::NewL( KtestsuplterminalinitapiLogPath, 
                          KtestsuplterminalinitapiLogFile,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );

    }

// -----------------------------------------------------------------------------
// CTestSuplTerminalInitApi::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestSuplTerminalInitApi* CTestSuplTerminalInitApi::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CTestSuplTerminalInitApi* self = new (ELeave) CTestSuplTerminalInitApi( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CTestSuplTerminalInitApi::~CTestSuplTerminalInitApi()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

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

    return ( CScriptBase* ) CTestSuplTerminalInitApi::NewL( aTestModuleIf );

    }


//  End of File
