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
* Description:    Implementation of Ctestlmksearchapi class
*
*/






// INCLUDE FILES
#include <Stiftestinterface.h>
#include "testlmksearchapi.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::Ctestlmksearchapi
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
Ctestlmksearchapi::Ctestlmksearchapi( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Ctestlmksearchapi::ConstructL()
    {
    iLog = CStifLogger::NewL( KtestlmksearchapiLogPath, 
                          KtestlmksearchapiLogFile,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );

    }

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Ctestlmksearchapi* Ctestlmksearchapi::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    Ctestlmksearchapi* self = new (ELeave) Ctestlmksearchapi( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
Ctestlmksearchapi::~Ctestlmksearchapi()
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

    return ( CScriptBase* ) Ctestlmksearchapi::NewL( aTestModuleIf );

    }


//  End of File
