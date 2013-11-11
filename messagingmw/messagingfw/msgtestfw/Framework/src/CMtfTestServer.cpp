// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/

#include "CMtfTestServer.h"
#include "CMtfTestCase.h"
#include "CMtfTestAction.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestBeginManager.h"
#include "CMtfTestActionNamePair.h"
#include "CMtfTestCaseNamePair.h"
#include "CMtfTestActionSectionComplete.h"
#include "CMtfTestActionManager.h"
#include "CMtfTestActionWait.h"
#include "CMtfEnumeratorConverter.h"
#include "CMtfSetConfigurationTestStep.h"
#include <msvstd.h>
#include <bif.h>
#include <msvapi.h>
#include <etelmm.h>

#include "CMtfTestActionSampleActionA.h"
#include "CMtfTestActionSampleActionB.h"
#include "CMtfTestActionSampleActionC.h"
#include "CMtfTestActionSampleActionD.h"
#include "CMtfTestActionSampleActionE.h"
#include "CMtfTestActionSampleActionF.h"

#include "CMtfTestActionStartTimer.h"
#include "CMtfTestActionStopTimer.h"
#include "CMtfTestActionSetHeapFailure.h"
#include "CMtfTestActionSleep.h"
#include "CMtfTestActionGetBaseConstants.h"
#include "CMtfTestActionGetEntryPriorityEnums.h"
#include "CMtfTestActionDeleteMessageStore.h"
#include "CMtfTestActionCloseServer.h"
#include "CMtfTestActionIsUsingHardcodedDefaultSettings.h"
#include "CMtfTestActionCreateSession.h"
#include "CMtfTestActionDeleteSession.h"
#include "CMtfTestActionResetMessageStore.h"
#include "CMtfTestActionCreateService.h"
#include "CMtfTestActionDeleteService.h"
#include "CMtfTestActionCreateRegistry.h"
#include "CMtfTestActionDeleteRegistry.h"
#include "CMtfTestActionCreateMtm.h"
#include "CMtfTestActionDeleteMtm.h"
#include "CMtfTestActionCreateEntry.h"
#include "CMtfTestActionDeleteEntry.h"
#include "CMtfTestActionCreateMessage.h"
#include "CMtfTestActionDeleteMessage.h"
#include "CMtfTestActionCopyEntry.h"
#include "CMtfTestActionMoveEntry.h"
#include "CMtfTestActionFindEntryByName.h"
#include "CMtfTestActionCreateFolder.h"
#include "CMtfTestActionDeleteFolder.h"
#include "CMtfTestActionCreateSelection.h"
#include "CMtfTestActionDeleteSelection.h"
#include "CMtfTestActionAppendToSelection.h"
#include "CMtfTestActionDeleteChildren.h"
#include "CMtfTestActionGetDefaultMtmCommandParameters.h"
#include "CMtfTestActionExecuteMtmCommand.h"
#include "CMtfTestActionSetDefaultDataFile.h"
#include "CMtfTestActionCheckChildrenCount.h"
#include "CMtfTestActionCheckChildrenCountWithFlagNew.h"
#include "CMtfTestActionCheckChildrenCountWithFlagComplete.h"
#include "CMtfTestActionCreateChildrenSelection.h"
#include "CMtfTestActionCopySelection.h"
#include "CMtfTestActionMoveSelection.h"
#include "CMtfTestActionChangeToExternalDrive.h"
#include "CMtfTestActionChangeToInternalDrive.h"
#include "CMtfTestActionRemoveMedia.h"
#include "CMtfTestActionInsertMedia.h"
#include "CMtfTestActionDeletePath.h"
#include "CMtfTestActionSetDefaultConfiguration.h"
#include "CMtfTestActionGetArrayCount.h"
#include "CmtfTestActionGetNumberOfChildren.h"
#include "CMtfTestActionCompareNumbers.h"
#include "CMtfTestActionClass0SmsNotifier.h"
#include "CMtfTestActionForwardSystemTime.h"

#include "CMtfTestActionAddRecipient.h"
#include "CMtfTestActionSetBodyText.h"
#include "CMtfTestActionCountAvailableAccounts.h"
#include "CMtfTestActionDeleteStore.h"

#include "CMtfTestActionCheckIfMessagePresent.h"
#include "CMtfTestActionFindFile.h"
#include "CMtfTestActionLaunchAutoSend.h"
#include "CMtfTestActionDeleteExistingService.h"
#include "CMtfTestActionInstallMtmGroup.h"
#include "CMtfTestActionDeInstallMtmGroup.h"

#include "CMtfTestActionCheckAttachmentPathFromEntrySelection.h"
#include "CMtfTestActionGetEmailConstants.h"
#include "CMtfTestActionInitNtRas.h"
#include "CMtfTestActionStopAllSocketConnections.h"

#include "CMtfTestActionCreateImap4Service.h"
#include "CMtfTestActionDeleteImap4Service.h"
#include "CMtfTestActionCreateImap4Mtm.h"
#include "CMtfTestActionDeleteImap4Mtm.h"
#include "CMtfTestActionSetImap4ServiceParameters.h"
#include "CMtfTestActionConnectImap4Server.h"
#include "CMtfTestActionConnectAndSyncImap4Server.h"
#include "CMtfTestActionDisconnectImap4Server.h"
#include "CMtfTestActionDoFullImap4Synchronisation.h"
#include "CMtfTestActionSetImap4ServiceIdleParameters.h"
#include "Cmtftestactionsetimap4serviceLongidleparameters.h"
#include "CMtfTestActionImap4Connected.h"
#include "CMtfTestActionSetImap4FullDownloadLimits.h"
#include "CMtfTestActionSetImap4PartialDownloadLimits.h"
#include "CMtfTestActionCreateMailSelection.h"
#include "CMtfTestActionCreatePartiallyDownloadedMailSelection.h"
#include "CMtfTestActionCompareEmailMessages.h"

#include "CMtfTestActionDoImap4ConnectAndPopulateAllMailAndDisconnect.h"
#include "CMtfTestActionDoImap4ConnectAndPopulateAllMailAndStayOnline.h"
#include "CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndDisconnect.h"
#include "CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline.h"
#include "CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect.h"
#include "CMtfTestActionDoImap4ConnectAndPopulateNewMailAndStayOnline.h"
#include "CMtfTestActionDoImap4Populate.h"
#include "CMtfTestActionDoImap4PopulateAllMailWhenAlreadyConnected.h"
#include "CMtfTestActionDoImap4PopulateMailSelectionWhenAlreadyConnected.h"
#include "CMtfTestActionDoImap4PopulateNewMailWhenAlreadyConnected.h"

// central repository tests
#include "CMtfTestActionCreateImapAccount.h"
#include "CMtfTestActionDeleteImapAccounts.h"
#include "CMtfTestActionSaveImapSettings.h"
#include "CMtfTestActionLoadImapSettings.h"
#include "CMtfTestActionPopulateDefaultImapSettings.h"
#include "CMtfTestActionCompareImapAccountId.h"
#include "CMtfTestActionCompareImapSettings.h"
#include "CMtfTestActionCountImapAccountsInArray.h"
#include "CMtfTestActionGetImapAccountInArray.h"
#include "CMtfTestActionGetDefaultImapService.h"
#include "CMtfTestActionChangeDefaultImapService.h"
#include "CMtfTestActionRemoveDefaultImapService.h"
#include "CMtfTestActionGetImapAccountSettings.h"
#include "CMtfTestActionModifyImapSettings.h"


#include "CMtfTestActionCreatePop3Service.h"
#include "CMtfTestActionDeletePop3Service.h"
#include "CMtfTestActionCreatePop3Mtm.h"
#include "CMtfTestActionDeletePop3Mtm.h"
#include "CMtfTestActionSetPop3ServiceParameters.h"
#include "CMtfTestActionConnectPop3Server.h"
#include "CMtfTestActionDisconnectPop3Server.h"
#include "CMtfTestActionPop3Populate.h"
#include "CMtfTestActionPop3PopulateAll.h"
#include "CMtfTestActionPop3PopulateNew.h"
#include "CMtfTestActionCreatePop3MailInfo.h"
#include "CMtfTestActionGetPopAccountSettings.h"
#include "CMtfTestActionModifyPopSettings.h"



// central repository tests
#include "CMtfTestActionCreatePopAccount.h"
#include "CMtfTestActionDeletePopAccounts.h"
#include "CMtfTestActionSavePopSettings.h"
#include "CMtfTestActionLoadPopSettings.h"
#include "CMtfTestActionPopulateDefaultPopSettings.h"
#include "CMtfTestActionComparePopAccountId.h"
#include "CMtfTestActionComparePopSettings.h"
#include "CMtfTestActionCountPopAccountsInArray.h"
#include "CMtfTestActionGetPopAccountInArray.h"
#include "CMtfTestActionGetDefaultPopService.h"
#include "CMtfTestActionChangeDefaultPopService.h"
#include "CMtfTestActionRemoveDefaultPopService.h"


#include "CMtfTestActionCreateSmtpService.h"
#include "CMtfTestActionDeleteSmtpService.h"
#include "CMtfTestActionCreateSmtpMtm.h"
#include "CMtfTestActionDeleteSmtpMtm.h"
#include "CMtfTestActionSetSmtpServiceParameters.h"
#include "CMtfTestActionCreateSmtpMessageFromEmailFile.h"
#include "CMtfTestActionCreateSmtpMessageVariable.h"
#include "CMtfTestActionVerifyEmail.h"
#include "CMtfTestActionVerifyReplyToAddress.h"

#include "CMtfTestActionCreateExpPop3MailInfo.h"
#include "CMtfTestActionGetDefaultSmtpService.h"
#include "CMtfTestActionChangeDefaultSmtpService.h"
#include "CMtfTestActionRemoveDefaultSmtpService.h"

#include "CMtfTestActionGetSmsConstants.h"
#include "CMtfTestActionCreateSmsService.h"
#include "CMtfTestActionSetDefaultSmsSettings.h"
#include "CMtfTestActionSetSmsServiceSettings.h"

#include "CMtfTestActionSmsSettingsSaveExternal.h"
#include "CMtfTestActionCompareSmsSettings.h"
#include "CMtfTestActionReceiveSmsMessages.h"
#include "CMtfTestActionReceiveSmsMessages2.h"
#include "CmtfTestActionCompareSmsMessages.h"


#include "CMtfTestActionInitialiseSmsDefaultSettings.h"
#include "CMtfTestActionInitialiseDefaultScheduleSettings.h"
#include "CMtfTestActionLoadSmsSettings.h"
#include "CMtfTestActionLoadScheduleSendSettings.h"
#include "CMtfTestActionSaveSmsSettings.h"
#include "CMtfTestActionSaveScheduleSendSettings.h"
#include "CMtfTestActionCompareSmsSettingsCR.h"
#include "CMtfTestActionCompareScheduleSendSettings.h"
#include "CMtfTestActionGetDefaultSmsService.h"
#include "CMtfTestActionChangeDefaultSmsService.h"
#include "CMtfTestActionRemoveDefaultSmsService.h"
#include "CMtfTestActionCheckSmsService.h"
#include "CMtfTestActionCompareLastSegmentReport.h"

#include "CMtfTestActionSetDiskSpaceMonitorLimits.h"
#include "CMtfTestActionSetDiskSpace.h"
#include "CMtfTestActionFreeDiskSpace.h"


#include "CMtfTestActionSendEMsvGetEntry.h"
#include "CMtfTestActionSendEMsvGetChildren.h"
#include "CMtfTestActionSendEMsvGetRemainingChildren.h"
#include "CMtfTestActionSendEMsvChangeEntry.h"
#include "CMtfTestActionSendEMsvCreateEntry.h"
#include "CMtfTestActionSendEMsvDeleteEntries.h"
#include "CMtfTestActionSendEMsvMoveEntries.h"
#include "CMtfTestActionSendEMsvCopyEntries.h"
#include "CMtfTestActionSendEMsvNotifySessionEvent.h"
#include "CMtfTestActionSendEMsvCancelSessionEventNotification.h"
#include "CMtfTestActionSendEMsvReadStore.h"
#include "CMtfTestActionSendEMsvLockStore.h"
#include "CMtfTestActionSendEMsvReleaseStore.h"
#include "CMtfTestActionSendEMsvOperationData.h"
#include "CMtfTestActionSendEMsvCommandData.h"
#include "CMtfTestActionSendEMsvCancelOperation.h"
#include "CMtfTestActionSendEMsvOperationProgress.h"
#include "CMtfTestActionSendEMsvOperationCompletion.h"
#include "CMtfTestActionSendEMsvOperationMtm.h"
#include "CMtfTestActionSendEMsvMtmCommand.h"
#include "CMtfTestActionSendEMsvFillRegisteredMtmDllArray.h"
#include "CMtfTestActionSendEMsvInstallMtmGroup.h"
#include "CMtfTestActionSendEMsvDeInstallMtmGroup.h"
#include "CMtfTestActionSendEMsvUseMtmGroup.h"
#include "CMtfTestActionSendEMsvReleaseMtmGroup.h"
#include "CMtfTestActionSendEMsvGetMtmGroupData.h"
#include "CMtfTestActionSendEMsvCloseServer.h"
#include "CMtfTestActionSendEMsvStopService.h"
#include "CMtfTestActionSendEMsvServiceActive.h"
#include "CMtfTestActionSendEMsvServiceProgress.h"
#include "CMtfTestActionSendEMsvRemoveEntry.h"
#include "CMtfTestActionSendEMsvSlotAvailable.h"
#include "CMtfTestActionSendEMsvSetSessionAsObserver.h"
#include "CMtfTestActionSendEMsvSetFailure.h"
#include "CMtfTestActionSendEMsvChangeAttributes.h"
#include "CMtfTestActionSendEMsvGetChildIds.h"
#include "CMtfTestActionSendEMsvChangeDrive.h"
#include "CMtfTestActionSendEMsvOutstandingOperations.h"
#include "CMtfTestActionSendEMsvGetNotifySequence.h"
#include "CMtfTestActionSendEMsvReceiveEntyEvents.h"
#include "CMtfTestActionSendEMsvDecStoreReaderCount.h"
#include "CMtfTestActionSendEMsvGetMessageDrive.h"
#include "CMtfTestActionSendEMsvGetMtmRequiredCapabilities.h"
#include "CMtfTestActionSendEMsvCreateAttachmentForWrite.h"
#include "CMtfTestActionSendEMsvOpenAttachment.h"
#include "CMtfTestActionSendEMsvOpenAttachmentForWrite.h"
#include "CMtfTestActionSendEMsvDeleteAttachment.h"
#include "CMtfTestActionSendEMsvOpenFileStoreForRead.h"
#include "CMtfTestActionSendEMsvOpenTempStoreFile.h"
#include "CMtfTestActionSendEMsvReplaceFileStore.h"
#include "CMtfTestActionSendEMsvDeleteFileStore.h"
#include "CMtfTestActionSendEMsvFileStoreExists.h"
#include "CMtfTestActionSendEMsvGetAndClearIndexCorruptFlag.h"
#include "CMtfTestActionSendEMsvCopyStore.h"
#include "CMtfTestActionSendEMsvDeleteStore.h"
#include "CMtfTestActionSendEMsvDriveContainsStore.h"

