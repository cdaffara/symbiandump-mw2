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
// ConnectImap4Server
// [Paramaters]
// ImapAccountName			<input>	: Name of the Imap account
// Connects to the IMAP4 Server
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CImap4ClientMtm::InvokeAsyncFunctionL
// 
//

/* User Includes */
#include "T_ConnectImap4Server.h"
#include <t_utilscentralrepository.h>
#include "T_MsgAsyncStepIMAP.h"



/* Epoc includes */
#include <miutset.h>
#include <imapcmds.h>
#include <mtclbase.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KExpectedErr, "ExpectedErr");



/**
  Function : CT_MsgConnectImap4Server
  Description : Constructor 
  @param : aSharedDataIMAP		Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgConnectImap4Server::CT_MsgConnectImap4Server(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KConnectImap4Server);
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgConnectImap4Server 				 
  @param :		aSharedDataIMAP		Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgConnectImap4Server* CT_MsgConnectImap4Server::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgConnectImap4Server* self = new(ELeave) CT_MsgConnectImap4Server(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					
	CleanupStack::Pop(self);
	return self;
	}


/**
  Function : ~CT_MsgConnectImap4Server
  Description : Destructor
  @return : N/A
*/
CT_MsgConnectImap4Server::~CT_MsgConnectImap4Server()
	{
	delete iOperation;
	iOperation=NULL;
	}


/**
  Function : doTestStepL
  Description : Reads the  Imap account name from the ini file.
				It establishes connection with the IMAP server.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgConnectImap4Server::doTestStepL()
	{	
	INFO_PRINTF1(_L(" Test Step : ConnectIMAP4Server"));
	/* Reads the name of the account from the ini file */
	TPtrC	imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		/* Retrieves the Imap service Id for the given Imap account*/
		TMsvId	imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );

		/* Changes the current context and switches to the IMAP service Id*/
		iSharedDataIMAP.iMtm->SwitchCurrentEntryL(imapServiceId);
		
		/* Creates a selection object*/
		CMsvEntrySelection*	selection = new (ELeave) CMsvEntrySelection;
		CleanupStack::PushL(selection);

		/* Appends the IMAP ServiceId onto the end of the selection */
		selection->AppendL(imapServiceId);

		// Get any expected error
		TInt expectedErr = 0;
		GetIntFromConfig(ConfigSection(), KExpectedErr, expectedErr);

		TBuf8<1>	param;

		/* Attempts to connect to the IMAP server*/
		CT_MsgActive&	active=Active();
		delete iOperation;
		iOperation=NULL;
		/* Attempts to establish connection with the IMAP server */
		iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMConnect, *selection,param, active.iStatus);

		active.Activate();
		CActiveScheduler::Start();

		if ((expectedErr != KErrNone) || (active.Result() != KErrNone))
			{
			if(active.Result() == expectedErr)
				{
				INFO_PRINTF1(_L("Failed with Expected Error"));
				SetTestStepResult(EPass);
				}
			else
				{
				ERR_PRINTF2(_L("Failed with %d error"), active.Result());
				SetTestStepResult(EFail);
				}
			}
		SetTestStepError(active.Result());
		CleanupStack::PopAndDestroy(selection); /* selection */
		}
		
	return TestStepResult();
	}



/**
  Function : ProgressL
  Description : Displays the progress information of the asynchronous operation
  @return : N/A
*/
void CT_MsgConnectImap4Server::ProgressL(TBool bFinal)
	{
	//	TODO
	if ( bFinal )
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
void CT_MsgConnectImap4Server::CancelStep()
	{
	iOperation->Cancel();
	}
