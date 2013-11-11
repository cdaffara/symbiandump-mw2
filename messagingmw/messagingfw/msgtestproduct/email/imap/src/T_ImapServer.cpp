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

/* User includes */
#include "T_MsgServer.h"
#include "T_ImapServer.h"
#include "T_MsgSharedDataImap.h"

/* Header files of Test Steps IMAP */
#include "T_CreateImapAccount.h"
#include "T_DeleteImapAccount.h"
#include "T_VerifyImapAccountsCount.h"
#include "T_VerfiyImapSettings.h"
#include "T_CompareImapSettings.h" 
#include "T_ModifyImapSettings.h"
#include "T_ConnectImap4Server.h"
#include "T_DisConnectImap4Server.h"
#include "T_DeleteAllImapChildren.h"
#include "T_DoFullImap4Synchronisation.h"
#include "T_Imap4Synchronisation.h"
#include "T_SetImap4FullDownloadLimits.h"
#include "T_SetImap4PartialDownloadLimits.h"
#include "T_SetImap4ServiceIdleParameters.h"
#include "T_CopyImapSelection.h"
#include "T_ImapInvokeAsyncFunction.h"
#include "T_FindEntryByName.h"
#include "T_CompareEmailMessages.h"
#include "T_CheckChildrenCountImap.h"
#include "T_ConnectAndSyncImap4Server.h"
#include "T_CheckChildrenCountFlagBase.h"
#include "T_DeleteMessage.h"
#include "T_CopyMessage.h"
#include "T_RenameImapFolder.h"
#include "T_SelectImapFolder.h"
#include "T_CancelOfflineOperations.h"
#include "T_CreateImap4Folder.h"
#include "T_DeleteImap4Folder.h"
#include "T_CopyImapSelectionRemote.h"
#include "T_ChangeImap4SubAndSyncSettings.h"
#include "T_CheckImap4Subscription.h"
#include "T_SubscribeImap4Folder.h"
#include "T_UnsubscribeImap4Folder.h"
#include "T_IMAP4CheckIfConnected.h"
#include "T_MoveMessage.h"
#include "T_UnDeleteAll.h"
#include "T_InitialiseImap4Account.h"
#include "T_SetImap4EmailPriority.h"
#include "T_CheckImap4EmailPriority.h"
#include "T_IMAP4ObserverCkEventNCount.h"
#include "T_IMAP4SetEventObserver.h"
#include "T_IMAP4RemoveEventObserver.h"
#include "T_IMAP4MoveAsyncFunction.h"
#include "T_IMAP4CopyAsyncFunction.h"
#include "T_FolderFullSync.h"
#include "T_InboxNewEmailsSync.h"
#include "T_DeleteAllMessage.h"
#include "T_IsImap4Connected.h"
#include "T_SetUnReadFlag.h"
#include "T_StartRamUsageTimer.h"
#include "T_StopRamUsageTimer.h"
#include "T_ChangeImap4FolderSyncSettings.h"
#include "T_CreateImapAccountWithoutSmtp.h"
#include "T_CheckImapSNAPSetting.h"
#include "T_RemoveImapSNAPSetting.h"
#include "T_CheckImapBMSetting.h"
#include "T_CreateImapAccountWithDLRules.h"
#include "T_CheckImapSyncDLRules.h"
#include "T_ModifyImapSyncDLRules.h"
#include "T_DeleteImapSyncDLRules.h"
#include "T_CheckImapDLRulesSetting.h"
#include "T_AddImapSyncDLRules.h"
#include "T_AddImap4TransportBufferSizes.h"
#include "T_CheckImap4AccessPoint.h"
#include "T_IsImapMessageHeaderOnly.h"

#include "T_ChangeToExternalDrive.h"
#include "T_UpdateToExternalDrive.h"
#include "T_CopyStoreImapToRemoteDrive.h"
#include "T_DeleteStoreOfRemoteDrive.h"
#include "T_ChangeToInternalDrive.h"
#include "T_InsertMedia.h"
#include "T_RemoveMedia.h"
#include "T_CopyMsgsToExternalDrive.h"
#include "t_cimap4clientmtm.h"
#include "t_verifyheaders.h"

