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

#include "t_mediatestwrapper.h"

// Utils includes
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>
#include <messagingtestutility2.h>
#include <msvids.h>
#include <smsclnt.h>
#include <mtclreg.h>
#include <smuthdr.h>
#include <csmsaccount.h>
#include <SMUTSET.H>
#include <test\testexecutestepbase.h>

/*@{*/

// Enumerations
_LIT(KEMsvMessageStoreNotSupported,			"EMsvMessageStoreNotSupported");
_LIT(KEMsvMessageStoreCorrupt,				"EMsvMessageStoreCorrupt");
_LIT(KEMsvMediaChanged,						"EMsvMediaChanged");
_LIT(KEMsvRefreshMessageView,				"EMsvRefreshMessageView");
_LIT(KEMsvDiskNotAvailable,					"EMsvDiskNotAvailable");
_LIT(KEMsvMisMatch,							"Event does not match!");

// Commands
_LIT(KNew,									"New");
_LIT(KAddDrive,							    "AddDrive");
_LIT(KRemoveDrive,							"RemoveDrive");
_LIT(KUpdateDrive,							"UpdateDrive");
_LIT(KCopyStore,							"CopyStore");
_LIT(KGetCurrentDrive,						"GetCurrentDrive");
_LIT(KGetDriveList,							"GetDrivelist");
_LIT(KGetAvailableDriveList,				"GetAvailableDriveList");
_LIT(KChildrenOfAvailableDrives,			"ChildrenOfAvailableDrives");
_LIT(KIsMessageStoreSupported,				"MessageStoreSupported");
_LIT(KGetChildIdsAll,						"GetChildIdsAll");
_LIT(KChangeDrive,                          "ChangeDrive");
// added as part of defect DEF136478
_LIT(KCreateSMSAccount,						"CreateSMSAccount");
_LIT(KCreateSMS,							"CreateSMS");
_LIT(KCreateBulkSMS,						"CreateBulkSMS");
_LIT(KVerifyCount,							"VerifyCount");
_LIT(KCount,								"count");
_LIT(KDeleteMsg,							"DeleteMsg");
_LIT(KFolder,								"folder2");
_LIT(KSearchMessages,						"SearchMessages");
_LIT(KRemoveMMC,							"RemoveMMC");
_LIT(KPromptRemoveMMC,						"Please remove MMC card");
_LIT(KPressKeyToContinue,					"Press Enter key to continue.\n");
_LIT(KNewLine,								"\n");
_LIT(KConsoleName,							"MessConsole");

_LIT(KMountDrive,							"MountDrive");
_LIT(KDisMountDrive,						"DisMountDrive");

// Local Folder Ids
_LIT(KTestDraftEntryId,"Draft");
// Command parameters
_LIT(KObjectValue,							"object_value");
_LIT(KPriority,								"priority");
_LIT(KDrive,								"drive");
_LIT(KIsMsgStoreSupported,					"is_msgstore");
_LIT(KParentFolderName,						"parent_folder_name");
_LIT(KFolderName,							"folder_name");
_LIT(KExpRes,								"exp_result");
_LIT(KExpDrives,							"exp_drives");
_LIT(KExpAvailDrives,						"exp_avail_drives");
_LIT(KEventType,						    "event_type");

_LIT(KMsvServerPattern, 					"!MsvServer*");

// Phone numbers
_LIT( KScriptItemSC1, "SC" );
_LIT( KScriptItemSC1Def, "+447785016005" );

_LIT( KScriptItemSC2, "radiolinja0" );
_LIT( KScriptItemSC2Def, "+358508771010" );

_LIT( KScriptItemSC3, "radiolinja1" );
_LIT( KScriptItemSC3Def, "+358508771010" );

/*@}*/

/**
Purpose: Constructor of CT_MediaTestWrapper class

@internalComponent
*/
CT_MediaTestWrapper::CT_MediaTestWrapper()
:	iObject(NULL),
	iSession(NULL),
	iObserver(NULL)
	{
	}
/**
Purpose: Destructor of CT_MediaTestWrapper class

@internalComponent
*/
CT_MediaTestWrapper::~CT_MediaTestWrapper()
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
Purpose: Command fuction of CT_MediaTestWrapper class

@internalComponent
*/
CT_MediaTestWrapper* CT_MediaTestWrapper::NewL()
	{
	CT_MediaTestWrapper*	ret = new (ELeave) CT_MediaTestWrapper();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}
