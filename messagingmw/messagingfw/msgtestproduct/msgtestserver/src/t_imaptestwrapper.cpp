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

#include "t_imaptestwrapper.h"

// Utils includes
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>
#include <messagingtestutility2.h>


// Commands
_LIT(KNew,									"New");
_LIT(KCopyMessage,							"CopyMessage");
_LIT(KMoveMessage,							"MoveMessage");
_LIT(KDeleteMessage,						"DeleteMessage");

// Command parameters
_LIT(KSrcFolderName,						"SrcFolderName");
_LIT(KSrcParentFolderName,					"SrcParentFolderName");
_LIT(KTargetFolderName, 					"TargetFolderName");
_LIT(KTargetParentFolderName, 				"TargetParentFolderName");
_LIT(KSrcLookUpIn, 							"SrcLookUpIn");
_LIT(KTrgtLookUpIn, 						"TargetLookUpIn");
_LIT(KImapAccountName,						"ImapAccountName");
_LIT(KSubject,								"Subject");
_LIT(KExpectedErr, 							"ExpectedErr");

_LIT(KFolderName,							"FolderName");
_LIT(KParentFolderName,						"ParentFolderName");
_LIT(KLookUpIn, 							"LookUpIn");

_LIT(KObjectValue,							"object_value");


/**
Purpose: Constructor of CT_ImapTestWrapper class

@internalComponent
*/
CT_ImapTestWrapper::CT_ImapTestWrapper()
:	iObject(NULL),
	iSession(NULL),
	iObserver(NULL),
	iOperation(NULL),
	iActiveCallback(NULL)
	{
	}
/**
Purpose: Destructor of CT_ImapTestWrapper class

@internalComponent
*/
CT_ImapTestWrapper::~CT_ImapTestWrapper()
	{
	delete iOperation;

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

		delete iActiveCallback;
		iActiveCallback = NULL;

		}
	}
/**
Purpose: Command fuction of CT_ImapTestWrapper class

@internalComponent
*/
CT_ImapTestWrapper* CT_ImapTestWrapper::NewL()
	{
	CT_ImapTestWrapper*	ret = new (ELeave) CT_ImapTestWrapper();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}
/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
void CT_ImapTestWrapper::ConstructL()
	{
	iObject			= new (ELeave) TInt;
	iObserver		= new(ELeave)CT_ImapSessionObserver;
	iSession		= CMsvSession::OpenSyncL(*iObserver);
	iActiveCallback  = CActiveCallback::NewL(*this);
	}

