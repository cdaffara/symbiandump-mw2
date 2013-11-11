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

#include "ContextParser.h"
#include "intentparser.h"
#include "stringparser.h"
#include "drmrights.h"
#include "rightsobject.h"
#include "tags.h"
#include "rtapanic.h"

using namespace ReferenceTestAgent;

_LIT8(KNewLineConst, "\n");
CContextparser* CContextparser::NewL(CRightsObject& aRights)
	{
	CContextparser* self = new (ELeave) CContextparser(aRights);
	return self;
	}
		
CContextparser::CContextparser(CRightsObject& aRights) : iRights(aRights)

	{
	}

CContextparser::~CContextparser()
	{
	}


void CContextparser::ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& /*aAttributes*/)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	if(KContexttag().CompareF(tag) == 0)
		{
		//Parse the context
		iContextParsed=ETrue;
		iSiblingsCount++;
		}
	else if(KVersion().CompareF(tag)==0 && iContextParsed)
		{
		//Parse the version 
		iVersionparsed=ETrue;
		iSiblingsCount++;
		}
	else if(KUid().CompareF(tag)==0 && iContextParsed)
		{
		//Parse the Uid
		iUidParsed=ETrue;
		iSiblingsCount++;
		}
	else
		{
		LogL(_L("ERROR: Unexpected Start Element <%S>"), tag);
		
		// unexpected start tag
		User::Leave(KErrCorrupt);	
		}
	}
		

void CContextparser::ParseEndElementL(const RTagInfo& aElement)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	// Check the end tag is </context>. The version and uid flags should be false.
	if(KContexttag().CompareF(tag)==0 && !iVersionparsed && !iUidParsed)
		{
		iContextParsed=EFalse;
		iSiblingsCount--;		
		}
	else if(KVersion().CompareF(tag)==0 && iVersionparsed)
		{
		iVersionparsed=EFalse;
		iSiblingsCount--;		
		}
	else if(KUid().CompareF(tag)==0 && iUidParsed)
		{
		iUidParsed=EFalse;
		iSiblingsCount--;		
		}
	else
		{
		LogL(_L("  End Element <%S> not expected"), aElement.LocalName().DesC());
		User::Leave(KErrCorrupt);		
		}
	}

void CContextparser::ParseContentL(const TDesC8& aBytes)
	{
	if(aBytes.CompareF(KNewLineConst)!=0)
		{
		if(iVersionparsed)
			{
			iRights.AddVersionL(aBytes);
			}
		else if(iUidParsed)
			{
			iRights.AddUidL(aBytes);		
			}
		}
	}
