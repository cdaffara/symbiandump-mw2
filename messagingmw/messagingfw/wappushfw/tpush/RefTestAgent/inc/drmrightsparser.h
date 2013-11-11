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

/**
 @file
 @internalComponent
 @released
*/


#ifndef __DRMRIGHTSPARSER_H__
#define __DRMRIGHTSPARSER_H__

#include <xml/parser.h>
#include "parsehandlerbase.h"

using namespace Xml;

namespace ReferenceTestAgent
	{
	class CDrmRights;
	class CVirtualFile;
	class CRightsObject;
	/** parser for the <DrmRights> tag
	*/
	class CDrmRightsParser : public CParseHandlerBase
		{
	public:
		IMPORT_C static CDrmRightsParser* NewL();
		IMPORT_C static CDrmRightsParser* NewL(CVirtualFile& aVirtualFile);
		virtual ~CDrmRightsParser();

		IMPORT_C void ParseL(const TDesC8& aFragment);
		IMPORT_C CDrmRights& DrmRightsL();
		
		// Overrides CParseHandlerBase behavior.
		virtual void OnStartDocumentL(const RDocumentParameters& aDocParam, TInt aErrorCode);
		virtual void OnEndDocumentL(TInt aErrorCode);

		virtual void ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes);
		virtual void ParseEndElementL(const RTagInfo& aElement);
		
	private:
		CDrmRightsParser();
		void ConstructL();
		void ConstructL(CVirtualFile& aVirtualFile);

	private:
		TBool iFoundStartTag;
		CDrmRights* iDrmRights;
		CParser* iXmlParser;
		TBool iParseComplete;
		CRightsObject *iRights;
		TBool iDrmRightsParsed;
		TBool iResponseParsed;
		TBool iNonceParsed;
		TBool iProtectedROParsed;
		TBool iROParsed;
		TBool iSignatureParsed;
		};
	}

#endif