/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
TBool CT_ImapTestWrapper::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
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
	else if(KCopyMessage() == aCommand)
		{
		DoCmdCopyMessageL(aSection);
		}
	else if(KMoveMessage() == aCommand)
		{
		DoCmdMoveMessage(aSection);
		}
	else if(KDeleteMessage() == aCommand)
		{
		DoCmdDeleteMessage(aSection);
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
void CT_ImapTestWrapper::DoCmdNewL(const TDesC& aSection)
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
Purpose: Copy the message from source to destination

@internalComponent
@param  aSection Current ini file command section
*/
void CT_ImapTestWrapper::DoCmdCopyMessageL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Test Step : CopyMessage"));

	if(ReadIniCopyMessage(aSection))
		{
		TMsvId folderId;
		TInt err = KErrNone;
		if(iSrcLookupIn.Compare(_L("REMOTE")) == 0)
			{
			// Get the Id of the remote folder
			folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSession,
													iImapAccountName,iSrcfolderName,iSrcParentFolderName);
			}
		else
			{
			if(iSrcLookupIn.Compare(_L("LOCAL"))==0)
				{
				TRAP(err, folderId = CT_MsgUtils::FindEntryByNameL(iSession, KMsvLocalServiceIndexEntryId,iSrcfolderName,iSrcParentFolderName));
				//TRAP(err,folderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(iSrcfolderName));
				}
			else
				{
				ERR_PRINTF1(_L("Wrong Input"));
				SetBlockResult(EFail);
				}
			}

		//trgtfolderId lookup

		TMsvId targetId;
		if(iTrgtLookupIn.Compare(_L("REMOTE")) == 0)
			{
			// Get the Id of the remote folder
			TRAP(err, targetId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSession,
													iImapAccountName,iTrgtfolderName,iTrgtParentFolderName));
			}
		else
			{
			if(iTrgtLookupIn.Compare(_L("LOCAL"))==0)
				{
				TRAP(err, targetId = CT_MsgUtils::FindEntryByNameL(iSession, KMsvLocalServiceIndexEntryId,iTrgtfolderName,iTrgtParentFolderName));
				}
			else
				{
				ERR_PRINTF1(_L("Wrong Input"));
				SetBlockResult(EFail);
				}
			}

		INFO_PRINTF2(_L(" The trgtfolder Id is %d"),targetId);

		if(targetId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid folder name specified"));
			SetBlockResult(EFail);
			}
		else
			{
			CMsvEntry* entry = CMsvEntry::NewL(*iSession,folderId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
			CleanupStack::PushL(entry);
			entry->SetEntryL(folderId);
			// Retrieves the message Id based ont he message subject from the given local folder
			TMsvId messageId = KMsvNullIndexEntryId;
			TRAP(err,messageId = CT_MsgUtils::SearchMessageBySubjectL(iSession, folderId , iSubject));

			if( messageId == KMsvNullIndexEntryId)
				{
				ERR_PRINTF1(_L("The given message is not found"));

				if((iExpectedErr != KErrNone)||(err != KErrNone))
					{
					if(err == iExpectedErr )
						{
						INFO_PRINTF1(_L("Failed with Expected Error"));
						SetBlockResult(EPass);
						}
					else
						{
						ERR_PRINTF2(_L("Failed with %d error"),err);
						SetBlockResult(EFail);
						}
					}
				}
			// Message found
			else
				{
				INFO_PRINTF2(_L("The Message Id is %d"),messageId );

				delete iOperation;
				iOperation = NULL;
				TRAP(err, iOperation = entry->CopyL(messageId, targetId, iActiveCallback->iStatus));
				iActiveCallback->Activate(ETrue);
				CActiveScheduler::Start();
				if((iExpectedErr != KErrNone)||(err != KErrNone))
					{
					if(err == iExpectedErr)
						{
						INFO_PRINTF1(_L("Failed with Expected Error"));
						SetBlockResult(EPass);
						}
					else
						{
						ERR_PRINTF2(_L("Failed with %d error"),err);
						SetBlockResult(EFail);
						}
					}

				}

			CleanupStack::PopAndDestroy(entry);
			}
		}

	}


