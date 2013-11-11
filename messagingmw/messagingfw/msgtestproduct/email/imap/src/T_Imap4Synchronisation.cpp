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
// Imap4Synchronisation
// [Paramaters]
// ImapAccountName		:	Name of the Imap account
// FolderName			:   Name of the remote folder which needs to be synced.
// ParentFolderName		:   Name of the parent folder.
// Synchronises the current folder that is selected on the remote IMAP server.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CImap4ClientMtm::InvokeAsyncFunctionL
// 
//


/* user includes*/
#include "T_Imap4Synchronisation.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>


/* epoc includes */
#include <imapcmds.h>
#include <imapset.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KFolderName,"FolderName");



/**
  Function : CT_MsgImap4Synchronisation
  Description : Constructor
  @param : aSharedDataIMAP		Reference to the CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgImap4Synchronisation::CT_MsgImap4Synchronisation(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KImap4Synchronisation);
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgImap4Synchronisation 				 
  @return : N/A
*/
CT_MsgImap4Synchronisation* CT_MsgImap4Synchronisation::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgImap4Synchronisation* self = new(ELeave) CT_MsgImap4Synchronisation(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 
	CleanupStack::Pop(self);
	return self;
	}

	
/**
  Function : ~CT_MsgImap4Synchronisation
  Description : Destructor
  @return : N/A
*/
CT_MsgImap4Synchronisation::~CT_MsgImap4Synchronisation()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function : ProgressL
  Description : Displays the status of the asynchrous operation
  @return : N/A
*/
void CT_MsgImap4Synchronisation::ProgressL(TBool aFinal)
	{
	if ( aFinal )
		{
		}
	else
		{
		}
	}


/**
  Function : CancelStep
  Description : Cancels the asynchrous operation
  @return : N/A
*/
void CT_MsgImap4Synchronisation::CancelStep()
	{
	iOperation->Cancel();
	}


/**
  Function : doTestStepL
  Description : Reads the name of the IMAP account and the folder name
				to be synchronised from the ini file.
				Synchronises the current folder that is selected on the remote server.
  @return : TVerdict - Test step result
  @leave : KMsvNullIndexEntryId		Invalid IMAP account name specified.
  									Invalid Remote Folder name specified.

*/
TVerdict CT_MsgImap4Synchronisation::doTestStepL()
	{	
	INFO_PRINTF1(_L("Test Step : Imap4Synchronisation "));
	/* Reads the name of the IMAP account */
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		/* Retrieves the Imap service Id for the given Imap account */
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );
		
		/* Fails the test if the account name is invalid */
		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name specified"));
			SetTestStepResult(EFail);
			}
		else
			{			
			/* Create a selection object */
			CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
			CleanupStack::PushL(selection);

			/* Append the folder Id to the selection object created */
			selection->AppendL(imapServiceId);
			TBuf8<1> param;
			
			CT_MsgActive& active=Active();
			delete iOperation;
			iOperation=NULL;
			
			/* Synchronises the current folder that is selected on the remote server */		
			iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMSynchronise,*selection,param,active.iStatus);
			
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
		}
	return TestStepResult();
	}

	
					

