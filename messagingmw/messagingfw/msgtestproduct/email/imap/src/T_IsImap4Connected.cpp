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
// IsImap4Connected
// [Paramaters]
// ImapAccountName :  The name of the IMAP account.	  
// Connected       :  The state to test (0 = not connected, 1 = connected)
// [Test Step Description]
// Checks whether the MTM is connected or not connected to an external server
// [APIs Used]
// none
// 
//

/* User includes */
#include "T_IsImap4Connected.h"
#include <t_utilscentralrepository.h>


/* Epoc includes */
#include <imapcmds.h>


/* Literals used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KConnected,"Connected");


/**
  Function : CT_MsgIsImap4Connected
  Description : Constructor 
  @param : aSharedDataIMAP		Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgIsImap4Connected::CT_MsgIsImap4Connected(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KIsImap4Connected);
	}


/**
  Function : doTestStepL
  Description : Reads the time value from the ini file and
				requests an event after the time interval given.
  @return : TVerdict TestStep result			
*/
TVerdict CT_MsgIsImap4Connected::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: Imap4Connected"));
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
	
		TInt connected;
		if(!GetIntFromConfig( ConfigSection(), KConnected, connected))
			{
			ERR_PRINTF1(_L("Boolean state is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			TInt  expected = KErrNone;
			
			/* 
			 * If the parameter 0 is mentioned in the ini file
			 * the MTM is expected not to be connected to the server
			 */
			if(connected == 0)
			expected = KErrDisconnected;

			/* Creates a selection object */
			CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
			CleanupStack::PushL(selection);
			
			/* Appends the service ID */
			selection->AppendL(imapServiceId);
			TBuf8<16> param;
		
			/* 
			 * Invokes synchronous protocol-specific operations.
			 * Tests if the service is connected.
			 * Completes with either KErrNone if connected or KErrDisconnected if not. 
			 */
			iSharedDataIMAP.iMtm->InvokeSyncFunctionL(KIMAP4MTMIsConnected, *selection, param);
			
			CleanupStack::PopAndDestroy(selection);	
			
			TInt err = param[0];
		
			if((err != KErrNone && connected != 0 ) || (err == KErrNone && connected == 0 ))
			{
			ERR_PRINTF3( _L("Imap4Connected err(%d) != expected(%d)"), err, expected );
			SetTestStepResult(EFail);
			}
		}
	}
	return TestStepResult();
	}
