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

#include "AgreementParser.h"
#include "intentparser.h"
#include "stringparser.h"
#include "drmrights.h"
#include "rightsobject.h"
#include "tags.h"
#include "rtapanic.h"
#include "ContextParser.h"

using namespace ReferenceTestAgent;

_LIT8(KNewLineConst, "\n");
CAgreementParser* CAgreementParser::NewL(CRightsObject& aRights)
	{
	CAgreementParser* self = new (ELeave) CAgreementParser(aRights);
	return self;
	}
		
CAgreementParser::CAgreementParser(CRightsObject& aRights) : iRights(aRights)
	{
	}

CAgreementParser::~CAgreementParser()
	{
	}


void CAgreementParser::ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	if(KAgreementTag().CompareF(tag) == 0)
		{
		iAgreementParser=ETrue;
		iSiblingsCount++;				
		}
	else if(KAssetTag().CompareF(tag) == 0 && iAgreementParser)
		{
		iAssetParser=ETrue;
		iSiblingsCount++;				
		}
	else if(KKeyInfo().CompareF(tag) == 0 && iAssetParser)
		{
		iKeyInfoParser=ETrue;
		iSiblingsCount++;						
		}
	else if(KContexttag().CompareF(tag) == 0 && iAssetParser)
		{
		StartChildParserL(CContextparser::NewL(iRights), aElement, aAttributes);
		}
	else if(KPermissionTag().CompareF(tag) == 0 && iAgreementParser)
		{
		iPermissionParser=ETrue;
		iSiblingsCount++;				
		}
	else if(KPlayTag().CompareF(tag) == 0 || KDisplaytag().CompareF(tag) == 0 ||
			KExecutetag().CompareF(tag) == 0 || KPrinttag().CompareF(tag) == 0 
			&& iPermissionParser)
		{
		StartChildParserL(CIntentParser::NewL(iRights), aElement, aAttributes);
		}
	else if(KKeyValue().CompareF(tag) == 0 && iKeyInfoParser)
		{
		iKeyValueParser=ETrue;
		iSiblingsCount++;						
		}
	else
		{
		LogL(_L("ERROR: Unexpected Start Element <%S>"), tag);
		// unexpected tag
		User::Leave(KErrCorrupt);	
		}
	}
		

void CAgreementParser::ParseEndElementL(const RTagInfo& aElement)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	// Check the end tag is </agreement>
	if(KAgreementTag().CompareF(tag)==0 && !iAssetParser && !iPermissionParser)
		{
		iAgreementParser=EFalse;
		iSiblingsCount--;				
		}
	else if(KAssetTag().CompareF(tag)==0 && !iKeyInfoParser)
		{
		iAssetParser=EFalse;
		iSiblingsCount--;				
		}
	else if(KKeyInfo().CompareF(tag) == 0 && !iKeyValueParser)
		{
		iKeyInfoParser=EFalse;
		iSiblingsCount--;						
		}
	else if(KPermissionTag().CompareF(tag)==0)
		{
		iPermissionParser=EFalse;
		iSiblingsCount--;				
		}
	else if(KKeyValue().CompareF(tag) == 0 )
		{
		iKeyValueParser=EFalse;
		iSiblingsCount--;						
		}
	else
		{
		LogL(_L("  End Element <%S> not expected"), aElement.LocalName().DesC());
		User::Leave(KErrCorrupt);		
		}
	}

void CAgreementParser::ParseContentL(const TDesC8& aBytes)
	{
	if(aBytes.CompareF(KNewLineConst)!=0 && iKeyValueParser)
		{
		iRights.AddKeyValueL(aBytes);
		}
	}