// PREQ 557
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
#include "cmtftestactionsendemsvadddrive.h"
#include "cmtftestactionsendemsvremovedrive.h"
#include "cmtftestactionsendemsvupdatedrive.h"

#include "cmtftestactionsendemsvdrivelist.h"
#include "cmtftestactionsendemsvavailabledrivelist.h"
#include "cmtftestactionsendemsvcurrentdriveinfo.h"

#include "cmtftestactionsendemsvchildrenofavailabledrives.h"
#include "cmtftestactionsendemsvgetchildidsall.h"
#endif // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

// PREQ 2073
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
#include "cmtftestactionsendemsvcreatestore.h"
#include "cmtftestactionsendemsvcheckstore.h"
#include "cmtftestactionsendemsvcreateheaderentry.h"
#include "cmtftestactionsendemsvupdateheaderentry.h"
#include "cmtftestactionsendemsvloadheaderentry.h"
#include "cmtftestactionsendemsvdeleteheaderentry.h"
#endif // #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

#include "CMtfTestActionCorruptFile.h"

#include "CMtfTestActionSaveMsvIdParamToFile.h"
#include "CMtfTestActionLoadMsvIdParamToFile.h"


#include "CMtfTestActionStartWatchers.h"
#include "CMtfTestActionCheckIfSmsWatcherRunning.h"
#include "CMtfTestActionCreateSmsMessageFromFile.h"

#include "CMtfTestCaseSample001.h"
#include "CMtfTestCaseCreateSendReceivePop3Email.h"
#include "CMtfTestCaseCreateSendReceiveImap4Email.h"

#include "CMtfTestActionInitializeSimTsy.h"
#include "CMtfTestActionUnloadPhoneModule.h"
#include "CMtfTestActionSetNetRegStatus.h"
#include "CMtfTestActionSetSchSendSettings.h"
#include "CMtfTestActionCreateSmsMessage.h"
#include "CMtfTestActionCreateEmailOverSmsMessage.h"
#include "CMtfTestActionCompareEntryWithFile.h"
#include "CMtfTestActionCheckMessageSendingState.h"
#include "CMtfTestActionSetSignalStrength.h"

#include "CMtfTestActionCreatePigeonMessage.h"
#include "CMtfTestActionGetPigeonMtmConstant.h"
#include "CMtfTestActionSchedulePigeonMessage.h"
#include "CMtfTestActionCheckIfMessageScheduleDateExpired.h"

#include "CMtfTestActionLaunchSchSendExe.h"
#include "CMtfTestActionLaunchScheduleMessageExe.h"

#include "CMtfTestActionPruneMessages.h"

#include "CMtfTestActionAddFileAttachmentByPath.h"
#include "CMtfTestActionAddFileAttachmentByPathSync.h"
#include "CMtfTestActionAddFileAttachmentByHandle.h"
#include "CMtfTestActionAddFileAttachmentByHandleSync.h"
#include "CMtfTestActionAddFileAttachmentWithRevert.h"
#include "CMtfTestActionAddFileAttachmentWithDestroy.h"
#include "CMtfTestActionAddLinkedAttachment.h"
#include "CMtfTestActionAddEntryAttachment.h"
#include "CMtfTestActionGetAttachmentInfoById.h"
#include "CMtfTestActionGetAttachmentInfoByIndex.h"
#include "CMtfTestActionCreateAttachment.h"
#include "CMtfTestActionCompareAttachment.h"
#include "CMtfTestActionCreateAttachmentNoFileClose.h"
#include "CMtfTestActionCheckErrorCode.h"
#include "CMtfTestActionCountAttachments.h"
#include "CMtfTestActionGetAttachmentIndexFromId.h"
#include "CMtfTestActionGetAttachmentFileFromId.h"
#include "CMtfTestActionGetAttachmentFileFromIndex.h"
#include "CMtfTestActionGetAttachmentFileForWriteFromId.h"
#include "CMtfTestActionRemoveFileAttachmentById.h"
#include "CMtfTestActionRenameFileAttachmentById.h"
#include "CMtfTestActionRemoveFileAttachmentByIndex.h"
#include "CMtfTestActionRemoveLinkedAttachmentById.h"
#include "CMtfTestActionRemoveEntryAttachmentById.h"
#include "CMtfTestActionRemoveFileAttachmentWithRevert.h"
#include "CMtfTestActionRemoveFileAttachmentWithDestroy.h"
#include "CMtfTestActionCreateTestMimeHeaders.h"
#include "CMtfTestActionCreateAttachmentForWrite.h"
#include "CMtfTestActionOpenAttachment.h"
#include "CMtfTestActionDeleteAttachment.h"
#include "CMtfTestActionOpenFileStoreForRead.h"
#include "CMtfTestActionOpenTempStoreFile.h"
#include "CMtfTestActionFileStoreExists.h"
#include "CMtfTestActionDeleteFileStore.h"
#include "CMtfTestActionReplaceFileStore.h"

#include "CMtfTestActionVerifyAttachmentInfo.h"
#include "CMtfTestActionRemoveAllAttachments.h"
#include "CMtfTestActionAddAttachmentAsLink.h"

#include "CMtfTestActionGetEntryTypeConstants.h"

#include  "CMtfTestActionGetIRMTMUId.h"
#include "CMtfTestActionGetIrMTMCommandParameters.h"

#include "CMtfTestActionGetBTMTMUId.h"
#include "CMtfTestActionGetBtMTMCommandParameters.h"

#include "CMtfTestActionSmtpGetAttachmentFileFromIndex.h"
#include "CMtfTestActionSmtpAddEntryAttachment.h"
#include "CMtfTestActionSmtpAddFileAttachmentByHandleSync.h"
#include "CMtfTestActionSmtpAddFileAttachmentByPath.h"
#include "CMtfTestActionSmtpAddLinkedAttachment.h"
#include "CMtfTestActionSmtpCompareAttachment.h"
#include "CMtfTestActionSmtpCountAttachments.h"
#include "CMtfTestActionSmtpCreateAttachment.h"
#include "CMtfTestActionSmtpGetAttachmentFileFromId.h"
#include "CMtfTestActionSmtpGetAttachmentInfoById.h"
#include "CMtfTestActionSmtpGetAttachmentInfoByIndex.h"
#include "CMtfTestActionSmtpRemoveAllAttachments.h"
#include "CMtfTestActionSmtpRemoveFileAttachmentById.h"
#include "CMtfTestActionSmtpRemoveFileAttachmentByIndex.h"
#include "CMtfTestActionSmtpRemoveEntryAttachmentById.h"
#include "CMtfTestActionSmtpModifyAttachmentInfo.h"

#include "CMtfTestActionRenamePath.h"
#include "CMtfTestActionGetEntry.h"
#include "CMtfTestActionGetStore.h"
#include "CMtfTestActionReadMessage.h"
#include "CMtfTestActionSetUTCOffset.h"
#include "CMtfTestActionCreateOrderedChildrenSelection.h"
#include "CMtfTestActionCompareSelections.h"

#include "CMtfTestActionGetSMTPMTMUId.h"


#include "CMtfTestActionCheckDrmReceipt.h"
#include "CMtfTestActionCheckDrmMetaData.h"
#include "CMtfTestActionCheckMessageStore.h"
#include "CMtfTestActionSmtpSendEmail.h"

//SendAs Includes
#include "CMtfTestActionConnectSendAsServer.h"
#include "CMtfTestActionSendAsGetFilteredMessageTypes.h"
#include "CMtfTestActionSendAsGetMessageTypeUid.h"
#include "CMtfTestActionSendAsCreateMessageofType.h"
#include "CMtfTestActionSendAsCloseMessage.h"
#include "CMtfTestActionCloseSendAsServerConnection.h"

#include "CMtfTestActionSendAsAddAttachment.h"
#include "CMtfTestActionSendAsAddLinkedAttachment.h"
#include "CMtfTestActionSendAsAddRecipient.h"
#include "CMtfTestActionSendAsAddMultipleRecipient.h"
#include "CMtfTestActionSendAsCheckMultipleRecipientFlag.h"
#include "CMtfTestActionSendAsSaveMessageAndClose.h"
#include "CMtfTestActionSendAsLaunchEditorAndClose.h"
#include "CMtfTestActionSendAsCreateAttachment.h"
#include "CMtfTestActionSendAsCreateMessageWithAccount.h"
#include "CMtfTestActionSendAsGetAccount.h"
#include "CMtfTestActionSendAsGetAccountNames.h"
#include "CMtfTestActionSendAsGetAvailableAccounts.h"
#include "CMtfTestActionSendAsGetAvailableMessageTypes.h"
#include "CMtfTestActionSendAsSendMessage.h"
#include "CMtfTestActionSendAsSetBioType.h"
#include "CMtfTestActionSendAsSetSubject.h"
#include "CMtfTestActionVerifyBioType.h"
#include "CMtfTestActionVerifyBodyText.h"
#include "CMtfTestActionVerifyRecipients.h"
#include "CMtfTestActionVerifySubject.h"
#include "CMtfTestActionVerifySendAsMessageState.h"
#include "CMtfTestActionGetMessageAtIndex.h"
#include "CMtfTestActionSendAsCloseAndSendMessage.h"
#include "CMtfTestActionSendAsSetBodyText.h"

#include "CMtfTestActionGetSMTPMTMUId.h"
#include "CMtfTestActionCompareEmails.h"
#include "CMtfTestActionSetUserResponse.h"

#include "CMtfTestActionSendAsSendMessageNew.h"

// Preq 1667 - Enhanced Search Sort in message store includes
#include "CMtfTestActionSendEMsvGetResultCount.h"


_LIT(KBeginTestCase,"BEGIN_TEST_CASE");
_LIT(KBeginTestCaseAsync,"BEGIN_TEST_CASE_ASYNC");
_LIT(KSetupTestCase,"SETUP_TEST_CASE");
_LIT(KExecuteTestCase,"EXECUTE_TEST_CASE");
_LIT(KCheckPostconditions,"CHECK_POSTCONDITIONS");
_LIT(KEndTestCase,"END_TEST_CASE");
_LIT(KWaitForAll,"WaitForAll");
_LIT(KWaitForActions,"WaitForActions");
_LIT(KSetConfiguration,"SetConfiguration");

_LIT(KMtfInvalidScript,"Invalid Script");
_LIT(KMtfInternalError,"Internal Error");

// hack to ensure different test accounts are used on different builds
// needs to be updated each time we have a new build
// 9.0 builds (& 9.1 at the moment but we don't test 9.1 at the moment)
_LIT(KDefaultConfigImap4Settings,	"c:\\MsgTest\\TestFramework\\Configurations\\EmailSettings\\Imap4Settings-lon-cn-exchng2k_90.txt");
_LIT(KDefaultConfigPop3Settings,	"c:\\MsgTest\\TestFramework\\Configurations\\EmailSettings\\Pop3Settings-lon-cn-exchng2k_90.txt" );
_LIT(KDefaultConfigSmtpSettings,	"c:\\MsgTest\\TestFramework\\Configurations\\EmailSettings\\SmtpSettings-lon-cn-exchng2k_90.txt" );







// Default config for Email File.

