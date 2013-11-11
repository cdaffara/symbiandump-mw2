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

#include "drmrightsparser.h"
#include "ContextParser.h"
#include "AgreementParser.h"
#include "rightsparser.h"
#include "drmrights.h"
#include "virtualFile.h"
#include "tags.h"
#include "rightsobject.h"
#include "ROAPKeyParser.h"
using namespace ReferenceTestAgent;

_LIT8(KXmlMimeType, "text/xml");

EXPORT_C CDrmRightsParser* CDrmRightsParser::NewL()
	{
	CDrmRightsParser* self = new (ELeave) CDrmRightsParser;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CDrmRightsParser* CDrmRightsParser::NewL(CVirtualFile& aVirtualFile)
	{
	CDrmRightsParser* self = new (ELeave) CDrmRightsParser;
	CleanupStack::PushL(self);
	self->ConstructL(aVirtualFile);
	CleanupStack::Pop(self);
	return self;
	}

		
CDrmRightsParser::CDrmRightsParser() : iParseComplete(EFalse)
	{
	}

void CDrmRightsParser::ConstructL()	
	{	
	iDrmRights = CDrmRights::NewL();
	iRights = CRightsObject::NewL();
	iXmlParser = CParser::NewL(KXmlMimeType(), *this);
	iXmlParser->ParseBeginL();
	}

void CDrmRightsParser::ConstructL(CVirtualFile& aVirtualFile)
	{
	iDrmRights = CDrmRights::NewL();
	iRights = CRightsObject::NewL();
	CParser *xmlParser = CParser::NewLC(KXmlMimeType(), *this);
	xmlParser->ParseBeginL();
	
	// Parse the xml file in chunks
	TBuf8 <1024> buffer;
	TInt length = buffer.MaxLength();
	while(length > 0)
		{
		User::LeaveIfError(aVirtualFile.Read(buffer,length));
		length = buffer.Length();
		xmlParser->ParseL(buffer);
		}

	xmlParser->ParseEndL();
	// Finished parsing
	CleanupStack::PopAndDestroy(xmlParser); 
	
	if(!iParseComplete)
		{
		LogL(_L("Incomplete XML file"));
		User::Leave(KErrCorrupt);
		}
	}


CDrmRightsParser::~CDrmRightsParser()
	{
	delete iDrmRights;
	delete iXmlParser;
	delete iRights;
	}

EXPORT_C void CDrmRightsParser::ParseL(const TDesC8& aFragment)
	{
	iXmlParser->ParseL(aFragment);
	}

EXPORT_C CDrmRights& CDrmRightsParser::DrmRightsL()
	{
	// If parsing is still incomplete must be something wrong with the file	
	if(!iParseComplete && iSiblingsCount)
		{
		LogL(_L("Incomplete XML file"));			
		User::Leave(KErrCorrupt);
		}
	return *iDrmRights;
	}

void CDrmRightsParser::OnStartDocumentL(const RDocumentParameters& /*aDocParam*/, TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);
	LogL(_L("Start of rights XML document"));
	}

void CDrmRightsParser::OnEndDocumentL(TInt aErrorCode)
	{
	User::LeaveIfError(aErrorCode);
	LogL(_L("End of rights XML document"));
	}

