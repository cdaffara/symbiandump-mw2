// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include "T_PerformanceInitialisationClient.h"

CPerformanceInitialisationClient::CPerformanceInitialisationClient(CPerformanceStepBase& aTestStep) 
: CActive(EPriorityStandard), iTestStep(aTestStep) 
	{
	CActiveScheduler::Add(this);
	}

CPerformanceInitialisationClient* CPerformanceInitialisationClient::NewL(CPerformanceStepBase& aTestStep)
	{
	CPerformanceInitialisationClient* self = NewLC(aTestStep);
	CleanupStack::Pop(self);
	return self;
	}

CPerformanceInitialisationClient* CPerformanceInitialisationClient::NewLC(CPerformanceStepBase& aTestStep)
	{
	CPerformanceInitialisationClient* self = new(ELeave) CPerformanceInitialisationClient(aTestStep);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	
CPerformanceInitialisationClient::~CPerformanceInitialisationClient()
	{
	Cancel();
	delete iTestActive;
	delete iEntrySelectionToSend;
	delete iOperation;
	delete iInboxEntry;
	delete iChildren;
	delete iMsvOperation;	
	delete iSelection;
	delete iClientMtm;
	delete iClientRegistry;	
	delete iSession;
	delete iSessionObserver;
	}

void CPerformanceInitialisationClient::ConstructL()
	{
	//open the imap and smtp accounts that the test step has created
	CEmailAccounts* emailAccounts;
	emailAccounts = CEmailAccounts::NewLC();	
	RArray<TImapAccount> imapAccounts;
	CleanupClosePushL(imapAccounts);
	emailAccounts->GetImapAccountsL(imapAccounts);
	iImapAccount=imapAccounts[0];
	//smtp account
	RArray<TSmtpAccount> smtpAccounts;
	CleanupClosePushL(smtpAccounts);
	emailAccounts->GetSmtpAccountsL(smtpAccounts);
	iSmtpAccount=smtpAccounts[0];
	CleanupStack::PopAndDestroy(3,emailAccounts);
	
	// Session observer. Needed to create a session
	iSessionObserver = new (ELeave) TDummySessionObserver;
	// Session. Needed to create a client registry.
	iSession=CMsvSession::OpenSyncL(*iSessionObserver);
	// Client registry. Needed to get the MTM component
	iClientRegistry=CClientMtmRegistry::NewL(*iSession,KMsvDefaultTimeoutMicroSeconds32);
	//get the client mtm
	iClientMtm=iClientRegistry->NewMtmL(KUidMsgTypeIMAP4);	
	//used for asynchronous operation
	iTestActive=new(ELeave) CTestActive;
	
	// Load the serial comms device drivers.  If this is not done,
	// connecting via NT-RAS returns KErrNotFound (-1).
	TInt driverErr;
	driverErr=User::LoadPhysicalDevice(PDD_NAME);
	if (driverErr!=KErrNone && driverErr!=KErrAlreadyExists)
		{
		User::Leave(driverErr);	
		}
	driverErr=User::LoadLogicalDevice(LDD_NAME);
	if (driverErr!=KErrNone && driverErr!=KErrAlreadyExists)
		{
		User::Leave(driverErr);	
		}
		
	//initialise package object
	TImImap4GetMailInfo imap4GetMailInfo;
	imap4GetMailInfo.iMaxEmailSize = KMaxTInt;
	imap4GetMailInfo.iDestinationFolder = KMsvGlobalInBoxIndexEntryIdValue;
	imap4GetMailInfo.iGetMailBodyParts = EGetImap4EmailBodyTextAndAttachments;
	iPackage=imap4GetMailInfo;
	}
	
void CPerformanceInitialisationClient::DoCancel()
	{	
	__ASSERT_DEBUG(iMsvOperation!=NULL, User::Invariant());
	iMsvOperation->Cancel();
	}

void CPerformanceInitialisationClient::CompleteSelf()
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}
	