//only the email-simple.txt seems to work with CMtfTestActionCreateSmtpMessageFromEmailFile
_LIT(KDefaultConfigEmailFile,		"c:\\MsgTest\\TestFramework\\Configurations\\EmailMessage\\Email-Simple.txt");
//_LIT(KDefaultConfigEmailFile,		"c:\\MsgTest\\TestFramework\\Configurations\\EmailMessage\\2att.txt");
//_LIT(KDefaultConfigEmailFile,		"c:\\MsgTest\\TestFramework\\Configurations\\EmailMessage\\test1_mail.txt");
//_LIT(KDefaultConfigEmailFile,		"c:\\MsgTest\\TestFramework\\Configurations\\EmailMessage\\TCase_11_6KBAttchmnt.txt" );



// Default config for SMS settings
_LIT(KDefaultConfigSmsSettings, "c:\\MsgTest\\TestFramework\\Configurations\\SmsSettings\\SmsSettingsO2.txt");
_LIT(KDefaultConfigScheduleSendSettings, "c:\\MsgTest\\TestFramework\\Configurations\\SmsSettings\\SmsScheduleSettings.txt");

/**
 * @return - Instance of the test server
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
CMtfTestServer* CMtfTestServer::NewL()
    {
    CMtfTestServer* server = new(ELeave) CMtfTestServer();
    CleanupStack::PushL(server);

   	RProcess handle = RProcess();

	TParsePtrC serverName(handle.FileName());
server->StartL(serverName.Name());

    server->ConstructL();
    CleanupStack::Pop(server);
    return server;
    }

CMtfTestServer::CMtfTestServer()
:CTestServer(), iScriptState(CMtfTestCase::EMtfTestCaseInitialState)
    {
    }

CMtfTestServer::~CMtfTestServer()
    {
    // current test case does not need to be deleted
    // because TestExecute deletes it

    iAvailableTestActions.ResetAndDestroy();
    iAvailableTestCases.ResetAndDestroy();
    iDefaultConfigurations.ResetAndDestroy();
    iMainScriptConfigurations.ResetAndDestroy();
    iEnumeratorConverters.ResetAndDestroy();
    }

/** Constructs the data used by the Messaging test server. */
void CMtfTestServer::ConstructL()
    {
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSampleActionA,&CMtfTestActionSampleActionA::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSampleActionB,&CMtfTestActionSampleActionB::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSampleActionC,&CMtfTestActionSampleActionC::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSampleActionD,&CMtfTestActionSampleActionD::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSampleActionE,&CMtfTestActionSampleActionE::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSampleActionF,&CMtfTestActionSampleActionF::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionStartTimer,&CMtfTestActionStartTimer::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionStopTimer,&CMtfTestActionStopTimer::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetHeapFailure,&CMtfTestActionSetHeapFailure::NewL));

	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSleep,&CMtfTestActionSleep::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetBaseConstants,&CMtfTestActionGetBaseConstants::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetEntryPriorityEnums,&CMtfTestActionGetEntryPriorityEnums::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteMessageStore,&CMtfTestActionDeleteMessageStore::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCloseServer,&CMtfTestActionCloseServer::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionIsUsingHardcodedDefaultSettings,&CMtfTestActionIsUsingHardcodedDefaultSettings::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateSession,&CMtfTestActionCreateSession::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteSession,&CMtfTestActionDeleteSession::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionResetMessageStore,&CMtfTestActionResetMessageStore::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateService,&CMtfTestActionCreateService::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteService,&CMtfTestActionDeleteService::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateRegistry,&CMtfTestActionCreateRegistry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteRegistry,&CMtfTestActionDeleteRegistry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateMtm,&CMtfTestActionCreateMtm::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteMtm,&CMtfTestActionDeleteMtm::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateEntry,&CMtfTestActionCreateEntry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteEntry,&CMtfTestActionDeleteMessage::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateMessage,&CMtfTestActionCreateMessage::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteMessage,&CMtfTestActionDeleteMessage::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCopyEntry,&CMtfTestActionCopyEntry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionMoveEntry,&CMtfTestActionMoveEntry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionFindEntryByName,&CMtfTestActionFindEntryByName::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateFolder,&CMtfTestActionCreateFolder::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteFolder,&CMtfTestActionDeleteFolder::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateSelection,&CMtfTestActionCreateSelection::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteSelection,&CMtfTestActionDeleteSelection::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAppendToSelection,&CMtfTestActionAppendToSelection::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteChildren,&CMtfTestActionDeleteChildren::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetDefaultMtmCommandParameters,&CMtfTestActionGetDefaultMtmCommandParameters::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionExecuteMtmCommand,&CMtfTestActionExecuteMtmCommand::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetDefaultDataFile,&CMtfTestActionSetDefaultDataFile::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckChildrenCount,&CMtfTestActionCheckChildrenCount::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckChildrenCountWithFlagNew,&CMtfTestActionCheckChildrenCountWithFlagNew::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckChildrenCountWithFlagComplete,&CMtfTestActionCheckChildrenCountWithFlagComplete::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateChildrenSelection,&CMtfTestActionCreateChildrenSelection::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCopySelection,&CMtfTestActionCopySelection::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionMoveSelection,&CMtfTestActionMoveSelection::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionChangeToExternalDrive,&CMtfTestActionChangeToExternalDrive::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionChangeToInternalDrive,&CMtfTestActionChangeToInternalDrive::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionRemoveMedia,&CMtfTestActionRemoveMedia::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionInsertMedia,&CMtfTestActionInsertMedia::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteFile,&CMtfTestActionDeletePath::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetDefaultConfiguration,&CMtfTestActionSetDefaultConfiguration::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetArrayCount,&CMtfTestActionGetArrayCount::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetNumberOfChildren,&CMtfTestActionGetNumberOfChildren::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareNumbers,&CMtfTestActionCompareNumbers::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddRecipient,&CMtfTestActionAddRecipient::NewL));
 	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetBodyText,&CMtfTestActionSetBodyText::NewL));
 	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCountAvailableAccounts,&CMtfTestActionCountAvailableAccounts::NewL));
 	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteStore,&CMtfTestActionDeleteStore::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteExistingService,&CMtfTestActionDeleteExistingService::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckIfMessagePresent,&CMtfTestActionCheckIfMessagePresent::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionFindFile,&CMtfTestActionFindFile::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionLaunchAutoSend,&CMtfTestActionLaunchAutoSend::NewL));

	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetEmailConstants,&CMtfTestActionGetEmailConstants::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionInitNtRas,&CMtfTestActionInitNtRas::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionStopAllSocketConnections,&CMtfTestActionStopAllSocketConnections::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionInstallMtmGroup,&CMtfTestActionInstallMtmGroup::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeInstallMtmGroup,&CMtfTestActionDeInstallMtmGroup::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionLaunchAutoSend,&CMtfTestActionLaunchAutoSend::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckAttachmentPathFromEntrySelection,&CMtfTestActionCheckAttachmentPathFromEntrySelection::NewL));

    // IMAP Related

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateImap4Service,&CMtfTestActionCreateImap4Service::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteImap4Service,&CMtfTestActionDeleteImap4Service::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateImap4Mtm,&CMtfTestActionCreateImap4Mtm::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteImap4Mtm,&CMtfTestActionDeleteImap4Mtm::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetImap4ServiceParameters,&CMtfTestActionSetImap4ServiceParameters::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionConnectImap4Server,&CMtfTestActionConnectImap4Server::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionConnectAndSyncImap4Server,&CMtfTestActionConnectAndSyncImap4Server::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDisconnectImap4Server,&CMtfTestActionDisconnectImap4Server::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoFullImap4Synchronisation,&CMtfTestActionDoFullImap4Synchronisation::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetImap4ServiceIdleParameters,&CMtfTestActionSetImap4ServiceIdleParameters::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetImap4ServiceLongIdleParameters,&CMtfTestActionSetImap4ServiceLongIdleParameters::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionImap4Connected, &CMtfTestActionImap4Connected::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoFullImap4Synchronisation,&CMtfTestActionDoFullImap4Synchronisation::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoImap4ConnectAndPopulateAllMailAndDisconnect,&CMtfTestActionDoImap4ConnectAndPopulateAllMailAndDisconnect::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoImap4ConnectAndPopulateAllMailAndStayOnline,&CMtfTestActionDoImap4ConnectAndPopulateAllMailAndStayOnline::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoImap4ConnectAndPopulateMailSelectionAndDisconnect,&CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndDisconnect::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline,&CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect,&CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoImap4ConnectAndPopulateNewMailAndStayOnline,&CMtfTestActionDoImap4ConnectAndPopulateNewMailAndStayOnline::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoImap4Populate,&CMtfTestActionDoImap4Populate::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoImap4PopulateAllMailWhenAlreadyConnected,&CMtfTestActionDoImap4PopulateAllMailWhenAlreadyConnected::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoImap4PopulateMailSelectionWhenAlreadyConnected,&CMtfTestActionDoImap4PopulateMailSelectionWhenAlreadyConnected::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDoImap4PopulateNewMailWhenAlreadyConnected,&CMtfTestActionDoImap4PopulateNewMailWhenAlreadyConnected::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetImap4FullDownloadLimits,&CMtfTestActionSetImap4FullDownloadLimits::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetImap4PartialDownloadLimits,&CMtfTestActionSetImap4PartialDownloadLimits::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreatePartiallyDownloadedMailSelection,&CMtfTestActionCreatePartiallyDownloadedMailSelection::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateMailSelection,&CMtfTestActionCreateMailSelection::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareEmailMessages,&CMtfTestActionCompareEmailMessages::NewL));





    // CENTRAL REPOSITORY IMAP TESTS
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateImapAccount,&CMtfTestActionCreateImapAccount::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteImapAccounts,&CMtfTestActionDeleteImapAccounts::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSaveImapSettings,&CMtfTestActionSaveImapSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionLoadImapSettings,&CMtfTestActionLoadImapSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionPopulateDefaultImapSettings,&CMtfTestActionPopulateDefaultImapSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareImapAccountId, &CMtfTestActionCompareImapAccountId::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareImapSettings, &CMtfTestActionCompareImapSettings::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCountImapAccountsInArray, &CMtfTestActionCountImapAccountsInArray::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetImapAccountInArray, &CMtfTestActionGetImapAccountInArray::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionGetDefaultImapService, &CMtfTestActionGetDefaultImapService::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionChangeDefaultImapService, &CMtfTestActionChangeDefaultImapService::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionRemoveDefaultImapService, &CMtfTestActionRemoveDefaultImapService::NewL ) );

    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionGetImapAccountSettings, &CMtfTestActionGetImapAccountSettings::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionModifyImapSettings, &CMtfTestActionModifyImapSettings::NewL ) );



    // POP related
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreatePop3Service,&CMtfTestActionCreatePop3Service::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeletePop3Service,&CMtfTestActionDeletePop3Service::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreatePop3Mtm,&CMtfTestActionCreatePop3Mtm::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeletePop3Mtm,&CMtfTestActionDeletePop3Mtm::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetPop3ServiceParameters,&CMtfTestActionSetPop3ServiceParameters::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionConnectPop3Server,&CMtfTestActionConnectPop3Server::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDisconnectPop3Server,&CMtfTestActionDisconnectPop3Server::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionPop3Populate, &CMtfTestActionPop3Populate::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionPop3PopulateAll, &CMtfTestActionPop3PopulateAll::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionPop3PopulateNew, &CMtfTestActionPop3PopulateNew::NewL ) );


    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionCreatePop3MailInfo, &CMtfTestActionCreatePop3MailInfo::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionCreateExpPop3MailInfo, &CMtfTestActionCreateExpPop3MailInfo::NewL ) );




    // CENTRAL REPOSITORY POP TESTS
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionCreatePopAccount, &CMtfTestActionCreatePopAccount::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionSavePopSettings, &CMtfTestActionSavePopSettings::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionLoadPopSettings, &CMtfTestActionLoadPopSettings::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionPopulateDefaultPopSettings, &CMtfTestActionPopulateDefaultPopSettings::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionComparePopAccountId, &CMtfTestActionComparePopAccountId::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionComparePopSettings, &CMtfTestActionComparePopSettings::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionCountPopAccountsInArray, &CMtfTestActionCountPopAccountsInArray::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionGetPopAccountInArray, &CMtfTestActionGetPopAccountInArray::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionGetDefaultPopService, &CMtfTestActionGetDefaultPopService::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionChangeDefaultPopService, &CMtfTestActionChangeDefaultPopService::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionRemoveDefaultPopService, &CMtfTestActionRemoveDefaultPopService::NewL ) );

    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionGetPopAccountSettings, &CMtfTestActionGetPopAccountSettings::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionModifyPopSettings, &CMtfTestActionModifyPopSettings::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionDeletePopAccounts, &CMtfTestActionDeletePopAccounts::NewL ) );




    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetSmsConstants,&CMtfTestActionGetSmsConstants::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateSmsService,&CMtfTestActionCreateSmsService::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetDefaultSmsSettings,&CMtfTestActionSetDefaultSmsSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetSmsServiceSettings,&CMtfTestActionSetSmsServiceSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmsSettingsSaveExternal,&CMtfTestActionSmsSettingsSaveExternal::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareSmsSettings,&CMtfTestActionCompareSmsSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareSmsMessages,&CMtfTestActionCompareSmsMessages::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionReceiveSmsMessages,&CMtfTestActionReceiveSmsMessages::NewL));

	// SIM TSY Out of Disk space Tests
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetDiskSpaceMonitorLimits,&CMtfTestActionSetDiskSpaceMonitorLimits::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetDiskSpace,&CMtfTestActionSetDiskSpace::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionFreeDiskSpace,&CMtfTestActionFreeDiskSpace::NewL));


	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionReceiveSmsMessages2,&CMtfTestActionReceiveSmsMessages2::NewL));

    // CENTRAL REPOSITORY SMS TESTS
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionInitialiseSmsDefaultSettings,&CMtfTestActionInitialiseSmsDefaultSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionInitialiseDefaultScheduleSettings,&CMtfTestActionInitialiseDefaultScheduleSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionLoadSmsSettings,&CMtfTestActionLoadSmsSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionLoadScheduleSendSettings,&CMtfTestActionLoadScheduleSendSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSaveSmsSettings,&CMtfTestActionSaveSmsSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSaveScheduleSendSettings,&CMtfTestActionSaveScheduleSendSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareSmsSettingsCR,&CMtfTestActionCompareSmsSettingsCR::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareScheduleSendSettings,&CMtfTestActionCompareScheduleSendSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionGetDefaultSmsService, &CMtfTestActionGetDefaultSmsService::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionChangeDefaultSmsService, &CMtfTestActionChangeDefaultSmsService::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionRemoveDefaultSmsService, &CMtfTestActionRemoveDefaultSmsService::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionCheckSmsService, &CMtfTestActionCheckSmsService::NewL ) );
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareLastSegmentReport,&CMtfTestActionCompareLastSegmentReport::NewL));
// POP related
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreatePop3Service,&CMtfTestActionCreatePop3Service::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeletePop3Service,&CMtfTestActionDeletePop3Service::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreatePop3Mtm,&CMtfTestActionCreatePop3Mtm::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeletePop3Mtm,&CMtfTestActionDeletePop3Mtm::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetPop3ServiceParameters,&CMtfTestActionSetPop3ServiceParameters::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionConnectPop3Server,&CMtfTestActionConnectPop3Server::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDisconnectPop3Server,&CMtfTestActionDisconnectPop3Server::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionPop3Populate, &CMtfTestActionPop3Populate::NewL ) );
	AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionPop3PopulateAll, &CMtfTestActionPop3PopulateAll::NewL ) );
	AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionPop3PopulateNew, &CMtfTestActionPop3PopulateNew::NewL ) );


	AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionCreatePop3MailInfo, &CMtfTestActionCreatePop3MailInfo::NewL ) );
	AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionCreateExpPop3MailInfo, &CMtfTestActionCreateExpPop3MailInfo::NewL ) );



    // SMTP RELATED
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateSmtpService,&CMtfTestActionCreateSmtpService::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteSmtpService,&CMtfTestActionDeleteSmtpService::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateSmtpMtm,&CMtfTestActionCreateSmtpMtm::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteSmtpMtm,&CMtfTestActionDeleteSmtpMtm::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetSmtpServiceParameters,&CMtfTestActionSetSmtpServiceParameters::NewL));


    // CENTRAL REPOSITORY SMTP TESTS
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionGetDefaultSmtpService, &CMtfTestActionGetDefaultSmtpService::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionChangeDefaultSmtpService, &CMtfTestActionChangeDefaultSmtpService::NewL ) );
    AddTestActionL(CMtfTestActionNamePair::NewL( KTestActionRemoveDefaultSmtpService, &CMtfTestActionRemoveDefaultSmtpService::NewL ) );


    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionStartWatchers,&CMtfTestActionStartWatchers::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckIfSmsWatcherRunning,&CMtfTestActionCheckIfSmsWatcherRunning::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateSmsMessageFromFile, &CMtfTestActionCreateSmsMessageFromFile::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateSmtpMessageFromEmailFile,&CMtfTestActionCreateSmtpMessageFromEmailFile::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateSmtpMessageVariable,&CMtfTestActionCreateSmtpMessageVariable::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionVerifyEmail, &CMtfTestActionVerifyEmail::NewL ));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionVerifyReplyToAddress, &CMtfTestActionVerifyReplyToAddress::NewL ));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionInitializeSimTsy,&CMtfTestActionInitializeSimTsy::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionUnloadPhoneModule,&CMtfTestActionUnloadPhoneModule::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetNetRegStatus,&CMtfTestActionSetNetRegStatus::NewL));

    AddTestCaseL(CMtfTestCaseNamePair::NewL(KTestCaseSample001,&CMtfTestCaseSample001::NewL));
    AddTestCaseL(CMtfTestCaseNamePair::NewL(KTestCaseCreateSendReceivePop3Email,&CMtfTestCaseCreateSendReceivePop3Email::NewL));
    AddTestCaseL(CMtfTestCaseNamePair::NewL(KTestCaseCreateSendReceiveImap4Email,&CMtfTestCaseCreateSendReceiveImap4Email::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateSmsMessage,&CMtfTestActionCreateSmsMessage::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionClass0SmsNotifier,&CMtfTestActionClass0SmsNotifier::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionForwardSystemTime,&CMtfTestActionForwardSystemTime::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateEmailOverSmsMessage,&CMtfTestActionCreateEmailOverSmsMessage::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareEntryWithFile,&CMtfTestActionCompareEntryWithFile::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetSchSendSettings,&CMtfTestActionSetSchSendSettings::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckMessageSendingState,&CMtfTestActionCheckMessageSendingState::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetSignalStrength,&CMtfTestActionSetSignalStrength::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSchedulePigeonMessage,&CMtfTestActionSchedulePigeonMessage::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreatePigeonMessage,&CMtfTestActionCreatePigeonMessage::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetPigeonMtmConstant,&CMtfTestActionGetPigeonMtmConstant::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckIfMessageScheduleDateExpired,&CMtfTestActionCheckIfMessageScheduleDateExpired::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionLaunchSchSendExe,&CMtfTestActionLaunchSchSendExe::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionLaunchScheduleMessageExe,&CMtfTestActionLaunchScheduleMessageExe::NewL));

	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionPruneMessages,&CMtfTestActionPruneMessages::NewL));


	// Capability testing related
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvGetEntry,&CMtfTestActionSendEMsvGetEntry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvGetChildren,&CMtfTestActionSendEMsvGetChildren::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvGetRemainingChildren,&CMtfTestActionSendEMsvGetRemainingChildren::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvChangeEntry,&CMtfTestActionSendEMsvChangeEntry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCreateEntry,&CMtfTestActionSendEMsvCreateEntry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvDeleteEntries,&CMtfTestActionSendEMsvDeleteEntries::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvMoveEntries,&CMtfTestActionSendEMsvMoveEntries::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCopyEntries,&CMtfTestActionSendEMsvCopyEntries::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvNotifySessionEvent,&CMtfTestActionSendEMsvNotifySessionEvent::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCancelSessionEventNotification,&CMtfTestActionSendEMsvCancelSessionEventNotification::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvReadStore,&CMtfTestActionSendEMsvReadStore::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvLockStore,&CMtfTestActionSendEMsvLockStore::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvReleaseStore,&CMtfTestActionSendEMsvReleaseStore::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvOperationData,&CMtfTestActionSendEMsvOperationData::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCommandData,&CMtfTestActionSendEMsvCommandData::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCancelOperation,&CMtfTestActionSendEMsvCancelOperation::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvOperationProgress,&CMtfTestActionSendEMsvOperationProgress::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvOperationCompletion,&CMtfTestActionSendEMsvOperationCompletion::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvOperationMtm,&CMtfTestActionSendEMsvOperationMtm::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvMtmCommand,&CMtfTestActionSendEMsvMtmCommand::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvFillRegisteredMtmDllArray,&CMtfTestActionSendEMsvFillRegisteredMtmDllArray::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvInstallMtmGroup,&CMtfTestActionSendEMsvInstallMtmGroup::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvDeInstallMtmGroup,&CMtfTestActionSendEMsvDeInstallMtmGroup::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvUseMtmGroup,&CMtfTestActionSendEMsvUseMtmGroup::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvReleaseMtmGroup,&CMtfTestActionSendEMsvReleaseMtmGroup::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvGetMtmGroupData,&CMtfTestActionSendEMsvGetMtmGroupData::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCloseServer,&CMtfTestActionSendEMsvCloseServer::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvStopService,&CMtfTestActionSendEMsvStopService::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvServiceActive,&CMtfTestActionSendEMsvServiceActive::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvServiceProgress,&CMtfTestActionSendEMsvServiceProgress::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvRemoveEntry,&CMtfTestActionSendEMsvRemoveEntry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvSlotAvailable,&CMtfTestActionSendEMsvSlotAvailable::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvSetSessionAsObserver,&CMtfTestActionSendEMsvSetSessionAsObserver::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvSetFailure,&CMtfTestActionSendEMsvSetFailure::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvChangeAttributes,&CMtfTestActionSendEMsvChangeAttributes::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvGetChildIds,&CMtfTestActionSendEMsvGetChildIds::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvChangeDrive,&CMtfTestActionSendEMsvChangeDrive::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvOutstandingOperations,&CMtfTestActionSendEMsvOutstandingOperations::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvGetNotifySequence,&CMtfTestActionSendEMsvGetNotifySequence::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvReceiveEntyEvents,&CMtfTestActionSendEMsvReceiveEntyEvents::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvDecStoreReaderCount,&CMtfTestActionSendEMsvDecStoreReaderCount::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvGetMessageDrive,&CMtfTestActionSendEMsvGetMessageDrive::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvGetMtmRequiredCapabilities,&CMtfTestActionSendEMsvGetMtmRequiredCapabilities::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCreateAttachmentForWrite,&CMtfTestActionSendEMsvCreateAttachmentForWrite::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvOpenAttachment,&CMtfTestActionSendEMsvOpenAttachment::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvOpenAttachmentForWrite,&CMtfTestActionSendEMsvOpenAttachmentForWrite::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvDeleteAttachment,&CMtfTestActionSendEMsvDeleteAttachment::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvOpenFileStoreForRead,&CMtfTestActionSendEMsvOpenFileStoreForRead::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvOpenTempStoreFile,&CMtfTestActionSendEMsvOpenTempStoreFile::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvReplaceFileStore,&CMtfTestActionSendEMsvReplaceFileStore::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvDeleteFileStore,&CMtfTestActionSendEMsvDeleteFileStore::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvFileStoreExists,&CMtfTestActionSendEMsvFileStoreExists::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvGetAndClearIndexCorruptFlag,&CMtfTestActionSendEMsvGetAndClearIndexCorruptFlag::NewL));

	// PREQ 557
	#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvAddDrive,&CMtfTestActionSendEMsvAddDrive::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvRemoveDrive,&CMtfTestActionSendEMsvRemoveDrive::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvUpdateDrive,&CMtfTestActionSendEMsvUpdateDrive::NewL));

	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvDriveList,&CMtfTestActionSendEMsvDriveList::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvAvailableDriveList,&CMtfTestActionSendEMsvAvailableDriveList::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCurrentDriveInfo,&CMtfTestActionSendEMsvCurrentDriveInfo::NewL));

	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvChildrenOfAvailableDrives,&CMtfTestActionSendEMsvChildrenOfAvailableDrives::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvGetChildIdsAll,&CMtfTestActionSendEMsvGetChildIdsAll::NewL));
	#endif // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	// PREQ 2073
	#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCreateStore,&CMtfTestActionSendEMsvCreateStore::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCheckStore,&CMtfTestActionSendEMsvCheckStore::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCreateHeaderEntry,&CMtfTestActionSendEMsvCreateHeaderEntry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvUpdateHeaderEntry,&CMtfTestActionSendEMsvUpdateHeaderEntry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvLoadHeaderEntry,&CMtfTestActionSendEMsvLoadHeaderEntry::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvDeleteHeaderEntry,&CMtfTestActionSendEMsvDeleteHeaderEntry::NewL));
	#endif // #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCorruptFile,&CMtfTestActionCorruptFile::NewL));

	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSaveMsvIdParamToFile,&CMtfTestActionSaveMsvIdParamToFile::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionLoadMsvIdParamToFile,&CMtfTestActionLoadMsvIdParamToFile::NewL));


    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddFileAttachmentByPath,&CMtfTestActionAddFileAttachmentByPath::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddFileAttachmentByPathSync,&CMtfTestActionAddFileAttachmentByPathSync::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddFileAttachmentByHandle,&CMtfTestActionAddFileAttachmentByHandle::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddFileAttachmentByHandleSync,&CMtfTestActionAddFileAttachmentByHandleSync::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddFileAttachmentWithRevert,&CMtfTestActionAddFileAttachmentWithRevert::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddFileAttachmentWithDestroy,&CMtfTestActionAddFileAttachmentWithDestroy::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddLinkedAttachment,&CMtfTestActionAddLinkedAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddEntryAttachment,&CMtfTestActionAddEntryAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetAttachmentInfoById,&CMtfTestActionGetAttachmentInfoById::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetAttachmentInfoByIndex,&CMtfTestActionGetAttachmentInfoByIndex::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateAttachment,&CMtfTestActionCreateAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareAttachment,&CMtfTestActionCompareAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateAttachmentNoFileClose,&CMtfTestActionCreateAttachmentNoFileClose::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckErrorCode,&CMtfTestActionCheckErrorCode::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCountAttachments,&CMtfTestActionCountAttachments::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetAttachmentIndexFromId,&CMtfTestActionGetAttachmentIndexFromId::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetAttachmentFileFromId,&CMtfTestActionGetAttachmentFileFromId::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetAttachmentFileFromIndex,&CMtfTestActionGetAttachmentFileFromIndex::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetAttachmentFileForWriteFromId,&CMtfTestActionGetAttachmentFileForWriteFromId::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionRemoveFileAttachmentById,&CMtfTestActionRemoveFileAttachmentById::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionRenameFileAttachmentById,&CMtfTestActionRenameFileAttachmentById::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionRemoveFileAttachmentByIndex,&CMtfTestActionRemoveFileAttachmentByIndex::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionRemoveLinkedAttachmentById,&CMtfTestActionRemoveLinkedAttachmentById::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionRemoveEntryAttachmentById,&CMtfTestActionRemoveEntryAttachmentById::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionRemoveFileAttachmentWithRevert,&CMtfTestActionRemoveFileAttachmentWithRevert::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionRemoveFileAttachmentWithDestroy,&CMtfTestActionRemoveFileAttachmentWithDestroy::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateTestMimeHeaders,&CMtfTestActionCreateTestMimeHeaders::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateAttachmentForWrite,&CMtfTestActionCreateAttachmentForWrite::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionOpenAttachment,&CMtfTestActionOpenAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteAttachment,&CMtfTestActionDeleteAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionOpenFileStoreForRead,&CMtfTestActionOpenFileStoreForRead::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionOpenTempStoreFile,&CMtfTestActionOpenTempStoreFile::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionDeleteFileStore,&CMtfTestActionDeleteFileStore::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionFileStoreExists,&CMtfTestActionFileStoreExists::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionReplaceFileStore,&CMtfTestActionReplaceFileStore::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvCopyStore,&CMtfTestActionSendEMsvCopyStore::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvDeleteStore,&CMtfTestActionSendEMsvDeleteStore::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvDriveContainsStore,&CMtfTestActionSendEMsvDriveContainsStore::NewL));


    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionVerifyAttachmentInfo,&CMtfTestActionVerifyAttachmentInfo::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionRemoveAllAttachments,&CMtfTestActionRemoveAllAttachments::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddAttachmentAsLink,&CMtfTestActionAddAttachmentAsLink::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpGetAttachmentFileFromIndex,&CMtfTestActionSmtpGetAttachmentFileFromIndex::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpAddEntryAttachment,&CMtfTestActionSmtpAddEntryAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpAddFileAttachmentByHandleSync,&CMtfTestActionSmtpAddFileAttachmentByHandleSync::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpAddFileAttachmentByPath,&CMtfTestActionSmtpAddFileAttachmentByPath::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpAddLinkedAttachment,&CMtfTestActionSmtpAddLinkedAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpCompareAttachment,&CMtfTestActionSmtpCompareAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpCountAttachments,&CMtfTestActionSmtpCountAttachments::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpCreateAttachment,&CMtfTestActionSmtpCreateAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpGetAttachmentFileFromId,&CMtfTestActionSmtpGetAttachmentFileFromId::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpGetAttachmentInfoById,&CMtfTestActionSmtpGetAttachmentInfoById::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpGetAttachmentInfoByIndex,&CMtfTestActionSmtpGetAttachmentInfoByIndex::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpRemoveAllAttachments,&CMtfTestActionSmtpRemoveAllAttachments::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpRemoveFileAttachmentById,&CMtfTestActionSmtpRemoveFileAttachmentById::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpRemoveFileAttachmentByIndex,&CMtfTestActionSmtpRemoveFileAttachmentByIndex::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpRemoveEntryAttachmentById,&CMtfTestActionSmtpRemoveEntryAttachmentById::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpModifyAttachmentInfo,&CMtfTestActionSmtpModifyAttachmentInfo::NewL));


    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetEntryTypeConstants,&CMtfTestActionGetEntryTypeConstants::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionRenamePath,&CMtfTestActionRenamePath::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddGetStore,&CMtfTestActionGetStore::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddGetEntry,&CMtfTestActionGetEntry::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetIrMTMUId,&CMtfTestActionGetIRMTMUId::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetIrMTMCommandParameters,&CMtfTestActionGetIrMTMCommandParameters::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetBtMTMUId,&CMtfTestActionGetBTMTMUId::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetBtMTMCommandParameters,&CMtfTestActionGetBtMTMCommandParameters::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetSMTPMTMUId,&CMtfTestActionGetSMTPMTMUId::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetUTCOffset,&CMtfTestActionSetUTCOffset::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCreateOrderedChildrenSelection,&CMtfTestActionCreateOrderedChildrenSelection::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareSelections,&CMtfTestActionCompareSelections::NewL));

  	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionAddReadMessage,&CMtfTestActionReadMessage::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckDrmReceipt,&CMtfTestActionCheckDrmReceipt::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckDrmMetaData,&CMtfTestActionCheckDrmMetaData::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCheckMessageStore,&CMtfTestActionCheckMessageStore::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSmtpSendEmail,&CMtfTestActionSmtpSendEmail::NewL));

	// 1667  - Enhanced Search Sort in message store
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendEMsvGetResultCount,&CMtfTestActionSendEMsvGetResultCount::NewL));


    //SendAs

	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionConnectSendAsServer,&CMtfTestActionConnectSendAsServer::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsGetFilteredMessageTypes,&CMtfTestActionSendAsGetFilteredMessageTypes::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsGetMessageTypeUid,&CMtfTestActionSendAsGetMessageTypeUid::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsCreateMessageofType,&CMtfTestActionSendAsCreateMessageofType::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsCloseMessage,&CMtfTestActionSendAsCloseMessage::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCloseSendAsServerConnection,&CMtfTestActionCloseSendAsServerConnection::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsAddAttachment,&CMtfTestActionSendAsAddAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsAddLinkedAttachment,&CMtfTestActionSendAsAddLinkedAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsAddRecipient,&CMtfTestActionSendAsAddRecipient::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsAddMultipleRecipient,&CMtfTestActionSendAsAddMultipleRecipient::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsCheckMultipleRecipientFlag,&CMtfTestActionSendAsCheckMultipleRecipientFlag::NewL));
	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsSaveMessageAndClose,&CMtfTestActionSendAsSaveMessageAndClose::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsLaunchEditorAndClose,&CMtfTestActionSendAsLaunchEditorAndClose::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsCreateAttachment,&CMtfTestActionSendAsCreateAttachment::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsCreateMessageWithAccount,&CMtfTestActionSendAsCreateMessageWithAccount::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsGetAccount,&CMtfTestActionSendAsGetAccount::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsGetAccountNames,&CMtfTestActionSendAsGetAccountNames::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsGetAvailableAccounts,&CMtfTestActionSendAsGetAvailableAccounts::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsGetAvailableMessageTypes,&CMtfTestActionSendAsGetAvailableMessageTypes::NewL));

 	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsSendMessage,&CMtfTestActionSendAsSendMessage::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsSetBioType,&CMtfTestActionSendAsSetBioType::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsSetSubject,&CMtfTestActionSendAsSetSubject::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionVerifyBioType,&CMtfTestActionVerifyBioType::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionVerifyBodyText,&CMtfTestActionVerifyBodyText::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionVerifyRecipients,&CMtfTestActionVerifyRecipients::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionVerifySubject,&CMtfTestActionVerifySubject::NewL));

 	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionVerifySendAsMessageState,&CMtfTestActionVerifySendAsMessageState::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionGetMessageAtIndex,&CMtfTestActionGetMessageAtIndex::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsCloseAndSendMessage,&CMtfTestActionSendAsCloseAndSendMessage::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsSetBodyText,&CMtfTestActionSendAsSetBodyText::NewL));

	AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionCompareEmails,&CMtfTestActionCompareEmails::NewL));
    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSetUserResponse,&CMtfTestActionSetUserResponse::NewL));

    AddTestActionL(CMtfTestActionNamePair::NewL(KTestActionSendAsSendMessageNew,&CMtfTestActionSendAsSendMessageNew::NewL));


    CMtfConfigurationType* imap4Settings = CMtfConfigurationType::NewL(CMtfConfigurationType::EMtfImap4Settings);
    CleanupStack::PushL(imap4Settings);
    imap4Settings->AddConfigurationFilenameL(KDefaultConfigImap4Settings);
    CleanupStack::Pop(imap4Settings);
    SetDefaultConfigurationTypeL(imap4Settings);

    CMtfConfigurationType* pop3Settings = CMtfConfigurationType::NewL(CMtfConfigurationType::EMtfPop3Settings);
    CleanupStack::PushL(pop3Settings);
    pop3Settings->AddConfigurationFilenameL(KDefaultConfigPop3Settings);
    // now add the central repository tests
    CleanupStack::Pop(pop3Settings);
    SetDefaultConfigurationTypeL(pop3Settings);

    CMtfConfigurationType* smtpSettings = CMtfConfigurationType::NewL(CMtfConfigurationType::EMtfSmtpSettings);
    CleanupStack::PushL(smtpSettings);
    smtpSettings->AddConfigurationFilenameL(KDefaultConfigSmtpSettings);
    CleanupStack::Pop(smtpSettings);
    SetDefaultConfigurationTypeL(smtpSettings);

    CMtfConfigurationType* emailFile = CMtfConfigurationType::NewL(CMtfConfigurationType::EMtfEmailFile);
    CleanupStack::PushL(emailFile);
    emailFile->AddConfigurationFilenameL(KDefaultConfigEmailFile);
    CleanupStack::Pop(emailFile);
    SetDefaultConfigurationTypeL(emailFile);


    CMtfConfigurationType* smsSettings = CMtfConfigurationType::NewL(CMtfConfigurationType::EMtfSmsSettings);
    CleanupStack::PushL(smsSettings);
    smsSettings->AddConfigurationFilenameL(KDefaultConfigSmsSettings);
    smsSettings->AddConfigurationFilenameL(KDefaultConfigScheduleSendSettings);
    CleanupStack::Pop(smsSettings);
    SetDefaultConfigurationTypeL(smsSettings);


    _LIT(KMtfPriorityEnum,"TMsvPriority");
    _LIT(KMtfHighPriority,"EMsvHighPriority");
    _LIT(KMtfLowPriority,"EMsvLowPriority");
    _LIT(KMtfMediumPriority,"EMsvMediumPriority");

    CMtfEnumeratorConverter* tmsvpriorityConverter = CMtfEnumeratorConverter::NewL(
    		KNullDesC,KMtfPriorityEnum);
    CleanupStack::PushL(tmsvpriorityConverter);
    tmsvpriorityConverter->AddEnumeratorValueL(KMtfHighPriority,EMsvHighPriority);
    tmsvpriorityConverter->AddEnumeratorValueL(KMtfLowPriority,EMsvLowPriority);
    tmsvpriorityConverter->AddEnumeratorValueL(KMtfMediumPriority,EMsvMediumPriority);
    AddEnumeratorConverterL(tmsvpriorityConverter);
    CleanupStack::Pop(tmsvpriorityConverter);

    _LIT(KMtfSmsDeliveryEnum,"TSmsDelivery");
    _LIT(KMtfInmediately,"ESmsDeliveryImmediately");
    _LIT(KMtfUponRequest,"ESmsDeliveryUponRequest");
    _LIT(KMtfScheduled,"ESmsDeliveryScheduled");
    CMtfEnumeratorConverter* tsmsdeliveryconverter = CMtfEnumeratorConverter::NewL(KNullDesC, KMtfSmsDeliveryEnum);
    CleanupStack::PushL(tsmsdeliveryconverter);
    tsmsdeliveryconverter->AddEnumeratorValueL(KMtfInmediately, ESmsDeliveryImmediately);
    tsmsdeliveryconverter->AddEnumeratorValueL(KMtfUponRequest, ESmsDeliveryUponRequest);
    tsmsdeliveryconverter->AddEnumeratorValueL(KMtfScheduled, ESmsDeliveryScheduled);
    AddEnumeratorConverterL(tsmsdeliveryconverter);
    CleanupStack::Pop(tsmsdeliveryconverter);

    _LIT(KMtfCSmsSettingsName, "CSmsSettings");
    _LIT(KMtfSmsReportHandlingEnum,"TSmsReportHandling");
    _LIT(KMtfReportToInboxInvisible,"EMoveReportToInboxInvisible");
    _LIT(KMtfReportToInboxVisible,"EMoveReportToInboxVisible");
    _LIT(KMtfDiscardReport,"EDiscardReport");
    _LIT(KMtfDoNotWatchForReport,"EDoNotWatchForReport");
    CMtfEnumeratorConverter* tsmsrephandlingconverter = CMtfEnumeratorConverter::NewL(KMtfCSmsSettingsName, KMtfSmsReportHandlingEnum);
    CleanupStack::PushL(tsmsrephandlingconverter);
    tsmsrephandlingconverter->AddEnumeratorValueL(KMtfReportToInboxInvisible, CSmsSettings::EMoveReportToInboxInvisible);
    tsmsrephandlingconverter->AddEnumeratorValueL(KMtfReportToInboxVisible, CSmsSettings::EMoveReportToInboxVisible);
    tsmsrephandlingconverter->AddEnumeratorValueL(KMtfDiscardReport, CSmsSettings::EDiscardReport);
    tsmsrephandlingconverter->AddEnumeratorValueL(KMtfDoNotWatchForReport, CSmsSettings::EDoNotWatchForReport);
    AddEnumeratorConverterL(tsmsrephandlingconverter);
    CleanupStack::Pop(tsmsrephandlingconverter);

    _LIT(KMtfSmsCommdbActionEnum,"TSmsSettingsCommDbAction");
    _LIT(KMtfSmsCommdbNone,"ENone");
    _LIT(KMtfSmsCommdbStoreToCommdb,"EStoreToCommDb");
    CMtfEnumeratorConverter* tsmscommdbactionconverter = CMtfEnumeratorConverter::NewL(KMtfCSmsSettingsName, KMtfSmsCommdbActionEnum);
    CleanupStack::PushL(tsmscommdbactionconverter);
    tsmscommdbactionconverter->AddEnumeratorValueL(KMtfSmsCommdbNone, CSmsSettings::ENone);
    tsmscommdbactionconverter->AddEnumeratorValueL(KMtfSmsCommdbStoreToCommdb, CSmsSettings::EStoreToCommDb);
    AddEnumeratorConverterL(tsmscommdbactionconverter);
    CleanupStack::Pop(tsmscommdbactionconverter);

    _LIT(KMtfTSmsDataCodingSchemeName,"TSmsDataCodingScheme");
    _LIT(KMtfTSmsAlphabetEnum,"TSmsAlphabet");
    _LIT(KMtfSmsAlphabet7Bit,"ESmsAlphabet7Bit");
    _LIT(KMtfSmsAlphabet8Bit,"ESmsAlphabet8Bit");
    _LIT(KMtfSmsAlphabetUCS2,"ESmsAlphabetUCS2");
    CMtfEnumeratorConverter* tsmsalphabetconverter = CMtfEnumeratorConverter::NewL(KMtfTSmsDataCodingSchemeName, KMtfTSmsAlphabetEnum);
    CleanupStack::PushL(tsmsalphabetconverter);
    tsmsalphabetconverter->AddEnumeratorValueL(KMtfSmsAlphabet7Bit, TSmsDataCodingScheme::ESmsAlphabet7Bit);
    tsmsalphabetconverter->AddEnumeratorValueL(KMtfSmsAlphabet8Bit, TSmsDataCodingScheme::ESmsAlphabet8Bit);
    tsmsalphabetconverter->AddEnumeratorValueL(KMtfSmsAlphabetUCS2, TSmsDataCodingScheme::ESmsAlphabetUCS2);
    AddEnumeratorConverterL(tsmsalphabetconverter);
    CleanupStack::Pop(tsmsalphabetconverter);

    _LIT(KMtfTSmsPIDConversionEnum,"TSmsPIDConversion");
    _LIT(KMtfSmsConvPIDNone,"ESmsConvPIDNone");
    _LIT(KMtfSmsConvFax,"ESmsConvFax");
    _LIT(KMtfSmsConvX400,"ESmsConvX400");
    _LIT(KMtfSmsConvPaging,"ESmsConvPaging");
    _LIT(KMtfSmsConvMail,"ESmsConvMail");
    _LIT(KMtfSmsConvErmes,"ESmsConvErmes");
    _LIT(KMtfSmsConvSpeech,"ESmsConvSpeech");
    CMtfEnumeratorConverter* tsmsconversionconverter = CMtfEnumeratorConverter::NewL(KNullDesC, KMtfTSmsPIDConversionEnum);
    CleanupStack::PushL(tsmsconversionconverter);
    tsmsconversionconverter->AddEnumeratorValueL(KMtfSmsConvPIDNone, ESmsConvPIDNone);
    tsmsconversionconverter->AddEnumeratorValueL(KMtfSmsConvFax, ESmsConvFax);
    tsmsconversionconverter->AddEnumeratorValueL(KMtfSmsConvX400, ESmsConvX400);
    tsmsconversionconverter->AddEnumeratorValueL(KMtfSmsConvPaging, ESmsConvPaging);
    tsmsconversionconverter->AddEnumeratorValueL(KMtfSmsConvMail, ESmsConvMail);
    tsmsconversionconverter->AddEnumeratorValueL(KMtfSmsConvErmes, ESmsConvErmes);
    tsmsconversionconverter->AddEnumeratorValueL(KMtfSmsConvSpeech, ESmsConvSpeech);
    AddEnumeratorConverterL(tsmsconversionconverter);
    CleanupStack::Pop(tsmsconversionconverter);

    _LIT(KMtfTSmsFirstOctetName,"TSmsFirstOctet");
    _LIT(KMtfTSmsValidityPeriodFormatEnum,"TSmsValidityPeriodFormat");
    _LIT(KMtfSmsVPFNone,"ESmsVPFNone");
    _LIT(KMtfSmsVPFEnhanced,"ESmsVPFEnhanced");
    _LIT(KMtfSmsVPFInteger,"ESmsVPFInteger");
    _LIT(KMtfSmsVPFSemiOctet,"ESmsVPFSemiOctet");
    CMtfEnumeratorConverter* tsmsvalperiodformatconverter = CMtfEnumeratorConverter::NewL(KMtfTSmsFirstOctetName, KMtfTSmsValidityPeriodFormatEnum);
    CleanupStack::PushL(tsmsvalperiodformatconverter);
    tsmsvalperiodformatconverter->AddEnumeratorValueL(KMtfSmsVPFNone, TSmsFirstOctet::ESmsVPFNone);
    tsmsvalperiodformatconverter->AddEnumeratorValueL(KMtfSmsVPFEnhanced, TSmsFirstOctet::ESmsVPFEnhanced);
    tsmsvalperiodformatconverter->AddEnumeratorValueL(KMtfSmsVPFInteger, TSmsFirstOctet::ESmsVPFInteger);
    tsmsvalperiodformatconverter->AddEnumeratorValueL(KMtfSmsVPFSemiOctet, TSmsFirstOctet::ESmsVPFSemiOctet);
    AddEnumeratorConverterL(tsmsvalperiodformatconverter);
    CleanupStack::Pop(tsmsvalperiodformatconverter);

    _LIT(KMtfRMobileSmsMessagingName,"RMobileSmsMessaging");
    _LIT(KMtfTMobileSmsBearerEnum,"TMobileSmsBearer");
    _LIT(KMtfSmsBearerPacketOnly,"ESmsBearerPacketOnly");
    _LIT(KMtfSmsBearerCircuitOnly,"ESmsBearerCircuitOnly");
    _LIT(KMtfSmsBearerPacketPreferred,"ESmsBearerPacketPreferred");
    _LIT(KMtfSmsBearerCircuitPreferred,"ESmsBearerCircuitPreferred");
    CMtfEnumeratorConverter* tsmsbearerconverter = CMtfEnumeratorConverter::NewL(KMtfRMobileSmsMessagingName, KMtfTMobileSmsBearerEnum);
    CleanupStack::PushL(tsmsbearerconverter);
    tsmsbearerconverter->AddEnumeratorValueL(KMtfSmsBearerPacketOnly, CSmsSettings::ESmsBearerPacketOnly);
    tsmsbearerconverter->AddEnumeratorValueL(KMtfSmsBearerCircuitOnly, CSmsSettings::ESmsBearerCircuitOnly);
    tsmsbearerconverter->AddEnumeratorValueL(KMtfSmsBearerPacketPreferred, CSmsSettings::ESmsBearerPacketPreferred);
    tsmsbearerconverter->AddEnumeratorValueL(KMtfSmsBearerCircuitPreferred, CSmsSettings::ESmsBearerCircuitPreferred);
    AddEnumeratorConverterL(tsmsbearerconverter);
    CleanupStack::Pop(tsmsbearerconverter);



    _LIT(KMtfTFolderSyncType,"TFolderSyncType");
    _LIT(KMtfEUseCombination,"EUseCombination");
    _LIT(KMtfEUseLocal,"EUseLocal");
    _LIT(KMtfEUseRemote,"EUseRemote");
    CMtfEnumeratorConverter* tfoldersynctype = CMtfEnumeratorConverter::NewL(KNullDesC, KMtfTFolderSyncType);
    CleanupStack::PushL(tfoldersynctype);
    tfoldersynctype->AddEnumeratorValueL(KMtfEUseCombination, EUseCombination);
    tfoldersynctype->AddEnumeratorValueL(KMtfEUseLocal, EUseLocal);
    tfoldersynctype->AddEnumeratorValueL(KMtfEUseRemote, EUseRemote);
    AddEnumeratorConverterL(tfoldersynctype);
    CleanupStack::Pop(tfoldersynctype);



    _LIT(KMtfTFolderSubscribeType,"TFolderSubscribeType");
    _LIT(KMtfEUpdateNeither,"EUpdateNeither");
    _LIT(KMtfEUpdateLocal,"UpdateLocal");
    _LIT(KMtfEUpdateRemote,"EUpdateRemote");
    _LIT(KMtfEUpdateBoth,"EUpdateBoth");
    CMtfEnumeratorConverter* tfoldersubscribetype = CMtfEnumeratorConverter::NewL(KNullDesC, KMtfTFolderSubscribeType);
    CleanupStack::PushL(tfoldersubscribetype);
    tfoldersubscribetype->AddEnumeratorValueL(KMtfEUpdateNeither, EUpdateNeither);
    tfoldersubscribetype->AddEnumeratorValueL(KMtfEUpdateLocal, EUpdateLocal);
    tfoldersubscribetype->AddEnumeratorValueL(KMtfEUpdateRemote, EUpdateRemote);
    tfoldersubscribetype->AddEnumeratorValueL(KMtfEUpdateBoth, EUpdateBoth);
    AddEnumeratorConverterL(tfoldersubscribetype);
    CleanupStack::Pop(tfoldersubscribetype);



    _LIT(KMtfTImap4GetMailOptions,"TImap4GetMailOptions");
    _LIT(KMtfGetImap4EmailHeaders,"EGetImap4EmailHeaders");
    _LIT(KMtfGetImap4EmailBodyText,"EGetImap4EmailBodyText");
    _LIT(KMtfGetImap4EmailBodyTextAndAttachments,"EGetImap4EmailBodyTextAndAttachments");
    _LIT(KMtfGetImap4EmailAttachments,"EGetImap4EmailAttachments");
    _LIT(KMtfGetImap4EmailBodyAlternativeText,"EGetImap4EmailBodyAlternativeText");
    CMtfEnumeratorConverter* tfulldownloadmailoptions = CMtfEnumeratorConverter::NewL(KNullDesC, KMtfTImap4GetMailOptions);
    CleanupStack::PushL(tfulldownloadmailoptions);
    tfulldownloadmailoptions->AddEnumeratorValueL(KMtfGetImap4EmailHeaders, EGetImap4EmailHeaders);
    tfulldownloadmailoptions->AddEnumeratorValueL(KMtfGetImap4EmailBodyText, EGetImap4EmailBodyText);
    tfulldownloadmailoptions->AddEnumeratorValueL(KMtfGetImap4EmailBodyTextAndAttachments, EGetImap4EmailBodyTextAndAttachments);
    tfulldownloadmailoptions->AddEnumeratorValueL(KMtfGetImap4EmailAttachments, EGetImap4EmailAttachments);
    tfulldownloadmailoptions->AddEnumeratorValueL(KMtfGetImap4EmailBodyAlternativeText, EGetImap4EmailBodyAlternativeText);
    AddEnumeratorConverterL(tfulldownloadmailoptions);
    CleanupStack::Pop(tfulldownloadmailoptions);


    _LIT(KMtfTImImap4PartialMailOptions,"TImImap4PartialMailOptions");
    _LIT(KMtfImap4EmailNoSizeLimits,"ENoSizeLimits");
    _LIT(KMtfImap4EmailBodyTextOnly,"EBodyTextOnly");
    _LIT(KMtfImap4EmailAttachmentsOnly,"EAttachmentsOnly");
    _LIT(KMtfImap4EmailBodyTextAndAttachments,"EBodyTextAndAttachments");
    _LIT(KMtfImap4EmailCumulative,"ECumulative");
    _LIT(KMtfImapBodyAlternativeText,"EBodyAlternativeText");
    CMtfEnumeratorConverter* tpartialdownloadmailoptions = CMtfEnumeratorConverter::NewL(KNullDesC, KMtfTImImap4PartialMailOptions);
    CleanupStack::PushL(tpartialdownloadmailoptions);
    tpartialdownloadmailoptions->AddEnumeratorValueL(KMtfImap4EmailNoSizeLimits, ENoSizeLimits);
    tpartialdownloadmailoptions->AddEnumeratorValueL(KMtfImap4EmailBodyTextOnly, EBodyTextOnly);
    tpartialdownloadmailoptions->AddEnumeratorValueL(KMtfImap4EmailAttachmentsOnly, EAttachmentsOnly);
    tpartialdownloadmailoptions->AddEnumeratorValueL(KMtfImap4EmailBodyTextAndAttachments, EBodyTextAndAttachments);
    tpartialdownloadmailoptions->AddEnumeratorValueL(KMtfImap4EmailCumulative, ECumulative);
    tpartialdownloadmailoptions->AddEnumeratorValueL(KMtfImapBodyAlternativeText, EBodyAlternativeText);
    AddEnumeratorConverterL(tpartialdownloadmailoptions);
    CleanupStack::Pop(tpartialdownloadmailoptions);


    _LIT(KCMtfConfigurationType,"CMtfConfigurationType");
    _LIT(KTMtfConfigurationType,"TMtfConfigurationType");
    _LIT(KMtfCommDb,"EMtfCommDb");
    _LIT(KMtfDataFile,"EMtfDataFile");
    _LIT(KMtfSmsSettings,"EMtfSmsSettings");
    _LIT(KMtfImap4Settings,"EMtfImap4Settings");
    _LIT(KMtfPop3Settings,"EMtfPop3Settings");
    _LIT(KMtfSmtpSettings,"EMtfSmtpSettings");
    _LIT(KMtfSmsMessage,"EMtfSmsMessage");
    _LIT(KMtfSmilFile,"EMtfSmilFile");
    _LIT(KMtfEmailFile,"EMtfEmailFile");
    _LIT(KMtfInvalidSmsMessage,"EMtfInvalidSmsMessage");
    _LIT(KMtfInvalidSmilFile,"EMtfInvalidSmilFile");
    _LIT(KMtfInvalidEmailFile,"EMtfInvalidEmailFile");
    _LIT(KMtfEmailHeader,"EMtfEmailHeader");

    CMtfEnumeratorConverter* tMtfConfigurationTypeConverter = CMtfEnumeratorConverter::NewL(
    		KCMtfConfigurationType,KTMtfConfigurationType);
    CleanupStack::PushL(tMtfConfigurationTypeConverter);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfCommDb,CMtfConfigurationType::EMtfCommDb);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfDataFile,CMtfConfigurationType::EMtfDataFile);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfSmsSettings,CMtfConfigurationType::EMtfSmsSettings);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfImap4Settings,CMtfConfigurationType::EMtfImap4Settings);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfPop3Settings,CMtfConfigurationType::EMtfPop3Settings);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfSmtpSettings,CMtfConfigurationType::EMtfSmtpSettings);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfSmsMessage,CMtfConfigurationType::EMtfSmsMessage);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfSmilFile,CMtfConfigurationType::EMtfSmilFile);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfEmailFile,CMtfConfigurationType::EMtfEmailFile);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfInvalidSmsMessage,CMtfConfigurationType::EMtfInvalidSmsMessage);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfInvalidSmilFile,CMtfConfigurationType::EMtfInvalidSmilFile);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfInvalidEmailFile,CMtfConfigurationType::EMtfInvalidEmailFile);
    tMtfConfigurationTypeConverter->AddEnumeratorValueL(KMtfEmailHeader,CMtfConfigurationType::EMtfEmailHeader);
    AddEnumeratorConverterL(tMtfConfigurationTypeConverter);
    CleanupStack::Pop(tMtfConfigurationTypeConverter);


    _LIT(KMtfTMsgOutboxBodyEncoding,"TMsgOutboxBodyEncoding");
    _LIT(KMtfEMsgOutboxDefault,"EMsgOutboxDefault");
    _LIT(KMtfEMsgOutboxNoAlgorithm,"EMsgOutboxNoAlgorithm");
    _LIT(KMtfEMsgOutboxMIME,"EMsgOutboxMIME");
    _LIT(KMtfEMsgOutboxMHTMLAsMIME,"EMsgOutboxMHTMLAsMIME");
    _LIT(KMtfEMsgOutboxMHTMLAlternativeAsMIME,"EMsgOutboxMHTMLAlternativeAsMIME");
    CMtfEnumeratorConverter* tmsgoutboxbodyencoding = CMtfEnumeratorConverter::NewL(KNullDesC, KMtfTMsgOutboxBodyEncoding);
    CleanupStack::PushL(tmsgoutboxbodyencoding);
    tmsgoutboxbodyencoding->AddEnumeratorValueL(KMtfEMsgOutboxDefault, ENoSizeLimits);
    tmsgoutboxbodyencoding->AddEnumeratorValueL(KMtfEMsgOutboxNoAlgorithm, EBodyTextOnly);
    tmsgoutboxbodyencoding->AddEnumeratorValueL(KMtfEMsgOutboxMIME, EAttachmentsOnly);
    tmsgoutboxbodyencoding->AddEnumeratorValueL(KMtfEMsgOutboxMHTMLAsMIME, EBodyTextAndAttachments);
    tmsgoutboxbodyencoding->AddEnumeratorValueL(KMtfEMsgOutboxMHTMLAlternativeAsMIME, ECumulative);
    AddEnumeratorConverterL(tmsgoutboxbodyencoding);
    CleanupStack::Pop(tmsgoutboxbodyencoding);


    _LIT(KMtfTImSMTPSendMessageOption,"TImSMTPSendMessageOption");
    _LIT(KMtfESendMessageImmediately,"ESendMessageImmediately");
    _LIT(KMtfESendMessageOnNextConnection,"ESendMessageOnNextConnection");
    _LIT(KMtfESendMessageOnRequest,"ESendMessageOnRequest");
    CMtfEnumeratorConverter* timsmtpsendmessagingoption = CMtfEnumeratorConverter::NewL(KNullDesC, KMtfTImSMTPSendMessageOption);
    CleanupStack::PushL(timsmtpsendmessagingoption);
    timsmtpsendmessagingoption->AddEnumeratorValueL(KMtfESendMessageImmediately, ESendMessageImmediately);
    timsmtpsendmessagingoption->AddEnumeratorValueL(KMtfESendMessageOnNextConnection, ESendMessageOnNextConnection);
    timsmtpsendmessagingoption->AddEnumeratorValueL(KMtfESendMessageOnRequest, ESendMessageOnRequest);
    AddEnumeratorConverterL(timsmtpsendmessagingoption);
    CleanupStack::Pop(timsmtpsendmessagingoption);


    _LIT(KMtfTImSMTPSendCopyToSelf,"TImSMTPSendCopyToSelf");
    _LIT(KMtfESendNoCopy,"ESendNoCopy");
    _LIT(KMtfESendCopyAsToRecipient,"ESendCopyAsToRecipient");
    _LIT(KMtfESendCopyAsCcRecipient,"ESendCopyAsCcRecipient");
    _LIT(KMtfESendCopyAsBccRecipient,"ESendCopyAsBccRecipient");
    CMtfEnumeratorConverter* timsmtpsendcopytoself = CMtfEnumeratorConverter::NewL(KNullDesC, KMtfTImSMTPSendCopyToSelf);
    CleanupStack::PushL(timsmtpsendcopytoself);
    timsmtpsendcopytoself->AddEnumeratorValueL(KMtfESendNoCopy, ESendNoCopy);
    timsmtpsendcopytoself->AddEnumeratorValueL(KMtfESendCopyAsToRecipient, ESendCopyAsToRecipient);
    timsmtpsendcopytoself->AddEnumeratorValueL(KMtfESendCopyAsCcRecipient, ESendCopyAsCcRecipient);
    timsmtpsendcopytoself->AddEnumeratorValueL(KMtfESendCopyAsBccRecipient, ESendCopyAsBccRecipient);
    AddEnumeratorConverterL(timsmtpsendcopytoself);
    CleanupStack::Pop(timsmtpsendcopytoself);


    _LIT(KMtfTCommDbDialogPref,"TCommDbDialogPref");
    _LIT(KMtfECommDbDialogPrefUnknown,"ECommDbDialogPrefUnknown");
    _LIT(KMtfECommDbDialogPrefPrompt,"ECommDbDialogPrefPrompt");
    _LIT(KMtfECommDbDialogPrefWarn,"ECommDbDialogPrefWarn");
    _LIT(KMtfECommDbDialogPrefDoNotPrompt,"ECommDbDialogPrefDoNotPrompt");
    _LIT(KMtfECommDbDialogPrefPromptIfWrongMode,"ECommDbDialogPrefPromptIfWrongMode");
    CMtfEnumeratorConverter* tcommsbdialogpref = CMtfEnumeratorConverter::NewL(KNullDesC, KMtfTCommDbDialogPref);
    CleanupStack::PushL(tcommsbdialogpref);
    tcommsbdialogpref->AddEnumeratorValueL(KMtfECommDbDialogPrefUnknown, ECommDbDialogPrefUnknown);
    tcommsbdialogpref->AddEnumeratorValueL(KMtfECommDbDialogPrefPrompt, ECommDbDialogPrefPrompt);
    tcommsbdialogpref->AddEnumeratorValueL(KMtfECommDbDialogPrefWarn, ECommDbDialogPrefWarn);
    tcommsbdialogpref->AddEnumeratorValueL(KMtfECommDbDialogPrefDoNotPrompt, ECommDbDialogPrefDoNotPrompt);
    tcommsbdialogpref->AddEnumeratorValueL(KMtfECommDbDialogPrefPromptIfWrongMode, ECommDbDialogPrefPromptIfWrongMode);
    AddEnumeratorConverterL(tcommsbdialogpref);
    CleanupStack::Pop(tcommsbdialogpref);

	_LIT(KMtfRSendAsMessageName, "RSendAsMessage");
	_LIT(KMtfTSendAsRecipientType,"TSendAsRecipientType");
	_LIT(KMtfESendAsRecipientTo,"ESendAsRecipientTo");
	_LIT(KMtfESendAsRecipientCc,"ESendAsRecipientCc");
	_LIT(KMtfESendAsRecipientBcc,"ESendAsRecipientBcc");
	CMtfEnumeratorConverter* tSendAsRecipientType = CMtfEnumeratorConverter::NewL(KMtfRSendAsMessageName, KMtfTSendAsRecipientType);
	CleanupStack::PushL(tSendAsRecipientType);
	tSendAsRecipientType->AddEnumeratorValueL(KMtfESendAsRecipientTo, RSendAsMessage::ESendAsRecipientTo);
	tSendAsRecipientType->AddEnumeratorValueL(KMtfESendAsRecipientCc, RSendAsMessage::ESendAsRecipientCc);
	tSendAsRecipientType->AddEnumeratorValueL(KMtfESendAsRecipientBcc,RSendAsMessage::ESendAsRecipientBcc);
	AddEnumeratorConverterL(tSendAsRecipientType);
	CleanupStack::Pop(tSendAsRecipientType);


	_LIT(KMtfTImHeaderEncodingInfo, "TImHeaderEncodingInfo");
	_LIT(KMtfTFieldList,"TFieldList");
	_LIT(KMtfETo,"ETo");
	_LIT(KMtfECc,"ECc");
	_LIT(KMtfEBcc,"EBcc");
	CMtfEnumeratorConverter* tfieldList = CMtfEnumeratorConverter::NewL(KMtfTImHeaderEncodingInfo, KMtfTFieldList);
	CleanupStack::PushL(tfieldList);
	tfieldList->AddEnumeratorValueL(KMtfETo, TImHeaderEncodingInfo::ETo);
	tfieldList->AddEnumeratorValueL(KMtfECc, TImHeaderEncodingInfo::ECc);
	tfieldList->AddEnumeratorValueL(KMtfEBcc,TImHeaderEncodingInfo::EBcc);
	AddEnumeratorConverterL(tfieldList);
	CleanupStack::Pop(tfieldList);


	_LIT(KMtfTBioMsgIdType, "TBioMsgIdType");
	_LIT(KMtfEBioMsgIdUnknown,"EBioMsgIdUnknown");
	_LIT(KMtfEBioMsgIdIana,"EBioMsgIdIana");
	_LIT(KMtfEBioMsgIdNbs,"EBioMsgIdNbs");
	_LIT(KMtfEBioMsgIdWap,"EBioMsgIdWap");
	_LIT(KMtfEBioMsgIdWapSecure,"EBioMsgIdWapSecure");
	_LIT(KMtfEBioMsgIdWsp,"EBioMsgIdWsp");
	_LIT(KMtfEBioMsgIdWspSecure,"EBioMsgIdWspSecure");
	CMtfEnumeratorConverter* tBioMsgIdType = CMtfEnumeratorConverter::NewL(KNullDesC, KMtfTBioMsgIdType);
	CleanupStack::PushL(tBioMsgIdType);
	tBioMsgIdType->AddEnumeratorValueL(KMtfEBioMsgIdUnknown, EBioMsgIdUnknown);
	tBioMsgIdType->AddEnumeratorValueL(KMtfEBioMsgIdIana, EBioMsgIdIana);
	tBioMsgIdType->AddEnumeratorValueL(KMtfEBioMsgIdNbs,EBioMsgIdNbs);
	tBioMsgIdType->AddEnumeratorValueL(KMtfEBioMsgIdWap, EBioMsgIdWap);
	tBioMsgIdType->AddEnumeratorValueL(KMtfEBioMsgIdWapSecure, EBioMsgIdWapSecure);
	tBioMsgIdType->AddEnumeratorValueL(KMtfEBioMsgIdWsp, EBioMsgIdWsp);
	tBioMsgIdType->AddEnumeratorValueL(KMtfEBioMsgIdWspSecure, EBioMsgIdWspSecure);
	AddEnumeratorConverterL(tBioMsgIdType);
	CleanupStack::Pop(tBioMsgIdType);

	_LIT(KMtfCMsvSendOperation,"CMsvSendOperation");
	_LIT(KMtfTSendOperationState, "TSendOperationState");
	_LIT(KMtfESendStateInPreparation,"ESendStateInPreparation");
	_LIT(KMtfESendStateWaitingToSend, "ESendStateWaitingToSend");
	_LIT(KMtfESendStateConnecting, "ESendStateConnecting");
	_LIT(KMtfESendStateSending, "ESendStateSending");
	_LIT(KMtfESendStateDone, "ESendStateDone");
	_LIT(KMtfESendStateFailed, "ESendStateFailed");
	CMtfEnumeratorConverter* tSendOperationState = CMtfEnumeratorConverter::NewL(KMtfCMsvSendOperation, KMtfTSendOperationState);
	CleanupStack::PushL(tSendOperationState);
	tSendOperationState->AddEnumeratorValueL(KMtfESendStateInPreparation, CMsvSendOperation::ESendStateInPreparation);
	tSendOperationState->AddEnumeratorValueL(KMtfESendStateWaitingToSend, CMsvSendOperation::ESendStateWaitingToSend);
	tSendOperationState->AddEnumeratorValueL(KMtfESendStateConnecting, CMsvSendOperation::ESendStateConnecting);
	tSendOperationState->AddEnumeratorValueL(KMtfESendStateSending, CMsvSendOperation::ESendStateSending);
	tSendOperationState->AddEnumeratorValueL(KMtfESendStateDone, CMsvSendOperation::ESendStateDone);
	tSendOperationState->AddEnumeratorValueL(KMtfESendStateFailed, CMsvSendOperation::ESendStateFailed);
	AddEnumeratorConverterL(tSendOperationState);
	CleanupStack::Pop(tSendOperationState);
    }


    /** Converts an enumerator value to the corresponding numerical value. */