/**
Purpose: Copy the message from source to destination

@internalComponent
@param  aSection Current ini file command section
*/
void CT_ImapTestWrapper::DoCmdMoveMessage(const TDesC& aSection)
	{
	// Read the name of the source parent folder from the ini file
	TPtrC srcParentFolderName;
	if(!GetStringFromConfig( aSection, KSrcParentFolderName, srcParentFolderName))
		{
		ERR_PRINTF1(_L("Src Parent Folder name is not specified"));
		SetBlockResult(EFail);
		}
	// Read the name of the source folder from the ini file
	TPtrC srcfolderName;
	if(!GetStringFromConfig( aSection, KSrcFolderName, srcfolderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetBlockResult(EFail);
		}
	// Read the name of the target parent folder from the ini file
	TPtrC trgtParentFolderName;
	if(!GetStringFromConfig( aSection, KTargetParentFolderName, trgtParentFolderName))
		{
		ERR_PRINTF1(_L("Trgt Parent Folder name is not specified"));
		SetBlockResult(EFail);
		}
	// Read the name of the target folder from the ini file
	TPtrC trgtfolderName;
	if(!GetStringFromConfig( aSection, KTargetFolderName, trgtfolderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetBlockResult(EFail);
		}
	else
		{

		// Read the IMAP account name from the ini file
		TPtrC imapAccountName;
		if(!GetStringFromConfig( aSection, KImapAccountName, imapAccountName))
			{
			ERR_PRINTF1(_L("Imap account name is not specified"));
			SetBlockResult(EFail);
			}
		else
			{
			TInt err = KErrNone;
			// Read the name of source Lookup Folder from the ini file
			TPtrC srcLookupIn;
			if(!GetStringFromConfig( aSection, KSrcLookUpIn, srcLookupIn))
				{
				ERR_PRINTF1(_L("LookUpFolder name is not specified"));
				SetBlockResult(EFail);
				}
			// Lookup the source folder
			TMsvId folderId;
			if(srcLookupIn.Compare(_L("REMOTE")) == 0)
				{
				// Get the Id of the remote folder
				TRAP(err,folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSession,imapAccountName,srcfolderName,srcParentFolderName));
				}
			else
				{
				if(srcLookupIn.Compare(_L("LOCAL"))==0)
					{
					// Get the Id of the local folder
					TRAP(err, folderId = CT_MsgUtils::FindEntryByNameL(iSession, KMsvLocalServiceIndexEntryId,srcfolderName,srcParentFolderName));
					//TRAP(err,folderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(srcfolderName));
					}
				else
					{
					ERR_PRINTF1(_L("Wrong Input"));
					SetBlockResult(EFail);
					}
				}

			INFO_PRINTF2(_L(" The srcfolder Id is %d"),folderId);

			// Read the name of target Lookup Folder from the ini file
			TPtrC trgtLookupIn;
			if(!GetStringFromConfig( aSection, KTrgtLookUpIn, trgtLookupIn))
				{
				ERR_PRINTF1(_L("LookUpFolder name is not specified"));
				SetBlockResult(EFail);
				}

			// Lookup the target folder
			TMsvId targetId;
			if(trgtLookupIn.Compare(_L("REMOTE")) == 0)
				{
				// Get the Id of the remote folder
				TRAP(err,targetId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSession,imapAccountName,trgtfolderName,trgtParentFolderName));
				}
			else
				{
				if(trgtLookupIn.Compare(_L("LOCAL"))==0)
					{
					// Get the Id of the local folder
					TRAP(err, targetId = CT_MsgUtils::FindEntryByNameL(iSession, KMsvLocalServiceIndexEntryId,trgtfolderName,trgtParentFolderName));
					}
				else
					{
					ERR_PRINTF1(_L("Wrong Input"));
					SetBlockResult(EFail);
					}
				}

			INFO_PRINTF2(_L(" The trgtfolder Id is %d"),targetId);

			CMsvEntry* entry = CMsvEntry::NewL(*iSession,folderId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
			CleanupStack::PushL(entry);
			entry->SetEntryL(folderId);

			TPtrC aSubject;
			if( !GetStringFromConfig(aSection,KSubject,aSubject ))
				{
				ERR_PRINTF1(_L("The subject of the message is not specified"));
				SetBlockResult(EFail);
				}
			else
				{
				// Retrieves the message Id based on the message subject
				TMsvId messageId = CT_MsgUtils::SearchMessageBySubjectL(iSession, folderId , aSubject);
				if( messageId == KMsvNullIndexEntryId)
					{
					ERR_PRINTF1(_L("The given message is not found"));
					SetBlockResult(EFail);
					}
				else
					{
					INFO_PRINTF2(_L("The Message Id is %d"),messageId );

					TInt expectedErr = 0;
					GetIntFromConfig(aSection, KExpectedErr, expectedErr);

					delete iOperation;
					iOperation = NULL;
					TRAP(err, iOperation = entry->MoveL(messageId, targetId, iActiveCallback->iStatus));
					iActiveCallback->Activate(ETrue);
					CActiveScheduler::Start();

					if(expectedErr != KErrNone || err != KErrNone)
						{
						if(err == expectedErr)
							{
							INFO_PRINTF2(_L("Failed with Expected Error %d"), err);
							}
						else
							{
							ERR_PRINTF2(_L("Failed with %d error"),err);
							SetBlockResult(EFail);
							}
						}
					}
				}

			CleanupStack::PopAndDestroy(entry);
			}
		}
	}


/**
Purpose: Copy the message from source to destination

@internalComponent
@param  aSection Current ini file command section
*/
void CT_ImapTestWrapper::DoCmdDeleteMessage(const TDesC& aSection)
	{
	if(ReadIniDeleteMessage(aSection))
		{
		TMsvId folderId;
		TInt err =0;
		if(iLookUpIn.Compare(_L("REMOTE")) == 0)
			{
			// Get the Id of the remote folder
			TRAP(err, folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSession,
													iImapAccountName,iFolderName,iParentFolderName));
			}
		else
			{
			if(iLookUpIn.Compare(_L("LOCAL"))==0)
				{
				//TRAP(err, folderId = CT_MsgUtils::FindEntryByNameL(iSession, KMsvLocalServiceIndexEntryId,iFolderName,iParentFolderName));
				folderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(iFolderName);
				}
			else
				{
				ERR_PRINTF1(_L("Wrong Input"));
				SetBlockResult(EFail);
				}
			}

		INFO_PRINTF2(_L(" The folder Id is %d"),folderId);

		if(folderId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid folder name specified"));
			SetBlockResult(EFail);
			}
		else
			{
			CMsvEntry* entry = CMsvEntry::NewL(*iSession,folderId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
			CleanupStack::PushL(entry);
			entry->SetEntryL(folderId);


			// Retrieves the message Id based ont he message subject from the given local folder
			TMsvId messageId = KMsvNullIndexEntryId;
			TRAP(err,messageId = CT_MsgUtils::SearchMessageBySubjectL(iSession, folderId , iSubject));
			if( messageId == KMsvNullIndexEntryId)
				{
				INFO_PRINTF1(_L("The given message is not found"));

				if((iExpectedErr != KErrNone)||(err != KErrNone))
					{
					if(err == iExpectedErr )
						{
						INFO_PRINTF1(_L("Failed with Expected Error"));
						SetBlockResult(EPass);
						}
					else
						{
						ERR_PRINTF2(_L("Failed with %d error"),err);
						SetBlockResult(EFail);
						}
					}
				}
			// Message found
			else
				{
				INFO_PRINTF2(_L("The Message Id is %d"),messageId );

				delete iOperation;
				iOperation = NULL;
				iOperation = entry->DeleteL(messageId,iActiveCallback->iStatus);
				iActiveCallback->Activate(ETrue);
				CActiveScheduler::Start();

				}

			CleanupStack::PopAndDestroy(entry);
			}
		}
	}




/**
Purpose: Handle the session event

@internalComponent
@param  aEvent event type of session
@param  aArg1  NA
@param  aArg2  NA
@param  NA
*/
void CT_ImapSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny*)
	{
	if (aEvent)
		iType = aEvent;
	if (aArg1)
		iArg1 = *(TInt*)aArg1;
	if (aArg2)
		iArg2 = *(TInt*)aArg2;
	}

