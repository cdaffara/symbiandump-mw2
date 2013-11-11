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
// [TestStep Name]
// CompareEmailMessages
// [Parameters]
// ImapAccountName			:Name of the Imap account
// FolderName				:Name of the remote folder
// DownloadLimts			:Download limts set, partial or full
// 1 - Full, 0 - Partial
// DLRulesUsage				:ETrue => Compare messages using sync download rules.
// EFalse => Don't compare messages using sync download rules. [DEFAULT: EFalse]
// ImapBearerTypes			:The bearer type(s) that will be use the download rules for synchronisation.
// Options available -
// "Unknown": Bearer support unknown.
// "CSD": CSD Bearer support. 
// "Wcdma": WCDMA Bearer support.
// "LAN": LAN Bearer support. 
// "Cdma2000": CDMA2000 Bearer support.
// "Virtual":
// "PAN":
// "WLAN":
// "PSD": Support for both Wcdma and Cdma2000.
// "CSDLAN": Support for both CSD and LAN.
// Compares the sent and received email messages for size and contents based on
// the download limits set.  The sent and received messages with the same 
// Subject field are compared.  The Complete and Partial download flags set for 
// the message parts are verified. If sync download rules usage is indicated then
// comparison is based on the partial download limits set from the loaded partial mail info object for
// the given bearer type(s).
// 
//

// User includes
#include <t_utils.h>
#include <t_utilscentralrepository.h>
#include <t_utilsenumconverter.h>
#include "T_UtilsCompareEmailMessages.h"
#include "T_CompareEmailMessages.h"

// Literals Used
_LIT(KImapAccountName, "ImapAccountName");
_LIT(KFolderName, "FolderName");
_LIT(KDownloadLimits, "DownloadLimits");
_LIT(KDLRulesUsage, "DLRulesUsage");
_LIT(KImapBearerTypes, "ImapBearerTypes");

/**
  Function : CT_MsgCompareEmailMessages
  Description : Constructor
  @param	: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCompareEmailMessages::CT_MsgCompareEmailMessages(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCompareEmailMessages);
	}

/**
  Function : ReadIni
  Description : Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCompareEmailMessages::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KFolderName, iFolderName))
		{
		ERR_PRINTF1(_L("Remote folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetBoolFromConfig(ConfigSection(), KDownloadLimits, iDownloadLimits))
		{
		ERR_PRINTF1(_L("An expected \"ETrue\" or \"EFalse\" boolean value is not specified for the type of DL limits"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetBoolFromConfig(ConfigSection(), KDLRulesUsage, iDLRulesUsage))
		{
		INFO_PRINTF1(_L("An expected \"ETrue\" or \"EFalse\" boolean value is not specified for DL rules usage, "));
		INFO_PRINTF1(_L("so default value is set: EFalse."));
		iDLRulesUsage = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KImapBearerTypes, iImapBearerTypes))
		{
		if(iDLRulesUsage)
			{
			ERR_PRINTF1(_L("Bearer type is not specified - Options available: ")); 
			ERR_PRINTF1(_L("Unknown, CSD, Wcdma, LAN, Virtual, PAN, WLAN, PSD and CSDLAN"));
			SetTestStepResult(EFail);
			result = EFalse;
			}
		}
		
	return result;
	}

/**
  Function : doTestStepL
  Description : Reads the Imap account name and remote folder name from the .ini file.
				Obtains a selection of the entries under the sent items folder
				and the specified remote IMAP folder. It then calls the utility CompareEmailL or
				CompareEmailWithDLRulesL to compare the messages based on the download limits set.
  @return : TVerdict - Test step result
			ETrue if messages match as required
			EFalse if messages don't match as required
  @leave  : KMsvNullIndexEntryId	Invalid IMAP account name is specified or 
									Invalid remote folder name specified
									
*/
TVerdict CT_MsgCompareEmailMessages::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CompareEmailMessages"));
	if(ReadIni())
		{
	 	//Retrieves the Imap service Id for the given Imap account
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)iImapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );

		// If the IMAP Service Id is KMsvNullIndexEntryId then an invalid IMAP account name is specified
		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name is specified"));
			SetTestStepResult(EFail);
			}
			
		// Gets the Id of the remote folder based on the folder name
		TMsvId folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,
																		iImapAccountName, iFolderName);
		INFO_PRINTF2(_L("The remote folder Id is %d"), folderId);

		// If the folder Id is KMsvNullIndexEntryId, then the folder name is invalid
		if( folderId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid remote folder name specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			// Creates a selection of the children entries under the Sent Items folder
			CMsvEntrySelection*	sentMsgSelectionList = CT_MsgUtils::CreateChildrenSelectionL(
																	iSharedDataIMAP.iSession, KMsvSentEntryId);
			CleanupStack::PushL(sentMsgSelectionList);

			// Creates a selection of the children entries under the remote folder specified
			CMsvEntrySelection*	recvMsgSelectionList = CT_MsgUtils::CreateChildrenSelectionRemoteFolderL(
																	iSharedDataIMAP.iSession,
																	iImapAccountName, iFolderName);
			CleanupStack::PushL(recvMsgSelectionList);

			// If any of the sent items selection or received items selection is empty then flag an error
			if(sentMsgSelectionList == NULL || recvMsgSelectionList == NULL)
				{
				ERR_PRINTF1(_L("Invalid message selection object found"));
				SetTestStepResult(EFail);
				}
			else
				{
				// Compares the Email messages of the remote folder with those in the sent items folder
				CT_MsgUtilsCompareEmailMessages* obj = CT_MsgUtilsCompareEmailMessages::NewL(*this);
				CleanupStack::PushL(obj);
				TBool finalResult = EFalse;
				if (iDLRulesUsage)
					{
					TUint32 bearerTypes = CT_MsgUtilsEnumConverter::ConvertDesToBearerTypes(iImapBearerTypes);
					if (iFolderName.CompareF(_L("Inbox")) == 0)
						{
						finalResult = obj->CompareEmailWithDLRulesL(*iSharedDataIMAP.iSession,
																	imapServiceId, folderId, iDownloadLimits,
																	*recvMsgSelectionList, *sentMsgSelectionList,
																	CImapSyncDownloadRules::EInboxRulesType, bearerTypes);
						}
					else
						{
						finalResult = obj->CompareEmailWithDLRulesL(*iSharedDataIMAP.iSession,
																	imapServiceId, folderId, iDownloadLimits,
																	*recvMsgSelectionList, *sentMsgSelectionList,
																	CImapSyncDownloadRules::EFolderRulesType, bearerTypes);
						}
					}
				else
					{
					finalResult = obj->CompareEmailL(*iSharedDataIMAP.iSession,
																	imapServiceId, folderId, iDownloadLimits,
																	*recvMsgSelectionList, *sentMsgSelectionList);
					}
				if(!finalResult)
					{
					ERR_PRINTF1(_L("Compare Email Messages Failed"));
					SetTestStepResult(EFail);
					}
						
				CleanupStack::PopAndDestroy(obj);
				}
			CleanupStack::PopAndDestroy(2,sentMsgSelectionList); // recvMsgSelectionList,sentMsgSelectionList
			}
		}
	return TestStepResult();
	}
