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
// InboxNewEmailsSync
// [Paramaters]
// ImapAccountName		:	Name of the Imap account
// Selects the remote inbox and checks for new mails (but doesn't delete ones that have gone). 
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CImap4ClientMtm::InvokeAsyncFunctionL
// 
//


/* User includes*/
#include "T_InboxNewEmailsSync.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>


/* Epoc includes */
#include <miutset.h>
#include <imapcmds.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");



/**
  Function : CT_MsgInboxNewEmailsSync
  Description : Constructor
  @param : aSharedDataIMAP		Reference to the CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgInboxNewEmailsSync::CT_MsgInboxNewEmailsSync(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KInboxNewEmailsSync);
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgInboxNewEmailsSync 				 
  @return : N/A
*/
CT_MsgInboxNewEmailsSync* CT_MsgInboxNewEmailsSync::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgInboxNewEmailsSync* self = new(ELeave) CT_MsgInboxNewEmailsSync(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 
	CleanupStack::Pop(self);
	return self;
	}

	
/**
  Function : ~CT_MsgInboxNewEmailsSync
  Description : Destructor
  @return : N/A
*/
CT_MsgInboxNewEmailsSync::~CT_MsgInboxNewEmailsSync()
	{
	delete iOperation;
	iOperation=NULL;
	}


/**
  Function : ProgressL
  Description : Displays the status of the asynchrous operation
  @return : N/A
*/
void CT_MsgInboxNewEmailsSync::ProgressL(TBool aFinal)
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
void CT_MsgInboxNewEmailsSync::CancelStep()
	{
	iOperation->Cancel();
	}


/**
  Function : doTestStepL
  Description : Reads the name of the IMAP account
				Selects the remote inbox and checks for new mails (but doesn't delete ones that have gone).
  @return : TVerdict - Test step result
  @leave : KMsvNullIndexEntryId		Invalid IMAP account name specified.
  									Invalid Remote Folder name specified.

*/
TVerdict CT_MsgInboxNewEmailsSync::doTestStepL()
	{	
	INFO_PRINTF1(_L("Test Step : InboxNewEmailsSync "));
	
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
			CMsvEntrySelection*	selection =  new(ELeave) CMsvEntrySelection;
			CleanupStack::PushL(selection);

			/* Appends the imapServiceId to the array */
			selection->Reset();
			selection->AppendL(imapServiceId);
			TBuf8<1> param;
			
			CT_MsgActive& active=Active();
			
			/* 
			 * Selects the remote inbox and checks for new mails
			 * (but doesn't delete ones that have gone). 
			 */		
			iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMInboxNewSync,*selection,param,active.iStatus);
			
			active.Activate();
			CActiveScheduler::Start();
			User::LeaveIfError(active.Result());
		
			delete iOperation;
			iOperation=NULL;
		
			CleanupStack::PopAndDestroy(selection); /* selection*/
			}
		}
	return TestStepResult();
	}
	
					

