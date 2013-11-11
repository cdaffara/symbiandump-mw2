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
// CheckEventNCount
// [Paramaters]
// ImapAccountName<input>	: Name of the Imap account
// event<input>			: Event type.
// Can be one of three types:
// "newChildren","deletedChildren","movedChildren".	
// expectedNum<input>		: The total number of entries expected to be effected
// This test step 
// [APIs Used]
// NONE
// 
//

//User Includes 
#include "T_IMAP4ObserverCkEventNCount.h"
#include "T_MsgAsyncStepIMAP.h"
#include <t_utilscentralrepository.h>

// Epoc includes
#include <miutset.h>//implements common email settings
#include <imapcmds.h>//IMAP4-specific commands that can be issued through CImap4ClientMtm::InvokeAsyncFunctionL(). 
#include <mtclbase.h>//Specifies function IDs for standard MTM functions
#include<msvids.h>// Predefined entry ids

//literals used
_LIT(KAccountName,"ImapAccountName");
_LIT(KNumExpected,"NumberExpected");
_LIT(KEventName, "EventName");

/**
  Function : CT_MsgObserverCkEventNCount
  Description : Constructor
  @param :CT_MsgSharedDataImap&
  @return : N/A
*/
CT_MsgObserverCkEventNCount::CT_MsgObserverCkEventNCount(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP),iInboxEntry(NULL),iImapServiceEntry(NULL),iEntrySelection(NULL)
	{
	SetTestStepName(KCheckEventNCount);
	}



/**
  Function : CT_MsgObserverCkEventNCount
  Description : Destructor
  @return : N/A
*/
CT_MsgObserverCkEventNCount::~CT_MsgObserverCkEventNCount()
	{
	 
	
 	delete iInboxEntry;
 	iInboxEntry=NULL;
 	
 	delete iImapServiceEntry;
	iImapServiceEntry=NULL;
	
	delete iEntrySelection;
	iEntrySelection=NULL;
	


	}



/**
  Function : ProgressL
  Description : Stub
  @param :TBool
  @return : void
*/

void CT_MsgObserverCkEventNCount:: ProgressL(TBool bFinal)
{
	if (bFinal)
	{
		
	}

}


/**
  Function : CancelStep
  Description : Stub
  @return : void
*/

 void CT_MsgObserverCkEventNCount::CancelStep()
 {
 	
 }


	
/**
  Function : doTestStepL
  Description : Reads the  Imap account name from the ini file.
		        Reads the event type from the ini file.
		        And requests what msgs have been affected by that
		   		event, within the folder that this observer is watching.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgObserverCkEventNCount::doTestStepL()
	{	
	
	INFO_PRINTF1(_L(" Test Step : Checking event type and total number of msgs effected form observer"));
	TPtrC imapAccountName,event;
	TInt expectedNum;
	
        //retrieve account name from ini
	if(!GetStringFromConfig( ConfigSection(), KAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		}
	

	if( TestStepResult() != EFail )
		{
	     //retrieve event expected from ini 
	    if(!GetStringFromConfig( ConfigSection(), KEventName, event))
			{
			ERR_PRINTF1(_L("Event type not specified"));
			SetTestStepResult(EFail);
			}
		//ask observer how many entries have been affected by that event
		TInt actualNum=iSharedDataIMAP.iObserver->CheckEventAndCountL((TDesC&)event);
		INFO_PRINTF2(_L(" Total number of msgs effected are: %d"),actualNum);
				
		if(!GetIntFromConfig( ConfigSection(), KNumExpected, expectedNum))
			{
			ERR_PRINTF1(_L("Expected count not specified!"));
			}
			
		if (actualNum!=expectedNum)
			{
			ERR_PRINTF1(_L("Actual number of msgs effected by event does not equal to number expected! "));
			SetTestStepResult(EFail);
			}
		
		}
			
    
		return TestStepResult();	
		
			
}//end function  doTestStepL


