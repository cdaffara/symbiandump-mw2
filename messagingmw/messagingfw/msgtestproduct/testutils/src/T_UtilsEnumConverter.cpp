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
// This file contains the utility functions to map a string to the corresponding enumerator values.
// 
//

// User includes
#include <t_utilsenumconverter.h>

// epoc includes
#include <msvids.h>
#include <cdbcols.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

// Literals Used

// Local Folder Ids
_LIT(KStrGlobalInBoxIndexEntryId,"KMsvGlobalInBoxIndexEntryId");
_LIT(KStrGlobalOutBoxIndexEntryId,"KMsvGlobalOutBoxIndexEntryId");
_LIT(KStrSentEntryId,"KMsvSentEntryId");
_LIT(KStrDraftEntryId,"KMsvDraftEntryId");
_LIT(KStrDeletedEntryFolderEntryId,"KMsvDeletedEntryFolderEntryId");
_LIT(KStrRootIndexEntryId,"KMsvRootIndexEntryId");
_LIT(KStrTempIndexEntryId,"KMsvTempIndexEntryId");
_LIT(KStrLocalServiceIndexEntryId,"KMsvLocalServiceIndexEntryId");
_LIT(KStrUnknownServiceIndexEntryId,"KMsvUnknownServiceIndexEntryId");
_LIT(KStrFirstFreeEntryId,"KFirstFreeEntryId");


//Pop3 Specific Commands
_LIT(KStrPOP3MTMIsConnected, "KPOP3MTMIsConnected");
_LIT(KStrPOP3MTMConnect,"KPOP3MTMConnect");
_LIT(KStrPOP3MTMDisconnect,"KPOP3MTMDisconnect");	
_LIT(KStrPOP3MTMCopyNewMailWhenAlreadyConnected,"KPOP3MTMCopyNewMailWhenAlreadyConnected");
_LIT(KStrPOP3MTMConnectAndCopyNewMailAndStayOnline,"KPOP3MTMConnectAndCopyNewMailAndStayOnline");
_LIT(KStrPOP3MTMConnectAndCopyNewMailAndDisconnect,"KPOP3MTMConnectAndCopyNewMailAndDisconnect");
_LIT(KStrPOP3MTMMoveNewMailWhenAlreadyConnected,"KPOP3MTMMoveNewMailWhenAlreadyConnected");
_LIT(KStrPOP3MTMConnectAndMoveNewMailAndStayOnline,"KPOP3MTMConnectAndMoveNewMailAndStayOnline");
_LIT(KStrPOP3MTMConnectAndMoveNewMailAndDisconnect,"KPOP3MTMConnectAndMoveNewMailAndDisconnect");
_LIT(KStrPOP3MTMCopyMailSelectionWhenAlreadyConnected,"KPOP3MTMCopyMailSelectionWhenAlreadyConnected");
_LIT(KStrPOP3MTMConnectAndCopyMailSelectionAndStayOnline,"KPOP3MTMConnectAndCopyMailSelectionAndStayOnline");
_LIT(KStrPOP3MTMConnectAndCopyMailSelectionAndDisconnect,"KPOP3MTMConnectAndCopyMailSelectionAndDisconnect");
_LIT(KStrPOP3MTMMoveMailSelectionWhenAlreadyConnected,"KPOP3MTMMoveMailSelectionWhenAlreadyConnected");
_LIT(KStrPOP3MTMConnectAndMoveMailSelectionAndStayOnline,"KPOP3MTMConnectAndMoveMailSelectionAndStayOnline");
_LIT(KStrPOP3MTMConnectAndMoveMailSelectionAndDisconnect,"KPOP3MTMConnectAndMoveMailSelectionAndDisconnect");
_LIT(KStrPOP3MTMCopyAllMailWhenAlreadyConnected,"KPOP3MTMCopyAllMailWhenAlreadyConnected");
_LIT(KStrPOP3MTMConnectAndCopyAllMailAndStayOnline,"KPOP3MTMConnectAndCopyAllMailAndStayOnline");
_LIT(KStrPOP3MTMConnectAndCopyAllMailAndDisconnect,"KPOP3MTMConnectAndCopyAllMailAndDisconnect");
_LIT(KStrPOP3MTMMoveAllMailWhenAlreadyConnected,"KPOP3MTMMoveAllMailWhenAlreadyConnected");
_LIT(KStrPOP3MTMConnectAndMoveAllMailAndStayOnline,"KPOP3MTMConnectAndMoveAllMailAndStayOnline");
_LIT(KStrPOP3MTMConnectAndMoveAllMailAndDisconnect,"KPOP3MTMConnectAndMoveAllMailAndDisconnect");
_LIT(KStrPOP3MTMCancelOfflineOperations,"KPOP3MTMCancelOfflineOperations");
_LIT(KStrPOP3MTMCancelSendOnNextConnection,"KPOP3MTMCancelSendOnNextConnection");
_LIT(KStrPOP3MTMCreateNewEmailMessage,"KPOP3MTMCreateNewEmailMessage");
_LIT(KStrPOP3MTMCreateReplyEmailMessage,"KPOP3MTMCreateReplyEmailMessage");
_LIT(KStrPOP3MTMCreateForwardEmailMessage,"KPOP3MTMCreateForwardEmailMessage");
_LIT(KStrPOP3MTMCreateForwardAsAttachmentEmailMessage,"KPOP3MTMCreateForwardAsAttachmentEmailMessage");
_LIT(KStrPOP3MTMCreateReceiptEmailMessage,"KPOP3MTMCreateReceiptEmailMessage");
_LIT(KStrPOP3MTMPopulate,"KPOP3MTMPopulate");
_LIT(KStrPOP3MTMPopulateNew,"KPOP3MTMPopulateNew");
_LIT(KStrPOP3MTMPopulateAll,"KPOP3MTMPopulateAll");


