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
* Description:  STIF testing for MPX Harvester
*
*/


// INCLUDE FILES
#include <Stiftestinterface.h>
#include <SettingServerClient.h>
#include "mpxharvestertest.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Cmpxharvestertest::Cmpxharvestertest
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
Cmpxharvestertest::Cmpxharvestertest(
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    iHarvester = NULL;
    iCollection = NULL;
    iFileMan = NULL;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::ConstructL()
    {
    iLog = CStifLogger::NewL( KmpxharvestertestLogPath,
                          KmpxharvestertestLogFile,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
	User::LeaveIfError( iFs.Connect() );
	iFileMan = CFileMan::NewL(iFs);

	TBool err = KErrNone;
	err = iFs.MkDir(Kmpxharvester_soundsPath);
	if ( err == KErrNone || err == KErrAlreadyExists )
		{	
		err = iFs.MkDir(Kmpxharvester_digitalPath);
		if ( err != KErrNone && err != KErrAlreadyExists )
			{
			User::Leave(err);
			}	
		}
	else
		{
		User::Leave(err);
		}

	// Print title of the test case
    TName title;
    TestModuleIf().GetTestCaseTitleL(title);
    iLog->Log(_L(" "));
    iLog->Log(_L("[Title] %S"), &title);
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
Cmpxharvestertest* Cmpxharvestertest::NewL(
    CTestModuleIf& aTestModuleIf )
    {
    Cmpxharvestertest* self = new (ELeave) Cmpxharvestertest( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
Cmpxharvestertest::~Cmpxharvestertest()
    {
    // Delete resources allocated from test methods
    Delete();
    // Delete logger
    delete iLog;
	delete iFileMan;
	iFs.Close();
    }

// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// ---------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL(
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {
    return ( CScriptBase* ) Cmpxharvestertest::NewL( aTestModuleIf );
    }
