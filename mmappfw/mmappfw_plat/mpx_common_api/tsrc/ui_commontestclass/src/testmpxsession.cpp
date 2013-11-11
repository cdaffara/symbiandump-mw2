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
* Description:  RMPXSession testing implementation (mpxsession.h)
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

// Begin RMPXSession testing implementation (mpxsession.h)=======================================
// -----------------------------------------------------------------------------
// CCommonTestClass::VersionRMPXSession()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::VersionRMPXSession()
    {
	FTRACE(FPrint(_L("CCommonTestClass::VersionRMPXSession testing RMPXSession::Version begin")));
    iLog->Log(_L("CCommonTestClass::VersionRMPXSession testing RMPXSession::Version begin"));
    TInt err = KErrNone;
    RMPXSession session;
    session.Version();
	FTRACE(FPrint(_L("CCommonTestClass::VersionRMPXSession testing RMPXSession::Version end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::VersionRMPXSession testing RMPXSession::Version end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::ConnectRMPXSessionL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::ConnectRMPXSessionL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::ConnectRMPXSessionL testing RMPXSession::Connect begin")));
    iLog->Log(_L("CCommonTestClass::ConnectRMPXSessionL testing RMPXSession::Connect begin"));
    TInt err = KErrNone;
    RMPXSession session;
    User::LeaveIfError(
        session.Connect(KMPXCollectionServerName,
                     KMPXCollectionServerImg,
                     TVersion(KMPXCollectionServerMajorVersionNumber,
                     KMPXCollectionServerMinorVersionNumber,
                     KMPXCollectionServerBuildVersionNumber)));
    session.SendReceiveL(EMcsSetMode,TIpcArgs(KMcModeDefault.iUid));                      
    session.Close();
	FTRACE(FPrint(_L("CCommonTestClass::ConnectRMPXSessionL testing RMPXSession::Connect end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ConnectRMPXSessionL testing RMPXSession::Connect end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::ReconnectRMPXSessionL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::ReconnectRMPXSessionL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::ReconnectRMPXSessionL testing RMPXSession::Reconnect begin")));
    iLog->Log(_L("CCommonTestClass::ReconnectRMPXSessionL testing RMPXSession::Reconnect begin"));
    TInt err = KErrNone;
    RMPXSession session;
    User::LeaveIfError(
        session.Connect(KMPXCollectionServerName,
                     KMPXCollectionServerImg,
                     TVersion(KMPXCollectionServerMajorVersionNumber,
                     KMPXCollectionServerMinorVersionNumber,
                     KMPXCollectionServerBuildVersionNumber)));
    session.Close();
    
    User::LeaveIfError( session.Reconnect() );
    session.SendReceiveL(EMcsSetMode,TIpcArgs(KMcModeDefault.iUid));                      
    session.Close();
    
	FTRACE(FPrint(_L("CCommonTestClass::ReconnectRMPXSessionL testing RMPXSession::Reconnect end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ReconnectRMPXSessionL testing RMPXSession::Reconnect end err=%d"), err);
	return err;
    }

// CCommonTestClass::SendReceiveLRMPXSessionL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SendReceiveLRMPXSessionL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::SendReceiveLRMPXSessionL testing RMPXSession::SendReceiveL begin")));
    iLog->Log(_L("CCommonTestClass::SendReceiveLRMPXSessionL testing RMPXSession::SendReceiveL begin"));
    
    TInt err = KErrNone;
    RMPXSession session;
    User::LeaveIfError(
        session.Connect(KMPXCollectionServerName,
                     KMPXCollectionServerImg,
                     TVersion(KMPXCollectionServerMajorVersionNumber,
                     KMPXCollectionServerMinorVersionNumber,
                     KMPXCollectionServerBuildVersionNumber)));
    session.SendReceiveL(EMcsSetMode);                      
    session.Close();
	
    FTRACE(FPrint(_L("CCommonTestClass::SendReceiveLRMPXSessionL testing RMPXSession::SendReceiveL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::SendReceiveLRMPXSessionL testing RMPXSession::SendReceiveL end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::SendReceiveStatusRMPXSessionL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SendReceiveStatusRMPXSessionL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::SendReceiveStatusRMPXSessionL testing RMPXSession::SendReceiveL begin")));
    iLog->Log(_L("CCommonTestClass::SendReceiveStatusRMPXSessionL testing RMPXSession::SendReceiveL begin"));
    
    TInt err = KErrNone;
    RMPXSession session;
    User::LeaveIfError(
        session.Connect(KMPXCollectionServerName,
                     KMPXCollectionServerImg,
                     TVersion(KMPXCollectionServerMajorVersionNumber,
                     KMPXCollectionServerMinorVersionNumber,
                     KMPXCollectionServerBuildVersionNumber)));
    session.SendReceiveL(EMcsSetMode);                      
    TRequestStatus status = 5;
    session.SendReceive( EMcsGetUid, status );  
    User::WaitForRequest( status ); //added on 2009,2,12
    session.Close();
	
    FTRACE(FPrint(_L("CCommonTestClass::SendReceiveStatusRMPXSessionL testing RMPXSession::SendReceiveL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::SendReceiveStatusRMPXSessionL testing RMPXSession::SendReceiveL end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::SendReceiveStatus3RMPXSessionL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SendReceiveStatus3RMPXSessionL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::SendReceiveStatus3RMPXSessionL testing RMPXSession::SendReceiveL begin")));
    iLog->Log(_L("CCommonTestClass::SendReceiveStatus3RMPXSessionL testing RMPXSession::SendReceiveL begin"));
    
    TInt err = KErrNone;
    RMPXSession session;
    User::LeaveIfError(
        session.Connect(KMPXCollectionServerName,
                     KMPXCollectionServerImg,
                     TVersion(KMPXCollectionServerMajorVersionNumber,
                     KMPXCollectionServerMinorVersionNumber,
                     KMPXCollectionServerBuildVersionNumber)));
    TRequestStatus status;
    session.SendReceive(EMcsSetMode,TIpcArgs(KMcModeDefault.iUid), status); 
    User::WaitForRequest( status ); //added on 2009,2,12
    session.Close();
	
    FTRACE(FPrint(_L("CCommonTestClass::SendReceiveStatus3RMPXSessionL testing RMPXSession::SendReceiveL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::SendReceiveStatus3RMPXSessionL testing RMPXSession::SendReceiveL end err=%d"), err);
	return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::SendReceiveLTIpcArgsRMPXSessionL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SendReceiveLTIpcArgsRMPXSessionL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::SendReceiveLTIpcArgsRMPXSessionL testing RMPXSession::SendReceiveL begin")));
    iLog->Log(_L("CCommonTestClass::SendReceiveLTIpcArgsRMPXSessionL testing RMPXSession::SendReceiveL begin"));
    TInt err = KErrNone;
    RMPXSession session;
    User::LeaveIfError(
        session.Connect(KMPXCollectionServerName,
                     KMPXCollectionServerImg,
                     TVersion(KMPXCollectionServerMajorVersionNumber,
                     KMPXCollectionServerMinorVersionNumber,
                     KMPXCollectionServerBuildVersionNumber)));
    session.SendReceiveL(EMcsSetMode,TIpcArgs(KMcModeDefault.iUid));                      
    session.Close();
	FTRACE(FPrint(_L("CCommonTestClass::SendReceiveLTIpcArgsRMPXSessionL testing RMPXSession::SendReceiveL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::SendReceiveLTIpcArgsRMPXSessionL testing RMPXSession::SendReceiveL end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::CreateRMPXSessionL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateRMPXSession()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateRMPXSession testing RMPXSession::RMPXSession begin")));
    iLog->Log(_L("CCommonTestClass::CreateRMPXSession testing RMPXSession::RMPXSession begin"));
    TInt err = KErrNone;
    RMPXSession session;
	FTRACE(FPrint(_L("CCommonTestClass::CreateRMPXSession testing RMPXSession::RMPXSession end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateRMPXSession testing RMPXSession::RMPXSession end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::CloseAndDestructRMPXSession()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CloseAndDestructRMPXSession()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CloseAndDestructRMPXSession testing RMPXSession::~RMPXSession begin")));
    iLog->Log(_L("CCommonTestClass::CloseAndDestructRMPXSession testing RMPXSession::~RMPXSession begin"));
    TInt err = KErrNone;
    RMPXSession session;
    session.Close();
	FTRACE(FPrint(_L("CCommonTestClass::CloseAndDestructRMPXSession testing RMPXSession::~RMPXSession end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CloseAndDestructRMPXSession testing RMPXSession::~RMPXSession end err=%d"), err);
	return err;
    }

// end of RMPXSession (mpxsession.h) =============================================