// TImapCmds specific Enumerations
_LIT(KStrIMAP4MTMIsConnected, "KIMAP4MTMIsConnected");
_LIT(KStrIMAP4MTMConnect, "KIMAP4MTMConnect");
_LIT(KStrIMAP4MTMConnectAndSynchronise, "KIMAP4MTMConnectAndSynchronise");
_LIT(KStrIMAP4MTMCancelBackgroundSynchronise, "KIMAP4MTMCancelBackgroundSynchronise");
_LIT(KStrIMAP4MTMStartBatch, "KIMAP4MTMStartBatch");
_LIT(KStrIMAP4MTMEndBatch, "KIMAP4MTMEndBatch");
_LIT(KStrIMAP4MTMBusy, "KIMAP4MTMBusy");
_LIT(KStrIMAP4MTMSelect, "KIMAP4MTMSelect");
_LIT(KStrIMAP4MTMSynchronise, "KIMAP4MTMSynchronise");
_LIT(KStrIMAP4MTMSyncTree, "KIMAP4MTMSyncTree");
_LIT(KStrIMAP4MTMReserved10, "KIMAP4MTMReserved10");
_LIT(KStrIMAP4MTMFullSync, "KIMAP4MTMFullSync");
_LIT(KStrIMAP4MTMDisconnect, "KIMAP4MTMDisconnect");
_LIT(KStrIMAP4MTMLocalSubscribe, "KIMAP4MTMLocalSubscribe");
_LIT(KStrIMAP4MTMLocalUnsubscribe, "KIMAP4MTMLocalUnsubscribe");
_LIT(KStrIMAP4MTMReserved15, "KIMAP4MTMReserved15");
_LIT(KStrIMAP4MTMReserved16, "KIMAP4MTMReserved16");
_LIT(KStrIMAP4MTMInboxNewSync, "KIMAP4MTMInboxNewSync");
_LIT(KStrIMAP4MTMFolderFullSync, "KIMAP4MTMFolderFullSync");
_LIT(KStrIMAP4MTMWaitForBackground, "KIMAP4MTMWaitForBackground");
_LIT(KStrIMAP4MTMRenameFolder, "KIMAP4MTMRenameFolder");
_LIT(KStrIMAP4MTMUndeleteAll, "KIMAP4MTMUndeleteAll");
_LIT(KStrIMAP4MTMCancelOffLineOperations, "KIMAP4MTMCancelOffLineOperations");
_LIT(KStrIMAP4MTMPopulate, "KIMAP4MTMPopulate");
_LIT(KStrIMAP4MTMSyncSubscription, "KIMAP4MTMSyncSubscription");
_LIT(KStrIMAP4MTMConnectAndSyncCompleteAfterConnect, "KIMAP4MTMConnectAndSyncCompleteAfterConnect");
_LIT(KStrIMAP4MTMConnectAndSyncCompleteAfterFullSync, "KIMAP4MTMConnectAndSyncCompleteAfterFullSync");
_LIT(KStrIMAP4MTMConnectAndSyncCompleteAfterDisconnect, "KIMAP4MTMConnectAndSyncCompleteAfterDisconnect");
_LIT(KStrIMAP4MTMCancelSendOnNextConnection, "KIMAP4MTMCancelSendOnNextConnection");
_LIT(KStrIMAP4MTMCopyNewMailWhenAlreadyConnected, "KIMAP4MTMCopyNewMailWhenAlreadyConnected");
_LIT(KStrIMAP4MTMConnectAndCopyNewMailAndStayOnline, "KIMAP4MTMConnectAndCopyNewMailAndStayOnline");
_LIT(KStrIMAP4MTMConnectAndCopyNewMailAndDisconnect, "KIMAP4MTMConnectAndCopyNewMailAndDisconnect");
_LIT(KStrIMAP4MTMMoveNewMailWhenAlreadyConnected, "KIMAP4MTMMoveNewMailWhenAlreadyConnected");
_LIT(KStrIMAP4MTMConnectAndMoveNewMailAndStayOnline, "KIMAP4MTMConnectAndMoveNewMailAndStayOnline");
_LIT(KStrIMAP4MTMConnectAndMoveNewMailAndDisconnect, "KIMAP4MTMConnectAndMoveNewMailAndDisconnect");
_LIT(KStrIMAP4MTMCopyMailSelectionWhenAlreadyConnected, "KIMAP4MTMCopyMailSelectionWhenAlreadyConnected");
_LIT(KStrIMAP4MTMConnectAndCopyMailSelectionAndStayOnline, "KIMAP4MTMConnectAndCopyMailSelectionAndStayOnline");
_LIT(KStrIMAP4MTMConnectAndCopyMailSelectionAndDisconnect, "KIMAP4MTMConnectAndCopyMailSelectionAndDisconnect");
_LIT(KStrIMAP4MTMMoveMailSelectionWhenAlreadyConnected, "KIMAP4MTMMoveMailSelectionWhenAlreadyConnected");
_LIT(KStrIMAP4MTMConnectAndMoveMailSelectionAndStayOnline, "KIMAP4MTMConnectAndMoveMailSelectionAndStayOnline");
_LIT(KStrIMAP4MTMConnectAndMoveMailSelectionAndDisconnect, "KIMAP4MTMConnectAndMoveMailSelectionAndDisconnect");
_LIT(KStrIMAP4MTMCopyAllMailWhenAlreadyConnected, "KIMAP4MTMCopyAllMailWhenAlreadyConnected");
_LIT(KStrIMAP4MTMConnectAndCopyAllMailAndStayOnline, "KIMAP4MTMConnectAndCopyAllMailAndStayOnline");
_LIT(KStrIMAP4MTMConnectAndCopyAllMailAndDisconnect, "KIMAP4MTMConnectAndCopyAllMailAndDisconnect");
_LIT(KStrIMAP4MTMMoveAllMailWhenAlreadyConnected, "KIMAP4MTMMoveAllMailWhenAlreadyConnected");
_LIT(KStrIMAP4MTMConnectAndMoveAllMailAndStayOnline, "KIMAP4MTMConnectAndMoveAllMailAndStayOnline");
_LIT(KStrIMAP4MTMConnectAndMoveAllMailAndDisconnect, "KIMAP4MTMConnectAndMoveAllMailAndDisconnect");
_LIT(KStrIMAP4MTMPopulateMailSelectionWhenAlreadyConnected, "KIMAP4MTMPopulateMailSelectionWhenAlreadyConnected");
_LIT(KStrIMAP4MTMConnectAndPopulateMailSelectionAndStayOnline, "KIMAP4MTMConnectAndPopulateMailSelectionAndStayOnline");
_LIT(KStrIMAP4MTMConnectAndPopulateMailSelectionAndDisconnect, "KIMAP4MTMConnectAndPopulateMailSelectionAndDisconnect");
_LIT(KStrIMAP4MTMPopulateNewMailWhenAlreadyConnected, "KIMAP4MTMPopulateNewMailWhenAlreadyConnected");
_LIT(KStrIMAP4MTMPopulateAllMailWhenAlreadyConnected, "KIMAP4MTMPopulateAllMailWhenAlreadyConnected");
_LIT(KStrIMAP4MTMConnectAndPopulateNewMailAndStayOnline, "KIMAP4MTMConnectAndPopulateNewMailAndStayOnline");
_LIT(KStrIMAP4MTMConnectAndPopulateNewMailAndDisconnect, "KIMAP4MTMConnectAndPopulateNewMailAndDisconnect");
_LIT(KStrIMAP4MTMConnectAndPopulateAllMailAndStayOnline, "KIMAP4MTMConnectAndPopulateAllMailAndStayOnline");
_LIT(KStrIMAP4MTMConnectAndPopulateAllMailAndDisconnect, "KIMAP4MTMConnectAndPopulateAllMailAndDisconnect");
_LIT(KStrIMAP4MTMCreateNewEmailMessage, "KIMAP4MTMCreateNewEmailMessage");
_LIT(KStrIMAP4MTMCreateReplyEmailMessage, "KIMAP4MTMCreateReplyEmailMessage");
_LIT(KStrIMAP4MTMCreateForwardEmailMessage, "KIMAP4MTMCreateForwardEmailMessage");
_LIT(KStrIMAP4MTMCreateForwardAsAttachmentEmailMessage, "KIMAP4MTMCreateForwardAsAttachmentEmailMessage");
_LIT(KStrIMAP4MTMCreateReceiptEmailMessage, "KIMAP4MTMCreateReceiptEmailMessage");


// TSmtpCmds specific Enumerations
_LIT(KStrSMTPMTMCreateNewEmailMessage, "KSMTPMTMCreateNewEmailMessage");
_LIT(KStrSMTPMTMCreateReplyEmailMessage, "KSMTPMTMCreateReplyEmailMessage");
_LIT(KStrSMTPMTMCreateForwardEmailMessage, "KSMTPMTMCreateForwardEmailMessage");
_LIT(KStrSMTPMTMCreateForwardAsAttachmentEmailMessage, "KSMTPMTMCreateForwardAsAttachmentEmailMessage");
_LIT(KStrSMTPMTMCreateReceiptEmailMessage, "KSMTPMTMCreateReceiptEmailMessage");
_LIT(KStrSMTPMTMSendOnNextConnection, "KSMTPMTMSendOnNextConnection");

// DownloadType specific Enumerations
//_LIT(KStrPartialDownload,"PartialDownload");
//_LIT(KStrFullDownload,"FullDownload");


// TCommDbDialogPref specific Enumerations
//_LIT(KStrECommDbDialogPrefUnknown, "TCommDbDialogPref::ECommDbDialogPrefUnknown");
_LIT(KStrECommDbDialogPrefPrompt, "TCommDbDialogPref::ECommDbDialogPrefPrompt");
_LIT(KStrECommDbDialogPrefWarn, "TCommDbDialogPref::ECommDbDialogPrefWarn");
_LIT(KStrECommDbDialogPrefDoNotPrompt,"TCommDbDialogPref::ECommDbDialogPrefDoNotPrompt");
_LIT(KStrECommDbDialogPrefPromptIfWrongMode, "TCommDbDialogPref::ECommDbDialogPrefPromptIfWrongMode");


// TImap4GetMailOptions specific Enumerations
//_LIT(KStrEGetImap4EmailHeaders, "TImap4GetMailOptions::EGetImap4EmailHeaders");
_LIT(KStrEGetImap4EmailBodyText, "TImap4GetMailOptions::EGetImap4EmailBodyText");
_LIT(KStrEGetImap4EmailBodyTextAndAttachments, "TImap4GetMailOptions::EGetImap4EmailBodyTextAndAttachments");
_LIT(KStrEGetImap4EmailAttachments, "TImap4GetMailOptions::EGetImap4EmailAttachments");
_LIT(KStrEGetImap4EmailBodyAlternativeText, "TImap4GetMailOptions::EGetImap4EmailBodyAlternativeText");


// TFolderSyncType specific Enumerations
_LIT(KStrEUseCombination, "TFolderSyncType::EUseCombination");
_LIT(KStrEUseLocal, "TFolderSyncType::EUseLocal");
//_LIT(KStrEUseRemote, "TFolderSyncType::EUseRemote");	


