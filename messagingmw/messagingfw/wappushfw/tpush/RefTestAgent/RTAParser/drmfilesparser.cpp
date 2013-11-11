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
// drmfileparser.cpp
// 
//

#include "drmfilesparser.h"
#include "drmfilecontainer.h"
#include "containerparser.h"
#include "contentparser.h"
#include "drmfiles.h"
#include "virtualFile.h"
#include "tags.h"

using namespace ReferenceTestAgent;

_LIT8(KXmlMimeType, "text/xml");

EXPORT_C CDrmFilesParser* CDrmFilesParser::NewL(CVirtualFile& aVirtualFile)
	{
	CDrmFilesParser* self = new (ELeave) CDrmFilesParser;
	CleanupStack::PushL(self);
	self->ConstructL(aVirtualFile);
	CleanupStack::Pop(self);
	return self;
	}

CDrmFilesParser* CDrmFilesParser::NewL()
	{
	CDrmFilesParser* self = new (ELeave) CDrmFilesParser;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

		
CDrmFilesParser::CDrmFilesParser() : iParseComplete(EFalse)
	{
	}

void CDrmFilesParser::ConstructL(CVirtualFile& aVirtualFile)	
	{
	iDrmFiles = CDrmFiles::NewL();
	CParser* xmlParser = CParser::NewLC(KXmlMimeType(), *this);
	xmlParser->ParseBeginL();
	
	// Parse the file in chunks
	TBuf8 <1024> buffer;
	TInt length = buffer.MaxLength();
	while(length > 0)
		{
		User::LeaveIfError(aVirtualFile.Read(buffer,length));
		length = buffer.Length();
		xmlParser->ParseL(buffer);
		}

	// Finished parsing
	xmlParser->ParseEndL();
	CleanupStack::PopAndDestroy(xmlParser); 
	if(!iParseComplete)
		{
		LogL(_L("Incomplete XML file"));
		User::Leave(KErrCorrupt);
		}
	}

void CDrmFilesParser::ConstructL()	
	{
	_LIT8(KXmlMimeType, "text/xml");
	iDrmFiles = CDrmFiles::NewL();
	iXmlParser = CParser::NewL(KXmlMimeType(), *this);
	iXmlParser->ParseBeginL();
	}

CDrmFilesParser::~CDrmFilesParser()
	{
	delete iDrmFiles;
	delete iXmlParser;
	}

void CDrmFilesParser::ParseL(const TDesC8& aFragment)
	{
	iXmlParser->ParseL(aFragment);
	}

EXPORT_C CDrmFiles& CDrmFilesParser::DrmFilesL()
	{
	if(!iParseComplete && iXmlParser)
		{
		iXmlParser->ParseEndL();

		// If parsing is still incomplete must be something wrong with the file	
		if(!iParseComplete)
			{
			LogL(_L("Incomplete XML file"));			
			User::Leave(KErrCorrupt);
			}
		}
	return *iDrmFiles;
	}

void CDrmFilesParser::OnStartDocumentL(const RDocumentParameters& /*aDocParam*/, TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);
	LogL(_L("Start of content files XML document"));
	}

void CDrmFilesParser::OnEndDocumentL(TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);
	LogL(_L("End of content files XML document"));
	}

void CDrmFilesParser::ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	if(KDrmFileTag().CompareF(tag) == 0)
		{
		if(iFoundStartTag)
			{
			LogL(_L("ERROR: <drmfiles> tag found inside <drmfiles> tag"));
			// We have already found the <DRMFiles> tag, there shouldn't be two of them
			User::Leave(KErrCorrupt);
			}
		iFoundStartTag = ETrue;
		LogL(_L("Start element <drmfiles>"));
		}
	else if(KContainerTag().CompareF(tag) == 0)
		{
		// <container>

		// add a new container to the root container
		CDrmFileContainer& newContainer = iDrmFiles->RootContainer().AddContainerL();
				
		// create a parser for the new container
		StartChildParserL(CContainerParser::NewL(newContainer), aElement, aAttributes);
		}
	else if(KContentTag().CompareF(tag) == 0)
		{
		// <content cid = "...."> 
		// Create a parser for the content object
		StartChildParserL(CContentParser::NewL(iDrmFiles->RootContainer()), aElement, aAttributes);
		}
	else
		{
		LogL(_L("ERROR: Unexpected start element <%S>"), tag);
		// unexpected start tag
		User::Leave(KErrCorrupt);	
		}
	}
		

void CDrmFilesParser::ParseEndElementL(const RTagInfo& aElement)
	{
	TPtrC8 tag(aElement.LocalName().DesC());	
	
	// Check the end tag is </drmfiles>
	if(KDrmFileTag().CompareF(tag) != 0)
		{
		LogL(_L("ERROR: Unexpected End element <%S>, expected </drmfiles>"), tag);
		User::Leave(KErrCorrupt);
		}

	LogL(_L("  End element </drmfiles>"));
	iParseComplete = ETrue;
	iDrmFiles->UpdateContentL();
	}
