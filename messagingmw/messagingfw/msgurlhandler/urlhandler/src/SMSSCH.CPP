// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains the implementation for the class defined in SmsSch.h
// 
//

/**
 @file
 @see	SmsSch.h
*/

// System includes
//
#include <inetprottextutils.h>
#include <sendas2.h>

// Local includes
//
#include "SMSSCH.H"
#include "msgurlparser.h"

// Allowed characters
_LIT(KAllowedChars,"0123456789-.");

// Field name
_LIT(KVia,"via");

// Constants
//
const TUid KSmsMtm = {0x1000102c};
const TInt KPlusChar = '+';
const TInt KSemiColonChar = ';';
const TInt KEqualChar = '=';

CSmsSchemeHandler::CSmsSchemeHandler()
	{
	}

CSmsSchemeHandler* CSmsSchemeHandler::NewLC()
	{
	CSmsSchemeHandler* self=new (ELeave) CSmsSchemeHandler();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CSmsSchemeHandler* CSmsSchemeHandler::NewL()
	{
	CSmsSchemeHandler* self = NewLC();
	CleanupStack::Pop(self);
	return self;
	}

void CSmsSchemeHandler::ConstructL()
	{
	CMsgSchemeHandlerBase::ConstructL();
	}

CSmsSchemeHandler::~CSmsSchemeHandler()
	{
	iNumbers.Close();
	}

void CSmsSchemeHandler::ParseUrlL(const TDesC& aUrl)
	{
	// Get the scheme specific part
	TPtrC smsPart; 
	GetSchemeSpecificPartL(aUrl, smsPart);

	// Format of sms url:	sms:<segment>,<segment>,....
	// Format of segment is: <sms-number>;via=<service center number>
	TPtrC segment;

	// The sms-number
	TPtrC numData;

	// The service center segment i.e	via=<service center number>
	TPtrC serviceData;

	// Parse the scheme specific part
	TDelimitedSmsParser parser;
	parser.Parse(smsPart);

	SSmsNumber num;
	while (parser.GetNext(segment) != KErrNotFound)
		{
		// Look for field separator
		TInt pos = segment.Locate(KSemiColonChar);
		if (pos != KErrNotFound)
			{
			numData.Set(segment.Left(pos));

			// Set everything after the semicolon.
			serviceData.Set(segment.Mid(pos+1));
			}
		else 
			{
			numData.Set(segment);
			serviceData.Set(KNullDesC);
			}

		// Parse the sms-number
		InetProtTextUtils::RemoveWhiteSpace(numData, InetProtTextUtils::ERemoveBoth);
		ParseSmsNumber(numData, num.iNumber);

		// Parse the service center segment
		TPtrC serviceCenter;
		GetServiceCenter(serviceData, serviceCenter);
		ParseSmsNumber(serviceCenter, num.iServiceCentre);

		// This contains sms-number and service center number
		User::LeaveIfError(iNumbers.Append(num));	
		}	
	}

TInt CSmsSchemeHandler::ParseSmsNumber(const TPtrC& aNumber, TPtrC& aParsedNumber)
	{
	if (aNumber.Length() == 0)
		{
		aParsedNumber.Set(KNullDesC);
		return KErrNotFound;
		}

	// Look for '+' at start of data. 	
	if (aNumber[0] != KPlusChar)
		// Local number 
		{
		// Parse and set the sms-number. Sets the leftmost part of the descriptor up 
		// to the first non-allowable character.
		ParseUtil::Subset(aNumber, KAllowedChars, aParsedNumber);
		}
	else
		// International number
		{
		// Parse the number without the '+'. Sets the leftmost part of the descriptor up 
		// to the first non-allowable character.
		TPtrC number;
		ParseUtil::Subset(aNumber.Mid(1), KAllowedChars, number); 

		// Number length including the '+'
		TInt numberLength = 1 + number.Length();
		
		// Set the parsed number
		aParsedNumber.Set(aNumber.Left(numberLength));
		}

	return KErrNone;
	}

void CSmsSchemeHandler::GetServiceCenter(const TPtrC& aComponent, TPtrC& aServiceCenter)
	{
	// Look for field separator
	TInt pos = aComponent.Locate(KEqualChar);
	if (pos != KErrNotFound)
		{
		// Set field name
		TPtrC fieldName = aComponent.Left(pos);
		InetProtTextUtils::RemoveWhiteSpace(fieldName, InetProtTextUtils::ERemoveBoth);

		// Check field name is valid 
		if (fieldName.CompareF(KVia) == 0)
			{
			// Set service center number - excludes '='
			aServiceCenter.Set(aComponent.Mid(pos+1));
			InetProtTextUtils::RemoveWhiteSpace(aServiceCenter, InetProtTextUtils::ERemoveBoth);
			}
		}
	else 
		aServiceCenter.Set(KNullDesC);
	} 

void CSmsSchemeHandler::SendL()
	{
	RSendAs sendAs;
	CleanupClosePushL(sendAs);
	User::LeaveIfError(sendAs.Connect());

	RSendAsMessage smsMessage;
	CleanupClosePushL(smsMessage);
	smsMessage.CreateL(sendAs, KSmsMtm);

	const TInt count = iNumbers.Count();
	for (TInt i = 0; i < count; ++i)
		{
		smsMessage.AddRecipientL(iNumbers[i].iNumber, RSendAsMessage::ESendAsRecipientTo);
		}
	smsMessage.LaunchEditorAndCloseL();
	CleanupStack::Pop(&smsMessage); // Closed by LaunchEditorAndCloseL
	CleanupStack::PopAndDestroy(&sendAs);

	}

