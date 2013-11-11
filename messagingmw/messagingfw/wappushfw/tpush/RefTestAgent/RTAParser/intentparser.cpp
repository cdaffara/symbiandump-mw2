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

#include "intentparser.h"
#include "expiryparser.h"
#include "stringparser.h"
#include "drmrights.h"
#include "intent.h"
#include "rightsobject.h"
#include "rightscriteriacount.h"
#include "tags.h"
#include "rtapanic.h"

using namespace ReferenceTestAgent;

using namespace ContentAccess;
_LIT8(KNewLineConst, "\n");

CIntentParser* CIntentParser::NewL(CRightsObject& aRights)
	{
	CIntentParser* self = new (ELeave) CIntentParser(aRights);
	return self;
	}
		
CIntentParser::CIntentParser(CRightsObject& aRights) : iRights(aRights)
	{
	}

CIntentParser::~CIntentParser()
	{
	}

void CIntentParser::ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& /*aAttributes*/)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	if(KPlayTag().CompareF(tag)==0 && !iPlayParser)
		{
		iPlayParser = ETrue;
		iConstraints |= 0x01;
		} 
	else if(KDisplaytag().CompareF(tag)==0 && !iDisplayParser)
		{
		iDisplayParser = ETrue;
		iConstraints |= 0x02;
		}
	else if(KExecutetag().CompareF(tag)==0 && !iExecuteParser)
		{
		iExecuteParser = ETrue;
		iConstraints |= 0x04;
		}
	else if(KPrinttag().CompareF(tag)==0 && !iPrintParser)
		{
		iPrintParser = ETrue;
		iConstraints |= 0x08;
		}
	else if(KConstrainttag().CompareF(tag) == 0 && !iConstraintParser)
		{
		iConstraintParser=ETrue;
		iSiblingsCount++;
		}
	else if(KCounttag().CompareF(tag)==0 && iConstraintParser)
		{
		iCountParser=ETrue;
		iSiblingsCount++;
		}
	}
		

void CIntentParser::ParseEndElementL(const RTagInfo& aElement)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	if(KPlayTag().CompareF(tag)==0 && iPlayParser)
		{
		iPlayParser = EFalse;
		} 
	else if(KDisplaytag().CompareF(tag)==0 && iDisplayParser)
		{
		iDisplayParser = EFalse;
		}
	else if(KExecutetag().CompareF(tag)==0 && iExecuteParser)
		{
		iExecuteParser = EFalse;
		}
	else if(KPrinttag().CompareF(tag)==0 && iPrintParser )
		{
		iPrintParser = EFalse;
		}
	else if(KConstrainttag().CompareF(tag) == 0 && !iCountParser && iConstraintParser)
		{
		iSiblingsCount--;
		iConstraintParser=EFalse;
		}
	else if(KCounttag().CompareF(tag)==0)
		{
		iSiblingsCount--;
		iCountParser=EFalse;
		}
	
	}

void CIntentParser::ParseContentL(const TDesC8& aBytes)
	{
	if(aBytes.CompareF(KNewLineConst)!=0 && iCountParser)
		{
		iRights.AddCountL(aBytes);
		}
	}
