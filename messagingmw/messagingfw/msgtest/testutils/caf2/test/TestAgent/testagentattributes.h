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

#ifndef __TESTAGENTATTRIBUTES_H__
#define __TESTAGENTATTRIBUTES_H__

#include <caf/virtualpathptr.h>
#include <caf/attributeset.h>
#include <caf/stringattributeset.h>
#include "TestAgentFileInterface.h"

using namespace ContentAccess;

	/**
	All attribute values are common between the CAgentData, CAgentContent and
	CAgentManager. The OMA DCF attributes are mostly fixed anyway so this class
	implements helper functions for all of the attribute functions

	 */
class TTestAgentAttributes
		{
	public:
		static TInt GetAttributeSet(CTestAgentFileInterface& aFile, RAttributeSet& aAttributeSet);
		static TInt GetStringAttributeSet(CTestAgentFileInterface& aFile, RStringAttributeSet& aStringAttributeSet);
		static TInt CheckUniqueId(const TDesC& aUniqueId);
		static TInt CheckVirtualPath(const TVirtualPathPtr& aVirtualPath);	
		};

#endif // __TESTAGENTATTRIBUTES_H__
