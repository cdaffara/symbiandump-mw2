/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains member functions and library entry point.
*
*/

// INCLUDE FILES
#include <stiftestinterface.h>
#include <ecom/ecom.h>
#include <e32cmn.h>
#include <epos_comasuplposhandlerbase.h>

#include "epos_comasuplpostestermodulecfg.h"
#include "epos_comasuplecomhandler.h"
#include "epos_comasuplconsolemanager.h"
#include "epos_comasupltesthandler.h"
#include "epos_comasuplpostesterlogger.h"
#include "epos_comasuplpostestercategory.h"
#include "epos_comasuplthreadprocesstest.h"
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::COMASuplPosTesterModuleCfg
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
COMASuplPosTesterModuleCfg::COMASuplPosTesterModuleCfg( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    iTestingCancelled = EFalse;
    iTestingStatus = ETestNotStarted;
    }

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplPosTesterModuleCfg::ConstructL()
    {
    iLog = CStifLogger::NewL( KPosTesterModuleCfgLogPath, 
								KPosTesterModuleCfgLogFile,
								CStifLogger::ETxt,
								CStifLogger::EFile,
								EFalse,// overwrite is not used
								ETrue,// Time stamp is used
								ETrue,// Line break is used
								EFalse,// Even ranking is not used
								EFalse,// Thread id is not added after the log file
								ETrue,// Log directory will create
								2048);//Buffer size
	iEComHandler = COMASuplEComHandler::NewL(0x102073D4);
	iConsoleManager = COMASuplConsoleManager::NewL( 
							iEComHandler->GetImplementationList());
	iTestHandler = COMASuplTestHandler::NewL(iConsoleManager->GetConsole());
	iThreadTester = COMASuplThreadProcessTest::NewL();
	iThreadTester->StatusBeforeTestsL();
    }

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplPosTesterModuleCfg* COMASuplPosTesterModuleCfg::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    COMASuplPosTesterModuleCfg* self = new (ELeave) COMASuplPosTesterModuleCfg( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;

    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplPosTesterModuleCfg::~COMASuplPosTesterModuleCfg()
    { 

    // Delete resources allocated from test methods
    Delete();

	delete iPosHandler;//delete pos handler before deleting thread tester
    delete iThreadTester;
    // Delete logger
    delete iLog;
	
	delete iPosLogger;
	delete iTestHandler;
	delete iConsoleManager;
	delete iEComHandler;
	REComSession::FinalClose();
    }

// ---------------------------------------------------------------------------
// Prepares filename from the displayname of the implementation.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterModuleCfg::PrepareFileName(TFileName& aOutputFileName, 
							const CImplementationInformation& aImplInfo)
	{
	TInt count = 0;
	TInt displayNameLength = aImplInfo.DisplayName().Length();

	for(count = 0; count < displayNameLength 
					&& count < KMaxFileName - 12; count++)
		{
		if(IsValidFileNameChar(aImplInfo.DisplayName().Ptr() + count))
			{
			aOutputFileName.Append(aImplInfo.DisplayName().Ptr() + count, 1);
			}
		}

	aOutputFileName.Append(_L("_"));
	aOutputFileName.Append(aImplInfo.ImplementationUid().Name().Ptr() + 1
					, aImplInfo.ImplementationUid().Name().Length() - 2);
	//Append file extension
	aOutputFileName.Append(_L(".txt"));
	}

// ---------------------------------------------------------------------------
// Prepares filename from the displayname of the implementation.
// ---------------------------------------------------------------------------
//
TBool COMASuplPosTesterModuleCfg::IsValidFileNameChar(const TUint16* aChar)
	{
	_LIT(KInvalidCharList, " &^+-@$%*()|\\/[]{}<>?;:,\"'");
	for(TInt counter = 0; counter < KInvalidCharList().Length(); counter++)
		{
		if(KInvalidCharList()[counter] == *aChar)
			{
			return EFalse;
			}
		}
	return ETrue;
	}

// ---------------------------------------------------------------------------
// Adds number of info, warning and error messages to member variables.
// ---------------------------------------------------------------------------
//	
void COMASuplPosTesterModuleCfg::UpdateSummary(
							COMASuplPosTesterCategory* aTestCat)
	{
	TInt infoCount = 0;
	TInt warningCount = 0;
	TInt errorCount = 0;
	aTestCat->GetTestSummary(infoCount, warningCount, errorCount);
	iNetInfos += infoCount;
	iNetWarnings += warningCount;
	iNetErrors += errorCount;
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
    return ( CScriptBase* ) COMASuplPosTesterModuleCfg::NewL( aTestModuleIf );
    }

//  End of File