void CT_ImapTestWrapper:: RunL(CActive* /*aActive*/, TInt aIndex)
	{
	TInt err = iActiveCallback->iStatus.Int();
	SetAsyncError(aIndex,err);
	CActiveScheduler::Stop();
	}

/**
Purpose: Read INI file
@internalComponent
@param  aSection Current ini file command section
*/

TBool CT_ImapTestWrapper::ReadIniCopyMessage(const TDesC& aSection)
	{
	TBool result = ETrue;

	TPtrC subject;
	if( !GetStringFromConfig(aSection,KSubject,subject))
		{
		ERR_PRINTF1(_L("The subject of the message is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iSubject.Set(subject);

		// Read the name of trgt Lookup Folder from the ini file

	TPtrC trgtLookUpIn;
	if(!GetStringFromConfig( aSection, KTrgtLookUpIn, trgtLookUpIn))
		{
		ERR_PRINTF1(_L("LookUpFolder name is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iTrgtLookupIn.Set(trgtLookUpIn);

		// Read the name of src Lookup Folder from the ini file
	TPtrC srcLookupIn;
	if(!GetStringFromConfig( aSection, KSrcLookUpIn, srcLookupIn))
		{
		ERR_PRINTF1(_L("LookUpFolder name is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iSrcLookupIn.Set(srcLookupIn);

		// Read the name of the source folder from the ini file
	TPtrC srcfolderName;
	if(!GetStringFromConfig( aSection, KSrcFolderName, srcfolderName))
		{
		ERR_PRINTF1(_L("Src Folder name is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iSrcfolderName.Set(srcfolderName);

	TPtrC srcParentFolderName;
	if(!GetStringFromConfig( aSection, KSrcParentFolderName, srcParentFolderName))
		{
		ERR_PRINTF1(_L("Src Parent Folder name is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iSrcParentFolderName.Set(srcParentFolderName);

	// Read the name of the source folder from the ini file
	TPtrC trgtfolderName;
	if(!GetStringFromConfig( aSection, KTargetFolderName, trgtfolderName))
		{
		ERR_PRINTF1(_L("Trgt Folder name is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iTrgtfolderName.Set(trgtfolderName);

		// Read the name of the source folder from the ini file
	TPtrC trgtParentFolderName;
	if(!GetStringFromConfig( aSection, KTargetParentFolderName, trgtParentFolderName))
		{
		ERR_PRINTF1(_L("Trgt Parent Folder name is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iTrgtParentFolderName.Set(trgtParentFolderName);

	// Read the IMAP account name from the ini file
	TPtrC imapAccountName;
	if(!GetStringFromConfig( aSection, KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iImapAccountName.Set(imapAccountName);

	TInt expectedErr = 0;
	if(!GetIntFromConfig( aSection, KExpectedErr, expectedErr))
		{
		INFO_PRINTF1(_L("Expected Err is not specified"));
		}
	iExpectedErr = expectedErr;

	return result;
	}

/**
Purpose: Read INI file
@internalComponent
@param  aSection Current ini file command section
*/
TBool CT_ImapTestWrapper::ReadIniDeleteMessage(const TDesC& aSection)
	{
	TBool result = ETrue;

	// Read the name of the remote folder from the ini file
	TPtrC folderName;
	if(!GetStringFromConfig( aSection, KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iFolderName.Set(folderName);

	// Read the Parent name of the remote folder from the ini file
	TPtrC parFolderName;
	if(!GetStringFromConfig( aSection, KParentFolderName, parFolderName))
		{
		ERR_PRINTF1(_L("Parent Folder name is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iParentFolderName.Set(parFolderName);

	// Read the IMAP account name from the ini file
	TPtrC imapAccountName;
	if(!GetStringFromConfig( aSection, KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iImapAccountName.Set(imapAccountName);

	// Read the name of Lookup Folder from the ini file
	TPtrC lookupIn;
	if(!GetStringFromConfig( aSection, KLookUpIn, lookupIn))
		{
		ERR_PRINTF1(_L("LookUpFolder name is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iLookUpIn.Set(lookupIn);

	TPtrC subject;
	if( !GetStringFromConfig(aSection,KSubject,subject ))
		{
		ERR_PRINTF1(_L("The subject of the message is not specified"));
		SetBlockResult(EFail);
		result = EFalse;
		}
	iSubject.Set(subject);

	TInt expectedErr = 0;
	if(!GetIntFromConfig( aSection, KExpectedErr, expectedErr))
		{
		ERR_PRINTF1(_L("Expected Err is not specified"));
		SetBlockResult(EFail);
		}
	iExpectedErr = expectedErr;

	return result;
	}
