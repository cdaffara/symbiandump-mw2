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
* Description:  Contains member functions.
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <stifparser.h>
#include <stiftestinterface.h>
#include "epos_comasuplpostestermodulecfg.h"
#include <ecom/implementationinformation.h>
#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpospayload.h>

#include "epos_comasuplecomhandler.h"
#include "epos_comasuplconsolemanager.h"
#include "epos_comasuplposhandlercreationtest.h"
#include "epos_comasupltesthandler.h"
#include "epos_comasuplpostesterlogger.h"
#include <ecom/ecom.h>
#include "epos_comasuplpostesterlogger.h"
#include "epos_comasupluid_ecom_test.h"
#include "epos_comasuplpossessioncreationtest.h"
#include "epos_comasuplgetsuplinfotest.h"
#include "epos_comasuplgetsuplinfocanceltest.h"
#include "epos_comasuplmessagefilereader.h"
#include "epos_comasuplmsgfilereadertest.h"
#include "epos_comasuplpositionvelocitytest.h"
#include "epos_comasuplpositioncanceltest.h"
#include "epos_comasuplcancelinitializetest.h"
#include "epos_comasuplpositiontest.h"
#include "epos_comasuplgetsuplinfomanager.h"
#include "epos_comasuplsessiontest.h"
#include "epos_comasuplthreadprocesstest.h"
#include "epos_comasuplmultiplesessiontest.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void COMASuplPosTesterModuleCfg::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "SuplGatewayTest", COMASuplPosTesterModuleCfg::SuplGatewayTestL ),
        ENTRY( "GetUserSelection", COMASuplPosTesterModuleCfg::GetUserSelectionL ),
        ENTRY( "Uid_EComTest", COMASuplPosTesterModuleCfg::Uid_ECom_TestL ),
        ENTRY( "PosHandlerCreation", COMASuplPosTesterModuleCfg::PosHandlerCreationL ),
        ENTRY( "PosSessionCreation", COMASuplPosTesterModuleCfg::PosSessionCreationL ),
        ENTRY( "GetSuplInfoTest", COMASuplPosTesterModuleCfg::GetSuplInfoTestL ),
        ENTRY( "GetSuplInfoCancelTest", COMASuplPosTesterModuleCfg::GetSuplInfoCancelTestL ),
        ENTRY( "MessageReader", COMASuplPosTesterModuleCfg::MessageReaderL ),
        ENTRY( "WriteSummary", COMASuplPosTesterModuleCfg::WriteSummaryL ),
        ENTRY( "PositionVelocityTest", COMASuplPosTesterModuleCfg::PositionVelocityTestL),
        ENTRY( "PositionCancellationTest", COMASuplPosTesterModuleCfg::PositionCancellationTestL),
        ENTRY( "InitializeCancelTest", COMASuplPosTesterModuleCfg::InitializeCancelTestL),
        ENTRY( "PositionTest", COMASuplPosTesterModuleCfg::PositionTestL),
        ENTRY( "SessionTest", COMASuplPosTesterModuleCfg::SessionTestL),
        ENTRY( "MultipleSessionTest", COMASuplPosTesterModuleCfg::MultipleSessionTestL),
        ENTRY( "WriteTestSummary", COMASuplPosTesterModuleCfg::WriteSummaryL),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt COMASuplPosTesterModuleCfg::SuplGatewayTestL( CStifItemParser& /*aItem*/ )
	{
	TInt threadCount = 0;
	TFindThread findThread;
	TFullName threadName;
	TFullName processName(_L("epossuplgateway"));
	iThreadTester->CountThreadsL(processName, threadCount);
	if(threadCount == 0)
		{
		return KErrNone;
		}
	else
		{
		iLog->Log(_L8("SuplGateway is running."));
		return KErrAlreadyExists;
		}
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::GetUserSelectionL( CStifItemParser& /*aItem*/ )
    {
    const RImplInfoPtrArray& implList = iEComHandler->GetImplementationList();
	TInt userSelectedIndex = iConsoleManager->GetUserSelection();
	if(-4 == userSelectedIndex)
		{
		iLog->Log(_L8("Testing canceled before selecting plug-in."));
		iTestingCancelled = ETrue;
		return KErrNone;
		}
	if(0 > userSelectedIndex)
		{
		iTestingCancelled = ETrue;
		iLog->Log(_L8("No plugin found."));
		return KErrNotFound;
		}
	//Store user selected implementation in member variable
    iSelectedImpl = implList[userSelectedIndex];
    PrepareFileName(iLogFileName, *iSelectedImpl);
    iPosLogger = COMASuplPosTesterLogger::NewL(iLogFileName);
    iThreadTester->InitializeL(iTestHandler, *iPosLogger);
	iPosLogger->WriteImplementationInfoL(*iSelectedImpl);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::Uid_ECom_TestL( CStifItemParser& /*aItem*/ )
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}
	iPosLogger->WriteTestHeader(KUid_ECom_Test);
	COMASuplPosTesterCategory* uidEcomTest = COMASuplUid_ECom_Test::NewL(*iSelectedImpl);
	CleanupStack::PushL(uidEcomTest);
	iTestHandler->InitializeL(uidEcomTest);
	uidEcomTest->InitializeL(iTestHandler, *iPosLogger);
	iTestHandler->StartTest();
	UpdateSummary(uidEcomTest);
	iTestingStatus = uidEcomTest->GetTestingStatus();
	CleanupStack::PopAndDestroy(uidEcomTest);
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::PosHandlerCreationL( CStifItemParser& /*aItem*/ )
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}

	iPosLogger->WriteTestHeader(KPosHandlerCreationTest);
	COMASuplPosTesterCategory* posHandler = COMASuplPosHandlerCreationTest
								::NewL(iSelectedImpl->ImplementationUid());
	CleanupStack::PushL(posHandler);
	iTestHandler->InitializeL(posHandler);
	posHandler->InitializeL(iTestHandler, *iPosLogger);
	iTestHandler->StartTest();
	UpdateSummary(posHandler);
	iTestingStatus = posHandler->GetTestingStatus();
	CleanupStack::PopAndDestroy(posHandler);
	if(ETestComplete != iTestingStatus)
		{
		iTestingCancelled = ETrue;
		}
	else
		{
		iPosHandler = COMASuplPosHandlerBase::NewL(iSelectedImpl->ImplementationUid());
		TRequestStatus status;
		iPosHandler->InitializeL(status);
		User::WaitForRequest(status);
		}
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::PosSessionCreationL( CStifItemParser& /*aItem*/ )
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}
	iPosLogger->WriteTestHeader(KPosSessionCreationTest);
	COMASuplPosTesterCategory* posSessionCreationTest 
					= COMASuplPosSessionCreationTest::NewL(*iSelectedImpl);
	CleanupStack::PushL(posSessionCreationTest);
	iTestHandler->InitializeL(posSessionCreationTest);
	posSessionCreationTest->InitializeL(iTestHandler, *iPosLogger);
	iTestHandler->StartTest();
	UpdateSummary(posSessionCreationTest);
	iTestingStatus = posSessionCreationTest->GetTestingStatus();
	CleanupStack::PopAndDestroy(posSessionCreationTest);
	if(ETestComplete != iTestingStatus)
		{
		iTestingCancelled = ETrue;
		}
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::GetSuplInfoTestL(CStifItemParser& aItem)
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}
	iPosLogger->WriteTestHeader(KGetSuplInfoTest);
	TInt setCapReqCount = 1;
	TInt reqAsstDataCount = 1;
	TInt velocityCount = 1;
	TInt positionCount = 1;
	TInt payloadCount = 1;
	TInt counter = 0;
	TInt value = 1;
	//read Integer values from aItem
	while(KErrNone == aItem.GetNextInt( value ))
		{
		switch (++counter)
			{
			case 1:setCapReqCount = value;break;
			case 2:reqAsstDataCount = value;break;
			case 3:velocityCount = value;break;
			case 4:positionCount = value;break;
			case 5:payloadCount = value;break;
			default:break;
			}
		}
		
	COMASuplGetSuplInfoTest* getSuplInfo = COMASuplGetSuplInfoTest::NewL(
								*iPosHandler);
	COMASuplPosTesterCategory* getsuplInfoTest = getSuplInfo;
	CleanupStack::PushL(getsuplInfoTest);
	iTestHandler->InitializeL(getsuplInfoTest);
	getsuplInfoTest->InitializeL(iTestHandler, *iPosLogger);
	TRAPD(err, getSuplInfo->CreateRequestListL(setCapReqCount
									, reqAsstDataCount
									, positionCount
									, velocityCount
									, payloadCount));
	if(KErrNone != err)
		{
		iPosLogger->WriteStatus(KLeaveRequestListCreation(), err, 0);
		User::Leave(err);
		}

	iTestHandler->StartTest();
	UpdateSummary(getSuplInfo);
	iTestingStatus = getSuplInfo->GetTestingStatus();
	CleanupStack::PopAndDestroy(getsuplInfoTest);
	if(ETestComplete != iTestingStatus)
		{
		iTestingCancelled = ETrue;
		}
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::GetSuplInfoCancelTestL( CStifItemParser& aItem)
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}
	iPosLogger->WriteTestHeader(KGetSuplInfoCancelTest);
	TInt setCapReqCount = 1;
	TInt reqAsstDataCount = 1;
	TInt velocityCount = 1;
	TInt positionCount = 1;
	TInt payloadCount = 1;
	TInt counter = 0;
	TInt value = 1;
	//read Integer values from aItem
	while(KErrNone == aItem.GetNextInt( value ))
		{
		switch (++counter)
			{
			case 1:setCapReqCount = value;break;
			case 2:reqAsstDataCount = value;break;
			case 3:velocityCount = value;break;
			case 4:positionCount = value;break;
			case 5:payloadCount = value;break;
			default:break;
			}
		}
		
	COMASuplGetSuplInfoCancelTest* getsuplInfoCancel
	 = COMASuplGetSuplInfoCancelTest::NewL(*iPosHandler);
	 
	COMASuplPosTesterCategory* testerCat = getsuplInfoCancel;
	CleanupStack::PushL(getsuplInfoCancel);
	iTestHandler->InitializeL(testerCat);
	testerCat->InitializeL(iTestHandler, *iPosLogger);
	TRAPD(err, getsuplInfoCancel->CreateRequestListL(setCapReqCount
									, reqAsstDataCount
									, positionCount
									, velocityCount
									, payloadCount));
	if(KErrNone != err)
		{
		iPosLogger->WriteStatus(KLeaveRequestListCreation(), err, 0);
		User::Leave(err);
		}

	iTestHandler->StartTest();
	UpdateSummary(testerCat);
	iTestingStatus = testerCat->GetTestingStatus();
	CleanupStack::PopAndDestroy(getsuplInfoCancel);
	if(ETestComplete != iTestingStatus)
		{
		iTestingCancelled = ETrue;
		}
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::MessageReaderL(CStifItemParser& aItem)
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}
	TPtrC string;
	if( aItem.GetNextString ( string ) != KErrNone )
		{
		//Do nothing. default file given in rss file will be used
		//return KErrNotFound;
		}
	iPosLogger->WriteTestHeader(KMessageReaderTest);
	TFileName testFile(string);
	COMASuplPosTesterCategory* fileReaderTest = NULL;
	TRAPD(err, fileReaderTest = COMASuplMsgFileReaderTest
										::NewL(testFile));
	if(err != KErrNone)
		{
		iPosLogger->WriteLine(_L8("Error Creating fileReaderTest"));
		return err;
		}
	CleanupStack::PushL(fileReaderTest);
	iTestHandler->InitializeL(fileReaderTest);
	iPosLogger->WriteLine(_L8("TestHandler Initialized"));
	fileReaderTest->InitializeL(iTestHandler, *iPosLogger);
	iPosLogger->WriteLine(_L8("fileReaderTest Initialized"));
	iTestHandler->StartTest();
	UpdateSummary(fileReaderTest);
	iTestingStatus = fileReaderTest->GetTestingStatus();
	CleanupStack::PopAndDestroy(fileReaderTest);
	if(ETestComplete != iTestingStatus)
		{
		iTestingCancelled = ETrue;
		}
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::PositionVelocityTestL(CStifItemParser& aItem)
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}
	TInt posReqCount = 1;
	if(KErrNone != aItem.GetNextInt(posReqCount))
		{
		return KErrNotFound;
		}

	iPosLogger->WriteTestHeader(KPositionVelocityTest);
	TPtrC string;
	if( aItem.GetNextString ( string ) != KErrNone )
		{
		//Do nothing. default file given in rss file will be used
		//return KErrNotFound;
		}
	TFileName testFile(string);
	COMASuplPosTesterCategory* posVeloTest = NULL;
	TRAPD(err, posVeloTest = COMASuplPositionVelocityTest::NewL(iPosHandler
											, posReqCount, testFile));
	if(err != KErrNone)
		{
		return err;
		}
	CleanupStack::PushL(posVeloTest);
	iTestHandler->InitializeL(posVeloTest);
	posVeloTest->InitializeL(iTestHandler, *iPosLogger);
	iTestHandler->StartTest();
	UpdateSummary(posVeloTest);
	iTestingStatus = posVeloTest->GetTestingStatus();
	CleanupStack::PopAndDestroy(posVeloTest);
	if(ETestComplete != iTestingStatus)
		{
		iTestingCancelled = ETrue;
		}
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::PositionCancellationTestL(CStifItemParser& aItem)
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}
	iPosLogger->WriteTestHeader(KPositionCancelTest);
	TInt posReqCount = 1;
	if(KErrNone != aItem.GetNextInt(posReqCount))
		{
		return KErrNotFound;
		}

	TPtrC string;
	if( aItem.GetNextString ( string ) != KErrNone )
		{
		//Do nothing. default file given in rss file will be used
		//return KErrNotFound;
		}
	TFileName testFile(string);
	
	COMASuplPosTesterCategory* posCancelTest = NULL;
	TRAPD(err, posCancelTest = COMASuplPositionCancelTest::NewL(iPosHandler
												, posReqCount, testFile));
	if(err != KErrNone)
		{
		return err;
		}
	CleanupStack::PushL(posCancelTest );
	iTestHandler->InitializeL(posCancelTest );
	posCancelTest ->InitializeL(iTestHandler, *iPosLogger);
	iTestHandler->StartTest();
	UpdateSummary(posCancelTest );
	iTestingStatus = posCancelTest ->GetTestingStatus();
	CleanupStack::PopAndDestroy(posCancelTest);
	if(ETestComplete != iTestingStatus)
		{
		iTestingCancelled = ETrue;
		}	
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::InitializeCancelTestL(CStifItemParser& /*aItem*/)
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}
	iPosLogger->WriteTestHeader(KCancelInitializeTest);
	COMASuplCancelInitializeTest* initializeCancelTest 
					= COMASuplCancelInitializeTest::NewL(iSelectedImpl->ImplementationUid());
	CleanupStack::PushL(initializeCancelTest);
	iTestHandler->InitializeL(initializeCancelTest);
	initializeCancelTest->InitializeL(iTestHandler, *iPosLogger);
	iTestHandler->StartTest();
	UpdateSummary(initializeCancelTest);
	iTestingStatus = initializeCancelTest->GetTestingStatus();
	CleanupStack::PopAndDestroy(initializeCancelTest);
	if(ETestComplete != iTestingStatus)
		{
		iTestingCancelled = ETrue;
		}
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::PositionTestL(CStifItemParser& /*aItem*/)
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}
	iPosLogger->WriteTestHeader(KPositionTest);
	COMASuplPosTesterCategory* positionTest 
					= COMASuplPositionTest::NewL(iPosHandler);
	CleanupStack::PushL(positionTest);
	iTestHandler->InitializeL(positionTest);
	positionTest->InitializeL(iTestHandler, *iPosLogger);
	iTestHandler->StartTest();
	UpdateSummary(positionTest);
	iTestingStatus = positionTest->GetTestingStatus();
	CleanupStack::PopAndDestroy(positionTest);
	if(ETestComplete != iTestingStatus)
		{
		iTestingCancelled = ETrue;
		}
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::SessionTestL(CStifItemParser& aItem)
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}
	iPosLogger->WriteTestHeader(KSessionTest);
	TInt setCapReqCount = 1;
	TInt reqAsstDataCount = 1;
	TInt velocityCount = 1;
	TInt positionCount = 1;
	TInt payloadCount = 1;
	TInt counter = 0;
	TInt value = 1;
	//read Integer values from aItem
	while(KErrNone == aItem.GetNextInt( value ))
		{
		switch (++counter)
			{
			case 1:setCapReqCount = value;break;
			case 2:reqAsstDataCount = value;break;
			case 3:velocityCount = value;break;
			case 4:positionCount = value;break;
			case 5:payloadCount = value;break;
			default:break;
			}
		}
		
	COMASuplSessionTest* sessionTest
	 = COMASuplSessionTest::NewL(*iPosHandler);
	 
	COMASuplPosTesterCategory* testerCat = sessionTest;
	CleanupStack::PushL(sessionTest);
	iTestHandler->InitializeL(testerCat);
	testerCat->InitializeL(iTestHandler, *iPosLogger);
	TRAPD(err, sessionTest->CreateRequestListL(setCapReqCount
									, reqAsstDataCount
									, positionCount
									, velocityCount
									, payloadCount));
	if(KErrNone != err)
		{
		iPosLogger->WriteStatus(KLeaveRequestListCreation(), err, 0);
		User::Leave(err);
		}

	iTestHandler->StartTest();
	UpdateSummary(testerCat);
	iTestingStatus = testerCat->GetTestingStatus();
	CleanupStack::PopAndDestroy(sessionTest);
	if(ETestComplete != iTestingStatus)
		{
		iTestingCancelled = ETrue;
		}
	return KErrNone;

	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::MultipleSessionTestL(CStifItemParser& aItem)
	{
	if( iTestingCancelled )
		{
		return KErrNone;
		}
	TInt value = 2;
		if(KErrNone == aItem.GetNextInt( value ))
		{
		//Do nothing
		}
	iPosLogger->WriteTestHeader(KMultipleSessionTest);
	COMASuplPosTesterCategory* multipleSessionTest 
					= COMASuplMultipleSessionTest::NewL(iPosHandler
													, *iSelectedImpl, value);
	CleanupStack::PushL(multipleSessionTest);
	iTestHandler->InitializeL(multipleSessionTest);
	multipleSessionTest->InitializeL(iTestHandler, *iPosLogger);
	iTestHandler->StartTest();
	UpdateSummary(multipleSessionTest);
	iTestingStatus = multipleSessionTest->GetTestingStatus();
	CleanupStack::PopAndDestroy(multipleSessionTest);
	if(ETestComplete != iTestingStatus)
		{
		iTestingCancelled = ETrue;
		}
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASuplPosTesterModuleCfg::SuplGatewayTestL
// Test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplPosTesterModuleCfg::WriteSummaryL (CStifItemParser& /*aItem*/)
	{
	if(iPosLogger)
		{
		iThreadTester->StatusAfterTestsL();
    	iThreadTester->MatchAndLogStatus();
    	COMASuplPosTesterCategory* test
    	 = static_cast<COMASuplPosTesterCategory*>(iThreadTester);
    	UpdateSummary(test);
		iPosLogger->WriteSummaryL(iTestingStatus, iNetInfos
											, iNetWarnings, iNetErrors);
		}
	
	return KErrNone;
	}