TInt CMtfTestServer::ConvertEnumeratorL(const TDesC& aParameter) const
    {
  	return FindEnumeratorConverterL(aParameter).ConvertL(aParameter);
    }

    /** Find the correct converter */
const CMtfEnumeratorConverter& CMtfTestServer::FindEnumeratorConverterL(const TDesC& aParameter) const
    {
    TInt count = iEnumeratorConverters.Count();
    TInt i;
    TBool found = EFalse;

    CMtfEnumeratorConverter* converter = CMtfEnumeratorConverter::NewL(aParameter);

    for (i=0; i<count; i++)
	    {
    	if (*iEnumeratorConverters[i] == *converter)
	    	{
    		found = ETrue;
    		break;
	    	}
    }

    delete converter;

    if (!found)
	    {
    	User::Leave(KErrNotFound);
	    }

    return *iEnumeratorConverters[i];
    }

    /** Returns true if the specified enumerator converter already exists. */
TBool CMtfTestServer::EnumeratorConverterExists(const CMtfEnumeratorConverter& aConverter) const
    {
    TInt count = iEnumeratorConverters.Count();
    TInt i;

    for (i=0; i<count; i++)
	    {
    	if (*iEnumeratorConverters[i] == aConverter)
			{
    		return ETrue;
			}
		}

    return EFalse;
    }

