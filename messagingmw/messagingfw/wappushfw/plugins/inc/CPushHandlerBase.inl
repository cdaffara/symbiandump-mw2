// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Local includes
// 
//

/**
 @file
 @publishedPartner
 @released
*/

// System includes
//
#include <ecom/ecom.h>
#include <push/cpushhandlerbase.h>


/** 
Constructor. 
*/
inline CPushHandlerBase::CPushHandlerBase()
: CActive(EPriorityStandard)
	{
	}


/** 
Destructor. 
*/
inline CPushHandlerBase::~CPushHandlerBase()
	{
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}


/** 
Finds and instantiates an ECom WAP Push Application plug-in that matches the 
specified Application ID.

@return  
WAP Push Application plug-in that handles the specified Application ID.

@param aMatchData
Application ID.
*/
inline CPushHandlerBase* CPushHandlerBase::NewL(const TDesC& aMatchData)
	{
	return NewL(aMatchData, KUidPushHandlerBase);
	}


/** 
Finds and instantiates an ECom plug-in that implements an interface specialised 
from CPushHandlerBase.

This is a utility function for use by ECom interface classes that specialise 
CPushHandlerBase, such as CContentHandlerBase.

@param aMatchData 
ECom match data.

@param aInterfaceUid 
UID of the required derived ECom interface.

@return 
WAP Push plug-in that handles the specified Application ID.
*/
inline CPushHandlerBase* CPushHandlerBase::NewL(const TDesC& aMatchData, const TUid& aInterfaceUid)
	{
	// Convert match data to 8-bits
	HBufC8* buf = HBufC8::NewLC(aMatchData.Length());
	TPtr8 data = buf->Des();
	data.Copy(aMatchData);
	data.LowerCase();	// change to lower case

	// Set resolving parameters
	TEComResolverParams resolverParams;
	resolverParams.SetDataType(data);
	resolverParams.SetWildcardMatch(ETrue);

	// Get the instantiation
	CPushHandlerBase* pushHandler = REINTERPRET_CAST(CPushHandlerBase*,
									REComSession::CreateImplementationL(aInterfaceUid,
																		_FOFF(CPushHandlerBase, iDtor_ID_Key),
																		resolverParams));
	CleanupStack::PopAndDestroy(buf);
	return pushHandler;
	}


/** 
Specifies an interface to handle logging.

Log messages are intended to aid debugging.

@param aLog 
Log interface. This is stored in iLog. 
*/
inline void CPushHandlerBase::SetLogger(MWapPushLog& aLog)
	{
	iLog = &aLog;
	}


/** 
Called by the framework to supply a CPluginKiller object.

The plug-in calls CPluginKiller::KillPushPlugin() to delete itself when it 
has completed handling a message.

@param aPluginKiller 
Plugin killer. This is stored in iPluginKiller. 
*/
inline void CPushHandlerBase::SetKiller(CPluginKiller& aPluginKiller)
	{
	iPluginKiller = &aPluginKiller;
	}

/**
Set the connection manager.
 
@param aManager Reference to connection manager object to be set.*/
inline void CPushHandlerBase::SetManager(MConnManObserver& aManager)
	{
	iManager = &aManager;
	}


/** 
Stores the passed TRequestStatus and sets it to KRequestPending.

@param aStatus 
Asynchronous status, stored in iConfirmStatus. 
*/
inline void CPushHandlerBase::SetConfirmationStatus(TRequestStatus& aStatus)
	{
	aStatus=KRequestPending;
	iConfirmStatus = &aStatus;
	}

/** 
Completes the stored asynchronous status with the specified error code.

@param aErr 
Error code. 
*/
inline void CPushHandlerBase::SignalConfirmationStatus(TInt aErr)
	{
	User::RequestComplete(iConfirmStatus, aErr);
	}