/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
void CT_MediaTestWrapper::ConstructL()
	{
	iObject			= new (ELeave) TInt;
	iObserver		= new(ELeave)CT_MediaSessionObserver;
	iSession		= CMsvSession::OpenSyncL(*iObserver);
	}

/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
TBool CT_MediaTestWrapper::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
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
	else if(KAddDrive() == aCommand)
		{
		DoCmdAddDrive(aSection);
		}
	else if(KRemoveDrive() == aCommand)
		{
		DoCmdRemoveDrive(aSection);
		}
	else if(KUpdateDrive() == aCommand)
		{
		DoCmdUpdateDrive(aSection);
		}
	else if(KGetCurrentDrive() == aCommand)
		{
		DoCmdGetCurrentDrive(aSection);
		}
	else if(KGetDriveList() == aCommand)
		{
		DoCmdGetDriveList(aSection);
		}
	else if(KGetAvailableDriveList() == aCommand)
		{
		DoCmdGetAvailableDriveList(aSection);
		}
	else if(KChildrenOfAvailableDrives() == aCommand)
		{
		DoCmdChildrenOfAvailableDrives(aSection);
		}
	else if(KGetChildIdsAll() == aCommand)
		{
		DoCmdGetChildIdsAllL(aSection);
		}
	else if(KIsMessageStoreSupported() == aCommand)
		{
		DoCmdIsMessageStoreSupported(aSection);
		}
	else if(KMountDrive() == aCommand)
		{
		DoCmdMountDrive(aSection);
		}
	else if(KDisMountDrive() == aCommand)
		{
		DoCmdDisMountDrive(aSection);
		}
	else if(KCreateSMS() == aCommand)
		{
		DoCmdCreateSMS();
		}
	else if(KCreateBulkSMS() == aCommand)
		{
		DoCmdCreateBulkSMS();
		}		
	else if(KVerifyCount() == aCommand)
		{
		DoCmdVerifyCount(aSection);
		}		
	else if(KCreateSMSAccount() == aCommand)
		{
		DoCmdCreateSMSAccount();
		}
	else if(KDeleteMsg() == aCommand)
		{
		DoCmdDeleteMessages();
		}
	else if(KSearchMessages() == aCommand)
		{
		DoCmdSearchMessages(aSection);
		}				
	else if(KRemoveMMC() == aCommand)
		{
		DoCmdRemoveMMC();
		}				
	else if(KCopyStore() == aCommand)
		{
		DoCmdCopyStore(aSection);
		}		
   else if(KChangeDrive() == aCommand)
        {
        DoCmdChangeDriveL(aSection);
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
void CT_MediaTestWrapper::DoCmdNewL(const TDesC& aSection)
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
Purpose: Add a drive to the priority drive list. 

@internalComponent
@param  aSection Current ini file command section
*/
void CT_MediaTestWrapper::DoCmdAddDrive(const TDesC& aSection)
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

	TRAPD( err, iSession->AddDriveL(TDriveNumber(drive), (TUint&)priority));

	// Handling the Errors
	if (err != KErrNone)
		{
		if ( err == KErrAlreadyExists )
			{
			ERR_PRINTF2(_L("<font color=FF0000>Priority drive list already contains the drive: %d error</font>"),err);
			SetError( err );
			}
		else if ( err == KErrArgument )
			{
			ERR_PRINTF2(_L("<font color=FF0000>Priority value is not within valid range ( Range 1-7): %d error</font>"),err);
			SetError( err );
			}
		else if ( err == KErrGeneral )
			{
			ERR_PRINTF2(_L("<font color=FF0000>Any DB related: %d error</font>"),err);
			SetError( err );
			}
		else if ( err == KErrServerBusy )
			{
			ERR_PRINTF2(_L("<font color=FF0000>Outstanding Message Server operations Running: %d error</font>"),err);
			SetError( err );
			}
		else
			{
			ERR_PRINTF2(_L("<font color=FF0000>Other system wide error codes: %d error</font>"),err);
			SetError( err );
			}		
		}
	TPtrC event;
	// Capture the Notifications
	if ( GetStringFromConfig(aSection, KEventType, event))
		{
		if (!CaptureEvent(aSection))
			SetBlockResult( EFail );
		else
			SetBlockResult( EPass );
		}
	else
		SetBlockResult( EPass );

	}

