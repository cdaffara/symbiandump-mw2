// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include <rsendas.h>

#include <s32mem.h>

#include "sendasservername.h"
#include "sendasserverdefs.h"
#include <csendasmessagetypes.h>
#include <csendasaccounts.h>
#include <tsendasmessagetypefilter.h>

const TUid KSendAsServerExeUid	= {0x10204286};
const TInt KSendAsRetryTimeout	= 100000;
const TInt KSendAsRetryCount	= 10;

static TInt StartServer()
	{
	const TUidType serverUid(KNullUid, KNullUid, KSendAsServerExeUid);
	RProcess server;
	TInt r = server.Create(KSendAsServerExe, KNullDesC, serverUid);

	if( r != KErrNone )
		{
		return r;
		}
	
	TRequestStatus status;
	server.Rendezvous(status);
	if( status != KRequestPending )
		{
		server.Kill(0); // abort start-up
		}
	else
		{
		server.Resume();	// wait for server start-up.
		}
	User::WaitForRequest(status);
	
	// If the server panics on start-up, then exit reason may still be zero,
	// which is not distinguishable from KErrNone.		
	r = (server.ExitType() == EExitPanic ) ? KErrGeneral : status.Int();
	server.Close();
	return r;	
	}

/**
Establishes a connection to the SendAs Server.

@return
KErrNone on success, a system wide error code otherwise.
*/
EXPORT_C TInt RSendAs::Connect()
	{
	return Connect(KSendAsDefaultMessageSlots);	
	}


/**
Establishes a connection to the SendAs Server.


@param aMessageSlots
The number of message slots available to this session. This determines the
number of outstanding requests the client may have with the server at any one
time. The maximum number of slots is 255. If aMessageSlots==-1 then this
indicates that the RSendAs connection should use messages from the global free
pool of messages.

@return
KErrNone on success, a system wide error code otherwise.
*/
EXPORT_C TInt RSendAs::Connect(TInt aMessageSlots)
	{
	TInt retry = KSendAsRetryCount;
	TVersion version(KSendAsServerVersionMajor, KSendAsServerVersionMinor, KSendAsServerVersionBuild);
	FOREVER
		{
		TInt r = CreateSession(KSendAsServerName, version, aMessageSlots);
	 	if( r != KErrNotFound && r != KErrServerTerminated && r != KErrServerBusy )
			{
			return r;
			}
		if( --retry == 0 )
			{
			return r;
			}
		if( r == KErrServerBusy )
			{
			User::After(KSendAsRetryTimeout);
			}
		r = StartServer();
		if( r != KErrNone && r != KErrAlreadyExists )
			{
			return r;
			}
		}
	}

/**
Retrieves the connected session's available message types from the SendAs Server.
This will filter all MTMs that can send messages. This list can be refined 
by applying filters using FilterAgainstCapability.


@param aMessageTypeInfo
This is the CSendAsMessageType into which the session's MTM list information 
will be stored.

@see CSendAsMessageTypes
*/
EXPORT_C void RSendAs::FilteredMessageTypesL(CSendAsMessageTypes& aMessageTypeInfo)
	{
	TPckgBuf<TInt> desLen(0);
	
	// request needed buffer size for the UID/name list
	User::LeaveIfError(SendReceive(ESASGetMessageTypeListLength, TIpcArgs(&desLen)));
	if (desLen() > 0)
		{
		HBufC8* clientBuf = HBufC8::NewLC(desLen());
		TPtr8 ptr(clientBuf->Des());

		// request server populate list
		User::LeaveIfError(SendReceive(ESASGetMessageTypes, TIpcArgs(&ptr))); 
		
		RDesReadStream readStream;
		readStream.Open(*clientBuf);
		CleanupClosePushL(readStream);

		aMessageTypeInfo.InternalizeL(readStream);
		CleanupStack::PopAndDestroy(2, clientBuf); // clientBuf, readStream.
		}
	}

/**
Refines the connected session's available message types. Applying a filter will 
remove all mtms which do not support the supplied capability


@param aMessageCapability
This defines the capability describing which of the available MTMs held by the
SendAs session should remain.

@return
KErrNone on success, a system wide error code otherwise.
*/
EXPORT_C TInt RSendAs::FilterAgainstCapability(TUid aMessageCapability)
	{
	TSendAsMessageTypeFilter filter(aMessageCapability);
	TPckg<TSendAsMessageTypeFilter> buf(filter);

	return SendReceive(ESASSetFilter,TIpcArgs((TDes8*)&buf));
	}

/**
Refines the connected session's available message types. Applying a filter will
remove all mtms which do not satisfy the supplied condition.


@param aMessageCapability
This defines the capability describing which MTMs held by the SendAs session 
should be filtered. Other mtms will be removed.

@param aValue
The value to be applied to the available MTMs in combination with the filter
condition. MTMs which satify the condition will not be removed from the 
available MTM list.

@param aConditionType
The condition which to apply to the list of MTMs. 

@return
KErrNone on success, a system wide error code otherwise.

@see TSendAsConditionType
*/
EXPORT_C TInt RSendAs::FilterAgainstCapability(TUid aMessageCapability, TInt aValue, TSendAsConditionType aConditionType)
	{
	TSendAsMessageTypeFilter filter(aMessageCapability, aValue, aConditionType);
	TPckg<TSendAsMessageTypeFilter> buf(filter);
	
	return SendReceive(ESASSetFilter,TIpcArgs((TDes8*)&buf));
	}

/**
Refreshes the connected session's available message types to all MTMs which can
send messages.


@return
KErrNone on success, a system wide error code otherwise.
*/
EXPORT_C TInt RSendAs::ResetMessageFilter()
	{
	return SendReceive(ESASClearFilter, TIpcArgs());
	}


/**
Returns the connected session's available accounts for message types.


@param aMessageType
The message type UID which the accounts should support.

@param aAccounts
On success this will be populated with the available accounts which support the
supplied message type.
*/	
EXPORT_C void RSendAs::AvailableAccountsL(TUid aMessageType, CSendAsAccounts& aAccounts)
	{
	TPckgBuf<TInt> desLen(0);
	TPckgBuf<TUid> sendUid(aMessageType);
	
	// request needed buffer size for the UID/name list
	User::LeaveIfError(SendReceive(ESASGetAccountListLength, TIpcArgs(&desLen, &sendUid)));
	if(desLen() > 0)
		{
		HBufC8* clientBuf = HBufC8::NewLC(desLen());
		TPtr8 ptr(clientBuf->Des());
					
		// request server populate list
		User::LeaveIfError(SendReceive(ESASGetAccountList,TIpcArgs(&ptr, &sendUid))); 
				
		RDesReadStream readStream;
		readStream.Open(*clientBuf);
		CleanupClosePushL(readStream);

		aAccounts.InternalizeL(readStream);
		CleanupStack::PopAndDestroy(2, clientBuf); // clientBuf, readStream.
		}
	}

