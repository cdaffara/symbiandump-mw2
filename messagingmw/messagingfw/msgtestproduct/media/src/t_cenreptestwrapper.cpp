// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file
// 
//

#include "t_cenreptestwrapper.h"

// Utils includes
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>
#include <messagingtestutility2.h>

/*@{*/

// Commands
_LIT(KNew,									"New");
_LIT(KResetCentralRepository,				"ResetCentralRepository");

// Command parameters
_LIT(KObjectValue,							"object_value");

_LIT(KMsvServerPattern, 					"!MsvServer*");

/*@}*/

/**
Purpose: Constructor of CT_CenRepTestWrapper class

@internalComponent
*/
CT_CenRepTestWrapper::CT_CenRepTestWrapper()
:	iObject(NULL),
	iSession(NULL),
	iObserver(NULL)
	{
	}
/**
Purpose: Destructor of CT_CenRepTestWrapper class

@internalComponent
*/
CT_CenRepTestWrapper::~CT_CenRepTestWrapper()
	{
	delete iObject;
	iObject = NULL;

	// Close Message Server
	if (iSession)
		{
		iSession->CloseMessageServer();
		delete iSession;
		iSession = NULL;
		delete iObserver;
		iObserver = NULL;

		TFindProcess find(KMsvServerPattern);
		TFullName name;
		if (find.Next(name) == KErrNone)
			{
			TRequestStatus status;
			RProcess process;
			User::LeaveIfError(process.Open(name));
			process.Logon(status);
			User::WaitForRequest(status);
			User::After(9000000);
			}
		}
	}
/**
Purpose: Command fuction of CT_CenRepTestWrapper class

@internalComponent
*/
CT_CenRepTestWrapper* CT_CenRepTestWrapper::NewL()
	{
	CT_CenRepTestWrapper*	ret = new (ELeave) CT_CenRepTestWrapper();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}
/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
void CT_CenRepTestWrapper::ConstructL()
	{
	iObject			= new (ELeave) TInt;
	iObserver		= new(ELeave)CT_CenRepSessionObserver;
	iSession		= CMsvSession::OpenSyncL(*iObserver);
	}

/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
TBool CT_CenRepTestWrapper::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool ret = ETrue;

	// Print out the parameters for debugging
	INFO_PRINTF2( _L("<font size=2 color=990000><b>aCommand = %S</b></font>"), &aCommand );
	INFO_PRINTF2( _L("aSection = %S"), &aSection );
	INFO_PRINTF2( _L("aAsyncErrorIndex = %D"), aAsyncErrorIndex );

	if(KNew() == aCommand)
		{
		DoCmdNewL(aSection);
		}
	else if(KResetCentralRepository() == aCommand)
		{
		DoCmdResetCentralRepository();
		}
	else
		{
		ret = EFalse;
		}

	return ret;
	}

/**
Purpose: To create a new object of the CTEFTest type through the API.

Ini file options:
	iniData - The data from the ini file at the section provided.

@internalComponent
@param  aSection Current ini file command section
*/
void CT_CenRepTestWrapper::DoCmdNewL(const TDesC& aSection)
	{
	TInt objectValue = 0;
	if (!GetIntFromConfig(aSection, KObjectValue(), objectValue))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KObjectValue());
		SetBlockResult(EFail);
		}
	else
		{
		delete iObject;
		iObject = new (ELeave) TInt(objectValue);
		}
	}

/**
Purpose: Reset central repository.

@internalComponent
@param  aSection Current ini file command section
*/
void CT_CenRepTestWrapper::DoCmdResetCentralRepository()
	{
	TRAPD(err, iSession->ResetRepositoryL());
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("<font color=FF0000>Unable to reset the central repository: %d error</font>"),err);
		SetError( err );
		}
	SetBlockResult( EPass );
	}

/**
Purpose: Handle the session event

@internalComponent
@param  aEvent event type of session
@param  aArg1  NA
@param  aArg2  NA
@param  NA
*/
void CT_CenRepSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny*)
	{
	if (aEvent)
		iType = aEvent;
	if (aArg1)
		iArg1 = *(TInt*)aArg1;
	if (aArg2)
		iArg2 = *(TInt*)aArg2;
	CActiveScheduler::Stop();
	}