/**
Purpose: Remove a drive from the priority drive list

@internalComponent
@param  aSection Current ini file command section
*/
void CT_MediaTestWrapper::DoCmdRemoveDrive(const TDesC& aSection)
	{
	TInt drive = 0;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
		}
	
	TRAPD( err, iSession->RemoveDriveL(TDriveNumber(drive)));
	
	// Handling the Errors
	if (err != KErrNone)
		{
		if ( err == KErrNotFound )
			{
			ERR_PRINTF2(_L("<font color=FF0000>A non-existent drive number is passed: %d error</font>"),err);
			SetError( err );
			}
		else if ( err == KErrArgument )
			{
			ERR_PRINTF2(_L("<font color=FF0000>System drive is passed: %d error</font>"),err);
			SetError( err );
			}
		else if ( err == KErrGeneral )
			{
			ERR_PRINTF2(_L("<font color=FF0000>Any DB related: %d error</font>"),err);
			SetError( err );
			}
		else if ( err == KErrServerBusy )
			{
			ERR_PRINTF2(_L("<font color=FF0000>Outstanding Message Server operations Running: %d error</font>"),err);
			SetError( err );
			}
		else
			{
			ERR_PRINTF2(_L("<font color=FF0000>Other system wide error codes: %d error</font>"),err);
			SetError( err );
			}		
		}
	TPtrC event;
	// Capture the Notifications
	if ( GetStringFromConfig(aSection, KEventType, event))
		{
		if (!CaptureEvent(aSection))
			SetBlockResult( EFail );
		else
			SetBlockResult( EPass );
		}
	else
		SetBlockResult( EPass );
	}

/**
Purpose: Update the priority of a drive. 

@internalComponent
@param  aSection Current ini file command section
*/ 
void CT_MediaTestWrapper::DoCmdUpdateDrive(const TDesC& aSection)
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
	
	TRAPD( err, iSession->UpdateDrivePriorityL(TDriveNumber(drive), (TUint&)priority));
	
	// Handling the Errors
	if (err != KErrNone)
		{
		if ( err == KErrNotFound )
			{
			ERR_PRINTF2(_L("<font color=FF0000>A non-existent drive number is passed: %d error</font>"),err);
			SetError( err );
			}
		else if ( err == KErrArgument )
			{
			ERR_PRINTF2(_L("<font color=FF0000>Priority value is not within valid range (Range 1-7) OR the drive number is not within valid range (0-25): %d error</font>"),err);
			SetError( err );
			}
		else if ( err == KErrGeneral )
			{
			ERR_PRINTF2(_L("<font color=FF0000>Any DB related: %d error</font>"),err);
			SetError( err );
			}
		else if ( err == KErrServerBusy )
			{
			ERR_PRINTF2(_L("<font color=FF0000>Outstanding Message Server operations Running: %d error</font>"),err);
			SetError( err );
			}
		else
			{
			ERR_PRINTF2(_L("<font color=FF0000>Other system wide error codes: %d error</font>"),err);
			SetError( err );
			}
		}

	TPtrC event;
	// Capture the Notifications
	if ( GetStringFromConfig(aSection, KEventType, event))
		{
		if (!CaptureEvent(aSection))
			SetBlockResult( EFail );
		else
			SetBlockResult( EPass );
		}
	else
		SetBlockResult( EPass );
	}


/**
Purpose: Get the current drive number and the priority

@internalComponent
@param  aSection Current ini file command section
*/
void CT_MediaTestWrapper::DoCmdGetCurrentDrive(const TDesC& aSection)
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

	TRAPD( err, iSession->CurrentDriveInfoL((TDriveNumber&)drive, (TUint&)priority));
	
	// Handling the Errors
	if (err != KErrNone)
		{
			ERR_PRINTF2(_L("<font color=FF0000>Other system wide error codes: %d error</font>"),err);
			SetError( err );
		}
	else
		SetBlockResult( EPass );

	}

