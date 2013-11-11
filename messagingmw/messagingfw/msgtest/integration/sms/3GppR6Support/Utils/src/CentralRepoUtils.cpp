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

/**
 @file
*/

#include <smutset.h>
#include <csmsaccount.h>

#include <mtclreg.h>
#include "msvuids.h" //KUidMsvServiceEntry
#include "msvids.h" //KUidMsgTypeSMS

#include "CentralRepoUtils.h"
#include "CSmsScriptUtils.h"
#include "CSchSendScriptUtils.h"

#include <msvschedulesettings.h>
#include <msvoffpeaktime.h>
#include <msvsysagentaction.h>


/**
CreateMtm for given AccountName and Mtm type UID.
NOTE: The client is responsible for deletion of Client Registry passed and The Mtm created.

@param aMtmRegistry		ClientRegistry reference
@param aMtmUid			Uid of Mtm to be created
@internalTechnology
*/
CBaseMtm* CentralRepoUtils::CreateMtmL(TUid aMtmUid, CClientMtmRegistry* aMtmRegistry)
	{
	CBaseMtm* paramMtm = aMtmRegistry->NewMtmL(aMtmUid);
	return paramMtm;
	}

/**
Delete Existing Service

@param aSession		Current Session reference
@internalTechnology
*/
void CentralRepoUtils::DeleteServiceL(CMsvSession* aSession)
	{
	TMsvId serviceId;
	TRAPD(err,TSmsUtilities::ServiceIdL(*aSession,serviceId));
	if(err !=KErrNotFound)
		{
		CMsvEntry* entry = CMsvEntry::NewL(*aSession,serviceId,TMsvSelectionOrdering());
		CleanupStack::PushL(entry);
		entry->SetEntryL(serviceId);
		entry->SetEntryL(entry->Entry().Parent());
		entry->DeleteL(serviceId);
		CleanupStack::PopAndDestroy(entry);
		}
	}

/**
Creates a New Service
If a Service Exists it doesnot create a new one

@param aSession		Current Session reference
@param aDescription	Entry Description Descriptor
@param aDetail		Entry Details Descriptor
@internalTechnology
*/
void CentralRepoUtils::CreateServiceL(CMsvSession* aSession, const TDesC& aDescription, const TDesC& aDetail)
	{
	TMsvId serviceId=0;
	TRAPD(err,TSmsUtilities::ServiceIdL(*aSession,serviceId));

		if(err == KErrNotFound) // If Service Already Exists Do not create new One
			{
			TInt priority = EMsvMediumPriority;
			TInt readOnlyFlag = EFalse;
			TInt visibleFlag = ETrue;

			TMsvEntry indexEntry;
			indexEntry.iType = KUidMsvServiceEntry;
			indexEntry.iMtm = KUidMsgTypeSMS;
			indexEntry.SetReadOnly(readOnlyFlag);
			indexEntry.SetVisible(visibleFlag);	
			indexEntry.SetPriority(TMsvPriority(priority));
			indexEntry.iDescription.Set(aDescription);
			indexEntry.iDetails.Set(aDetail);
			indexEntry.iDate.HomeTime();
			
			CMsvEntry* entry = CMsvEntry::NewL(*aSession,KMsvRootIndexEntryId,TMsvSelectionOrdering());
			CleanupStack::PushL(entry);
			entry->CreateL(indexEntry);
		    CleanupStack::PopAndDestroy(entry);
			}
	}
	

/**
Create SMS settings from the file

@param aFileName	Name of file from which the settings need to be loaded
@internalTechnology
*/
void CentralRepoUtils::SetSmsServiceSettingsL(TDesC& aFileName)
	{
	CSmsSettings* smsSettings = CSmsSettings::NewL();
	CleanupStack::PushL(smsSettings);

	CSmsAccount* account = CSmsAccount::NewLC();
	account->InitialiseDefaultSettingsL(*smsSettings);

	// now override the settings with the config file contents
	CSmsScriptUtils::ReadSmsSettingsFromConfigurationFileL(aFileName, *smsSettings);
	
	//Set these settings which will keep the PDU VP bits for the msg constant...
	smsSettings->SetValidityPeriod(ESmsVPWeek);	// week
	smsSettings->SetValidityPeriodFormat(TSmsFirstOctet::ESmsVPFInteger); //relative
	smsSettings->SetReplyQuoted(EFalse);
	smsSettings->SetRejectDuplicate(ETrue);
	smsSettings->SetDelivery(ESmsDeliveryImmediately);
	smsSettings->SetDeliveryReport(ETrue);
	smsSettings->SetReplyPath(EFalse);
	smsSettings->SetMessageConversion(ESmsConvPIDNone);
	smsSettings->SetCanConcatenate(ETrue);
	smsSettings->SetDeliveryReport(ETrue);
	smsSettings->SetStatusReportHandling(CSmsSettings::EMoveReportToInboxVisibleAndMatch);

	account->SaveSettingsL(*smsSettings);
	CleanupStack::PopAndDestroy(2, smsSettings); // account smsSettings		
	}
	
/**
Create Schedule settings from the file

@param aFileName	Name of file from which the settings need to be loaded
@internalTechnology
*/

void CentralRepoUtils::SetSchSendSettingsL(TDesC& aFileName)
	{	
	CMsvScheduleSettings* scheduleSettings = CMsvScheduleSettings::NewLC();
	CSchSendScriptUtils::ReadScheduleSettingsFromConfigurationFileL(aFileName, *scheduleSettings);

	CMsvOffPeakTimes* offPeakTimes = new (ELeave) CMsvOffPeakTimes;
	CleanupStack::PushL(offPeakTimes);	
	
	CMsvSendErrorActions* sendErrorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(sendErrorActions);
	
	CMsvSysAgentActions* sysAgentActions = new (ELeave) CMsvSysAgentActions;
	CleanupStack::PushL(sysAgentActions);
	CSchSendScriptUtils::ReadScheduleSettingsFromConfigurationFileL(aFileName, *sysAgentActions);

	// save the settings to the central repository 
	CSmsAccount *smsAccount = CSmsAccount::NewLC();
	smsAccount->SaveSettingsL( *scheduleSettings, *offPeakTimes, *sendErrorActions, *sysAgentActions);

	CleanupStack::PopAndDestroy(5, scheduleSettings); // scheduleSettings, offPeakTimes, sendErrorActions, sysAgentActions, smsAccount	
	}
