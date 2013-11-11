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
// [Test StepName]
// DisConnectImap4Server
// [Parameters]
// ImapAccountName	:	Name of the IMAP account
// DisConnects with the IMAP4 Server
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CImap4ClientMtm::InvokeAsyncFunctionL
// 
//

/* User Includes */
#include "T_DisConnectImap4Server.h"
#include <t_utilscentralrepository.h>


/* epoc includes */
#include <miutset.h>
#include <imapcmds.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KExpectedErr, "ExpectedErr");


/**
  Function : CT_MsgDisConnectImap4Server
  Description : Constructor
  @param : aSharedDataIMAP		Reference to the CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgDisConnectImap4Server::CT_MsgDisConnectImap4Server(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KDisConnectImapServer);
	}
	
	
/**
  Function : NewL
  Description : Creates an object of CT_MsgDisConnectImap4Server 				 
  @param : aSharedDataIMAP		Reference to the CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgDisConnectImap4Server* CT_MsgDisConnectImap4Server::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgDisConnectImap4Server* self = new(ELeave) CT_MsgDisConnectImap4Server(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : ~CT_MsgDisConnectImap4Server
  Description : Destructor
  @return : N/A
*/
CT_MsgDisConnectImap4Server::~CT_MsgDisConnectImap4Server()
	{
	delete iOperation;
	iOperation=NULL;
	}



/**
  Function : ProgressL
  Description : Displays the progress of the asynchronous operation
  @param	:	bFinal	Decides the status of the asynchronous operation
  @return : N/A
*/
void CT_MsgDisConnectImap4Server::ProgressL(TBool bFinal)
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
  Description : Cancels the asynchronous operation
  @param	:
  @return : N/A
*/
void CT_MsgDisConnectImap4Server::CancelStep()
	{
	iOperation->Cancel();
	}


	
/**
  Function : doTestStepL
  Description : DisConnects with the IMAP server.	
  @return : TVerdict - Test step result
*/	
TVerdict CT_MsgDisConnectImap4Server::doTestStepL()
	{	
	INFO_PRINTF1(_L(" Test Step : DisConnectIMAP4Server"));
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
	
		/* Changes the current context and sets entry on the  IMAP Service Id */
		iSharedDataIMAP.iMtm->SwitchCurrentEntryL(imapServiceId);

		/* Creates a selection object */
		CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
		CleanupStack::PushL(selection);

		/* Appends the IMAP Service Id onto the end of the array*/
		selection->AppendL(imapServiceId);

		// Get any expected error
		TInt expectedErr = 0;
		GetIntFromConfig(ConfigSection(), KExpectedErr, expectedErr);

		TBuf8<1> param;

		/* Attempts to connect to the Imap4 Service */
		CT_MsgActive&	active=Active();

		/* Performs the disconnect with the IMAP server */
		iOperation=iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMDisconnect,*selection,param, active.iStatus);

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

		delete iOperation;
		iOperation=NULL;
		CleanupStack::PopAndDestroy(selection); /*operation,selection*/
		}
	return TestStepResult();
	}