/**
Purpose: Get all the drives in the priority drive list

@internalComponent
@param  aSection Current ini file command section
*/
void CT_MediaTestWrapper::DoCmdGetDriveList(const TDesC& aSection)
	{
	TInt expDrives = 0;
	if (!GetIntFromConfig(aSection, KExpDrives(), expDrives))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KExpDrives());
		SetBlockResult(EFail);
		}
	
	RArray<TDriveNumber> driveList;
	
	TRAPD( err, iSession->DriveListL(driveList));
	
	// Handling the Errors
	if (err != KErrNone)
		{
			ERR_PRINTF2(_L("<font color=FF0000>Other system wide error codes: %d error</font>"),err);
			SetError( err );
		}
	TInt actualDrives = driveList.Count();
	if( actualDrives != expDrives)
		{
		ERR_PRINTF3(_L("<font color=FF0000>Expected drives No: %d is not equal to Actual No: %d</font>"), expDrives, actualDrives);
		SetBlockResult(EFail);
		}
	else
		SetBlockResult( EPass );
	
	driveList.Close();
	}

/**
Purpose: Get the list of drives from the priority drive list that have valid message store

@internalComponent
@param  aSection Current ini file command section
*/
void CT_MediaTestWrapper::DoCmdGetAvailableDriveList(const TDesC& aSection)
	{
	TInt expAvailDrives = 0;
	if (!GetIntFromConfig(aSection, KExpAvailDrives(), expAvailDrives))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KExpAvailDrives());
		SetBlockResult(EFail);
		}
	
	RArray<TDriveNumber> driveList;
	
	TRAPD( err, iSession->AvailableDriveListL(driveList));
	
	// Handling the Errors
	if (err != KErrNone)
		{
			ERR_PRINTF2(_L("<font color=FF0000>Other system wide error codes: %d error</font>"),err);
			SetError( err );
		}
	TInt actualAvailDrives = driveList.Count();
	if( actualAvailDrives != expAvailDrives)
		{
		ERR_PRINTF3(_L("<font color=FF0000>Expected drives No: %d is not equal to Actual No: %d</font>"), expAvailDrives, actualAvailDrives);
		SetBlockResult(EFail);
		}
	else
		SetBlockResult( EPass );
	
	driveList.Close();
	}

/**
Purpose: Returns a selection of TMsvIds of message entries present in all available drives 
		 in the priority drive list. The CMsvEntry context should be set to one of the standard folders only.

@internalComponent
@param  aSection Current ini file command section
*/
void CT_MediaTestWrapper::DoCmdChildrenOfAvailableDrives(const TDesC& aSection)
	{	
	TPtrC folderName;
	if(!GetStringFromConfig( aSection, KFolderName, folderName))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KFolderName());
		SetBlockResult(EFail);
		}

	TMsvId folderId = NULL;
	TPtrC parentFolderName;
	if(GetStringFromConfig( aSection, KParentFolderName, parentFolderName))
		{
			TInt err = KErrNone;
			// Retrives the Id of the local folder
			TRAP(err, folderId = CT_MsgUtils::FindEntryByNameL(iSession, KMsvLocalServiceIndexEntryId,folderName,parentFolderName));
		}
		else
		{
			// Retrieves the folder Id based on the folder name specified 
			folderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(folderName);
		}
	
	if( folderId == KErrNotFound)
		{
		ERR_PRINTF2(_L("<font color=FF0000>None existing %d</font>"), folderId);
		SetBlockResult(EFail);
		}

	CMsvEntry* entry =  CMsvEntry::NewL(*iSession,folderId,TMsvSelectionOrdering(),ETrue);
    CleanupStack::PushL(entry);
				
	CMsvEntrySelection* selection =  entry->ChildrenOfAvailableDrivesL();
	CleanupStack::PushL(selection);

	TInt expRes = 0;
	if (!GetIntFromConfig(aSection, KExpRes(), expRes))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KExpRes());
		SetBlockResult(EFail);
		}
	
	// Validating the entries
	TInt actualRes = selection->Count();
	if( actualRes != expRes)
		{
		ERR_PRINTF3(_L("<font color=FF0000>Expected children No: %d is not equal to Actual No: %d</font>"), expRes, actualRes);
		SetBlockResult(EFail);
		}
	else
		SetBlockResult( EPass );
	
	CleanupStack::PopAndDestroy(selection); // selection
	CleanupStack::PopAndDestroy(entry); // entry
	}
	
