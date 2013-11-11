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
// ConnectAndSyncImap4Server
// [Parameters]
// ImapAccountName   :  Name of the IMAP account
// Time		     	 :	Time interval in seconds
// IsCancel          :  Integer flag to cancel background synchronisation
// This flag is set 1 if background synchronisation is to be 
// cancelled.
// Connects and sync with the IMAP4 Server and waits for background sync to complete.
// Background synchronisation can also be cancelled within this Teststep
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CImap4ClientMtm::InvokeAsyncFunctionL
// 
//



// User Includes
#include "T_ConnectAndSyncImap4Server.h"
#include <t_utilscentralrepository.h>
#include <t_timerutils.h>

// epoc includes
#include <miutset.h>
#include <imapcmds.h>
#include <e32std.h>

// Literals Used
_LIT(KImapAccountName,	"ImapAccountName");
_LIT(KTime,				"Time");
_LIT(KCancelSync, "IsCancel");


/**
  Function : CT_MsgConnectAndSyncImap4Server
  Description : Constructor
  @param :CT_MsgSharedDataImap&
  @return : N/A
*/
CT_MsgConnectAndSyncImap4Server::CT_MsgConnectAndSyncImap4Server(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KConnectAndSyncImapServer);
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgConnectAndSyncImap4Server 				 
  @param :CT_MsgSharedDataImap&
  @return : N/A
*/
CT_MsgConnectAndSyncImap4Server* CT_MsgConnectAndSyncImap4Server::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgConnectAndSyncImap4Server* self = new(ELeave) CT_MsgConnectAndSyncImap4Server(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 
	CleanupStack::Pop(self);
	return self;
	}





/**
  Function : ~CT_MsgConnectAndSyncImap4Server
  Description : Destructor
  @return : N/A
*/
CT_MsgConnectAndSyncImap4Server::~CT_MsgConnectAndSyncImap4Server()
	{
		if(iOperation!=NULL)
		{
		delete iOperation;
		iOperation=NULL;
		}
	}



/**
  Function : ProgressL
  Description : Stub
  @param :TBool
  @return : void
*/
void CT_MsgConnectAndSyncImap4Server::ProgressL(TBool bFinal)
	{

	if ( bFinal )
		{
		//	Display final progress iOperation
		}
	else
		{
		//	Display current progress iOperation
		}
	}



/**
  Function : CancelStep
  Description : Stub
  @return : void
*/
void CT_MsgConnectAndSyncImap4Server::CancelStep()
	{
		if(iOperation!=NULL)
		{					
		iOperation->Cancel();	
		delete iOperation;
	    iOperation=NULL;
		}
	
	}


/**
  Function : doTestStepL
  Description : Reads the  Imap account name from the ini file.
				It establishes connection with the IMAP server and performs 
				a background synchronisation.
  @internalTechnology
  @param :
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgConnectAndSyncImap4Server::doTestStepL()
	{
	
	INFO_PRINTF1(_L(" Test Step : ConnectAndSyncIMAP4Server"));
	TPtrC	imapAccountName;
	
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		}



	if( TestStepResult() == EPass )
		{

		TInt	time = 0;
		GetIntFromConfig( ConfigSection(),KTime,time);
		
		// Retrieving the Imap service Id for the given Imap account
		TMsvId	imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );
		
		// Change the current context
		iSharedDataIMAP.iMtm->SwitchCurrentEntryL(imapServiceId);
		CMsvEntrySelection*	selection = new (ELeave) CMsvEntrySelection;
		CleanupStack::PushL(selection);
		
		// Appends the imapServiceId onto the end of the array
		selection->AppendL(imapServiceId);
		TBuf8<1> param;
		
		// Attempts to connect and sync to the Imap4 Service
		CT_MsgActive&	active=Active();
		iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMConnectAndSynchronise, *selection, param, active.iStatus);
		active.Activate();
		CActiveScheduler::Start();
		
		//Check result of connect and sync operation for errors
		TInt errConnection=active.Result();
		if (errConnection<KErrNone)
		{
		ERR_PRINTF2(_L("Error connecting to service,, failed with %d error"),errConnection);
		SetTestStepResult(EFail);
		}
				
		//Reads Int flag to cancel from ini file if aCancelFlag is 1,
		//then cancel operation, otherwise  ignore 
		TInt aCancelFlag=0;
		GetIntFromConfig(ConfigSection(),KCancelSync,aCancelFlag);
		
		//do we want to cancel the synchronisation?
		if (aCancelFlag==1)
			{	
			delete iOperation;
	    	iOperation=NULL;
	    	User::After(1000000);
	    	TBuf8<16> param2;
			CT_MsgActive&	active2=Active();	
	  		iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMCancelBackgroundSynchronise, *selection, param2, active2.iStatus);
           	active2.Activate();
		   	CActiveScheduler::Start();
		    TInt errCancel=active2.Result();
		    
		    //check result of operation 
		   	//If there is a background synchronisation in progress,
		   	//then stops it and completes with KErrCancel;
		   	//otherwise, completes with KErrNone					     					     	
		    if (errCancel!=KErrCancel && errCancel!=KErrNone)
		     	{
		     	ERR_PRINTF2(_L("Error during cancel background sync, failed with %d error"),errCancel);
				SetTestStepResult(EFail);
			    }
		     				
		    delete iOperation;
	    	iOperation=NULL;
			}
			
		else //Wait for background synchronisation to finish
			{
			      
			TBuf8<16> param3;
			CT_MsgActive&	active3=Active();	
	  		delete iOperation;
	    	iOperation=NULL;
	    	
	  		//Completes (with KErrNone) only when the background synchronisation has finished.
  		    iOperation =iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMWaitForBackground,*selection,param3,active3.iStatus);
		 	INFO_PRINTF1(_L("we are waiting for bk sync to complete  now..."));
    		active3.Activate();
    		CActiveScheduler::Start();
    		
			//Check result of wait for bkground operation for errors
			TInt errSync=active3.Result();
			
			//if result of operation is not KErrNone then fail	
        	if (errSync!=KErrNone)
				{
				ERR_PRINTF2(_L("Error during background sync, failed with %d error"),errSync);
			    SetTestStepResult(EFail);
				}
			
			delete iOperation;
	    	iOperation=NULL;
			
			}
			
		CleanupStack::PopAndDestroy(selection);	//selection
	
		}
		
	return TestStepResult();
	
	
}
