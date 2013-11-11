// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "expiryparser.h"
#include "rightscriteriaexpiry.h"
#include "intent.h"
#include "tags.h"
#include "rtapanic.h"

using namespace ReferenceTestAgent;


CExpiryParser* CExpiryParser::NewL(CIntent& aIntent)
	{
	CExpiryParser* self = new (ELeave) CExpiryParser(aIntent);
	return self;
	}
		
CExpiryParser::CExpiryParser(CIntent& aIntent) : iIntent(aIntent)
	{
	}

CExpiryParser::~CExpiryParser()
	{
	}


void CExpiryParser::ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	if(KExpiryTag().CompareF(tag) == 0)
		{
		if(iFoundStartTag)
			{
			LogL(_L("ERROR: start element <expiry> found inside <expiry> element"));					
			// We have already found the <expiry> tag, there shouldn't be two of them
			User::Leave(KErrCorrupt);
			}
			
		iFoundStartTag = ETrue;
		LogL(_L("Start Element <expiry>"));					
		}
	else if(KDayTag().CompareF(tag) == 0)
		{
		// <day>
		LogL(_L("Start Element <day>"));					
		iWaitingForTag = EDayTag;
		StartChildParserL(CStringParser::NewL(*this), aElement, aAttributes);
		}
	else if(KMonthTag().CompareF(tag) == 0)
		{
		// <month>
		LogL(_L("Start Element <month>"));					
		iWaitingForTag = EMonthTag;
		StartChildParserL(CStringParser::NewL(*this), aElement, aAttributes);
		}
	else if(KYearTag().CompareF(tag) == 0)
		{
		// <year>
		LogL(_L("Start Element <year>"));					
		iWaitingForTag = EYearTag;
		StartChildParserL(CStringParser::NewL(*this), aElement, aAttributes);
		}
	else
		{
		LogL(_L("ERROR: unexpected start element <%S>"),tag);					
		// unexpected start tag
		User::Leave(KErrCorrupt);	
		}
	}
		

void CExpiryParser::ParseEndElementL(const RTagInfo& aElement)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	
	// Check the end tag is </expiry> 
	if(KExpiryTag().CompareF(tag) != 0 || !iDay || !iMonth || !iYear)
		{
		LogL(_L("ERROR: Unexpected end element </%S>"), aElement.LocalName().DesC());					
		User::Leave(KErrCorrupt);
		}

	LogL(_L("  End element </expiry>"));						
	
	TMonth month = EJanuary;
	switch(iMonth)
		{
		case 1:
			month = EJanuary;
			break;
		case 2:
			month = EFebruary;
			break;
		case 3:
			month = EMarch;
			break;
		case 4:
			month = EApril;
			break;
		case 5:
			month = EMay;
			break;
		case 6:
			month = EJune;
			break;
		case 7:
			month = EJuly;
			break;
		case 8:
			month = EAugust;
			break;
		case 9:
			month = ESeptember;
			break;
		case 10:
			month = EOctober;
			break;
		case 11:
			month = ENovember;
			break;
		case 12:
			month = EDecember;
			break;
		default:
			User::Leave(KErrCorrupt);
		}

	// TDateTime has zero based days (ie. December days 0 - 30)
	MCriteria* criteria = CRightsCriteriaExpiry::NewL(iDay - 1, month, iYear);
	CleanupStack::PushL(criteria);
	iIntent.AddCriteriaL(criteria);		
	CleanupStack::Pop(criteria);
	}

void CExpiryParser::StringParseCompleteL(const TDesC& aString)
	{
	TInt value = 0;
	TLex16 lex(aString);
	lex.Val(value);

	switch(iWaitingForTag)
		{
	case EDayTag:
		iDay = value;
		break;
	case EMonthTag:
		iMonth = value;
		break;
	case EYearTag:
		iYear = value;
		break;
	
	default:
		User::Panic(KRtaPanicString(), ERtaPanicParserInvalidState);
		break;
		}
	}