/**
Purpose: Get TMsvIds of all entries under a standard folder

@internalComponent
@param  aSection Current ini file command section
*/
void CT_MediaTestWrapper::DoCmdGetChildIdsAllL(const TDesC& aSection)
	{	
	TPtrC folderName;
	if(!GetStringFromConfig( aSection, KFolderName, folderName))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KFolderName());
		SetBlockResult(EFail);
		}
	
	TMsvId folderId = NULL;
	TPtrC parentFolderName;
	if(GetStringFromConfig( aSection, KParentFolderName, parentFolderName))
		{
			TInt err = KErrNone;
			// Retrives the Id of the local folder
			TRAP(err, folderId = CT_MsgUtils::FindEntryByNameL(iSession, KMsvLocalServiceIndexEntryId,folderName,parentFolderName));
		}
		else
		{
			// Retrieves the folder Id based on the folder name specified 
			folderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(folderName);
		}
	
	if( folderId == KErrNotFound)
		{
		ERR_PRINTF2(_L("<font color=FF0000>None existing %d</font>"), folderId);
		SetBlockResult(EFail);
		}
		
	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
	filter->SetOrder(TMsvSelectionOrdering(0, EMsvSortByNone, ETrue));
		
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TRAPD( err, iSession->GetChildIdsAllL(folderId, *filter, *selection));			
	
	// Handling the Errors
	if (err != KErrNone)
		{
		if(err == KErrArgument)
			{
			ERR_PRINTF2(_L("<font color=FF0000>Id other than standard folder Id is not allowed: %d error</font>"),err);
			SetError( err );
			}
		else
			{
			ERR_PRINTF2(_L("<font color=FF0000>Other system wide error codes: %d error</font>"),err);
			SetError( err );
			}
		}
	TInt expRes = 0;
	if (!GetIntFromConfig(aSection, KExpRes(), expRes))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KExpRes());
		SetBlockResult(EFail);
		}
	
	TInt actualRes = selection->Count();
	if( actualRes != expRes)
		{
		ERR_PRINTF3(_L("<font color=FF0000>Expected children No: %d is not equal to Actual No: %d</font>"), expRes, actualRes);
		SetBlockResult(EFail);
		}
	else
		SetBlockResult( EPass );

	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(filter);
	}

/**
Purpose: Check if the message store in a drive is valid

@internalComponent
@param  aSection Current ini file command section
*/
void CT_MediaTestWrapper::DoCmdIsMessageStoreSupported(const TDesC& aSection)
	{
	TInt drive = 0;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
		}
	
	TBool expMsgStore = ETrue;
	if (!GetBoolFromConfig(aSection, KIsMsgStoreSupported(), expMsgStore))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KIsMsgStoreSupported());
		SetBlockResult(EFail);
		}
		
	TBool actualMsgStore = MessageServer::IsMessageStoreSupported(TDriveNumber(drive));
	
	// Validating the message store
	if(actualMsgStore != expMsgStore)
		{
		INFO_PRINTF3(_L("<font color=FF0000>Expected Message store supported value:%d is not equal to Actual value:%d</font>"),expMsgStore,expMsgStore);
		SetBlockResult(EFail);
		}
	else
		SetBlockResult( EPass );

	}
/**
Purpose: Mount the drive

@internalComponent
@param  aSection Current ini file command section
*/
void CT_MediaTestWrapper::DoCmdMountDrive(const TDesC& aSection)
	{
	TInt drive = 0;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
		}

	RMessagingTestUtilityServer2	ts;
	User::LeaveIfError(ts.Connect());
	CleanupClosePushL(ts);
	ts.MountDrive(drive);
	CleanupStack::PopAndDestroy(&ts);
	
	SetBlockResult( EPass );
	}
/**
Purpose: Dismount the drive

@internalComponent
@param  aSection Current ini file command section
*/
void CT_MediaTestWrapper::DoCmdDisMountDrive(const TDesC& aSection)
	{
	TInt drive = 0;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
		}

	RMessagingTestUtilityServer2	ts;
	User::LeaveIfError(ts.Connect());
	CleanupClosePushL(ts);
	ts.UnMountDrive(drive);
	CleanupStack::PopAndDestroy(&ts);

	SetBlockResult( EPass );
	}

