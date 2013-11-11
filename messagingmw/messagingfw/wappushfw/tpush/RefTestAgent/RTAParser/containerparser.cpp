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

#include "containerparser.h"
#include "contentparser.h"
#include "drmfilecontainer.h"
#include "tags.h"
#include "rtapanic.h"

using namespace ReferenceTestAgent;

// Operates on a container to parse subcontainers
CContainerParser*  CContainerParser::NewL(CDrmFileContainer& aCurrentContainer)
	{
	CContainerParser* self = new (ELeave) CContainerParser(aCurrentContainer);
	return self;
	}

CContainerParser::CContainerParser(CDrmFileContainer& aCurrentContainer) : 
iFoundOwnContainerTag(EFalse), iCurrentContainer(aCurrentContainer)
	{
	}	

CContainerParser::~CContainerParser()
	{	
	delete iName;
	delete iUniqueId;
	}


void CContainerParser::ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes)
		{
		TPtrC8 tag(aElement.LocalName().DesC());
		if(KContainerTag().CompareF(tag) == 0)
			{
			// <container>
			if(!iFoundOwnContainerTag)
				{
				// This is the first time we have been called. This tag relates to our
				// current container, it is not embedded within the current container
				iFoundOwnContainerTag = ETrue;
				LogL(_L("Start Element <container>"));
				}
			else
				{
				// We have already processed our own <container> tag so this must just
				// be some other container embedded within our current container

				// add a new container to the current container
				// iCurrentContainer has ownership of the new container
				CDrmFileContainer& newContainer = iCurrentContainer.AddContainerL();
				
				// create a parser for the new container
				StartChildParserL(CContainerParser::NewL(newContainer), aElement, aAttributes);
				}
			}
		else if(KNameTag().CompareF(tag) == 0)
			{
			if(iName)
				{
				LogL(_L("ERROR: Second <name> element found within <container>"));
				// we already found our name tag
				User::Leave(KErrCorrupt);
				}
			LogL(_L("Start element <name>"));

			// <name>
			iWaitingForTag = ENameTag;
			// This will parse the name and call StringParseCompleteL() when done
			StartChildParserL(CStringParser::NewL(*this), aElement, aAttributes);
			}	
		else if(KUniqueIdTag().CompareF(tag) == 0)
			{
			if(iUniqueId)
				{
				LogL(_L("ERROR: Second <uniqueid> element found within <container>"));
				// we already found our uniqueid tag
				User::Leave(KErrCorrupt);
				}

			LogL(_L("Start element <uniqueid>"));
			// <uniqueid>
			iWaitingForTag = EUniqueIdTag;

			StartChildParserL(CStringParser::NewL(*this), aElement, aAttributes);
			}	
		else if(KContentTag().CompareF(tag) == 0)
			{
			// <content cid = "...."> 
			// Create a parser for the content object
			StartChildParserL(CContentParser::NewL(iCurrentContainer), aElement, aAttributes);
			}
		else
			{
			LogL(_L("ERROR: Unexpected tag <%S> found"),tag);	
			
			// unexpected start tag
			User::Leave(KErrCorrupt);	
			}
		}

void CContainerParser::ParseEndElementL(const RTagInfo& aElement)
		{
		if(KContainerTag().CompareF(aElement.LocalName().DesC()) == 0)
			{
			// Validate any container that is not the root container
			if(iCurrentContainer.ParentContainer())
				{
				// Containers must have a name and unique Id specified
				if(iName && iUniqueId)
					{
					iCurrentContainer.SetNameL(*iName);
					iCurrentContainer.SetUniqueIdL(*iUniqueId);
					LogL(_L("  End element </container>"));	
					}
				else
					{
					LogL(_L("ERROR: </container> tag not expected yet, name and uniqueid must be specified first"));	
					User::Leave(KErrCorrupt);
					}
				}
			}
		else
			{
			LogL(_L("ERROR: Unexpected tag <%S>, expected </container> tag "), aElement.LocalName().DesC());	
			// We expect <\container> to be the closing tag
			User::Leave(KErrCorrupt);	
			}
		}

void CContainerParser::StringParseCompleteL(const TDesC& aString)
	{
	switch(iWaitingForTag)
		{
	case ENameTag:
		iName = aString.AllocL();
		break;
	case EUniqueIdTag:
		iUniqueId = aString.AllocL();
		break;
	default:
		User::Panic(KRtaPanicString(), ERtaPanicParserInvalidState);
		break;
		};
	}

