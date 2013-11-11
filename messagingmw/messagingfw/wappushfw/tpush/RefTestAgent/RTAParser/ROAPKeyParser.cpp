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

#include "ROAPKeyParser.h"
#include "intentparser.h"
#include "stringparser.h"
#include "drmrights.h"
#include "rightsobject.h"
#include "tags.h"
#include "rtapanic.h"
#include "ContextParser.h"
#include "drmrightsparser.h"
using namespace ReferenceTestAgent;

CRoapKeyParser* CRoapKeyParser::NewL()
	{
	CRoapKeyParser* self = new (ELeave) CRoapKeyParser();
	return self;
	}
		
CRoapKeyParser::CRoapKeyParser() 
	{
	}

CRoapKeyParser::~CRoapKeyParser()
	{
	}


void CRoapKeyParser::ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& /*aAttributes*/)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	if(KDeviceId().CompareF(tag) == 0)
		{
		iDeviceIdParsed=ETrue;
		iSiblingsCount++;
		}
	else if(KriID().CompareF(tag) == 0)
		{
		iRiIdParsed=ETrue;
		iSiblingsCount++;
		}
	else if(KKeyIdentifier().CompareF(tag) == 0 && (iDeviceIdParsed || iRiIdParsed))
		{
		iKeyIdentifierParsed=ETrue;
		iSiblingsCount++;
		}
	else if(KHash().CompareF(tag) == 0 && iKeyIdentifierParsed)
		{
		iHashParsed=ETrue;
		iSiblingsCount++;
		}
	else
		{
		LogL(_L("ERROR: Unexpected start element <%S>"), tag);
		// unexpected start tag
		User::Leave(KErrCorrupt);	
		}
	}
		

void CRoapKeyParser::ParseEndElementL(const RTagInfo& aElement)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	if(KDeviceId().CompareF(tag) == 0 && !iKeyIdentifierParsed)
		{
		iDeviceIdParsed=EFalse;
		iSiblingsCount--;
		}
	else if(KriID().CompareF(tag) == 0 && !iKeyIdentifierParsed)
		{
		iRiIdParsed=EFalse;
		iSiblingsCount--;
		}
	else if(KKeyIdentifier().CompareF(tag) == 0 && !iHashParsed)
		{
		iKeyIdentifierParsed=EFalse;
		iSiblingsCount--;
		}
	else if(KHash().CompareF(tag) == 0)
		{
		iHashParsed=EFalse;
		iSiblingsCount--;
		}
	else
		{
		LogL(_L("ERROR: Unexpected end element </%S>, expected </drmrights>"), aElement.LocalName().DesC());
		User::Leave(KErrCorrupt);
		}
	}

void CRoapKeyParser::ParseContentL(const TDesC8& /*aBytes*/)
	{
	
	}