/** Takes ownership of enumerator converter at the end. */
void CMtfTestServer::AddEnumeratorConverterL(CMtfEnumeratorConverter* aEnumeratorConverter)
    {
    // check that the same converter does not already exist
    __ASSERT_ALWAYS(EnumeratorConverterExists(*aEnumeratorConverter)==EFalse,User::Panic(KMtfInternalError,0));

    User::LeaveIfError(iEnumeratorConverters.Append(aEnumeratorConverter));
    }

/** This function takes ownership of aConfiguration immediately
and therefore SHOULD NOT be called with aConfiguration already on the Cleanup stack. */
void CMtfTestServer::SetMainScriptConfigurationTypeL(CMtfConfigurationType* aConfiguration)
    {
    CMtfConfigurationType::SetConfigurationTypeL(aConfiguration,iMainScriptConfigurations);
    }

/** This function takes ownership of aConfiguration immediately
and therefore SHOULD NOT be called with aConfiguration already on the Cleanup stack. */
void CMtfTestServer::SetDefaultConfigurationTypeL(CMtfConfigurationType* aConfiguration)
    {
    CMtfConfigurationType::SetConfigurationTypeL(aConfiguration,iDefaultConfigurations);
    }

const TDesC& CMtfTestServer::GetDefaultConfigurationFileL(const CMtfConfigurationType::TMtfConfigurationType& aType) const
    {
    return iDefaultConfigurations[CMtfConfigurationType::FindConfigurationL(aType,iDefaultConfigurations)]->ConfigurationFileL(0);
    }