/**
Purpose: This gives the actual count of entries under a standard folder.

@internalComponent
@param  aSection Current ini file command section
*/
void CT_MediaTestWrapper::DoCmdVerifyCount(const TDesC& aSection)
	{
	TPtrC folder2;
	
	if (!GetStringFromConfig(aSection, KFolder(), folder2))			
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KFolder());
		SetBlockResult(EFail);
		}
    	
  	TInt expCount=0;
  	if(GetIntFromConfig(aSection, KCount(),expCount) && KTestDraftEntryId() == folder2)
	   	{	
        TMsvId folderId = KMsvDraftEntryId;     	
     	CMsvEntry* entry =  CMsvEntry::NewL(*iSession,folderId,TMsvSelectionOrdering(),ETrue);
		CleanupStack::PushL(entry);	
		entry->SetEntryL(folderId);			
		CMsvEntrySelection* selection =  entry->ChildrenOfAvailableDrivesL();	
		CleanupStack::PushL(selection);		        
        TInt count = selection->Count();
        if(count != expCount)
        	{
           	ERR_PRINTF3(_L("Expected Count=%d. Actual Count=%d"),expCount,count);	
           	SetBlockResult(EFail);
        	}
        else
        	{
           	SetBlockResult(EPass);	
        	}	        
      	CleanupStack::PopAndDestroy(selection);
        CleanupStack::PopAndDestroy(entry);  
    	}
	}

/**
Purpose: This creates SMS account
*/
void CT_MediaTestWrapper::DoCmdCreateSMSAccount()
	{			
	CSmsSettings* settings = CSmsSettings::NewLC( );		
	CSmsAccount* account = CSmsAccount::NewLC( );	
	account->InitialiseDefaultSettingsL(*settings);

	settings->AddServiceCenterL( KScriptItemSC1, KScriptItemSC1Def );
	settings->AddServiceCenterL( KScriptItemSC2, KScriptItemSC2Def );
	settings->AddServiceCenterL( KScriptItemSC3, KScriptItemSC3Def );
	settings->SetDefaultServiceCenter( 2 );	
	settings->SetValidityPeriod( ESmsVPWeek );
	settings->SetReplyQuoted( ETrue );
	settings->SetRejectDuplicate( ETrue );
	settings->SetDelivery( ESmsDeliveryImmediately );
	settings->SetDeliveryReport( ETrue );
	settings->SetReplyPath( EFalse );
	settings->SetMessageConversion( ESmsConvPIDNone );
	settings->SetCanConcatenate( ETrue );
	settings->SetCharacterSet( TSmsDataCodingScheme::ESmsAlphabet8Bit );
	settings->SetValidityPeriodFormat( TSmsFirstOctet::ESmsVPFSemiOctet );
	settings->SetStatusReportHandling( CSmsSettings::EMoveReportToInboxVisible );
	settings->SetSpecialMessageHandling( CSmsSettings::EMoveReportToInboxVisible );	
	account->SaveSettingsL(*settings);
	
	CleanupStack::PopAndDestroy(account);
	CleanupStack::PopAndDestroy(settings);
	}

/**
Purpose: This creates SMS message in draft with default values...
*/
void CT_MediaTestWrapper::DoCmdCreateSMS()
	{
	CClientMtmRegistry*	registry = CClientMtmRegistry::NewL(*iSession);	
	CleanupStack::PushL(registry);
	
	CSmsClientMtm* smsClientMtm = static_cast<CSmsClientMtm*> (registry->NewMtmL( KUidMsgTypeSMS ));
	CleanupStack::PushL(smsClientMtm);	
	smsClientMtm->SwitchCurrentEntryL( KMsvDraftEntryId );	
	smsClientMtm->CreateMessageL( smsClientMtm->DefaultServiceL() );
	smsClientMtm->SaveMessageL();	
	TMsvEntry smsEntry = smsClientMtm->Entry().Entry();	
	smsEntry.SetUnread(ETrue);	
	smsClientMtm->Entry().ChangeL(smsEntry);
		
	CleanupStack::PopAndDestroy(smsClientMtm);
	CleanupStack::PopAndDestroy(registry);
	}

