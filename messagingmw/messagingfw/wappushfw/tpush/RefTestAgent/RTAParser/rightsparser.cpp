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
// drmrightsparser.cpp
// 
//

#include "rightsparser.h"
#include "intentparser.h"
#include "stringparser.h"
#include "drmrights.h"
#include "rightsobject.h"
#include "tags.h"
#include "rtapanic.h"

using namespace ReferenceTestAgent;


CRightsParser* CRightsParser::NewL(CDrmRights& aDrmRights)
	{
	CRightsParser* self = new (ELeave) CRightsParser(aDrmRights);
	return self;
	}
		
CRightsParser::CRightsParser(CDrmRights& aDrmRights) : iDrmRights(aDrmRights)

	{
	}

CRightsParser::~CRightsParser()
	{
	delete iCid;
	delete iRights;
	}


void CRightsParser::ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	if(KRightsTag().CompareF(tag) == 0)
		{
		if(iCid)
			{
			LogL(_L("ERROR: <rights> element not expected inside <rights> element"));
			
			// We have already found the <rights> tag, there shouldn't be two of them
			User::Leave(KErrCorrupt);
			}

		//We may need check the attribute and store them in the rights.db. need to implement if required
		TRAPD(err, iCid = GetTagAttributeL(aAttributes, KCidTag()).AllocL());
		if(err != KErrNone)
			{
			LogL(_L("ERROR: Start element <rights> missing \"cid\" attribute"));	
			User::Leave(err);
			}
		LogL(_L("Start Element <rights cid=\"%S\">"),*iCid);
		}
/*	else if(KForwardLockTag().CompareF(tag) == 0)
		{
		if(iRights)
			{
			LogL(_L("  Only one of <forwardlock>, <pending> or <intent> can appear inside a single rights element"));
			// must be too many tags within the rights tag
			User::Leave(KErrCorrupt);
			}

		// <forwardlock>
		// create a parser for forwardlock element
		LogL(_L("Start Element <forwardlock>"));
		iWaitingForTag = EForwardLockTag;
		StartChildParserL(CStringParser::NewL(*this), aElement, aAttributes);
		}
	else if(KPendingTag().CompareF(tag) == 0)
		{
		if(iRights)
			{
			LogL(_L("  Only one of <forwardlock>, <pending> or <intent> can appear inside a single rights element"));
			// must be too many tags within the rights tag
			User::Leave(KErrCorrupt);
			}

		// <pending>
		// create a parser for forwardlock element		
		LogL(_L("Start Element <pending>"));
		iWaitingForTag = EPendingTag;
		StartChildParserL(CStringParser::NewL(*this), aElement, aAttributes);
		}
	else if(KIntentTag().CompareF(tag) == 0)
		{
		if(iRights)
			{
			LogL(_L("  Only one of <forwardlock>, <pending> or <intent> can appear inside a single rights element"));
			// must be too many tags within the rights tag
			User::Leave(KErrCorrupt);
			}

		// <intent>
		// create a parser for the intent tag
		iRights = CRightsObject::NewL(*iCid, EFalse, EFalse);
		StartChildParserL(CIntentParser::NewL(*iRights), aElement, aAttributes);
		}
*/	else
		{
		LogL(_L("ERROR: Unexpected Start Element <%S>"), tag);
		
		// unexpected start tag
		User::Leave(KErrCorrupt);	
		}
	}
		

void CRightsParser::ParseEndElementL(const RTagInfo& aElement)
	{
	// Check the end tag is </rights> and that we now have a rights element we can add
	if(KRightsTag().CompareF(aElement.LocalName().DesC()) != 0 || !iRights)
		{
		LogL(_L("  End Element <%S> not expected"), aElement.LocalName().DesC());
		User::Leave(KErrCorrupt);
		}
	
	LogL(_L("  End Element </rights>"));
	// Validate and add rights object to iDrmRights
//	iDrmRights.AddL(iRights);
	iRights = NULL;
	}

void CRightsParser::StringParseCompleteL(const TDesC& aString)
	{
	TInt delay = 0;
	TLex16 lex(aString);
	
	switch(iWaitingForTag)
		{
	case EForwardLockTag:
//		iRights = CRightsObject::NewL(*iCid, ETrue, EFalse);
		break;
	case EPendingTag:
		lex.Val(delay);
//		iRights = CRightsObject::NewL(*iCid, EFalse, ETrue, delay);
		break;

	default:
		User::Panic(KRtaPanicString(), ERtaPanicParserInvalidState);
		break;
		}
	}