// TFolderSubscribeType specific Enumerations
//_LIT(KStrEUpdateNeither, "TFolderSubscribeType::EUpdateNeither");
_LIT(KStrEUpdateLocal, "TFolderSubscribeType::EUpdateLocal");
_LIT(KStrEUpdateRemote, "TFolderSubscribeType::EUpdateRemote");
_LIT(KStrEUpdateBoth, "TFolderSubscribeType::EUpdateBoth");


// TImImap4PartialMailOptions specific Enumerations
//_LIT(KStrENoSizeLimits, "TImImap4PartialMailOptions::ENoSizeLimits");
_LIT(KStrEBodyTextOnly, "TImImap4PartialMailOptions::EBodyTextOnly");
_LIT(KStrEAttachmentsOnly, "TImImap4PartialMailOptions::EAttachmentsOnly");
_LIT(KStrEBodyTextAndAttachments, "TImImap4PartialMailOptions::EBodyTextAndAttachments");
_LIT(KStrECumulative, "TImImap4PartialMailOptions::ECumulative");
_LIT(KStrEBodyAlternativeText, "TImImap4PartialMailOptions::EBodyAlternativeText");


// TMsgOutboxBodyEncoding specific Enumerations
//_LIT(KStrEMsgOutboxDefault, "TMsgOutboxBodyEncoding::EMsgOutboxDefault");
_LIT(KStrEMsgOutboxNoAlgorithm, "TMsgOutboxBodyEncoding::EMsgOutboxNoAlgorithm");
_LIT(KStrEMsgOutboxMIME, "TMsgOutboxBodyEncoding::EMsgOutboxMIME");
_LIT(KStrEMsgOutboxMHTMLAsMIME, "TMsgOutboxBodyEncoding::EMsgOutboxMHTMLAsMIME");
_LIT(KStrEMsgOutboxMHTMLAlternativeAsMIME, "TMsgOutboxBodyEncoding::EMsgOutboxMHTMLAlternativeAsMIME");


// TImSMTPSendMessageOption specific Enumerations
//_LIT(KStrESendMessageImmediately , "TImSMTPSendMessageOption::ESendMessageImmediately ");
_LIT(KStrESendMessageOnNextConnection, "TImSMTPSendMessageOption::ESendMessageOnNextConnection");
_LIT(KStrESendMessageOnRequest, "TImSMTPSendMessageOption::ESendMessageOnRequest");


// TImSMTPSendCopyToSelf specific Enumerations
//_LIT(KStrESendNoCopy, "TImSMTPSendCopyToSelf::ESendNoCopy");
_LIT(KStrESendCopyAsToRecipient, "TImSMTPSendCopyToSelf::ESendCopyAsToRecipient");
_LIT(KStrESendCopyAsCcRecipient, "TImSMTPSendCopyToSelf::ESendCopyAsCcRecipient");
_LIT(KStrESendCopyAsBccRecipient, "TImSMTPSendCopyToSelf::ESendCopyAsBccRecipient");


// TPop3GetMailOptions specific Enumerations
//_LIT(KStrEGetPop3EmailHeaders , "TPop3GetMailOptions::EGetPop3EmailHeaders");
_LIT(KStrEGetPop3EmailMessages, "TPop3GetMailOptions::EGetPop3EmailMessages");


// RAM Usage Logging Eventcodes
_LIT(KStrImapRamUsageStart, "TInstrumentationPoint::EImapRamUsageStart");
_LIT(KStrImapRamUsageStop, "TInstrumentationPoint::EImapRamUsageStop");
_LIT(KStrPopRamUsageStart, "TInstrumentationPoint::EPopRamUsageStart");
_LIT(KStrPopRamUsageStop, "TInstrumentationPoint::EPopRamUsageStop");
_LIT(KStrSmtpRamUsageStart, "TInstrumentationPoint::ESmtpRamUsageStart");
_LIT(KStrSmtpRamUsageStop, "TInstrumentationPoint::ESmtpRamUsageStop");
_LIT(KStrSmtpRamUsageCreateMessageStart, "TInstrumentationPoint::ESmtpRamUsageCreateMessageStart");
_LIT(KStrSmtpRamUsageCreateMessageStop, "TInstrumentationPoint::ESmtpRamUsageCreateMessageStop");
_LIT(KStrSmtpRamUsageSendMessageStart, "TInstrumentationPoint::ESmtpRamUsageSendMessageStart");
_LIT(KStrSmtpRamUsageSendMessageStop, "TInstrumentationPoint::ESmtpRamUsageSendMessageStop");

// Performance Speed Eventcodes
_LIT(KStrSmtpCreateMessageSpeedStart, "TInstrumentationPoint::ESmtpCreateMessageSpeedStart");
_LIT(KStrSmtpCreateMessageSpeedStop, "TInstrumentationPoint::ESmtpCreateMessageSpeedStop");
_LIT(KStrSmtpSendMessageSpeedStart, "TInstrumentationPoint::ESmtpSendMessageSpeedStart");
_LIT(KStrSmtpSendMessageSpeedStop, "TInstrumentationPoint::ESmtpSendMessageSpeedStop");
_LIT(KStrImapDownloadMessageSpeedStart, "TInstrumentationPoint::EImapDownloadMessageSpeedStart");
_LIT(KStrImapDownloadMessageSpeedStop, "TInstrumentationPoint::EImapDownloadMessageSpeedStop");
_LIT(KStrESearchSortStart, "TInstrumentationPoint::ESearchSortStart");
_LIT(KStrESearchSortStop, "TInstrumentationPoint::ESearchSortStop");

_LIT(KStrImapCopyMessageSpeedStart, "TInstrumentationPoint::EImapCopyMessageSpeedStart");
_LIT(KStrImapCopyMessageSpeedStop, "TInstrumentationPoint::EImapCopyMessageSpeedStop");
_LIT(KStrImapMoveMessageSpeedStart, "TInstrumentationPoint::EImapMoveMessageSpeedStart");
_LIT(KStrImapMoveMessageSpeedStop, "TInstrumentationPoint::EImapMoveMessageSpeedStop");
_LIT(KStrImapDeleteMessageSpeedStart, "TInstrumentationPoint::EImapDeleteMessageSpeedStart");
_LIT(KStrImapDeleteMessageSpeedStop, "TInstrumentationPoint::EImapDeleteMessageSpeedStop");


// Bearer types
_LIT(KStrBearerTypeUnknown, "Unknown");
_LIT(KStrBearerTypeCsd, "CSD");
_LIT(KStrBearerTypeWcdma, "Wcdma");
_LIT(KStrBearerTypeLan, "LAN");
_LIT(KStrBearerTypeVirtual, "Virtual");
_LIT(KStrBearerTypePan, "PAN");
_LIT(KStrBearerTypeWlan, "WLAN");
_LIT(KStrBearerTypePsd, "PSD");
_LIT(KStrBearerTypeCsdLan, "CSDLAN");

//SearchSortOptions
_LIT(KSearchForToAddress,"To");
_LIT(KSearchForCCField,"CC");
_LIT(KSearchForBCCField,"BCC");
_LIT(KSearchForSubjectField,"Subject");
_LIT(KSearchForDateField,"Date");
_LIT(KSearchForTimeField,"Time");
_LIT(KSearchForBodyPart,"BodyPart");

// Message parts used for search/sort operation
_LIT(KStrMsvAttachment, "TMsvMessagePart::EMsvAttachment");
_LIT(KStrMsvBcc, "TMsvMessagePart::EMsvBcc");
_LIT(KStrMsvBody, "TMsvMessagePart::EMsvBody");
_LIT(KStrMsvCc, "TMsvMessagePart::EMsvCc");
_LIT(KStrMsvDate, "TMsvMessagePart::EMsvDate");
_LIT(KStrMsvDescription, "TMsvMessagePart::EMsvDescription");
_LIT(KStrMsvDetails, "TMsvMessagePart::EMsvDetails");
_LIT(KStrMsvFrom, "TMsvMessagePart::EMsvFrom");
_LIT(KStrMsvPriority, "TMsvMessagePart::EMsvPriority");
_LIT(KStrMsvNew, "TMsvMessagePart::EMsvNew");
_LIT(KStrMsvSize, "TMsvMessagePart::EMsvSize");
_LIT(KStrMsvSubject, "TMsvMessagePart::EMsvSubject");
_LIT(KStrMsvTo, "TMsvMessagePart::EMsvTo");
_LIT(KStrEMsvMtmTypeUID, "TMsvMessagePart::EMsvMtmTypeUID");
_LIT(KStrMsvUnreadMessages, "TMsvMessagePart::EMsvUnreadMessages");

