// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __INTENTPARSER_H__
#define __INTENTPARSER_H__

#include <caf/cafplatform.h>
#include "parsehandlerbase.h"
#include "stringparser.h"

using namespace ContentAccess;

namespace ReferenceTestAgent
	{
	class CRightsObject;
	class CIntent;
	
	/** parser for the <Intent> tag
	*/
	class CIntentParser : public CParseHandlerBase
		{
	public:
		static CIntentParser* NewL(CRightsObject& iRights);
		virtual ~CIntentParser();
		
		virtual void ParseStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes);
		virtual void ParseEndElementL(const RTagInfo& aElement);
		
		void ParseContentL(const TDesC8& aBytes);

	private:
		CIntentParser(CRightsObject& aDrmRights);

	private:
		CRightsObject& iRights;
		TInt iCount;
		TInt iConstraints;
		TBool iConstraintParser;
		TBool iCountParser;
		TBool iPlayParser;
		TBool iDisplayParser;
		TBool iExecuteParser;
		TBool iPrintParser;
		
		};
	}

#endif
