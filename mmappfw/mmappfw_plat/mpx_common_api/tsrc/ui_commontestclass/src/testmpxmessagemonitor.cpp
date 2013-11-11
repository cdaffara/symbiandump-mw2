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
* Description: MPXMessageMonitor testing implementation (mpxmessagemonitor.h)
*
*/


#include <s32mem.h>
#include <mpxcollectionframeworkdefs.h>

#include "commontestclass.h"

_LIT(KMPXCollectionServerName,"MPXCollectionServer");
_LIT(KMPXCollectionServerImg,"mpxcollectionserver");        // DLL/EXE name
// UID
const TUid KMPXCollectionServerUid3={0x101FFC31};  
// Serverversion number
const TUint KMPXCollectionServerMajorVersionNumber=0;
const TUint KMPXCollectionServerMinorVersionNumber=1;
const TUint KMPXCollectionServerBuildVersionNumber=1;

// Begin CMPXMessageMonitor testing implementation (mpxmessagemonitor.h)=======================================
// -----------------------------------------------------------------------------
// CCommonTestClass::CreateCMPXMessageMonitorL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateCMPXMessageMonitorL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXMessageMonitorL testing CMPXMessageMonitor::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::CreateCMPXMessageMonitorL testing CMPXMessageMonitor::NewL() begin"));
    TInt err = KErrNone;
    if ( iMsgMonitor != NULL)
        {
        delete iMsgMonitor;
        }
    iMsgMonitor = NULL;

    User::LeaveIfError(
        iMessageSession.Connect(KMPXCollectionServerName,
                     KMPXCollectionServerImg,
                     TVersion(KMPXCollectionServerMajorVersionNumber,
                     KMPXCollectionServerMinorVersionNumber,
                     KMPXCollectionServerBuildVersionNumber)));
    iMessageSession.SendReceiveL(EMcsSetMode,TIpcArgs(KMcModeDefault.iUid));                      

    iMsgMonitor = CMPXMessageMonitor::NewL(iMessageSession, *this );
    
    if ( iMsgMonitor == NULL)
        {
        err = KErrNotFound;
        }
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXMessageMonitorL testing CMPXMessageMonitor::NewL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateCMPXMessageMonitorL testing CMPXMessageMonitor::NewL() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::DeleteCMPXMessageMonitorL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::DeleteCMPXMessageMonitorL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXMessageMonitorL testing CMPXMessageMonitorL::~ begin")));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXMessageMonitorL testing CMPXMessageMonitorL::~ begin"));
    TInt err = KErrNone;
    
    iMsgMonitor->Cancel();
    delete iMsgMonitor;
    iMessageSession.Close();
    iMsgMonitor = NULL;
    
	FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXMessageMonitorL testing CMPXMessageMonitorL::~ end=%d"), err));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXMessageMonitorL testing CMPXMessageMonitorL::~ end=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::RunLCMPXMessageMonitorL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::RunLCMPXMessageMonitorL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::RunLCMPXMessageMonitorL testing CMPXMessageMonitor::RunL begin")));
    iLog->Log(_L("CCommonTestClass::RunLCMPXMessageMonitorL testing CMPXMessageMonitor::RunL begin"));
    TInt err = KErrNone;

    if ( iMsgMonitor != NULL )
        {
        iMessageSession.SendReceiveL(EMcsNotifyEvent);                              
        iMessageSession.SendReceiveL(EMcsGetSupportedTypes);
    	FTRACE(FPrint(_L("CCommonTestClass::RunLCMPXMessageMonitorL")));
        iLog->Log(_L("CCommonTestClass::RunLCMPXMessageMonitorL"));
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::RunLCMPXMessageMonitorL Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::RunLCMPXMessageMonitorL Stif test script is wrong."));
        }

	FTRACE(FPrint(_L("CCommonTestClass::GetNextMessage testing CMPXMessageMonitor::RunL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::GetNextMessage testing CMPXMessageMonitor::RunL end err=%d"), err);
	return err;
    }
    