// Relational operators to be used along with search criteria
_LIT(KStrEMsvEqual, "TMsvRelationOp::EMsvEqual");
_LIT(KStrEMsvNotEqual, "TMsvRelationOp::EMsvNotEqual");
_LIT(KStrEMsvLessThan, "TMsvRelationOp::EMsvLessThan");
_LIT(KStrEMsvGreaterThan, "TMsvRelationOp::EMsvGreaterThan");
_LIT(KStrEMsvLessThanOrEqual, "TMsvRelationOp::EMsvLessThanOrEqual");
_LIT(KStrEMsvGreaterThanOrEqual, "TMsvRelationOp::EMsvGreaterThanOrEqual");

// Message priority enumerations
_LIT(KStrEMsvLowPriority, "TMsvPriority::EMsvLowPriority");
_LIT(KStrEMsvMediumPriority, "TMsvPriority::EMsvMediumPriority");
_LIT(KStrEMsvHighPriority, "TMsvPriority::EMsvHighPriority");

// Message sorting order enumerations
_LIT(KStrEMsvSortAscending, "TMsvSortOrder::EMsvSortAscending");
_LIT(KStrEMsvSortDescending, "TMsvSortOrder::EMsvSortDescending");


/**
CT_MsgUtilsEnumConverter()
Constructor
*/
EXPORT_C CT_MsgUtilsEnumConverter::CT_MsgUtilsEnumConverter()
	{
	}

/**
~CT_MsgUtilsEnumConverter()
Destructor
*/
CT_MsgUtilsEnumConverter::~CT_MsgUtilsEnumConverter()
	{
	}


/**
FindFolderIdByName()
Returns the ID of the local folder based on the folder name.
If the folder name given does not match then it returns -1.

@param aFolderName
Folder name whose ID need to be returned. Folder name is assumed to be in string form
of the standard folder ID. Eg., GlobalInBoxIndexEntryId

@return
ID of the folder
*/
EXPORT_C TMsvId CT_MsgUtilsEnumConverter::FindFolderIdByName(TDesC& aFolderName)
	{
	TMsvId folderId;

	STR_SWITCH(aFolderName)
	
	STR_CASE(KStrGlobalInBoxIndexEntryId)
		{
		folderId = KMsvGlobalInBoxIndexEntryId;
		break;
		}
	STR_CASE(KStrGlobalOutBoxIndexEntryId)
		{
		folderId = KMsvGlobalOutBoxIndexEntryId;
		break;
		}
	STR_CASE(KStrSentEntryId)
		{
		folderId = KMsvSentEntryId;
		break;
		}
	STR_CASE(KStrDraftEntryId)
		{
		folderId = KMsvDraftEntryId;
		break;
		}
	STR_CASE(KStrDeletedEntryFolderEntryId)
		{
		folderId = KMsvDeletedEntryFolderEntryId;
		break;
		}
	STR_CASE(KStrRootIndexEntryId)
		{
		folderId = KMsvRootIndexEntryId;
		break;
		}
	STR_CASE(KStrTempIndexEntryId)
		{
		folderId = KMsvTempIndexEntryId;
		break;
		}
	STR_CASE(KStrLocalServiceIndexEntryId)
		{
		folderId = KMsvLocalServiceIndexEntryId;
		break;
		}
	STR_CASE(KStrUnknownServiceIndexEntryId)
		{
		folderId = KMsvUnknownServiceIndexEntryId;
		break;
		}
	STR_CASE(KStrFirstFreeEntryId)
		{
		folderId = KFirstFreeEntryId;
		break;
		}
	DEFAULT_CASE()	// match not found
		{
		folderId = KErrNotFound;
		}
		
	STR_SWITCH_END()

	return folderId;
	}


/**
ConvertDesToTPop3Cmds()
Returns the corresponding Pop3 Command

@param aPopCmdName
POP command name

@return
POP3-specific commands
*/
EXPORT_C TPop3Cmds CT_MsgUtilsEnumConverter::ConvertDesToTPop3Cmds(const TDesC& aPopCmdName)
	{
	TPop3Cmds ret = KPOP3MTMConnect;

	STR_SWITCH(aPopCmdName)

	STR_CASE(KStrPOP3MTMIsConnected)
		{
		ret = KPOP3MTMIsConnected;
		break;
		}	
	STR_CASE(KStrPOP3MTMConnect)
		{
		ret = KPOP3MTMConnect;
		break;
		}
	STR_CASE(KStrPOP3MTMDisconnect )
		{
		ret = KPOP3MTMDisconnect;
		break;
		}
	STR_CASE(KStrPOP3MTMCopyNewMailWhenAlreadyConnected )
		{
		ret = KPOP3MTMCopyNewMailWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndCopyNewMailAndStayOnline)
		{
		ret = KPOP3MTMConnectAndCopyNewMailAndStayOnline;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndCopyNewMailAndDisconnect)
		{
		ret = KPOP3MTMConnectAndCopyNewMailAndDisconnect;
		break;
		}
	STR_CASE(KStrPOP3MTMMoveNewMailWhenAlreadyConnected)
		{
		ret = KPOP3MTMMoveNewMailWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndMoveNewMailAndStayOnline)
		{
		ret = KPOP3MTMConnectAndMoveNewMailAndStayOnline;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndMoveNewMailAndDisconnect)
		{
		ret = KPOP3MTMConnectAndMoveNewMailAndDisconnect;
		break;
		}
	STR_CASE(KStrPOP3MTMCopyMailSelectionWhenAlreadyConnected)
		{
		ret = KPOP3MTMCopyMailSelectionWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndCopyMailSelectionAndStayOnline)
		{
		ret = KPOP3MTMConnectAndCopyMailSelectionAndStayOnline;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndCopyMailSelectionAndDisconnect)
		{
		ret = KPOP3MTMConnectAndCopyMailSelectionAndDisconnect;
		break;
		}
	STR_CASE(KStrPOP3MTMMoveMailSelectionWhenAlreadyConnected)
		{
		ret = KPOP3MTMMoveMailSelectionWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndMoveMailSelectionAndStayOnline)
		{
		ret = KPOP3MTMConnectAndMoveMailSelectionAndStayOnline;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndMoveMailSelectionAndDisconnect)
		{
		ret = KPOP3MTMConnectAndMoveMailSelectionAndDisconnect;
		break;
		}
	STR_CASE(KStrPOP3MTMCopyAllMailWhenAlreadyConnected)
		{
		ret = KPOP3MTMCopyAllMailWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndCopyAllMailAndStayOnline)
		{
		ret = KPOP3MTMConnectAndCopyAllMailAndStayOnline;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndCopyAllMailAndDisconnect)
		{
		ret = KPOP3MTMConnectAndCopyAllMailAndDisconnect;
		break;
		}
	STR_CASE(KStrPOP3MTMMoveAllMailWhenAlreadyConnected)
		{
		ret = KPOP3MTMMoveAllMailWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndMoveAllMailAndStayOnline)
		{
		ret = KPOP3MTMConnectAndMoveAllMailAndStayOnline;
		break;
		}
	STR_CASE(KStrPOP3MTMConnectAndMoveAllMailAndDisconnect)
		{
		ret = KPOP3MTMConnectAndMoveAllMailAndDisconnect;
		break;
		}
	STR_CASE(KStrPOP3MTMCancelOfflineOperations)
		{
		ret = KPOP3MTMCancelOfflineOperations;
		break;
		}
	STR_CASE(KStrPOP3MTMCancelSendOnNextConnection)
		{
		ret = KPOP3MTMCancelSendOnNextConnection;
		break;
		}
	STR_CASE(KStrPOP3MTMCreateNewEmailMessage)
		{
		ret = KPOP3MTMCreateNewEmailMessage;
		break;
		}
	STR_CASE(KStrPOP3MTMCreateReplyEmailMessage)
		{
		ret = KPOP3MTMCreateReplyEmailMessage;
		break;
		}		
	STR_CASE(KStrPOP3MTMCreateForwardEmailMessage)
		{
		ret = KPOP3MTMCreateForwardEmailMessage;
		break;
		}
	STR_CASE(KStrPOP3MTMCreateForwardAsAttachmentEmailMessage)
		{
		ret = KPOP3MTMCreateForwardAsAttachmentEmailMessage;
		break;
		}
	STR_CASE(KStrPOP3MTMCreateReceiptEmailMessage)
		{
		ret = KPOP3MTMCreateReceiptEmailMessage;
		break;
		}
	STR_CASE(KStrPOP3MTMPopulate)
		{
		ret = KPOP3MTMPopulate;
		break;
		}
	STR_CASE(KStrPOP3MTMPopulateNew)
		{
		ret = KPOP3MTMPopulateNew;
		break;
		}		
	STR_CASE(KStrPOP3MTMPopulateAll)
		{
		ret = KPOP3MTMPopulateAll;
		break;
		}

	DEFAULT_CASE()
		{
		ret = KPOP3MTMConnect;
		}
		
	STR_SWITCH_END()
	
	return ret;
	}


