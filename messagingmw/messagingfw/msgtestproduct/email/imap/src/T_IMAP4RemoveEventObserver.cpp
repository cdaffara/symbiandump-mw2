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
// RemoveEventObserver
// [Paramaters]
// ImapAccountName<input>	: Name of the Imap account
// This test step removes an event observer
// [APIs Used]
// NONE
// 
//

//User Includes 
#include "T_IMAP4RemoveEventObserver.h"
#include "T_MsgAsyncStepIMAP.h"
#include <t_utilscentralrepository.h>

// Epoc includes
#include <miutset.h>
#include <imapcmds.h>
#include <mtclbase.h>
#include<msvids.h>

//literals used
_LIT(KAccountName,"ImapAccountName");



/**
  Function : CT_MsgRemoveEventObserver
  Description : Constructor
  @param :CT_MsgSharedDataImap&
  @return : N/A
*/
CT_MsgRemoveEventObserver::CT_MsgRemoveEventObserver(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP),iInboxEntry(NULL),iImapServiceEntry(NULL),iEntrySelection(NULL)
	{
	SetTestStepName(KRemoveEventObserver);
	}



/**
  Function : ~CT_MsgRemoveEventObserver
  Description : Destructor
  @return : N/A
*/
CT_MsgRemoveEventObserver::~CT_MsgRemoveEventObserver()
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

void CT_MsgRemoveEventObserver:: ProgressL(TBool bFinal)
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
 void CT_MsgRemoveEventObserver::CancelStep()
 {
 	
 }


	
/**
  Function : doTestStepL
  Description : Reads the  Imap account name from the ini file.
				& checks from new mail within inbox
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgRemoveEventObserver::doTestStepL()
	{	
	
	INFO_PRINTF1(_L(" Test Step : removing  observer"));
	TPtrC imapAccountName,event;
	
	if(!GetStringFromConfig( ConfigSection(), KAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		}
	

	if( TestStepResult() != EFail )
		{
		//remove observer
		iSharedDataIMAP.iObserver->RemoveObserverEntry();
		}
			
    
		return TestStepResult();	
		
			
}//end function  doTestStepL