LOCAL_C void MainL()
/**
 * Secure variant
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
	{
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif

	CActiveScheduler*	sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CT_MsgImapServer* server = NULL;

	/* Creates the CT_MsgServer derived IMAP server*/
	TRAPD(err, server = CT_MsgImapServer::NewL());
	if ( err == KErrNone )
		{
		/* Syncs with the client and enter the active scheduler */
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}

	/*delete the servers*/
	delete server;
	delete sched;
	}


/**
  E32Main()
  Description :	It is the entry point 
  @return	:	Returns the error code
*/
GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on process exit
 * Secure variant only
 * Process entry point. Called by client using RProcess API
 */
	{
	__UHEAP_MARK;
	
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAP_IGNORE(MainL());	

	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
    }
 
   
/**
  NewL()
  Description :		Constructs a CT_MsgImapServer object.
					Uses two phase construction and leaves nothing on the CleanupStack.
  @return	:		Created object of type CT_MsgImapServer
*/
CT_MsgImapServer* CT_MsgImapServer::NewL()
	{
	CT_MsgImapServer*	self=new(ELeave) CT_MsgImapServer();
	CleanupStack::PushL(self);
	self->ConstructL(KImapServer);
	CleanupStack::Pop(self);
	return self;
	}



/**
  Function : CT_MsgImapServer
  Description : Constructor
  @return : N/A
*/
CT_MsgImapServer::CT_MsgImapServer()
:	iSharedDataIMAP(NULL)
	{
	}


/**
  Function : NewSharedDataL
  Description : Creates an object of Shared Data
  @return : A pointer of type CT_MsgSharedDataBase
*/
CT_MsgSharedDataBase* CT_MsgImapServer::NewSharedDataL()
	{
	return iSharedDataIMAP = CT_MsgSharedDataImap::NewL();
	}



