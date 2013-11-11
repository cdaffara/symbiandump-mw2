// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// SMS settings Parser
// 
//

#include <msvids.h>   // KMsvRootIndexEntryId
#include <msvuids.h>
#include <msventry.h>
#include <msvruids.h>
//
#include "BSP.H"
#include "IACP.H"
#include "SMSP.H"
#include "IACPDEF.H"
#include "IACPERR.H"
#include <csmsaccount.h>

#include "IMPMACRO.H"


//--------------------------------------
//
// Constructor
//
CSmsParser::CSmsParser()
	{
	}

//
// Factory fns
//
CSmsParser* CSmsParser::NewLC()
	{
	CSmsParser* self=new (ELeave) CSmsParser();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CSmsParser* CSmsParser::NewL()
	{
	CSmsParser* self=CSmsParser::NewLC();
	CleanupStack::Pop();
	return self;
	}

//
// 2nd stage of construction
//
void CSmsParser::ConstructL()
	{
	// --- Create the blank settings objects ---
	iSmsSettings= CSmsSettings::NewL();
	}

//
// Destruction
//
CSmsParser::~CSmsParser()
	{
	delete iSmsSettings;
	delete iServiceCentreName;
	delete iServiceCentreAddress;
	}

//
// Parse/Set data members of CSmsSettings
//
void CSmsParser::ParseL(CParsedFieldCollection& aIacpFields)
	{
	TPtrC fieldValueBuf;

	// service center name
	if (aIacpFields.GetFieldValueAndLength(SMS_SERVICE_CENTER_NAME, fieldValueBuf) != 0)
		{
		delete iServiceCentreName;
		iServiceCentreName = NULL;
		iServiceCentreName=fieldValueBuf.AllocL();
		}

	// service name
	if (aIacpFields.GetFieldValueAndLength(SMS_SERVICE_CENTER_ADDRESS, fieldValueBuf) != 0)
		{
		delete iServiceCentreAddress;
		iServiceCentreAddress = NULL;
		iServiceCentreAddress=fieldValueBuf.AllocL();
		}

	if(iServiceCentreName == 0 || iServiceCentreAddress == 0 ||
	   !((*iServiceCentreName).Length()>0 && (*iServiceCentreAddress).Length() >0))
		User::Leave(KIacpMandatoryDataNotSet);
	}
//
// Create sms service
//
void CSmsParser::ProcessL(CMSVENTRY& aEntry)
	{
	TMsvId entryId = aEntry.Entry().Id();
	CSmsAccount* smsAccount = CSmsAccount::NewLC();
	TRAPD(error, smsAccount->LoadSettingsL(*iSmsSettings));
	if (error == KErrNone)
		{
		// update or create service centre address
		FindServiceCentreAddressL();
		}
	else if (error == KErrNotFound)	
		{
		// settings not found, create new
		smsAccount->InitialiseDefaultSettingsL(*iSmsSettings);
		iSmsSettings->AddServiceCenterL(*iServiceCentreName, *iServiceCentreAddress);
		}
	else
		{
		User::Leave(error);
		}
	smsAccount->SaveSettingsL(*iSmsSettings);
	CleanupStack::PopAndDestroy(smsAccount);
	// go back to msg's context
	SETENTRYL(entryId);
	}



void CSmsParser::FindServiceCentreAddressL()
	{
	TBool foundAddress = EFalse;
	TInt numSCentres = iSmsSettings->ServiceCenterCount();
	TInt currentSC = 0;
	while(currentSC < numSCentres && !foundAddress)
		{
		CSmsServiceCenter& number = iSmsSettings->GetServiceCenter(currentSC);
		// compare name if there update else look at next SC
		if(number.Name().CompareC(*iServiceCentreName)==0)
			{
			number.SetAddressL(*iServiceCentreAddress);
			foundAddress = ETrue;
			}
		else
			currentSC++;
		}
	if(!foundAddress)
		{
		iSmsSettings->AddServiceCenterL(*iServiceCentreName,*iServiceCentreAddress);
		iSmsSettings->SetDefaultServiceCenter(numSCentres);
		}
	}