const TDesC& CMtfTestServer::GetMainScriptConfigurationFileL(const CMtfConfigurationType::TMtfConfigurationType& aType, TInt aIndex) const
    {
    return iMainScriptConfigurations[CMtfConfigurationType::FindConfigurationL(aType,iMainScriptConfigurations)]->ConfigurationFileL(aIndex);
    }

/** Finds the correct test case name pair and uses it to create a new test case instance. */
CMtfTestCase* CMtfTestServer::CreateTestCaseL(const TDesC& aCaseName) const
    {
    TInt count = iAvailableTestCases.Count();

    for (TInt i=0; i<count; i++)
    	{
    	if (iAvailableTestCases[i]->Name() == aCaseName)
    		{
    		return iAvailableTestCases[i]->CreateTestCaseL(*this);
    		}
    	}

    User::Leave(KErrNotFound);
    return NULL;
    }

/** Takes ownership of action parameters at the end. The new action is owned by
the current test case. */
const CMtfTestAction& CMtfTestServer::CreateTestActionL(const TDesC& aActionName,
    	CMtfTestActionParameters* aActionParameters, const TDesC& aActionId) const
    {
    CMtfTestAction* newAction = CreateTestActionPtrL(aActionName,aActionParameters);
    newAction->SetActionIdL(aActionId);
    return *newAction;
    }

