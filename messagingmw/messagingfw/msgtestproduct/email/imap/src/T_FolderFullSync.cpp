// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// [TestStep Name]
// FolderFullSync
// [Paramaters]
// ImapAccountName			:	Name of the Imap account
// ParentFolderName			:	Name of the remote parent folder
// FolderName				:	Folder Name that needs to be synced.
// Selects and synchronises the folder specified.
// [APIs Used]
// 
//

/* User includes */
#include "T_FolderFullSync.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>


/* Epoc includes */
#include <miuthdr.h>
#include <imapcmds.h>
#include <imapset.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KFolderName,"FolderName");
_LIT(KExpectedErr,"ExpectedError");

/**
  Function : CT_MsgFolderFullSync
  Description : Constructor
  @internalTechnology
  @param :	aSharedDataIMAP		A reference to the CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgFolderFullSync::CT_MsgFolderFullSync(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,iOperation(NULL)
	{
	SetTestStepName(KFolderFullSync);
	}



/**
  Function : NewL
  Description : Creates an object of CT_MsgFolderFullSync 				 
  @internalTechnology
 @param :	aSharedDataIMAP		A reference to the CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgFolderFullSync* CT_MsgFolderFullSync::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgFolderFullSync* self = new(ELeave) CT_MsgFolderFullSync(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


	
/**
  Function : ~CT_MsgFolderFullSync
  Description : Destructor
  @internalTechnology
  @return : N/A
*/
CT_MsgFolderFullSync::~CT_MsgFolderFullSync()
	{
	delete iOperation;
	iOperation=NULL;
	}


	
/**
  Function : doTestStepL
  Description : Gets the Id of the remote folder, create a selection object,append the folder Id.
  			 	Call the command KIMAP4MTMFolderFullSync with the selection object created.
  @internalTechnology
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgFolderFullSync::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : FolderFullSync "));		
	if(ReadIni())
		{
		/* Gets the Id of the remote folder based on the folder name given*/
		TMsvId folderId;
		TRAPD(r,folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession, iImapAccountName, iFolderName, iParentName));
		if(r == KErrNotFound)
			{
			ERR_PRINTF1(_L("Invalid remote Source folder name specified"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}
		
		/* Creates selection object */
		CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
		CleanupStack::PushL(selection);

		/* Appends the folderId onto the end of the array */
		selection->AppendL(folderId);

		TBuf8<1> param;
		
		/* Obtain the service Id*/	
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)iImapAccountName);
	
		/* Switch the context to the Service Id */
		(iSharedDataIMAP.iMtm)->SwitchCurrentEntryL(imapServiceId);
			
		CT_MsgActive& active = Active();

		delete iOperation;
		iOperation = NULL;
	
		/* Selects and synchronises the folder specified by the TMsvId given by aSelection[0]*/
		iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMFolderFullSync,*selection,param,active.iStatus);
		
		active.Activate();
		CActiveScheduler::Start();
		User::LeaveIfError(active.Result());

		CleanupStack::PopAndDestroy(selection); /* selection*/

		TImap4GenericProgress temp;	
		TPckgC<TImap4GenericProgress> paramPack(temp);
		paramPack.Set(iOperation->ProgressL());
		TImap4GenericProgress progress=paramPack();
		TBool err = progress.iErrorCode;

		if(err==KErrNone)
			{
			INFO_PRINTF1(_L("iErrorCode equals KErrNone"));
			}
		else
			{
			ERR_PRINTF2(_L("iErrorCode does not equal KErrNone; iErrorCode=%d"),err);
			SetTestStepError(err);
			}
		}
	return TestStepResult();
	}

		
/**
  Function : ReadIni
  Description : Reads the data from the .ini and returns EPass or EFail if any of the input is missing
  @internalTechnology
  @return : TBool
*/	
TBool CT_MsgFolderFullSync::ReadIni()
	{
	TBool result = ETrue;
	
	/* Reads the account name from the ini file */
	TPtrC imapAccountName;
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iImapAccountName.Set(imapAccountName);
	
	/* Reads the parent folder name from the ini file */
	TPtrC parentName;
	if(!GetStringFromConfig(ConfigSection(), KParentFolderName, parentName))
		{
		ERR_PRINTF1(_L("Parent Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iParentName.Set(parentName);
	
	/* Reads the folder name from the ini file */
	TPtrC folderName;
	if(!GetStringFromConfig(ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iFolderName.Set(folderName);
	
	TInt expectedErr = 0;
	GetIntFromConfig( ConfigSection(), KExpectedErr, expectedErr);
		
	iExpectedErr = expectedErr;
	return result;
	}



/**
  Function : ProgressL
  Description : Displays the progress information of the asynchronous operation
  @return : N/A
*/
void CT_MsgFolderFullSync::ProgressL(TBool aFinal)
	{
	if ( aFinal )
		{
		/*	Display final progress iOperation*/
		}
	else
		{
		/*	Display current progress iOperation */
		}
	}



/**
  Function : ProgressL
  Description : Cancels the asynchronous operation
  @return : N/A
*/
void CT_MsgFolderFullSync::CancelStep()
	{
	iOperation->Cancel();
	}