/**
ConvertDesToTImap4Cmds()
Returns the corresponding IMAP4 Command

@param aImapCmdName
IMAP command name

@return
IMAP4-specific commands
*/
EXPORT_C TImap4Cmds CT_MsgUtilsEnumConverter::ConvertDesToTImap4Cmds(TDesC& aImapCmdName)
	{
	TImap4Cmds  ret;
	STR_SWITCH(aImapCmdName)
		
	STR_CASE(KStrIMAP4MTMIsConnected)
		{
		ret = KIMAP4MTMIsConnected;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnect)
		{
		ret = KIMAP4MTMConnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndSynchronise)
		{
		ret = KIMAP4MTMConnectAndSynchronise;
		break;
		}				
	STR_CASE(KStrIMAP4MTMCancelBackgroundSynchronise)
		{
		ret = KIMAP4MTMCancelBackgroundSynchronise;
		break;
		}
	STR_CASE(KStrIMAP4MTMStartBatch)
		{
		ret = KIMAP4MTMStartBatch;
		break;
		}
	STR_CASE(KStrIMAP4MTMEndBatch)
		{
		ret = KIMAP4MTMEndBatch;
		break;
		}
	STR_CASE(KStrIMAP4MTMBusy)
		{
		ret = KIMAP4MTMBusy;
		break;
		}
	STR_CASE(KStrIMAP4MTMSelect)
		{
		ret = KIMAP4MTMSelect;
		break;
		}
	STR_CASE(KStrIMAP4MTMSynchronise)
		{
		ret = KIMAP4MTMSynchronise;
		break;
		}
	STR_CASE(KStrIMAP4MTMSyncTree)
		{
		ret = KIMAP4MTMSyncTree;
		break;
		}				
	STR_CASE(KStrIMAP4MTMReserved10)
		{
		ret = KIMAP4MTMReserved10;
		break;
		}
	STR_CASE(KStrIMAP4MTMFullSync)
		{
		ret = KIMAP4MTMFullSync;
		break;
		}
	STR_CASE(KStrIMAP4MTMDisconnect)
		{
		ret = KIMAP4MTMDisconnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMLocalSubscribe)
		{
		ret = KIMAP4MTMLocalSubscribe;
		break;
		}
	STR_CASE(KStrIMAP4MTMLocalUnsubscribe)
		{
		ret = KIMAP4MTMLocalUnsubscribe;
		break;
		}					
	STR_CASE(KStrIMAP4MTMReserved15)
		{
		ret = KIMAP4MTMReserved15;
		break;
		}					
	STR_CASE(KStrIMAP4MTMReserved16)
		{
		ret = KIMAP4MTMReserved16;
		break;
		}					
	STR_CASE(KStrIMAP4MTMInboxNewSync)
		{
		ret = KIMAP4MTMInboxNewSync;
		break;
		}					
	STR_CASE(KStrIMAP4MTMFolderFullSync)
		{
		ret = KIMAP4MTMFolderFullSync;
		break;
		}					
	STR_CASE(KStrIMAP4MTMWaitForBackground)
		{
		ret = KIMAP4MTMWaitForBackground;
		break;
		}					
	STR_CASE(KStrIMAP4MTMRenameFolder)
		{
		ret = KIMAP4MTMRenameFolder;
		break;
		}					
	STR_CASE(KStrIMAP4MTMUndeleteAll)
		{
		ret = KIMAP4MTMUndeleteAll;
		break;
		}			
							
	STR_CASE(KStrIMAP4MTMCancelOffLineOperations)
		{
		ret = KIMAP4MTMCancelOffLineOperations;
		break;
		}
	STR_CASE(KStrIMAP4MTMPopulate)
		{
		ret = KIMAP4MTMPopulate;
		break;
		}
	STR_CASE(KStrIMAP4MTMSyncSubscription)
		{
		ret = KIMAP4MTMSyncSubscription;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndSyncCompleteAfterConnect)
		{
		ret = KIMAP4MTMConnectAndSyncCompleteAfterConnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndSyncCompleteAfterFullSync)
		{
		ret = KIMAP4MTMConnectAndSyncCompleteAfterFullSync;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndSyncCompleteAfterDisconnect)
		{
		ret = KIMAP4MTMConnectAndSyncCompleteAfterDisconnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMCancelSendOnNextConnection)
		{
		ret = KIMAP4MTMCancelSendOnNextConnection;
		break;
		}
	STR_CASE(KStrIMAP4MTMCopyNewMailWhenAlreadyConnected)
		{
		ret = KIMAP4MTMCopyNewMailWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndCopyNewMailAndStayOnline)
		{
		ret = KIMAP4MTMConnectAndCopyNewMailAndStayOnline;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndCopyNewMailAndDisconnect)
		{
		ret = KIMAP4MTMConnectAndCopyNewMailAndDisconnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMMoveNewMailWhenAlreadyConnected)
		{
		ret = KIMAP4MTMMoveNewMailWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndMoveNewMailAndStayOnline)
		{
		ret = KIMAP4MTMConnectAndMoveNewMailAndStayOnline;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndMoveNewMailAndDisconnect)
		{
		ret = KIMAP4MTMConnectAndMoveNewMailAndDisconnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMCopyMailSelectionWhenAlreadyConnected)
		{
		ret = KIMAP4MTMCopyMailSelectionWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndCopyMailSelectionAndStayOnline)
		{
		ret = KIMAP4MTMConnectAndCopyMailSelectionAndStayOnline;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndCopyMailSelectionAndDisconnect)
		{
		ret = KIMAP4MTMConnectAndCopyMailSelectionAndDisconnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMMoveMailSelectionWhenAlreadyConnected)
		{
		ret = KIMAP4MTMMoveMailSelectionWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndMoveMailSelectionAndStayOnline)
		{
		ret = KIMAP4MTMConnectAndMoveMailSelectionAndStayOnline;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndMoveMailSelectionAndDisconnect)
		{
		ret = KIMAP4MTMConnectAndMoveMailSelectionAndDisconnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMCopyAllMailWhenAlreadyConnected)
		{
		ret = KIMAP4MTMCopyAllMailWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndCopyAllMailAndStayOnline)
		{
		ret = KIMAP4MTMConnectAndCopyAllMailAndStayOnline;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndCopyAllMailAndDisconnect)
		{
		ret = KIMAP4MTMConnectAndCopyAllMailAndDisconnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMMoveAllMailWhenAlreadyConnected)
		{
		ret = KIMAP4MTMMoveAllMailWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndMoveAllMailAndStayOnline)
		{
		ret = KIMAP4MTMConnectAndMoveAllMailAndStayOnline;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndMoveAllMailAndDisconnect)
		{
		ret = KIMAP4MTMConnectAndMoveAllMailAndDisconnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMPopulateMailSelectionWhenAlreadyConnected)
		{
		ret = KIMAP4MTMPopulateMailSelectionWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndPopulateMailSelectionAndStayOnline)
		{
		ret = KIMAP4MTMConnectAndPopulateMailSelectionAndStayOnline;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndPopulateMailSelectionAndDisconnect)
		{
		ret = KIMAP4MTMConnectAndPopulateMailSelectionAndDisconnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMPopulateNewMailWhenAlreadyConnected)
		{
		ret = KIMAP4MTMPopulateNewMailWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrIMAP4MTMPopulateAllMailWhenAlreadyConnected)
		{
		ret = KIMAP4MTMPopulateAllMailWhenAlreadyConnected;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndPopulateNewMailAndStayOnline)
		{
		ret = KIMAP4MTMConnectAndPopulateNewMailAndStayOnline;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndPopulateNewMailAndDisconnect)
		{
		ret = KIMAP4MTMConnectAndPopulateNewMailAndDisconnect;
		break;
		}		
	STR_CASE(KStrIMAP4MTMConnectAndPopulateAllMailAndStayOnline)
		{
		ret = KIMAP4MTMConnectAndPopulateAllMailAndStayOnline;
		break;
		}
	STR_CASE(KStrIMAP4MTMConnectAndPopulateAllMailAndDisconnect)
		{
		ret = KIMAP4MTMConnectAndPopulateAllMailAndDisconnect;
		break;
		}
	STR_CASE(KStrIMAP4MTMCreateNewEmailMessage)
		{
		ret = KIMAP4MTMCreateNewEmailMessage;
		break;
		}
	STR_CASE(KStrIMAP4MTMCreateReplyEmailMessage)
		{
		ret = KIMAP4MTMCreateReplyEmailMessage;
		break;
		}
	STR_CASE(KStrIMAP4MTMCreateForwardEmailMessage)
		{
		ret = KIMAP4MTMCreateForwardEmailMessage;
		break;
		}
	STR_CASE(KStrIMAP4MTMCreateForwardAsAttachmentEmailMessage)
		{
		ret = KIMAP4MTMCreateForwardAsAttachmentEmailMessage;
		break;
		}
	STR_CASE(KStrIMAP4MTMCreateReceiptEmailMessage)
		{
		ret = KIMAP4MTMCreateReceiptEmailMessage;
		break;
		}
	DEFAULT_CASE()
		{
		ret = KIMAP4MTMConnectAndSynchronise;
		}
		
	STR_SWITCH_END()	
	return ret;
	}