void CDrmRightsParser::ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	// <Rights>
	if(KDrmRightsTag().CompareF(tag) == 0)
		{
		iDrmRightsParsed=ETrue;
		iSiblingsCount++;
		}
	else if(KContexttag().CompareF(tag) == 0)
		{
		// <Context>
		// create a child parser for context object
		StartChildParserL(CContextparser::NewL(*iRights), aElement, aAttributes);
		}
	else if(KAgreementTag().CompareF(tag) == 0)
		{
		// <Agreement>
		// create a child parser for agreement object
		StartChildParserL(CAgreementParser::NewL(*iRights), aElement, aAttributes);
		}
	//ROAP Response Tags.  	
	else if(KRoapResponse().CompareF(tag)==0)
		{
		iResponseParsed=ETrue;
		iSiblingsCount++;
		}
	else if(KDeviceId().CompareF(tag) == 0 && iResponseParsed)
		{
		StartChildParserL(CRoapKeyParser::NewL(), aElement, aAttributes);
		}
	else if(KriID().CompareF(tag) == 0 )
		{
		StartChildParserL(CRoapKeyParser::NewL(), aElement, aAttributes);
		}
	else if(KNonce().CompareF(tag) == 0 )
		{
		iNonceParsed=ETrue;
		iSiblingsCount++;						
		}
	else if(KprotectedRO().CompareF(tag) == 0 && iResponseParsed)
		{
		iProtectedROParsed=ETrue;
		iSiblingsCount++;						
		}
	else if(KSignature().CompareF(tag) == 0 )
		{
		iSignatureParsed=ETrue;
		iSiblingsCount++;
		}
	else if(KRightsObject().CompareF(tag)==0 && iProtectedROParsed )
		{
		iROParsed=ETrue;
		iSiblingsCount++;	
		}
	//ROAP Trigger Tags	
	else if(KRoapTrigger().CompareF(tag)==0)
		{
		iSiblingsCount++;	
		}
	else if(KRegRequest().CompareF(tag)==0)
		{
		iSiblingsCount++;	
		}
	else if(KROAquisation().CompareF(tag)==0)
		{
		iSiblingsCount++;	
		}
	else if(KJoinDomain().CompareF(tag)==0)
		{
		iSiblingsCount++;	
		}
	else if(KLeaveDomain().CompareF(tag)==0)
		{
		iSiblingsCount++;	
		}
	else if(KRiAlias().CompareF(tag)==0)
		{
		iSiblingsCount++;	
		}
	else if(KRoapURL().CompareF(tag)==0)
		{
		iSiblingsCount++;	
		}
	else if(KDomainID().CompareF(tag)==0)
		{
		iSiblingsCount++;	
		}
	else if(KRoID().CompareF(tag)==0)
		{
		iSiblingsCount++;	
		}
	else if(KContentID().CompareF(tag)==0)
		{
		iSiblingsCount++;	
		}
	else
		{
		LogL(_L("ERROR: Unexpected start element <%S>"), tag);
		// unexpected start tag
		User::Leave(KErrCorrupt);	
		}
	}
		

void CDrmRightsParser::ParseEndElementL(const RTagInfo& aElement)
	{
	TPtrC8 tag(aElement.LocalName().DesC());
	// Check the end tag is </DRMRights>
	if(KDrmRightsTag().CompareF(tag)==0)
		{
		iDrmRightsParsed=EFalse;
		iSiblingsCount--;
		iDrmRights->AddL(iRights);
		iRights = NULL;
		if(!iROParsed)
			{
			iParseComplete = ETrue;
			}
		}
	else if(KRoapResponse().CompareF(tag)==0)
		{
		iResponseParsed=EFalse;
		iSiblingsCount--;				
		}
	else if(KNonce().CompareF(tag) == 0)
		{
		iNonceParsed=EFalse;
		iSiblingsCount--;						
		}
	else if(KSignature().CompareF(tag) == 0 )
		{
		iSignatureParsed=EFalse;
		iSiblingsCount--;
		}
	else if(KprotectedRO().CompareF(tag) == 0 && !iROParsed)
		{
		iProtectedROParsed=EFalse;
		iSiblingsCount--;						
		}
	else if(KRightsObject().CompareF(tag)==0)
		{
		iROParsed=EFalse;
		iSiblingsCount--;
		iParseComplete = ETrue;
		}
	else if(KRoapTrigger().CompareF(tag)==0)
		{
		iSiblingsCount--;
		iParseComplete = ETrue;
		}
	else if(KRegRequest().CompareF(tag)==0)
		{
		iSiblingsCount--;	
		}
	else if(KROAquisation().CompareF(tag)==0)
		{
		iSiblingsCount--;	
		}
	else if(KJoinDomain().CompareF(tag)==0)
		{
		iSiblingsCount--;	
		}
	else if(KLeaveDomain().CompareF(tag)==0)
		{
		iSiblingsCount--;	
		}
	else if(KRiAlias().CompareF(tag)==0)
		{
		iSiblingsCount--;	
		}
	else if(KRoapURL().CompareF(tag)==0)
		{
		iSiblingsCount--;	
		}
	else if(KDomainID().CompareF(tag)==0)
		{
		iSiblingsCount--;	
		}
	else if(KRoID().CompareF(tag)==0)
		{
		iSiblingsCount--;	
		}
	else if(KContentID().CompareF(tag)==0)
		{
		iSiblingsCount--;	
		}
	else
		{
		LogL(_L("ERROR: Unexpected end element </%S>, expected </drmrights>"), aElement.LocalName().DesC());
		User::Leave(KErrCorrupt);
		}
	LogL(_L("  End Element </drmrights>"), aElement.LocalName().DesC());
	}