/**
  Function : CreateTestStepL
  Description : Creates the test steps based on the name read from the script file
  @param :	aStepName	The name of the test step to be created
  @return : The CTestStep object created
*/ 
CTestStep* CT_MsgImapServer::CreateTestStepL(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;

	 if(aStepName == KCreateImapAccount)
		{
		testStep = new(ELeave) CT_MsgCreateImapAccount(*iSharedDataIMAP);
		}
	else if(aStepName == KDeleteImapAccount)
		{
		testStep = new(ELeave) CT_MsgDeleteImapAccount(*iSharedDataIMAP);
		}
	else if(aStepName == KModifyImapSettings)
		{
		testStep = new(ELeave) CT_MsgModifyImapSettings(*iSharedDataIMAP);
		}
	else if(aStepName == KCompareImapSettings)
		{
		testStep = new(ELeave) CT_MsgCompareImapSettings(*iSharedDataIMAP);
		}
	else if(aStepName == KVerifyImapSettings)
		{
		testStep = new(ELeave) CT_MsgVerifyImapSettings(*iSharedDataIMAP);
		}
	else if(aStepName == KVerifyImapAccountsCount)
		{
		testStep = new(ELeave) CT_MsgVerifyImapAccountsCount(*iSharedDataIMAP);
		}
	else if(aStepName == KConnectImap4Server)
		{
		testStep = CT_MsgConnectImap4Server::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KDisConnectImapServer)
		{
		testStep = CT_MsgDisConnectImap4Server::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KDeleteAllImapChildren)
		{
		testStep = CT_MsgDeleteAllImapChildren::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KDoFullImap4Synchronisation)
		{
		testStep = CT_MsgDoFullImap4Synchronisation::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KImap4Synchronisation)
		{
		testStep = CT_MsgImap4Synchronisation::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KSetImap4FullDownloadLimits)
		{
		testStep = new(ELeave) CT_MsgSetImap4FullDownloadLimits(*iSharedDataIMAP);
		}
	else if(aStepName == KSetImap4PartialDownloadLimits)
		{
		testStep = new(ELeave) CT_MsgSetImap4PartialDownloadLimits(*iSharedDataIMAP);
		}
	else if(aStepName == KCompareEmailMessages)
		{
		testStep = new(ELeave) CT_MsgCompareEmailMessages(*iSharedDataIMAP);
		}
	else if(aStepName == KImapInvokeAsyncFunction)
		{
		testStep = CT_MsgImapInvokeAsyncFunction::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KCopyImapSelection)
		{
		testStep = CT_MsgCopyImapSelection::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KFindEntryByName)
		{
		testStep = new(ELeave) CT_MsgFindEntryByName(*iSharedDataIMAP);
		}
	else if(aStepName == KCheckChildrenCountImap)
		{
		testStep = new(ELeave) CT_MsgCheckChildrenCountImap(*iSharedDataIMAP);
		}
	else if(aStepName == KConnectAndSyncImapServer)
		{
		testStep = CT_MsgConnectAndSyncImap4Server::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KSetImap4ServiceIdleParamters)
		{
		testStep = new(ELeave)CT_MsgSetImap4SetImap4ServiceIdleParamters(*iSharedDataIMAP);
		}
	else if(aStepName == KCheckChildrenCountFlagBase)
		{
		testStep = new(ELeave) CT_MsgCheckChildrenCountFlagBase(*iSharedDataIMAP);
		}
	else if(aStepName == KDeleteMessage)
		{
		testStep = CT_MsgDeleteMsg::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KCopyMessage)
		{
		testStep = CT_MsgCopyMsg::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KRenameImapFolder)
		{
		testStep = CT_MsgRenameImapFolder::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KSelectImapFolder)
		{
		testStep = CT_MsgSelectImapFolder::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KCancelOfflineOperations)
		{
		testStep = CT_MsgCancelOfflineOperations::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KCreateImap4Folder)
		{
		testStep = CT_MsgCreateImap4Folder::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KDeleteImap4Folder)
		{
		testStep = CT_MsgDeleteImap4Folder::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KCopyImapSelectionRemote)
		{
		testStep = CT_MsgCopyImapSelectionRemote::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KCheckIfConnected)
		{
		testStep = CT_MsgCheckIfConnected::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KMoveMessage)
		{
		testStep = CT_MsgMove::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KUnDeleteAll)
		{
		testStep = CT_MsgUnDeleteAll::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KInitialiseImap4Account)
		{
		testStep = CT_MsgInitialiseImap4Account::NewL(*iSharedDataIMAP);
		}
	else if(aStepName ==KSetEventObserver)
		{
		testStep  = new(ELeave)CT_MsgSetEventObserver(*iSharedDataIMAP);
		}
	else if ( aStepName==KCheckEventNCount)
		{
		testStep = new(ELeave)CT_MsgObserverCkEventNCount(*iSharedDataIMAP);
		}
	else if(aStepName ==KRemoveEventObserver)
		{
		testStep = new(ELeave)CT_MsgRemoveEventObserver(*iSharedDataIMAP);
		}
	else if(aStepName == KSetImap4EmailPriority)
		{
		testStep = CT_MsgSetImap4EmailPriority::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KCheckImap4EmailPriority)
		{
		testStep = CT_MsgCheckImap4EmailPriority::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KChangeImap4SubAndSyncSettings)
		{
		testStep = CT_MsgChangeImap4SubAndSyncSettings::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KCheckImap4Subscription)
		{
		testStep = CT_MsgCheckImap4Subscription::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KSubscribeImap4Folder)
		{
		testStep = CT_MsgSubscribeImap4Folder::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KUnsubscribeImap4Folder)
		{
		testStep = CT_MsgUnsubscribeImap4Folder::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KImap4MoveAsyncFunction)
		{
		testStep = CT_MsgImapMoveAsyncFunction::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KImap4CopyAsyncFunction)
		{
		testStep = CT_MsgImapCopyAsyncFunction::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KFolderFullSync)
		{
		testStep = CT_MsgFolderFullSync::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KInboxNewEmailsSync)
		{
		testStep = CT_MsgInboxNewEmailsSync::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KDeleteAllMessage)
		{
		testStep = CT_MsgDeleteAllMsg::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KIsImap4Connected)
		{
		testStep = new(ELeave) CT_MsgIsImap4Connected(*iSharedDataIMAP);
		}
	else if(aStepName == KSetUnReadFlag)
		{
		testStep = new(ELeave) CT_MsgSetUnReadFlag(*iSharedDataIMAP);
		}
	else if(aStepName == KStartRamUsageTimer)
 		{
 		testStep = new(ELeave) CT_StartRamUsageTimer(*iSharedDataIMAP);
 		}
 	else if(aStepName == KStopRamUsageTimer)
 		{
 		testStep = new(ELeave) CT_StopRamUsageTimer(*iSharedDataIMAP);
 		}
	else if(aStepName == KChangeImap4FolderSyncSettings)
		{
		testStep = CT_MsgChangeImap4FolderSyncSettings::NewL(*iSharedDataIMAP);
		}
 	else if(aStepName == KCreateImapAccountWithoutSmtp)
		{
		testStep = new(ELeave) CT_MsgCreateImapAccountWithoutSmtp(*iSharedDataIMAP);
		}
	else if(aStepName == KCheckImapSNAPSetting)
		{
		testStep = new(ELeave) CT_MsgCheckImapSNAPSetting(*iSharedDataIMAP);
		}
	else if(aStepName == KRemoveImapSNAPSetting)
		{
		testStep = new(ELeave) CT_MsgRemoveImapSNAPSetting(*iSharedDataIMAP);
		}
	else if(aStepName == KCheckImapBMSetting)
		{
		testStep = new(ELeave) CT_MsgCheckImapBMSetting(*iSharedDataIMAP);
		}
	else if(aStepName == KCreateImapAccountWithDLRules)
		{
		testStep = new(ELeave) CT_MsgCreateImapAccountWithDLRules(*iSharedDataIMAP);
		}
	else if(aStepName == KCheckImapSyncDLRules)
		{
		testStep = new(ELeave) CT_MsgCheckImapSyncDLRules(*iSharedDataIMAP);
		}
	else if(aStepName == KModifyImapSyncDLRules)
		{
		testStep = new(ELeave) CT_MsgModifyImapSyncDLRules(*iSharedDataIMAP);
		}
	else if(aStepName == KDeleteImapSyncDLRules)
		{
		testStep = new(ELeave) CT_MsgDeleteImapSyncDLRules(*iSharedDataIMAP);
		}
	else if(aStepName == KCheckImapDLRulesSetting)
		{
		testStep = new(ELeave) CT_MsgCheckImapDLRulesSetting(*iSharedDataIMAP);
		}
	else if(aStepName == KAddImapSyncDLRules)
		{
		testStep = new(ELeave) CT_MsgAddImapSyncDLRules(*iSharedDataIMAP);
		}
	else if(aStepName == KAddImap4TransportBufferSizes)
		{
		testStep = new(ELeave) CT_MsgAddImap4TransportBufferSizes(*iSharedDataIMAP);		
		}
	else if(aStepName == KCheckImap4AccessPoint)
		{
		testStep = new(ELeave) CT_MsgCheckImap4AccessPoint(*iSharedDataIMAP);		
		}
	else if(aStepName == KIsImapMessageHeaderOnly)
		{
		testStep = CT_MsgIsImapMessageHeaderOnly::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KChangeToExternalDrive)
		{
		testStep = CT_ChangeToExternalDrive::NewL(*iSharedDataIMAP);
		}
#ifdef SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT		
	else if(aStepName == KUpdateToExternalDrive)
		{
		testStep = CT_UpdateToExternalDrive::NewL(*iSharedDataIMAP);
		}	
	else if(aStepName == KCopyStoreImapToRemoteDrive)
		{
		testStep = CT_MsgCopyStoreImapToRemoteDrive::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KDeleteStoreOfRemoteDrive)
		{
		testStep = CT_MsgDeleteStoreOfRemoteDrive::NewL(*iSharedDataIMAP);
		}			
#endif
	else if(aStepName == KChangeToInternalDrive)
		{
		testStep = CT_ChangeToInternalDrive::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KInsertMedia)
		{
		testStep = CT_InsertMedia::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KRemoveMedia)
		{
		testStep = CT_RemoveMedia::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KCopyMsgsToExternalDrive)
		{
		testStep = CT_CopyMsgsToExternalDrive::NewL(*iSharedDataIMAP);
		}
	else if(aStepName == KUnitTestCImap4ClientMtm)
		{
		testStep = new (ELeave)CT_CImap4ClientMtm(*iSharedDataIMAP);
		}
	else if(aStepName == KVerifyHeaders)
		{
		testStep = new (ELeave)CT_MsgVerifyHeaders(*iSharedDataIMAP);
		}		
		
	/* If none of the test steps match call base class CreateTestStepL */
	else 
		{
		testStep = CT_MsgServer::CreateTestStepL(aStepName);
		}	
	return testStep;
	}

