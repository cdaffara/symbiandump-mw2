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
// This file contains the implementation for the class defined in MtScheme.h
// 
//

/**
 @file
 @see	MtScheme.h
*/

// System includes
//		
#include <txtrich.h>
#include <inetprottextutils.h>
#include <escapeutils.h>
#include <sendas2.h>

// Local includes
//
#include "MTSCHEME.H"
#include "msgurlparser.h"

// Mailto field name
//
_LIT(KTo, "to");
_LIT(KCc, "cc");
_LIT(KBcc, "bcc");
_LIT(KSubject, "subject");
_LIT(KBody, "body");

// Constants
//
const TUid KMailMtm = {0x10001028};
const TInt KEqualChar = '=';


CMailtoSchemeHandler::CMailtoSchemeHandler()
	{
	}

CMailtoSchemeHandler* CMailtoSchemeHandler::NewLC()
	{
	CMailtoSchemeHandler* self=new (ELeave) CMailtoSchemeHandler();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CMailtoSchemeHandler* CMailtoSchemeHandler::NewL()
	{
	CMailtoSchemeHandler* self = NewLC();
	CleanupStack::Pop(self);
	return self;
	}

void CMailtoSchemeHandler::ConstructL()
	{
	CMsgSchemeHandlerBase::ConstructL();
	}

CMailtoSchemeHandler::~CMailtoSchemeHandler()
	{
	iTo.Close();
	delete iMailtoPart;
	}

void CMailtoSchemeHandler::ParseUrlL(const TDesC& aUrl)
	{
	// Get the scheme specific part
	TPtrC mailtoPart;
	GetSchemeSpecificPartL(aUrl, mailtoPart);

	// Convert %xx chars in the URL to ascii characters
	HBufC* temp = EscapeUtils::EscapeDecodeL(mailtoPart);
	delete iMailtoPart;
	iMailtoPart = temp;

	// Parse the scheme specific part
	TMailtoUrlParser parser;
	parser.Parse(*iMailtoPart);
	TPtrC segment;

	// Parse the address segment separated by ','
	TDelimitedAddressParser address = parser.ExtractAddresses();
	while (address.GetNext(segment) != KErrNotFound)
		{
		SetFieldL(ETo, segment);
		}

	// Parse the body segment separated by '&'
	TDelimitedBodyParser body = parser.ExtractBody();
	while (body.GetNext(segment) != KErrNotFound)
		{
		TPtrC value;
		TField field = GetFieldAndValue(segment, value);
		SetFieldL(field, value);
		}	
	}

void CMailtoSchemeHandler::SetFieldL(TField aField, const TDesC& aFieldValue)
	{
	
	TPtrC  value(aFieldValue); 
	InetProtTextUtils::RemoveWhiteSpace(value, InetProtTextUtils::ERemoveBoth);

	// Set the field values
	switch (aField)
		{
		case ETo:
		case ECc:
		case EBcc:
			User::LeaveIfError(iTo.Append(value));
			break;

		case ESubject:
			__ASSERT_DEBUG(!iSubject.Ptr(), User::Leave(KErrAlreadyExists));
			iSubject.Set(value);
			break;

		case EBody:
			__ASSERT_DEBUG(!iBody.Ptr(), User::Leave(KErrAlreadyExists));
			iBody.Set(value);
			break;

		case EUnknown:
		default:
			break;
		}
	}

CMailtoSchemeHandler::TField CMailtoSchemeHandler::GetFieldAndValue(const TDesC& aComponent, TPtrC& aFieldValue) const
	{
	// Initialise
	TField field = EUnknown;
	
	// Look for the field separator
	TInt pos = aComponent.Locate(KEqualChar);	

	if (pos != KErrNotFound)
		{
		// Set the field name
		TPtrC fieldName = aComponent.Left(pos);
		InetProtTextUtils::RemoveWhiteSpace(fieldName, InetProtTextUtils::ERemoveBoth);

		// Set the field value i.e everything after the equal.
		aFieldValue.Set(aComponent.Mid(pos+1));

		// Check the field name is valid and set the field.
		if (fieldName.CompareF(KTo) == 0)
			field = ETo;
		else if (fieldName.CompareF(KCc) == 0)
			field = ECc;
		else if (fieldName.CompareF(KBcc) == 0)
			field = EBcc;
		else if (fieldName.CompareF(KSubject) == 0)
			field = ESubject;
		else if (fieldName.CompareF(KBody) == 0)
			field = EBody;
		}
	else 
		aFieldValue.Set(KNullDesC);

	return field;
	}


void CMailtoSchemeHandler::SendL()
	{
	RSendAs sendAs;
	CleanupClosePushL(sendAs);
	User::LeaveIfError(sendAs.Connect()); // Connect to the SendAs2 server.

	RSendAsMessage mailMessage;
	CleanupClosePushL(mailMessage);

	mailMessage.CreateL(sendAs, KMailMtm);
	// Set recipient list
	const TInt count = iTo.Count();
	for (TInt i=0; i<count; ++i)
		{
		mailMessage.AddRecipientL(iTo[i], RSendAsMessage::ESendAsRecipientTo);
		}
	mailMessage.SetSubjectL(iSubject);
	mailMessage.SetBodyTextL(iBody);
	mailMessage.LaunchEditorAndCloseL();
	CleanupStack::Pop(&mailMessage); // Closed by LaunchEditorAndCloseL

	CleanupStack::PopAndDestroy(&sendAs);

	}