/**
ConvertDesToTSmtpCmdsL()
Returns the corresponding SMTP Command

@param aSmtpCmdName
SMTP command name

@return
SMTP-specific commands
*/
EXPORT_C TSmtpCmds CT_MsgUtilsEnumConverter::ConvertDesToTSmtpCmdsL(TDesC& aSmtpCmdName)
	{
	TSmtpCmds  ret = KSMTPMTMCreateNewEmailMessage;
	STR_SWITCH(aSmtpCmdName)
		
	STR_CASE(KStrSMTPMTMCreateNewEmailMessage)
		{
		ret = KSMTPMTMCreateNewEmailMessage;
		break;
		}
	STR_CASE(KStrSMTPMTMCreateReplyEmailMessage)
		{
		ret = KSMTPMTMCreateReplyEmailMessage;
		break;
		}
	STR_CASE(KStrSMTPMTMCreateForwardEmailMessage)
		{
		ret = KSMTPMTMCreateForwardEmailMessage;
		break;
		}
	STR_CASE(KStrSMTPMTMCreateForwardAsAttachmentEmailMessage)
		{
		ret = KSMTPMTMCreateForwardAsAttachmentEmailMessage;
		break;
		}
	STR_CASE(KStrSMTPMTMCreateReceiptEmailMessage)
		{
		ret = KSMTPMTMCreateReceiptEmailMessage;
		break;
		}
	STR_CASE(KStrSMTPMTMSendOnNextConnection)
		{
		ret = KSMTPMTMSendOnNextConnection;
		break;
		}

	DEFAULT_CASE()
		{
		User::Leave(KErrNotSupported);
		}
		
	STR_SWITCH_END()	
	return ret;
	}

	
/**
ConvertDesToTCommDbDialogPref()
Returns the corresponding TCommDbDialogPref Enumeration

@param aDialogPrefStr
@return 
TCommDbDialogPref enumeration
*/	
EXPORT_C TCommDbDialogPref CT_MsgUtilsEnumConverter::ConvertDesToTCommDbDialogPref(TDesC& aDialogPrefStr)
	{
	TCommDbDialogPref dialogPref;

	STR_SWITCH(aDialogPrefStr)

	STR_CASE(KStrECommDbDialogPrefPrompt)
		{
		dialogPref = ECommDbDialogPrefPrompt;
		break;
		}
	STR_CASE(KStrECommDbDialogPrefWarn)
		{
		dialogPref = ECommDbDialogPrefWarn;
		break;
		}
	STR_CASE(KStrECommDbDialogPrefDoNotPrompt)
		{
		dialogPref = ECommDbDialogPrefDoNotPrompt;
		break;
		}
	STR_CASE(KStrECommDbDialogPrefPromptIfWrongMode)
		{
		dialogPref = ECommDbDialogPrefPromptIfWrongMode;
		break;
		}
	DEFAULT_CASE()
		{
		dialogPref = ECommDbDialogPrefUnknown;
		}
	STR_SWITCH_END()

	return dialogPref;
	}	

	
/**
ConvertDesToTImap4GetMailOptions()
Returns the corresponding TImap4GetMailOptions Enumeration

@param aMailOption
@return
TImap4GetMailOptions enumeration
*/	
EXPORT_C TImap4GetMailOptions CT_MsgUtilsEnumConverter::ConvertDesToTImap4GetMailOptions(TDesC& aMailOption)
	{ 
	TImap4GetMailOptions options = EGetImap4EmailAttachments;
	
	STR_SWITCH(aMailOption)
	
	STR_CASE(KStrEGetImap4EmailBodyText)
		{
		options = EGetImap4EmailBodyText;
		break;
		}
	STR_CASE(KStrEGetImap4EmailBodyTextAndAttachments)
		{
		options = EGetImap4EmailBodyTextAndAttachments;
		break;
		}
	STR_CASE(KStrEGetImap4EmailAttachments)
		{
		options = EGetImap4EmailAttachments;
		break;
		}
	STR_CASE(KStrEGetImap4EmailBodyAlternativeText)
		{
		options = EGetImap4EmailBodyAlternativeText;
		break;
		}
	DEFAULT_CASE()
		{
		options = EGetImap4EmailHeaders;
		}
	STR_SWITCH_END()

	return options;
	}	


/**
ConvertDesToTFolderSyncType()
Returns the corresponding TFolderSyncType Enumeration

@param aFolderSyncType
@return
TFolderSyncType enumeration
*/	
EXPORT_C TFolderSyncType CT_MsgUtilsEnumConverter::ConvertDesToTFolderSyncType(TDesC& aFolderSyncType)
	{
	TFolderSyncType syncType;
	
	STR_SWITCH(aFolderSyncType)
		
	STR_CASE(KStrEUseCombination)
		{
		syncType = EUseCombination;
		break;
		}
	STR_CASE(KStrEUseLocal)
		{
		syncType = EUseLocal;
		break;
		}

	DEFAULT_CASE()
		{
		syncType = EUseRemote;
		}
		
	STR_SWITCH_END()

	return syncType;
	}


/**
ConvertDesToTFolderSubscribeType()
Returns the corresponding TFolderSubscribeType Enumeration

@param aFolderSubscriberType
@return
TFolderSubscribeType enumeration
*/	
EXPORT_C TFolderSubscribeType CT_MsgUtilsEnumConverter::ConvertDesToTFolderSubscribeType(TDesC& aFolderSubscriberType)
	{
	TFolderSubscribeType subscriberType;
	
	STR_SWITCH(aFolderSubscriberType)
		
	STR_CASE(KStrEUpdateLocal)
		{
		subscriberType = EUpdateLocal;
		break;
		}
	STR_CASE(KStrEUpdateRemote)
		{
		subscriberType = EUpdateRemote;
		break;
		}
	STR_CASE(KStrEUpdateBoth)
		{
		subscriberType = EUpdateBoth;
		break;
		}

	DEFAULT_CASE()
		{
		subscriberType = EUpdateNeither;
		}
		
	STR_SWITCH_END()

	return subscriberType;
	}


/**
ConvertDesToTImImap4PartialMailOptions()
Returns the corresponding TImImap4PartialMailOptions Enumeration

@param aPartialMailOption
@return
TImImap4PartialMailOptions
*/
EXPORT_C TImImap4PartialMailOptions CT_MsgUtilsEnumConverter::ConvertDesToTImImap4PartialMailOptions(TDesC& aPartialMailOption)
	{
	TImImap4PartialMailOptions partialOption;

	STR_SWITCH(aPartialMailOption)
		
	STR_CASE(KStrEBodyTextOnly)
		{
		partialOption = EBodyTextOnly;
		break;
		}
	STR_CASE(KStrEAttachmentsOnly)
		{
		partialOption = EAttachmentsOnly;
		break;
		}
	STR_CASE(KStrEBodyTextAndAttachments)
		{
		partialOption = EBodyTextAndAttachments;
		break;
		}
	STR_CASE(KStrECumulative)
		{
		partialOption = ECumulative;
		break;
		}
	STR_CASE(KStrEBodyAlternativeText)
		{
		partialOption = EBodyAlternativeText;
		break;
		}
	DEFAULT_CASE()
		{
		partialOption = ENoSizeLimits;
		}
		
	STR_SWITCH_END()

	return partialOption;
	}


