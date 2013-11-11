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


#ifndef __DRMFILESPARSER_H__
#define __DRMFILESPARSER_H__

#include <xml/parser.h>
#include "parsehandlerbase.h"

using namespace Xml;

namespace ReferenceTestAgent
	{
	class CDrmFiles;
	class CVirtualFile;
	
	/** parser for the DRMFile tag
	*/
	class CDrmFilesParser : public CParseHandlerBase
		{
	public:
		static CDrmFilesParser* NewL();
		IMPORT_C static CDrmFilesParser* NewL(CVirtualFile& aVirtualFile);
		virtual ~CDrmFilesParser();

		void ParseL(const TDesC8& aFragment);
		IMPORT_C CDrmFiles& DrmFilesL();
		
		// Overrides CParseHandlerBase behavior.
		virtual void OnStartDocumentL(const RDocumentParameters& aDocParam, TInt aErrorCode);
		virtual void OnEndDocumentL(TInt aErrorCode);

		virtual void ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes);
		virtual void ParseEndElementL(const RTagInfo& aElement);

	private:
		CDrmFilesParser();
		void ConstructL();
		void ConstructL(CVirtualFile& aVirtualFile);

	private:
		TBool iFoundStartTag;
		CDrmFiles* iDrmFiles;
		CParser* iXmlParser;
		TBool iParseComplete;
		};
	}

#endif
