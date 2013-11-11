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
// SetEventObserver
// [Paramaters]
// ImapAccountName<input>   :Name of the Imap account
// folderName<input>        :Name of folder to add the event observer to
// parentFolderName<input>  :Observed folders' immediate parent
// RootParentFolderName<input>:Observed folders' root parent
// This test step adds an event observer to the folder specified
// [APIs Used]
// NONE
// 
//

//User Includes
#include "T_IMAP4SetEventObserver.h"
#include "T_MsgAsyncStepIMAP.h"
#include <t_utilscentralrepository.h>

// Epoc includes
#include <miutset.h>//implements common email settings
#include <imapcmds.h>//IMAP4-specific commands that can be issued through CImap4ClientMtm::InvokeAsyncFunctionL(). 
#include <mtclbase.h>//Specifies function IDs for standard MTM functions
#include<msvids.h>// Predefined entry ids

//literals used
_LIT(KAccountName,"ImapAccountName");

_LIT(KFolderName,"FolderName");
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KRootParentFolderName,"RootParentFolderName");
_LIT(KFolderId, "FolderId");


/**
  Function : CT_MsgSetEventObserver
  Description : Constructor
  @param :CT_MsgSharedDataImap&
  @return : N/A
*/
CT_MsgSetEventObserver::CT_MsgSetEventObserver(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP),iInboxEntry(NULL),iImapServiceEntry(NULL),iEntrySelection(NULL)
	{
	SetTestStepName(KSetEventObserver);
	}



/**
  Function : ~CT_MsgSetEventObserver
  Description : Destructor
  @return : N/A
*/
CT_MsgSetEventObserver::~CT_MsgSetEventObserver()
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
  @return :void
*/
void CT_MsgSetEventObserver:: ProgressL(TBool bFinal)
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
 void CT_MsgSetEventObserver::CancelStep()
 {
 	
 }


	
	
/**
  Function : doTestStepL
  Description : Reads the  Imap account name from the ini file.
				& checks from new mail within inbox
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgSetEventObserver::doTestStepL()
	{	
	
INFO_PRINTF1(_L(" Test Step : Set up inbox observer"));
	
	TPtrC imapAccountName;
	
	if(!GetStringFromConfig( ConfigSection(), KAccountName, imapAccountName))
	  {	
	   ERR_PRINTF1(_L("Imap Account Name is not specified"));
       SetTestStepResult(EFail);	
	  }
	

	if( TestStepResult() != EFail )
	{	
  	//give observer the account name
    iSharedDataIMAP.iObserver->SetAccountL((TDes&)imapAccountName);
	//allow observer to set the cmsventry id for service entry
	iSharedDataIMAP.iObserver->SetServiceEntryL(*iSharedDataIMAP.iSession);
	// Reading the name of the remote folder from the ini file
	TPtrC folderName, parentFolderName,RootParentFolderName;
		
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("folder name is not specified"));
		SetTestStepResult(EFail);
		}		
	else if(!GetStringFromConfig( ConfigSection(), KParentFolderName, parentFolderName))
		{
		ERR_PRINTF1(_L("parent folder name is not specified"));
		SetTestStepResult(EFail);
		}
	else if(!GetStringFromConfig( ConfigSection(), KRootParentFolderName, RootParentFolderName))
		{
		ERR_PRINTF1(_L("Root parent folder name is not specified"));
		SetTestStepResult(EFail);
		}
	else 
		{
		//set the observer to specifed folder
		TMsvId folderId=iSharedDataIMAP.iObserver->FindFolderL(*iSharedDataIMAP.iSession,(TDes&)folderName,(TDes&)parentFolderName,(TDes&)RootParentFolderName,iSharedDataIMAP.iObserver->GetServiceIdL());	
		//write the folder id to the file to be use in later test steps 
		// when manipulating the observer
		if(!WriteIntToConfig( ConfigSection(), KFolderId, (TInt)folderId))
			{
		     ERR_PRINTF1(_L("Unable to write folder id to file"));
			 SetTestStepResult(EFail);
			}
				
		if(folderId==KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("folder name not found"));
			SetTestStepResult(EFail);	
			}
				
		INFO_PRINTF2(_L(" The entry Id is %d"),folderId);
			
		}
	}
			
   
	return TestStepResult();	
	
			
}//end function  doTestStepL


