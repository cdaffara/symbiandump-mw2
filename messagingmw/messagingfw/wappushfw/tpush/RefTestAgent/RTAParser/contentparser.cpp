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

#include "contentparser.h"
#include "attributeparser.h"
#include "stringattributeparser.h"
#include "stringparser.h"
#include "drmfilecontent.h"
#include "drmfilecontainer.h"
#include "tags.h"
#include "rtapanic.h"

using namespace ContentAccess;
using namespace ReferenceTestAgent;


CContentParser* CContentParser::NewL(CDrmFileContainer& aCurrentContainer)
	{
	CContentParser* self = new (ELeave) CContentParser(aCurrentContainer);
	return self;
	}

CContentParser::CContentParser(CDrmFileContainer& aCurrentContainer) : iCurrentContainer(aCurrentContainer)
	{	
	}

CContentParser::~CContentParser()
	{
	delete iCid;
	delete iName;
	delete iUniqueId;
	delete iFileName;
	
	iAttributeSet.Close();
	iStringAttributeSet.Close();
	}

void CContentParser::ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes)
	{
	TPtrC8 tag(aElement.LocalName().DesC());

	if(KContentTag().CompareF(tag) == 0)
		{
		if(iCid)
			{
			LogL(_L("ERROR: <content> found within a <content>"));	
			// Should not find a content tag within a content tag
			User::Leave(KErrCorrupt);
			}
		// content tag has the form <content cid="something">
		TRAPD(err, iCid = GetTagAttributeL(aAttributes, KCidTag()).AllocL());
		if(err != KErrNone)
			{
			LogL(_L("ERROR: Start Element <content> missing cid attribute"));	
			User::Leave(err);
			}
		LogL(_L("Start Element <content cid=\"%S\">"),*iCid);
		}
	else if(KNameTag().CompareF(tag) == 0)
		{
		if(iName)
			{
			LogL(_L("ERROR: Second <name> tag found for content"));
			// Should not find two name tags within a content tag
			User::Leave(KErrCorrupt);
			}
		
		LogL(_L("Start element <name>"));
		iWaitingForTag = ENameTag;
		// This will parse the name and call StringParseCompleteL() when done
		StartChildParserL(CStringParser::NewL(*this), aElement, aAttributes);
		}	
	else if(KUniqueIdTag().CompareF(tag) == 0)
		{
		if(iUniqueId)
			{
			LogL(_L("ERROR: Second <uniqueid> tag found for content"));
			// Should not find two UniqueId tags within a content tag
			User::Leave(KErrCorrupt);
			}
		
		LogL(_L("Start element <uniqueid>"));
		
		iWaitingForTag = EUniqueIdTag;
		StartChildParserL(CStringParser::NewL(*this), aElement, aAttributes);
		}	
	else if(KFileNameTag().CompareF(tag) == 0)
		{
		if(iFileName)
			{
			LogL(_L("ERROR: Second <filename> tag found for content"));
			// Should not find two filename tags within a content tag
			User::Leave(KErrCorrupt);
			}

		LogL(_L("Start element <filename>"));
		iWaitingForTag = EFileNameTag;
		StartChildParserL(CStringParser::NewL(*this), aElement, aAttributes);
		}
	else if(KAttributeTag().CompareF(tag) == 0)
		{
		// Set an attribute
		StartChildParserL(CAttributeParser::NewL(*this), aElement, aAttributes);
		}
	else if(KStringAttributeTag().CompareF(tag) == 0)
		{
		// Set a string attribute
		StartChildParserL(CStringAttributeParser::NewL(*this), aElement, aAttributes);		
		}
	else
		{
		LogL(_L("ERROR: Unexpected tag <%S> found"),tag);	
		// unexpected start tag
		User::Leave(KErrCorrupt);	
		}
	}
	
void CContentParser::ParseEndElementL(const RTagInfo& aElement)
	{
	TInt err = KErrNone;
	TBuf <KMaxDataTypeLength> mimeType;
	if(KContentTag().CompareF(aElement.LocalName().DesC()) == 0)
		{
		// Validate and add to iCurrentContainer
		// iCurrentContainer has ownership of the content
		if(iCid && iName && iUniqueId && iFileName)
			{
			LogL(_L("  End Element </content>"));
			
			err = iStringAttributeSet.GetValue(EMimeType, mimeType);
			if(err != KErrNone || mimeType.Length() == 0)
				{
				LogL(_L("ERROR: A string attribute \"mimetype\" not specified for content with cid: %S"), *iCid);
				User::Leave(KErrCorrupt);
				}
			else
				{
				iCurrentContainer.AddContentL(*iCid, *iName, *iUniqueId, *iFileName, iAttributeSet, iStringAttributeSet);
				}
			}
		else
			{
			LogL(_L("ERROR: </content> tag not expected yet, name, uniqueid, filename and the mimetype stringattribute must all be specified first"));
			User::Leave(KErrCorrupt);	
			}
		}
	else
		{
		LogL(_L("ERROR: Unexpected end element </%S>, expected </content>"),aElement.LocalName().DesC());
		// We were expecting the <\content> tag
		User::Leave(KErrCorrupt);	
		}
	}

void CContentParser::StringParseCompleteL(const TDesC& aString)
	{
	switch(iWaitingForTag)
		{
	case ENameTag:
		iName = aString.AllocL();
		break;
	case EUniqueIdTag:
		iUniqueId = aString.AllocL();
		break;
	case EFileNameTag:
		iFileName = aString.AllocL();
		break;
	default:
		User::Panic(KRtaPanicString(), ERtaPanicParserInvalidState);
		break;
		}
	}


void CContentParser::SetAttributeL(TInt aAttribute, TInt aValue)
	{
	// Called by CAttributeParser
	// This will leave if the same attribute tag appears twice
	iAttributeSet.AddL(aAttribute); 
	User::LeaveIfError(iAttributeSet.SetValue(aAttribute, aValue, KErrNone));
	}
	
void CContentParser::SetStringAttributeL(TInt aStringAttribute, const TDesC& aValue)
	{
	// Called by CStringAttributeParser
	// This will leave if the same attribute tag appears twice
	iStringAttributeSet.AddL(aStringAttribute); 
	User::LeaveIfError(iStringAttributeSet.SetValue(aStringAttribute, aValue, KErrNone));
	}