void CPerformanceInitialisationClient::CreateMessageL(const TMsvId aTMsvId,TInt aCount,TDesC& aBody,TPtrC& aAddress,TDesC& aSubject,TDesC& aAttachment1,TDesC& aAttachment2,TDesC& aAttachment3)
	{
	
	CParaFormatLayer* paraFormatLayer=CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer=CCharFormatLayer::NewL(); 
	CleanupStack::PushL(charFormatLayer);
	
	CRichText* bodyText=CRichText::NewL(paraFormatLayer, charFormatLayer, CEditableText::EFlatStorage, 256);
	CleanupStack::PushL(bodyText);	

	if(aBody!=KNullFileName)
		{
		//read body text form a file
		RFs fsSession;              
		User::LeaveIfError(fsSession.Connect()); 
		RFile infile; 
	 	User::LeaveIfError(infile.Open(fsSession, aBody, EFileRead));
		TInt size;
		TInt err=infile.Size(size);
		 
		HBufC8* filebuf;
		filebuf = HBufC8::NewLC(size);  
		TPtr8 ptr8=filebuf->Des();	  
		infile.Read(ptr8);
		HBufC* filebuf16;
		filebuf16 = HBufC::NewLC(size);
		TPtr ptr16=filebuf16->Des();
		
		ptr16.Copy(ptr8);
		//insert the body text
		bodyText->InsertL(0, ptr16);
		
		infile.Close();
		fsSession.Close();	

		CleanupStack::PopAndDestroy(2,filebuf); 
		}
	
	
	for(TInt i=0;i<aCount;i++)
		{
		//create the message
		TMsvPartList partList = KMsvMessagePartBody;
		CImEmailOperation* emailOp = CImEmailOperation::CreateNewL(iTestActive->iStatus,*iSession,aTMsvId,partList,0,KUidMsgTypeSMTP); 
		CleanupStack::PushL(emailOp);
			
		// Active object which stops the active scheduler
		iTestActive->StartL(); 
		CActiveScheduler::Start();
			
		// Get the new message id.
		TMsvId messageId;
		TPckg<TMsvId> param(messageId);
		param.Copy(emailOp->FinalProgress());
		CleanupStack::PopAndDestroy(emailOp);
		
		// Save the message body text
		TMsvSelectionOrdering ordering;	
		CMsvEntry* entry = CMsvEntry::NewL(*iSession, messageId,ordering);
		CleanupStack::PushL(entry);
		CImEmailMessage* message = CImEmailMessage::NewL(*entry);
		CleanupStack::PushL(message); 
		
		//add attachments
		if(aAttachment1!=KNullFileName)
			{
			CMsvAttachment* attachmentInfo = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
			CleanupStack::PushL(attachmentInfo);
			message->AttachmentManager().AddAttachmentL(aAttachment1, attachmentInfo, iTestActive->iStatus);
			CleanupStack::Pop(attachmentInfo);	
			iTestActive->StartL();
			CActiveScheduler::Start();	// wait for the asynch operation to complete
			}
		
		if(aAttachment2!=KNullFileName)
			{
			CMsvAttachment* attachmentInfo = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
			CleanupStack::PushL(attachmentInfo);
			message->AttachmentManager().AddAttachmentL(aAttachment2, attachmentInfo, iTestActive->iStatus);
			CleanupStack::Pop(attachmentInfo);	
			iTestActive->StartL();
			CActiveScheduler::Start();	// wait for the asynch operation to complete
			}
		
		if(aAttachment3!=KNullFileName)
			{
			CMsvAttachment* attachmentInfo = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
			CleanupStack::PushL(attachmentInfo);
			message->AttachmentManager().AddAttachmentL(aAttachment3, attachmentInfo, iTestActive->iStatus);
			CleanupStack::Pop(attachmentInfo);	
			iTestActive->StartL();
			CActiveScheduler::Start();	// wait for the asynch operation to complete
			}

		iTestActive->StartL();
		message->StoreBodyTextL(messageId, *bodyText, iTestActive->iStatus);
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(message); 
		
		// fill in header details for email message
		CMsvStore* store = 	entry->EditStoreL();
		CleanupStack::PushL(store);
		CImHeader* header = CImHeader::NewLC();
		header->RestoreL(*store);
		header->ToRecipients().AppendL(aAddress);
		header->SetSubjectL(aSubject);	
		header->StoreL(*store);
	
		store->CommitL();	
	 	CleanupStack::PopAndDestroy(3,entry); 
		}
		
	CleanupStack::PopAndDestroy(3,paraFormatLayer); 

	}
	
void CPerformanceInitialisationClient::DeleteRemoteMessagesL()
	{
	iState=EConnectAndSync;
	CompleteSelf();	
	}
	
void CPerformanceInitialisationClient::UploadMessagesL()
	{
	iState=EUploadMessages;
	CompleteSelf();		
	}