/** Takes ownership of action parameters at the end. The new action is owned by
the current test case. */
const CMtfTestAction& CMtfTestServer::CreateTestActionL(const TDesC& aActionName,
    	CMtfTestActionParameters* aActionParameters) const
    {
    return *CreateTestActionPtrL(aActionName,aActionParameters);
    }

/** Takes ownership of action parameters at the end. The new action is owned by
the current test case. */
CMtfTestAction* CMtfTestServer::CreateTestActionPtrL(const TDesC& aActionName,
    	CMtfTestActionParameters* aActionParameters) const
    {
    if (aActionName == KWaitForAll)
    	{
    	__ASSERT_ALWAYS(aActionParameters->Count()==0,User::Panic(KMtfInvalidScript,0));
    	return CMtfTestActionWait::NewL(CurrentTestCase(),aActionParameters);
    	}
    else if (aActionName == KWaitForActions)
    	{
    	__ASSERT_ALWAYS(aActionParameters->Count()>0,User::Panic(KMtfInvalidScript,0));
    	return CMtfTestActionWait::NewL(CurrentTestCase(),aActionParameters);
    	}

    TInt count = iAvailableTestActions.Count();

    for (TInt i=0; i<count; i++)
	    {
    	if (iAvailableTestActions[i]->Name() == aActionName)
			{
    		return iAvailableTestActions[i]->CreateTestActionL(
    			CurrentTestCase(),aActionParameters);
			}
		}

    User::Leave(KErrNotFound);
    return NULL;
    }

