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
// CheckIfConnected
// [Paramaters]
// ImapAccountName<input>	: Name of the Imap account
// Checks if there is a connection to the  IMAP4 Server
// [APIs Used]
// CImap4ClientMtm::InvokeAsyncFunctionL
// CMsvEntrySelection::AppendL
// 
//

//User Includes
#include "T_IMAP4CheckIfConnected.h"
#include "T_MsgAsyncStepIMAP.h"
#include <t_utilscentralrepository.h>

// Epoc includes
#include <miutset.h>
#include <imapcmds.h>
#include <mtclbase.h>

_LIT(KAccountName,"ImapAccountName");


/**
  Function : CT_MsgCheckIfConnected
  Description : Constructor
  @param : CT_MsgSharedDataImap&
  @return : N/A
*/
CT_MsgCheckIfConnected::CT_MsgCheckIfConnected(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCheckIfConnected);
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgCheckIfConnected				 
  @param : CT_MsgSharedDataImap&
  @return :CT_MsgCheckIfConnected*
*/
CT_MsgCheckIfConnected* CT_MsgCheckIfConnected::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgCheckIfConnected* self = new(ELeave) CT_MsgCheckIfConnected(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_MsgAsyncStep::ConstructL()	
	CleanupStack::Pop(self);
	return self;
	}
	
	



/**
  Function : ~CT_MsgCheckIfConnected
  Description : Destructor
  @return : N/A
*/
CT_MsgCheckIfConnected::~CT_MsgCheckIfConnected()
	{
	}
	
	
/**
  Function : ProgressL
  Description : Stub
  @param :TBool
  @return : N/A
*/
void CT_MsgCheckIfConnected:: ProgressL(TBool bFinal)
{
	if (bFinal)
	{
		
	}

}


/**
  Function : CancelStep
  Description : Stub
  @return : N/A
*/
 void CT_MsgCheckIfConnected::CancelStep()
 {
 	
 }



/**
  Function : doTestStepL
  Description : Reads the  Imap account name from the ini file.
		& checks if a connection exsists to the IMAP Server

  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCheckIfConnected::doTestStepL()
	{	
	INFO_PRINTF1(_L(" Test Step : CheckIfConnected"));
	TPtrC imapAccountName;
	
	if(!GetStringFromConfig( ConfigSection(), KAccountName, imapAccountName))
	  {
	  ERR_PRINTF1(_L("Imap Account Name is not specified"));
	  SetTestStepResult(EFail);
	  }
	
	if( TestStepResult() != EFail )
	  {
	  // Retrieving the Imap service Id for the given Imap account
	  TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
	  INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );
	  // Change the current context
	  iSharedDataIMAP.iMtm->SwitchCurrentEntryL(imapServiceId);
	  CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	  CleanupStack::PushL(selection);	
	  // Appends the imapServiceId onto the end of the array
	  selection->AppendL(imapServiceId);	
	  TBuf8<16> param;
	  INFO_PRINTF1(_L("Are we connected to the IMAP service? \n Checking now..."));
	  iSharedDataIMAP.iMtm->InvokeSyncFunctionL(KIMAP4MTMIsConnected,*selection,param);
	  TInt err = param[0];
   	  if(err != KErrNone)
   		{
   		ERR_PRINTF1(_L("Warning-No connection found!!"));
		SetTestStepResult(EFail);
   		}
   else
   		{
   		INFO_PRINTF1(_L("A Connection found!!"));
		SetTestStepResult(EPass);
   		}
			
		CleanupStack::PopAndDestroy(1,selection); //selection
		
	   }
		
	   INFO_PRINTF1(_L("Successful completetion of TestStep:'CheckIfConnected"));
	   return TestStepResult();
			
			
		
		
	}



