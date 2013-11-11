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
* Description:  Scripted STIF cases for MPXCollectionHelper
*
*/

// INCLUDE FILES
#include <Stiftestinterface.h>
#include "collectionhelpertest.h"


// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCollectionHelperTest::CCollectionHelperTest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCollectionHelperTest::CCollectionHelperTest( CTestModuleIf& aTestModuleIf ):
        CBaseTest( aTestModuleIf )
    {	
    }
    
CCollectionHelperTest::~CCollectionHelperTest()
    { 
    // Delete resources allocated from test methods
    Delete();
    
	iCollectionUtility->Close();
	iCollectionUtility = NULL;
    }

// -----------------------------------------------------------------------------
// CCollectionHelperTest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCollectionHelperTest* CCollectionHelperTest::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CCollectionHelperTest* self = new (ELeave) CCollectionHelperTest( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CCollectionHelperTest::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCollectionHelperTest::ConstructL()
    {        
    CBaseTest::ConstructL();
    
    iLog = CStifLogger::NewL( KCollectionHelperTestLogPath, 
                      KCollectionHelperTestLogFile,
                      CStifLogger::ETxt,
                      CStifLogger::EFile,
                      EFalse );
    
    iCollectionUtility = MMPXCollectionUtility::NewL(this, KMcModeDefault);
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

    return ( CScriptBase* ) CCollectionHelperTest::NewL( aTestModuleIf );

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