// -----------------------------------------------------------------------------
// CCommonTestClass::MessageReceived()
// Returns: none
// -----------------------------------------------------------------------------
void CCommonTestClass::MessageReceived(TInt /*aMsgData*/, TInt /*aError*/)
    {
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::GetNextMessageCMPXMessageMonitorL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::GetNextMessageCMPXMessageMonitorL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::GetNextMessageCMPXMessageMonitorL testing GetNextMessage begin")));
    iLog->Log(_L("CCommonTestClass::GetNextMessageCMPXMessageMonitorL testing GetNextMessage begin"));
    TInt err = KErrNone;
    iMsgMonitor->GetNextMessage();
    delete iMsgMonitor;
    iMessageSession.Close();
    iMsgMonitor = NULL;
	FTRACE(FPrint(_L("CCommonTestClass::GetNextMessageCMPXMessageMonitorL testing GetNextMessage end=%d"), err));
    iLog->Log(_L("CCommonTestClass::GetNextMessageCMPXMessageMonitorL testing GetNextMessage end=%d"), err);
	return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::DoCancelCMPXMessageMonitorL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::DoCancelCMPXMessageMonitorL()
    {
    FTRACE(FPrint(_L("CCommonTestClass::DoCancelCMPXMessageMonitorL testing CMPXMessageMonitor::DoCancel begin")));
    iLog->Log(_L("CCommonTestClass::DoCancelCMPXMessageMonitorL testing CMPXMessageMonitor::DoCancel begin"));
    TInt err = KErrNone;

    if ( iMsgMonitor != NULL )
        {
        iMessageSession.SendReceiveL(EMcsNotifyEvent);                              
        iMessageSession.SendReceiveL(EMcsGetSupportedTypes);
        iMessageSession.SendReceiveL(EMcsCancelGetMessage);
    	FTRACE(FPrint(_L("CCommonTestClass::DoCancelCMPXMessageMonitorL")));
        iLog->Log(_L("CCommonTestClass::DoCancelCMPXMessageMonitorL"));
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::DoCancelCMPXMessageMonitorL Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::DoCancelCMPXMessageMonitorL Stif test script is wrong."));
        }

	FTRACE(FPrint(_L("CCommonTestClass::DoCancelCMPXMessageMonitorL testing CMPXMessageMonitor::DoCancel end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::DoCancelCMPXMessageMonitorL testing CMPXMessageMonitor::DoCancel end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::RunErrorCMPXMessageMonitorL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::RunErrorCMPXMessageMonitorL(   )
    {
    FTRACE(FPrint(_L("CCommonTestClass::RunErrorCMPXMessageMonitorL testing CMPXMessageMonitor::RunError begin")));
    iLog->Log(_L("CCommonTestClass::RunErrorCMPXMessageMonitorL testing CMPXMessageMonitor::RunError begin"));
    TInt err = KErrNone;

    if ( iMsgMonitor != NULL )
        {
        iMessageSession.SendReceiveL(EMcsNotifyEvent);                              
        iMessageSession.SendReceiveL(EMcsGetSupportedTypes);
        iMessageSession.SendReceiveL(EMcsCancelRequest);
    	FTRACE(FPrint(_L("CCommonTestClass::CMPXMessageMonitorL")));
        iLog->Log(_L("CCommonTestClass::RunErrorCMPXMessageMonitorL"));
        }
    else
        {
        err = KErrBadTestParameter;
    	FTRACE(FPrint(_L("CCommonTestClass::RunErrorCMPXMessageMonitorL Stif test script is wrong.")));
        iLog->Log(_L("CCommonTestClass::RunErrorCMPXMessageMonitorL Stif test script is wrong."));
        }

	FTRACE(FPrint(_L("CCommonTestClass::RunErrorCMPXMessageMonitorL testing CMPXMessageMonitor::RunError end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::RunErrorCMPXMessageMonitorL testing CMPXMessageMonitor::RunError end err=%d"), err);
	return err;
    }
