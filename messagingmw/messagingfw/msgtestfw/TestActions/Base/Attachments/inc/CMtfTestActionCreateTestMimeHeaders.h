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


#ifndef __CMTFTESTACTIONCREATETESTMIMEHEADERS_H__
#define __CMTFTESTACTIONCREATETESTMIMEHEADERS_H__

#include "CMtfSynchronousTestAction.h"

#include <cmsvattachment.h>

class CMsvEntry;

_LIT(KTestActionCreateTestMimeHeaders,"CreateTestMimeHeaders");

class CMtfTestActionCreateTestMimeHeaders : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCreateTestMimeHeaders(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionCreateTestMimeHeaders(CMtfTestCase& aTestCase);
	void CompareMimeHeadersL(CMsvEntry& aEntry, TMsvAttachmentId aAttachId);
	};


#endif // __CMTFTESTACTIONCREATETESTMIMEHEADERS_H__
