// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32base.h>
#include <e32std.h>
#include <msvstd.h>
#include <msventry.h>
#include <msvuids.h>
#include <bautils.h>
#include <msvschedulesend.h>
#include <schedulebaseservermtm.h>
#include <schsend_panic.h>
#include <centralrepository.h>


//
//
//	CScheduleBaseServerMtm Implementation
//
//

EXPORT_C CScheduleBaseServerMtm::CScheduleBaseServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry)
	: CBaseServerMtm(aRegisteredMtmDll, aServerEntry)
/**
Constructor.

@param aRegisteredMtmDll Registration data for the MTM DLL
@param aServerEntry Context on which to operate
*/
	{
	//Do Nothing Else
	}

EXPORT_C CScheduleBaseServerMtm::~CScheduleBaseServerMtm()
/**
Destructor.

The derived class destructor should delete the iScheduleSend member.
*/
	{
	iResourceFile.Close();
	iFs.Close();
	}

EXPORT_C void CScheduleBaseServerMtm::SendScheduledL(CMsvEntrySelection& aSelection, const TBool aMove, const TDesC8& /*aParameter*/, TRequestStatus& aStatus)
/**
Sends messages now that were previously scheduled.

The messages are sent immediately, overriding the scheduling.

@param aSelection The selection of messages that were previously scheduled 
and now need to be sent
@param aMove Whether to perform a Move (ETrue) or a Copy (EFalse) when sending 
has completed
@param aParameter The parameter (MTM-specific) that is required when sending 
the messages
@param aStatus Asynchronous status word
*/
	{
	if (aMove)
		{
		MoveFromLocalL(aSelection, NULL, aStatus);
		}
	else
		{
		CopyFromLocalL(aSelection, NULL, aStatus);
		}
	}

EXPORT_C void CScheduleBaseServerMtm::LoadResourceFileL(const TDesC& aResFileName)
/**
Loads the specified resource file.

A utility function that can be used from the pure virtual LoadResourceFileL().

It leaves if the resource file reader cannot be opened.

@param aResFileName Resource file name
*/
	{
	TFileName fileName;
    Dll::FileName(fileName);
    TParse parse;
    parse.Set(aResFileName, &fileName, 0);
	fileName=parse.FullName();
	BaflUtils::NearestLanguageFile(iFs, fileName);
	iResourceFile.OpenL(iFs, fileName);
	}

EXPORT_C void CScheduleBaseServerMtm::ConstructL()
/**
Second phase constructor.

This calls LoadResourceFileL().

The derived class second phase constructor should call this function. It should 
also construct the iScheduleSend member.

@leave Leaves with one of the system-wide error if unable to connect to the
file server
@leave LoadResourceFileL()
*/
	{
	User::LeaveIfError(iFs.Connect());
	LoadResourceFileL();
	}


/**
Loads all the schedule settings either from CenRep or a resource file.

@param aRepository
CenRep repository to load settings from

@param aRestoreErrorsFromResource 
Whether to read error handling information (a SEND_ERROR_ACTIONS struct) from a resource file

@param aErrorsResourceId 
Resource ID of the error handling resource struct

@see CMsvScheduleSend::RestoreL
@see CMsvSendErrorActions::RestoreFromResourceL()

@panic ScheduleSend-DLL 24
The context is not a service entry
*/
EXPORT_C void CScheduleBaseServerMtm::LoadScheduleSettingsL(CRepository& aRepository, TBool aRestoreErrorsFromResource, TInt aErrorsResourceId)
	{
	iScheduleSend->LoadScheduleSettingsL(aRepository);

	if (aRestoreErrorsFromResource)
		{
		//Restore the errors from a resource file
		TResourceReader reader;
		HBufC8* rBuf = iResourceFile.AllocReadLC(aErrorsResourceId);
		reader.SetBuffer(rBuf);
		iScheduleSend->SendErrorActions().RestoreFromResourceL(reader);
		CleanupStack::PopAndDestroy(); //rBuf
		}
	}

EXPORT_C void CScheduleBaseServerMtm::ScheduleL(CMsvEntrySelection& aSelection, const TBool aMove, const TDesC8& aParameter, TRequestStatus& aStatus)
/**
Schedules a message. 

The functions should schedule a message using CMsvScheduleSend::ScheduleL().
Make sure RestoreScheduleSettingsL() is called before 
CMsvScheduleSend::ScheduleL().

@param aSelection The selection of messages that need to be scheduled
@param aMove Whether to perform a Move (ETrue) or a Copy (EFalse) when sending 
has completed
@param aParameter The parameter (MTM-specific) that is required when sending 
the messages
@param aStatus Asynchronous status word

@leave CMsvScheduleSend::ScheduleL()
*/
	{
	TMsvSchedulePackage package;
	PopulateSchedulePackage(aParameter, aMove, package);
	iScheduleSend->ScheduleL(aSelection, package);
	RequestComplete(&aStatus, KErrNone);
	}

EXPORT_C void CScheduleBaseServerMtm::Queue(TRequestStatus& aStatus)
/**
Specifies an asynchronous status word to be completed when an asynchronous 
MTM operation completes.

This provides a form of observer functionality. It sets the the iReport 
member.

@param aStatus Asynchronous status word to use

@panic ScheduleSend-DLL 28 iReport member not NULL. Debug build only.
*/
	{
	__ASSERT_DEBUG(iReport==NULL,gPanic(EReportNotNull));

	aStatus=KRequestPending;
	iReport=&aStatus;
	}

EXPORT_C void CScheduleBaseServerMtm::CheckScheduleL(const CMsvEntrySelection& aSelection, const TDesC8& /*aParameter*/, TRequestStatus& aStatus)
/**
Verifies that the schedule information stored in specified messages 
is the same as that on the task scheduler.

@see CMsvScheduleSend::CheckScheduleL()

@param aSelection Array of message IDs that need to be checked against the task scheduler
@param aParameter Unused
@param aStatus Asynchronous status word

@leave CMsvScheduleSend::CheckScheduleL()
*/
	{
	iScheduleSend->CheckScheduleL(aSelection);
	RequestComplete(&aStatus, KErrNone);
	}

EXPORT_C void CScheduleBaseServerMtm::DeleteScheduleL(const CMsvEntrySelection& aSelection, const TDesC8& /*aParameter*/, TRequestStatus& aStatus)
/**
Deletes the schedules for the specified messages from the task scheduler. 

The messages themselves are not deleted.

@see CMsvScheduleSend::DeleteScheduleL()

@param aSelection Array of message IDs that need to be deleted from the
task scheduler
@param aParameter Unused
@param aStatus Asynchronous status word

@leave CMsvScheduleSend::DeleteScheduleL()
*/
	{
	iScheduleSend->DeleteScheduleL(aSelection);
	RequestComplete(&aStatus, KErrNone);
	}

void CScheduleBaseServerMtm::RequestComplete(TRequestStatus* aStatus, TInt aError)
	{
	User::RequestComplete(aStatus, aError);
	}
