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
// This is the test class for TEF (PREQ1521).
// 
//

#include "t_oomtestwrapper.h"

// Utils includes
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>
#include <messagingtestutility2.h>

/*@{*/

// Commands
_LIT(KNew,									"New");

_LIT(KOOMAddRemoveDrive,					"OOMAddRemoveDrive");
_LIT(KOOMUpdateDrive,						"OOMUpdateDrive");

// Command parameters
_LIT(KObjectValue,							"object_value");
_LIT(KPriority,								"priority");
_LIT(KDrive,								"drive");

_LIT(KMsvServerPattern, 					"!MsvServer*");

/*@}*/

/**
Purpose: Constructor of CT_OomTestWrapper class

@internalComponent
*/
CT_OomTestWrapper::CT_OomTestWrapper()
:	iObject(NULL),
	iSession(NULL),
	iObserver(NULL)
	{
	}
/**
Purpose: Destructor of CT_OomTestWrapper class

@internalComponent
*/
CT_OomTestWrapper::~CT_OomTestWrapper()
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
Purpose: Command fuction of CT_OomTestWrapper class

@internalComponent
*/
CT_OomTestWrapper* CT_OomTestWrapper::NewL()
	{
	CT_OomTestWrapper*	ret = new (ELeave) CT_OomTestWrapper();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}
/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
void CT_OomTestWrapper::ConstructL()
	{
	iObject			= new (ELeave) TInt;
	iObserver		= new(ELeave)CT_OomSessionObserver;
	iSession		= CMsvSession::OpenSyncL(*iObserver);
	}

/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
TBool CT_OomTestWrapper::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
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
	else if(KOOMAddRemoveDrive() == aCommand)
		{
		DoCmdOOMAddRemoveDrive(aSection);
		}
	else if(KOOMUpdateDrive() == aCommand)
		{
		DoCmdOOMUpdateDrive(aSection);
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
void CT_OomTestWrapper::DoCmdNewL(const TDesC& aSection)
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
Purpose: OOM test - Add a drive to the priority drive list and remove it.

@internalComponent
@param  aSection Current ini file command section
*/
void CT_OomTestWrapper::DoCmdOOMAddRemoveDrive(const TDesC& aSection)
	{
	TInt drive = 0;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
		}

	TInt priority = 0;
	if (!GetIntFromConfig(aSection, KPriority(), priority))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KPriority());
		SetBlockResult(EFail);
		}

	TInt err = KErrNone;
	__UHEAP_MARK;
	for (TInt k=2;;++k)
		{
		RDebug::Print(_L("loop number - %d"),k);
		__UHEAP_SETFAIL(RHeap::EDeterministic,k);
		__UHEAP_MARK;

		TRAP( err, iSession->AddDriveL(TDriveNumber(drive), (TUint&)priority));
		TRAP( err, iSession->RemoveDriveL(TDriveNumber(drive)));

		__UHEAP_MARKEND;
		User::Heap().Check();
		if (err==KErrNone)
			break;
		}
	__UHEAP_MARKEND;
	__UHEAP_RESET;
	User::Heap().Check();
	__UHEAP_SETFAIL(RHeap::ENone,0);

	SetBlockResult( EPass );
	}


/**
Purpose: OOM Test - Update the priority of a drive.

@internalComponent
@param  aSection Current ini file command section
*/
void CT_OomTestWrapper::DoCmdOOMUpdateDrive(const TDesC& aSection)
	{
	TInt drive = 0;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
		}

	TInt priority = 0;
	if (!GetIntFromConfig(aSection, KPriority(), priority))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KPriority());
		SetBlockResult(EFail);
		}

	TInt err = KErrNone;
	__UHEAP_MARK;
	for (TInt k=2;;++k)
		{
		RDebug::Print(_L("loop number - %d"),k);
		__UHEAP_SETFAIL(RHeap::EDeterministic,k);
		__UHEAP_MARK;

		TRAP( err, iSession->UpdateDrivePriorityL(TDriveNumber(drive), (TUint&)priority));

		__UHEAP_MARKEND;
		User::Heap().Check();
		if (err==KErrNone)
			break;
		}
	__UHEAP_MARKEND;
	__UHEAP_RESET;
	User::Heap().Check();
	__UHEAP_SETFAIL(RHeap::ENone,0);

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
void CT_OomSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny*)
	{
	if (aEvent)
		iType = aEvent;
	if (aArg1)
		iArg1 = *(TInt*)aArg1;
	if (aArg2)
		iArg2 = *(TInt*)aArg2;
	CActiveScheduler::Stop();
	}