void CPerformanceInitialisationClient::RunL()
	{

	User::LeaveIfError(iStatus.Int());
	CMsvEntry* imapService=NULL;
	TMsvSelectionOrdering ordering;
	TPckg<MMsvImapConnectionObserver*> param(this);
	CMsvEntry* entry=NULL;
	TBuf8<4> buf;
	
	switch(iState)
		{
		
		case EConnectAndSync:
			iTestStep.INFO_PRINTF1(_L("Intialisation - Connect and sync..."));
			iSelection = new (ELeave) CMsvEntrySelection;
			//select the imap service entry
			iSelection->AppendL(iImapAccount.iImapService);
			//make the service entry the current context
			iClientMtm->SwitchCurrentEntryL(iImapAccount.iImapService);
			//sync the account
			iState=EWait;
			iMsvOperation = iClientMtm->InvokeAsyncFunctionL(KIMAP4MTMConnectAndSyncCompleteAfterFullSync,*iSelection,param,iStatus);			
			SetActive();
			break;
			
		case EWait:
			delete iMsvOperation;
			iMsvOperation=NULL;
			iMsvOperation = iClientMtm->InvokeAsyncFunctionL(KIMAP4MTMWaitForBackground,*iSelection,buf,iStatus);	
			SetActive();
			iState=EDeleteRemoteEntries;
			break;	
			
		case EDeleteRemoteEntries:
			iTestStep.INFO_PRINTF1(_L("Intialisation - Deleting remote entries..."));
			// delete all messages in the Inbox
			imapService = CMsvEntry::NewL(*iSession, iImapAccount.iImapService,ordering);
			CleanupStack::PushL(imapService);
			//retrieve the inbox from the imap service entry
			CMsvEntrySelection* msvEntrySelection;
			msvEntrySelection=imapService->ChildrenL();
			CleanupStack::PushL(msvEntrySelection);
			//create a CMsvEntry for the inbox entry
			iInboxEntry = CMsvEntry::NewL(*iSession, (*msvEntrySelection)[0],ordering);
			CleanupStack::PopAndDestroy(2,imapService);
			
			iChildren = iInboxEntry->ChildrenL();
			if(iChildren->Count()>0)
				{
				iInboxEntry->DeleteL(*iChildren,iStatus);
				SetActive();	
				}
			else
				{
				CompleteSelf();		
				}
			
			iState=EDisconnect;
			break;
		
		case EUploadMessages:
			iTestStep.INFO_PRINTF1(_L("Intialisation - Uploading messages..."));
			//select the message in the outbox
			ordering.SetShowInvisibleEntries(ETrue);
			entry = CMsvEntry::NewL(*iSession,KMsvGlobalOutBoxIndexEntryIdValue,ordering);
			CleanupStack::PushL(entry);
    		entry->SetEntryL(KMsvGlobalOutBoxIndexEntryIdValue);
    
			iEntrySelectionToSend=entry->ChildrenL();	
    		//copy the message to the smtp service
  			iOperation = entry->CopyL(*iEntrySelectionToSend,iSmtpAccount.iSmtpService,iStatus);
			CleanupStack::PopAndDestroy(1,entry); 
			SetActive();	
			iState=EComplete;
			break;
				
		case EDisconnect:
			iTestStep.INFO_PRINTF1(_L("Intialisation - Disconnecting..."));
			delete iMsvOperation;
			iMsvOperation=NULL;
			iMsvOperation = iClientMtm->InvokeAsyncFunctionL(KIMAP4MTMDisconnect,*iSelection,buf,iStatus);	
			SetActive();
			iState=EComplete;
			break;	
			
		case EComplete:
			User::After(10000000);
			iTestStep.INFO_PRINTF1(_L("EComplete"));
			//inform the owning object that the process is complete
			iTestStep.InitialisationComplete(KErrNone);
			break;
			
		default:
			__ASSERT_DEBUG(0, User::Panic(_L("CPerformanceInitialisationClient unknown state"), KErrUnknown));
			break;			
		}

	}
	
TInt CPerformanceInitialisationClient::RunError(TInt aError)
	{
	iTestStep.InitialisationComplete(aError);
	return KErrNone;
	}
	
void CPerformanceInitialisationClient::HandleImapConnectionEvent(TImapConnectionEvent /*aConnectionState*/)
	{
	//this method does nothing
	}



