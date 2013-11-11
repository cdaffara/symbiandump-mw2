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
// DoFullImap4Synchronisation
// [Paramaters]
// ImapAccountName		:	Name of the Imap account
// Performs full synchronisation with IMAP4 server.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CImap4ClientMtm::InvokeAsyncFunctionL
// 
//


/* user includes*/
#include "T_DoFullImap4Synchronisation.h"
#include <t_utilscentralrepository.h>

/* epoc includes */
#include <miutset.h>
#include <imapcmds.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KCount,"Count");


/**
  Function : CT_MsgConnectImap4Server
  Description : Constructor
  @param : aSharedDataIMAP		Reference to the CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgDoFullImap4Synchronisation::CT_MsgDoFullImap4Synchronisation(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KDoFullImap4Synchronisation);
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgDoFullImap4Synchronisation 				 
  @return : N/A
*/
CT_MsgDoFullImap4Synchronisation* CT_MsgDoFullImap4Synchronisation::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgDoFullImap4Synchronisation* self = new(ELeave) CT_MsgDoFullImap4Synchronisation(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}

	
/**
  Function : ~CT_MsgDoFullImap4Synchronisation
  Description : Destructor
  @return : N/A
*/
CT_MsgDoFullImap4Synchronisation::~CT_MsgDoFullImap4Synchronisation()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function : ProgressL
  Description : Displays the status of the asynchrous operation
  @return : N/A
*/
void CT_MsgDoFullImap4Synchronisation::ProgressL(TBool bFinal)
	{
	//	TODO
	if ( bFinal )
		{
		/*	Display final progress iOperation */
		}
	else
		{
		/*	Display current progress iOperation*/
		}
	}


/**
  Function : CancelStep
  Description : Cancels the asynchrous operation
  @return : N/A
*/
void CT_MsgDoFullImap4Synchronisation::CancelStep()
	{
	iOperation->Cancel();
	}


/**
  Function : doTestStepL
  Description : The connection to the server is established in the base class
				It performs a full synchronisation with the IMAP server.
				Disconnection is done in the doTestStepPostambleL of the base class
  @return : TVerdict - Test step result
  @leave : KMsvNullIndexEntryId		Invalid IMAP account name specified
*/
TVerdict CT_MsgDoFullImap4Synchronisation::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : DoFullImap4Synchronisation "));
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
		/* Switches the current context to the IMAP Service Id*/
		else
			{
			iSharedDataIMAP.iMtm->SwitchCurrentEntryL(imapServiceId);
			
			/* Creates the selection object */
			CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
			CleanupStack::PushL(selection);
			
			/* Appends the imapServiceId onto the end of the array*/
			selection->AppendL(imapServiceId);
			TBuf8<1> param;
		
			TInt index=0;
			TInt count=1;
			
			/* Reads the count to loop from the ini file */ 
			GetIntFromConfig(ConfigSection(),KCount,count);
			
			CT_MsgActive&	active=Active();
			for( index=0;index<count;index++)
				{
				/* Synchronises the service */
				iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMFullSync, *selection, param, active.iStatus);

				active.Activate();
				CActiveScheduler::Start();
				User::LeaveIfError(active.Result());	

				INFO_PRINTF2(_L("The %d execution of the DoFullSynchronisation"), index);
				delete iOperation;
				iOperation=NULL;
				}
			CleanupStack::PopAndDestroy(selection); /* operation,selection*/
			}
		}
	return TestStepResult();
	}