/**
ConvertDesToTMsgOutboxBodyEncoding()
Returns the corresponding TMsgOutboxBodyEncoding Enumeration

@param aOutboxBodyEncoding
@return
TMsgOutboxBodyEncoding 
*/
EXPORT_C TMsgOutboxBodyEncoding CT_MsgUtilsEnumConverter::ConvertDesToTMsgOutboxBodyEncoding(TDesC& aOutboxBodyEncoding)
	{
	TMsgOutboxBodyEncoding bodyEncoding;

	STR_SWITCH(aOutboxBodyEncoding)
		
	STR_CASE(KStrEMsgOutboxNoAlgorithm)
		{
		bodyEncoding = EMsgOutboxNoAlgorithm;
		break;
		}
	STR_CASE(KStrEMsgOutboxMIME)
		{
		bodyEncoding = EMsgOutboxMIME;
		break;
		}
	STR_CASE(KStrEMsgOutboxMHTMLAsMIME)
		{
		bodyEncoding = EMsgOutboxMHTMLAsMIME;
		break;
		}
	STR_CASE(KStrEMsgOutboxMHTMLAlternativeAsMIME)
		{
		bodyEncoding = EMsgOutboxMHTMLAlternativeAsMIME;
		break;
		}
	DEFAULT_CASE()
		{
		bodyEncoding = EMsgOutboxDefault;
		}
		
	STR_SWITCH_END()
											
	return bodyEncoding;
	}


/**
ConvertDesToTImSMTPSendMessageOption()
Returns the corresponding TImSMTPSendMessageOption Enumeration

@param aSendMsgOption
@return 
TImSMTPSendMessageOption 
*/
EXPORT_C TImSMTPSendMessageOption CT_MsgUtilsEnumConverter::ConvertDesToTImSMTPSendMessageOption(TDesC& aSendMsgOption)
	{
	TImSMTPSendMessageOption messageOption;

	STR_SWITCH(aSendMsgOption)
		
	STR_CASE(KStrESendMessageOnNextConnection)
		{
		messageOption = ESendMessageOnNextConnection;
		break;
		}
	STR_CASE(KStrESendMessageOnRequest)
		{
		messageOption = ESendMessageOnRequest;
		break;
		}	
	DEFAULT_CASE()
		{
		messageOption = ESendMessageImmediately;
		}
		
	STR_SWITCH_END()
						
	return messageOption;
	}


/**
ConvertDesToTImSMTPSendCopyToSelf()
Returns the corresponding TImSMTPSendCopyToSelf Enumeration

@param aSmtpCopyToSelf
@return
TImSMTPSendCopyToSelf 
*/
EXPORT_C TImSMTPSendCopyToSelf CT_MsgUtilsEnumConverter::ConvertDesToTImSMTPSendCopyToSelf(TDesC& aSmtpCopyToSelf)
	{
	TImSMTPSendCopyToSelf copyToSelf;

	STR_SWITCH(aSmtpCopyToSelf)
		
	STR_CASE(KStrESendCopyAsToRecipient)
		{
		copyToSelf= ESendCopyAsToRecipient;
		break;
		}
	STR_CASE(KStrESendCopyAsCcRecipient)
		{
		copyToSelf= ESendCopyAsCcRecipient;
		break;
		}
	STR_CASE(KStrESendCopyAsBccRecipient)
		{
		copyToSelf= ESendCopyAsBccRecipient;
		break;
		}
	DEFAULT_CASE()
		{
		copyToSelf= ESendNoCopy;
		}
		
	STR_SWITCH_END()

	return copyToSelf;
	}	


/**
ConvertDesToTPop3GetMailOptions()
Returns the corresponding TPop3GetMailOptions Enumeration

@param aPop3MailOption
@return
TPop3GetMailOptions 
*/
EXPORT_C TPop3GetMailOptions CT_MsgUtilsEnumConverter::ConvertDesToTPop3GetMailOptions(TDesC& aPop3MailOption)
	{
	TPop3GetMailOptions popMailOption;

	STR_SWITCH(aPop3MailOption)
		
	STR_CASE(KStrEGetPop3EmailMessages)
		{
		popMailOption = EGetPop3EmailMessages;
		break;
		}
	DEFAULT_CASE()
		{
		popMailOption = EGetPop3EmailHeaders;
		}
		
	STR_SWITCH_END()

	return popMailOption;
	}


/**
ConvertDesToEventCode()
Returns the corresponding TInstrumentationPoint Enumeration

@param aEventCodeString
@return
TInstrumentationPoint 
*/
EXPORT_C TInstrumentationPoint CT_MsgUtilsEnumConverter::ConvertDesToEventCode(TDesC& aEventCodeString)
	{
	TInstrumentationPoint eventCode;

	STR_SWITCH(aEventCodeString)
		
	STR_CASE(KStrImapRamUsageStart)
		{
		eventCode = EImapRamUsageStart;
		break;
		}
	STR_CASE(KStrImapRamUsageStop)
		{
		eventCode = EImapRamUsageStop;
		break;
		}
	STR_CASE(KStrPopRamUsageStart)
		{
		eventCode = EPopRamUsageStart;
		break;
		}
	STR_CASE(KStrPopRamUsageStop)
		{
		eventCode = EPopRamUsageStop;
		break;
		}
	STR_CASE(KStrSmtpRamUsageStart)
		{
		eventCode = ESmtpRamUsageStart;
		break;
		}
	STR_CASE(KStrSmtpRamUsageStop)
		{
		eventCode = ESmtpRamUsageStop;
		break;
		}
	STR_CASE(KStrSmtpRamUsageCreateMessageStart)
		{
		eventCode = ESmtpRamUsageCreateMessageStart;
		break;
		}
	STR_CASE(KStrSmtpRamUsageCreateMessageStop)
		{
		eventCode = ESmtpRamUsageCreateMessageStop;
		break;
		}
	STR_CASE(KStrSmtpRamUsageSendMessageStart)
		{
		eventCode = ESmtpRamUsageSendMessageStart;
		break;
		}
	STR_CASE(KStrSmtpRamUsageSendMessageStop)
		{
		eventCode = ESmtpRamUsageSendMessageStop;
		break;
		}
	STR_CASE(KStrSmtpCreateMessageSpeedStart)
		{
		eventCode = ESmtpCreateMessageSpeedStart;
		break;
		}
	STR_CASE(KStrSmtpCreateMessageSpeedStop)
		{
		eventCode = ESmtpCreateMessageSpeedStop;
		break;
		}
	STR_CASE(KStrSmtpSendMessageSpeedStart)
		{
		eventCode = ESmtpSendMessageSpeedStart;
		break;
		}
	STR_CASE(KStrSmtpSendMessageSpeedStop)
		{
		eventCode = ESmtpSendMessageSpeedStop;
		break;
		}
	STR_CASE(KStrImapDownloadMessageSpeedStart)
		{
		eventCode = EImapDownloadMessageSpeedStart;
		break;
		}
	STR_CASE(KStrImapDownloadMessageSpeedStop)
		{
		eventCode = EImapDownloadMessageSpeedStop;
		break;
		}
	STR_CASE(KStrImapCopyMessageSpeedStart)
		{
		eventCode = EImapCopyMessageSpeedStart;
		break;
		}
	STR_CASE(KStrImapCopyMessageSpeedStop)
		{
		eventCode = EImapCopyMessageSpeedStop;
		break;
		}
	STR_CASE(KStrImapMoveMessageSpeedStart)
		{
		eventCode = EImapMoveMessageSpeedStart;
		break;
		}
	STR_CASE(KStrImapMoveMessageSpeedStop)
		{
		eventCode = EImapMoveMessageSpeedStop;
		break;
		}
	STR_CASE(KStrImapDeleteMessageSpeedStart)
		{
		eventCode = EImapDeleteMessageSpeedStart;
		break;
		}
	STR_CASE(KStrImapDeleteMessageSpeedStop)
		{
		eventCode = EImapDeleteMessageSpeedStop;
		break;
		}
	STR_CASE(KStrESearchSortStart)
		{
		eventCode = ESearchSortStart;
		break;
		}
	STR_CASE(KStrESearchSortStop)
		{
		eventCode = ESearchSortStop;
		break;
		}
		
	DEFAULT_CASE()
		{
		eventCode = EBadEventCode;
		}
		
	STR_SWITCH_END()

	return eventCode;
	}