/** Takes ownership of a test case immediately. aTestCase should not be on the cleanup
stack when this function is called. */
void CMtfTestServer::SetCurrentTestCase(CMtfTestCase* aTestCase)
    {
    delete iCurrentTestCase;
    iCurrentTestCase = aTestCase;
    }

CMtfTestCase& CMtfTestServer::CurrentTestCase() const
    {
    return *iCurrentTestCase;
    }

/** Takes ownership of test action/name pair immediately. */
void CMtfTestServer::AddTestActionL(CMtfTestActionNamePair* aPair)
    {
    CleanupStack::PushL(aPair);
    User::LeaveIfError(iAvailableTestActions.Append(aPair));
    CleanupStack::Pop(aPair);
    }

/** Takes ownership of test case/name pair immediately. */
void CMtfTestServer::AddTestCaseL(CMtfTestCaseNamePair* aPair)
    {
    CleanupStack::PushL(aPair);
    User::LeaveIfError(iAvailableTestCases.Append(aPair));
    CleanupStack::Pop(aPair);
    }

CTestStep* CMtfTestServer::CreateTestStep(const TDesC& aStepName)
    {
    CTestStep* testStep=NULL;
    TRAPD(error,testStep=CreateTestStepL(aStepName));

    if (error)
    	{
    	return NULL;
    	}

    return testStep;
    }


/** Test server checks the structure of the test case and creates the
appropriate objects to handle the test case actions and sections.
@return - A CTestStep derived instance */
CTestStep* CMtfTestServer::CreateTestStepL(const TDesC& aStepName)
    {
    CTestStep* newTestStep = NULL;

    if (aStepName == KBeginTestCase)
    	{
    	__ASSERT_ALWAYS(iScriptState==CMtfTestCase::EMtfTestCaseInitialState,User::Panic(KMtfInvalidScript,0));
    	__ASSERT_ALWAYS(iInsideScriptedTestCase==EFalse,User::Panic(KMtfInvalidScript,0));
    	iInsideScriptedTestCase = ETrue;
    	iScriptState = CMtfTestCase::EMtfTestCaseExecutingSetup;
    	newTestStep = CMtfTestBeginManager::NewL(*this,ETrue);
    	}
    else if (aStepName == KSetConfiguration)
    	{
    	__ASSERT_ALWAYS(iInsideScriptedTestCase==EFalse,User::Panic(KMtfInvalidScript,0));
    	newTestStep = CMtfSetConfigurationTestStep::NewL(*this);
    	}
    else if (aStepName == KBeginTestCaseAsync)
    	{
    	__ASSERT_ALWAYS(iScriptState==CMtfTestCase::EMtfTestCaseInitialState,User::Panic(KMtfInvalidScript,0));
    	__ASSERT_ALWAYS(iInsideScriptedTestCase==EFalse,User::Panic(KMtfInvalidScript,0));
    	iInsideScriptedTestCase = ETrue;
    	iScriptState = CMtfTestCase::EMtfTestCaseExecutingSetup;
    	newTestStep = CMtfTestBeginManager::NewL(*this,EFalse);
    	}
    else if (aStepName == KSetupTestCase)
    	{
    	__ASSERT_ALWAYS(iScriptState==CMtfTestCase::EMtfTestCaseExecutingSetup,User::Panic(KMtfInvalidScript,0));
    	__ASSERT_ALWAYS(iInsideScriptedTestCase,User::Panic(KMtfInvalidScript,0));

    	iScriptState = CMtfTestCase::EMtfTestCaseExecutingTest;
    	// does nothing
    	newTestStep = CMtfTestActionManager::NewL(*this);
    	}
    else if (aStepName == KExecuteTestCase)
    	{
    	__ASSERT_ALWAYS(iScriptState==CMtfTestCase::EMtfTestCaseExecutingTest,User::Panic(KMtfInvalidScript,0));
    	__ASSERT_ALWAYS(iInsideScriptedTestCase,User::Panic(KMtfInvalidScript,0));
    	iScriptState = CMtfTestCase::EMtfTestCaseCheckingPostconditions;
    	CMtfTestActionSectionComplete::NewL(*iCurrentTestCase);
    	newTestStep = CMtfTestActionManager::NewL(*this);
    	}
    else if (aStepName == KCheckPostconditions)
    	{
    	__ASSERT_ALWAYS(iScriptState==CMtfTestCase::EMtfTestCaseCheckingPostconditions,User::Panic(KMtfInvalidScript,0));
    	__ASSERT_ALWAYS(iInsideScriptedTestCase,User::Panic(KMtfInvalidScript,0));
    	iScriptState = CMtfTestCase::EMtfTestCaseTestCompleted;
    	CMtfTestActionSectionComplete::NewL(*iCurrentTestCase);
    	newTestStep = CMtfTestActionManager::NewL(*this);
    	}
    else if (aStepName == KEndTestCase)
    	{
    	__ASSERT_ALWAYS(iScriptState==CMtfTestCase::EMtfTestCaseTestCompleted,User::Panic(KMtfInvalidScript,0));
    	__ASSERT_ALWAYS(iInsideScriptedTestCase,User::Panic(KMtfInvalidScript,0));
    	iInsideScriptedTestCase = EFalse;
    	CMtfTestActionSectionComplete::NewL(*iCurrentTestCase);
    	newTestStep = iCurrentTestCase;
    	}
    else if (iInsideScriptedTestCase)
    	{
    	// must be an action
    	newTestStep = CMtfTestActionManager::NewL(*this,aStepName);
    	}
    else
    	{
    	// must be a test case
    	newTestStep = CreateTestCaseL(aStepName);
    	}

    return newTestStep;
    }

LOCAL_C void MainL()
    {
#if (defined __DATA_CAGING__)
    RProcess().DataCaging(RProcess::EDataCagingOn);
    RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
    CActiveScheduler* sched=NULL;
    sched=new(ELeave) CActiveScheduler;
    CActiveScheduler::Install(sched);

    CMtfTestServer* server = NULL;
    TRAPD(err,server = CMtfTestServer::NewL());
    if(!err)
    	{
    	RProcess::Rendezvous(KErrNone);
    	sched->Start();
    	}
    delete server;
    delete sched;
    }



GLDEF_C TInt E32Main()
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