/**
Purpose: This creates Bulk SMS message in draft with default values...
*/
void CT_MediaTestWrapper::DoCmdCreateBulkSMS()
	{
	CClientMtmRegistry*	registry = CClientMtmRegistry::NewL(*iSession);	
	CleanupStack::PushL(registry);
	
	CSmsClientMtm* smsClientMtm = static_cast<CSmsClientMtm*> (registry->NewMtmL( KUidMsgTypeSMS ));
	CleanupStack::PushL(smsClientMtm);
	for (TInt i=0; i <5; i++)
		{		
		smsClientMtm->SwitchCurrentEntryL( KMsvDraftEntryId );			
		smsClientMtm->CreateMessageL( smsClientMtm->DefaultServiceL() );
		smsClientMtm->SaveMessageL();	
		TMsvEntry smsEntry = smsClientMtm->Entry().Entry();	
		smsEntry.SetUnread(ETrue);	
		smsClientMtm->Entry().ChangeL(smsEntry);
		}
		
	CleanupStack::PopAndDestroy(smsClientMtm);
	CleanupStack::PopAndDestroy(registry);	
	}

/**
Purpose: This deletes the SMS message in draft for current active drive only...
*/
void CT_MediaTestWrapper::DoCmdDeleteMessages()
	{
	TMsvId draft= KMsvDraftEntryId;
   	CMsvEntry* entry =  CMsvEntry::NewL(*iSession, draft, TMsvSelectionOrdering(),ETrue);
	CleanupStack::PushL(entry);				

	entry->SetEntryL(draft);	
	CMsvEntrySelection*	selection=entry->ChildrenL();	
	CleanupStack::PushL(selection);
	TInt count=selection->Count();
	TInt i=0;
	for (i=count; i>0; )
		{
		TMsvId	childId=selection->At(--i);
		entry->DeleteL(childId);
		}		
		
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(entry);			
	}
	
/**
Purpose: This check for entries in all available folder...
*/
void CT_MediaTestWrapper::DoCmdSearchMessages(const TDesC& aSection)
	{
	TInt expCount=0;
	if (!GetIntFromConfig(aSection, KCount(), expCount))	
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KCount());
		SetBlockResult(EFail);
		}

	TMsvId draft= KMsvDraftEntryId;
   	CMsvEntry* entry =  CMsvEntry::NewL(*iSession, draft, TMsvSelectionOrdering(),ETrue);
	CleanupStack::PushL(entry);					
	entry->SetEntryL(draft);	
	CMsvEntrySelection*	selection=entry->ChildrenOfAvailableDrivesL();
	CleanupStack::PushL(selection);
	TInt count=selection->Count();	
	
 	if(expCount != count)
    	{
       	ERR_PRINTF3(_L("Expected Count=%d. Actual Count=%d"),expCount, count);	
       	SetBlockResult(EFail);
    	}
    else
    	{
       	SetBlockResult(EPass);	
    	}	  
   	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(entry);			      	
	}
	
/**
Purpose: This copies the message Store to the specified drive.
*/	
void CT_MediaTestWrapper::DoCmdCopyStore(const TDesC& aSection)
	{
	TInt drive = 0;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
		}
	
	CMsvOperationWait* active = CMsvOperationWait::NewLC();	
	CMsvEntry* entry =  CMsvEntry::NewL(*iSession, KMsvDraftEntryId, TMsvSelectionOrdering(),ETrue);
	CleanupStack::PushL(entry);		
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	entry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidMsgTypeSMS;
	selection->AppendL(service.Id());
	
	TBuf8<256> progress;
	TBuf8<32> param;
	CMsvOperation* operation = NULL;
	active->Start();
	operation = iSession->CopyStoreL(TDriveNumber(drive), active->iStatus);
	CActiveScheduler::Start();
	delete operation;
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(entry);
	CleanupStack::PopAndDestroy(active);
	}
	
	
void CT_MediaTestWrapper::DoCmdRemoveMMC()
	{		
	CConsoleBase* console = Console::NewL(KConsoleName, TSize(KConsFullScreen,KConsFullScreen));
	console->Printf(KPromptRemoveMMC);
	console->Printf(KNewLine);
	console->Printf(KPressKeyToContinue);
	console->Getch();
	delete console;
	}