/**
ConvertDesToBearerTypes()
Returns the corresponding TUint32 representing bearer types

@param aBearerTypesString Bearer types string
@return Bearer types enum
*/
EXPORT_C TUint32 CT_MsgUtilsEnumConverter::ConvertDesToBearerTypes(const TDesC& aBearerTypesString)
	{
	TUint32 bearerTypes;

	STR_SWITCH(aBearerTypesString)

	STR_CASE(KStrBearerTypeUnknown)
		{
		bearerTypes = KCommDbBearerUnknown;
		break;
		}

	STR_CASE(KStrBearerTypeCsd)
		{
		bearerTypes = KCommDbBearerCSD;
		break;
		}

	STR_CASE(KStrBearerTypeWcdma)
		{
		bearerTypes = KCommDbBearerWcdma;
		break;
		}

	STR_CASE(KStrBearerTypeLan)
		{
		bearerTypes = KCommDbBearerLAN;
		break;
		}

	STR_CASE(KStrBearerTypeVirtual)
		{
		bearerTypes = KCommDbBearerVirtual;
		break;
		}

	STR_CASE(KStrBearerTypePan)
		{
		bearerTypes = KCommDbBearerPAN;
		break;
		}

	STR_CASE(KStrBearerTypeWlan)
		{
		bearerTypes = KCommDbBearerWLAN;
		break;
		}

	STR_CASE(KStrBearerTypePsd)
		{
		bearerTypes = KCommDbBearerPSD;
		break;
		}

	STR_CASE(KStrBearerTypeCsdLan)
		{
		bearerTypes = KCommDbBearerCSD | KCommDbBearerLAN;
		break;
		}

	DEFAULT_CASE()
		{
		bearerTypes = KCommDbBearerUnknown;
		}
		
	STR_SWITCH_END()

	return bearerTypes;
	}


/**
ConvertDesToSearchSortOption()
Returns the corresponding TInt representing search sort options

@param aSearchSortString search sort field types string
@return search sort types enum
*/
EXPORT_C TInt CT_MsgUtilsEnumConverter::ConvertDesToSearchSortOption(const TDesC& aSearchSortString)
	{
	TInt searchSortOption=0;

	STR_SWITCH(aSearchSortString)

	STR_CASE(KSearchForToAddress)
		{
		searchSortOption=1;
		break;
		}

	STR_CASE(KSearchForCCField)
		{
		searchSortOption=2;
		break;
		}

	STR_CASE(KSearchForBCCField)
		{
		searchSortOption=3;
		break;
		}

	STR_CASE(KSearchForSubjectField)
		{
		searchSortOption=4;
		break;
		}

	STR_CASE(KSearchForDateField)
		{
		searchSortOption=5;
		break;
		}

	STR_CASE(KSearchForTimeField)
		{
		searchSortOption=6;
		break;
		}

	STR_CASE(KSearchForBodyPart)
		{
		searchSortOption=7;
		break;
		}
	DEFAULT_CASE()
		{
		searchSortOption=0;
		}
		
	STR_SWITCH_END()

	return searchSortOption;
	}

/**
ConvertDesToMessagePart()
Returns the corresponding TMessagePart Enumeration

@param aMessagePart Message part string
@return TMessagePart enum
*/
EXPORT_C TMsvMessagePart CT_MsgUtilsEnumConverter::ConvertDesToMessagePart(const TDesC& aMessagePart)
	{
	TMsvMessagePart msgPart;
	STR_SWITCH(aMessagePart)
	STR_CASE(KStrMsvTo)
		{
		msgPart = EMsvTo;
		break;
		}
	STR_CASE(KStrMsvFrom)
		{
		msgPart = EMsvFrom;
		break;
		}
	STR_CASE(KStrMsvCc)
		{
		msgPart = EMsvCc;
		break;
		}
	STR_CASE(KStrMsvBcc)
		{
		msgPart = EMsvBcc;
		break;
		}
	STR_CASE(KStrMsvDetails)
		{
		msgPart = EMsvDetails;
		break;
		}
	STR_CASE(KStrMsvDescription)
		{
		msgPart = EMsvDescription;
		break;
		}
	STR_CASE(KStrMsvSubject)
		{
		msgPart = EMsvSubject;
		break;
		}
	STR_CASE(KStrMsvNew)
		{
		msgPart = EMsvNew;
		break;
		}
	STR_CASE(KStrMsvUnreadMessages)
		{
		msgPart = EMsvUnreadMessages;
		break;
		}
	STR_CASE(KStrMsvDate)
		{
		msgPart = EMsvDate;
		break;
		}
	STR_CASE(KStrMsvSize)
		{
		msgPart = EMsvSize;
		break;
		}
	STR_CASE(KStrMsvAttachment)
		{
		msgPart = EMsvAttachment;
		break;
		}
	STR_CASE(KStrEMsvMtmTypeUID)
		{
		msgPart = EMsvMtmTypeUID;
		break;
		}
	STR_CASE(KStrMsvBody)
		{
		msgPart = EMsvBody;
		break;
		}
	STR_CASE(KStrMsvPriority)
		{
		msgPart = EMsvPriority;
		break;
		}
	DEFAULT_CASE()
		{
		User::Panic(_L("TestData:MsgPart"),1);
		}
	STR_SWITCH_END()
	return msgPart;
	}

/**
ConvertDesToMsgRelationOp()
Returns the corresponding TMsgRelationOp Enumeration

@param aRelationOp Relational operators supported
@return TMsgRelationOp enum
*/
EXPORT_C TMsvRelationOp CT_MsgUtilsEnumConverter::ConvertDesToMsgRelationOp(const TDesC& aRelationOp)
	{
	TMsvRelationOp relationalOperator;
	STR_SWITCH(aRelationOp)
	STR_CASE(KStrEMsvEqual)
		{
		relationalOperator = EMsvEqual;
		break;
		}
	STR_CASE(KStrEMsvNotEqual)
		{
		relationalOperator = EMsvNotEqual;
		break;
		}
	STR_CASE(KStrEMsvLessThan)
		{
		relationalOperator = EMsvLessThan;
		break;
		}
	STR_CASE(KStrEMsvGreaterThan)
		{
		relationalOperator = EMsvGreaterThan;
		break;
		}
	STR_CASE(KStrEMsvLessThanOrEqual)
		{
		relationalOperator = EMsvLessThanOrEqual;
		break;
		}
	STR_CASE(KStrEMsvGreaterThanOrEqual)
		{
		relationalOperator = EMsvGreaterThanOrEqual;
		break;
		}

	DEFAULT_CASE()
		{
		User::Panic(_L("TestData:RltnOp"),2);
		}
	STR_SWITCH_END()
	return relationalOperator;
	}
	
/**
ConvertDesToMsgPriority()
Returns the corresponding TMsvPriority Enumeration

@param aMsgPriority Message priority
@return TMsvPriority enum
*/
EXPORT_C TMsvPriority CT_MsgUtilsEnumConverter::ConvertDesToMsgPriority(const TDesC& aMsgPriority)
	{
	TMsvPriority priority;
	STR_SWITCH(aMsgPriority)
	STR_CASE(KStrEMsvLowPriority)
		{
		priority = EMsvLowPriority;
		break;
		}
	STR_CASE(KStrEMsvMediumPriority)
		{
		priority = EMsvMediumPriority;
		break;
		}
	STR_CASE(KStrEMsvHighPriority)
		{
		priority = EMsvHighPriority;
		break;
		}

	DEFAULT_CASE()
		{
		User::Panic(_L("TestData:Prirty"),3);
		}
	STR_SWITCH_END()
	return priority;
	}

/**
ConvertDesToSortOrder()
Returns the corresponding TSortingOption Enumeration

@param aSortOrder Order in which messages should be arranged
@return TSortingOption enum
*/
EXPORT_C TMsvSortOrder CT_MsgUtilsEnumConverter::ConvertDesToSortOrder(const TDesC& aSortOrder)
	{
	TMsvSortOrder order;
	STR_SWITCH(aSortOrder)
	STR_CASE(KStrEMsvSortAscending)
		{
		order = EMsvSortAscending;
		break;
		}
	STR_CASE(KStrEMsvSortDescending)
		{
		order = EMsvSortDescending;
		break;
		}
	DEFAULT_CASE()
		{
		User::Panic(_L("TestData:Order"),3);
		}
	STR_SWITCH_END()
	return order;
	}