/**
Purpose: Changes Message Server current drive to a specified drive.

@internalComponent
@param  aSection Current ini file command section
*/ 
void CT_MediaTestWrapper::DoCmdChangeDriveL(const TDesC& aSection)
    {
    TInt drive = 0;
    if (!GetIntFromConfig(aSection, KDrive(), drive))
        {
        ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
        SetBlockResult(EFail);
        }
    
    CMsvOperation* operation = NULL;
    CMsvOperationWait* active = CMsvOperationWait::NewLC();
    active->Start();
    TRAPD(err, operation = iSession->ChangeDriveL(drive, active->iStatus));

    // Handling the Errors
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("<font color=FF0000>Error: %d</font>"), err);
        SetError(err);
        }

    TPtrC event;
    // Capture the Notifications
    if ( GetStringFromConfig(aSection, KEventType, event))
        {
        if (!CaptureEvent(aSection))
            SetBlockResult( EFail );
        else
            SetBlockResult( EPass );
        }
    else
        SetBlockResult( EPass );

    delete operation;
    CleanupStack::PopAndDestroy(active);
    }


/*Purpose: Get session event type based on similar name from configuration file section

@internalComponent
@param  aSection Current ini file command section
*/ 
TBool CT_MediaTestWrapper::GetVerdictFromConfig(const TDesC& aSection, const TDesC& aParameterName, MMsvSessionObserver::TMsvSessionEvent& aEvent)
	{
	// Read drives mapping operation name from INI file
	TPtrC eventStr;
	TBool ret = GetStringFromConfig(aSection, aParameterName, eventStr);

	if (ret)
		{
		if (eventStr == KEMsvMessageStoreNotSupported())
			{
			aEvent = MMsvSessionObserver::EMsvMessageStoreNotSupported;
			}
		else if (eventStr == KEMsvMessageStoreCorrupt())
			{
			aEvent = MMsvSessionObserver::EMsvMessageStoreCorrupt;
			}
	 	else if (eventStr == KEMsvMediaChanged())
			{
			aEvent = MMsvSessionObserver::EMsvMediaChanged;
			}
		else if (eventStr == KEMsvRefreshMessageView())
			{
			aEvent = MMsvSessionObserver::EMsvRefreshMessageView;
			}
		else if (eventStr == KEMsvDiskNotAvailable())
			{
			aEvent = MMsvSessionObserver::EMsvDiskNotAvailable;
			}
		}

	return ret;
	}
/**
Purpose: Handle the session event 

@internalComponent
@param  aEvent event type of session
@param  aArg1  NA 
@param  aArg2  NA
@param  NA 
*/
void CT_MediaSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny*)
	{
	if (aEvent)
		iType = aEvent;
	if (aArg1)
		iArg1 = *(TInt*)aArg1;
	if (aArg2)
		iArg2 = *(TInt*)aArg2;
	CActiveScheduler::Stop();
	}
/**
Purpose: Capture the notification

@internalComponent
@param  aSection Current ini file command section
*/ 
TBool CT_MediaTestWrapper::CaptureEvent(const TDesC& aSection)
	{
	CActiveScheduler::Start();
	MMsvSessionObserver::TMsvSessionEvent event;
	if (!GetVerdictFromConfig(aSection, KEventType, event))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KEventType());
		return EFalse;
		}
	const TDesC& expStr = ConvertToStr(event);
	INFO_PRINTF2( _L("<font color=990000>Expected event = %S</font>"), &expStr );

	const TDesC& actualStr = ConvertToStr(iObserver->iType);
	INFO_PRINTF2( _L("<font color=990000>Actual event = %S</font>"), &actualStr );
	
	if ( iObserver->iType != event )
		{
		ERR_PRINTF3(_L("<font color=FF0000>Expected event: %S is not equal to Actual event: %S </font>"), &expStr, &actualStr);
		return EFalse;
		}
	return ETrue;
	}	
/**
Purpose: Convert the event to string

@internalComponent
@param  aEvent session event
*/ 	
const TDesC& CT_MediaTestWrapper::ConvertToStr(MMsvSessionObserver::TMsvSessionEvent& aEvent ) 
	{
	if( aEvent == MMsvSessionObserver::EMsvMessageStoreNotSupported)
		return KEMsvMessageStoreNotSupported();
	else if ( aEvent == MMsvSessionObserver::EMsvMessageStoreCorrupt)
		return KEMsvMessageStoreCorrupt();
	else if ( aEvent == MMsvSessionObserver::EMsvMediaChanged)
		return KEMsvMediaChanged();
	else if ( aEvent == MMsvSessionObserver::EMsvRefreshMessageView)
		return KEMsvRefreshMessageView();
	else if ( aEvent == MMsvSessionObserver::EMsvDiskNotAvailable)
		return KEMsvDiskNotAvailable();
	else 
		return KEMsvMisMatch();
	}


