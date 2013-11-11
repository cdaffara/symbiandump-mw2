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


#ifndef __CMTFTESTACTIONADDLINKEDATTACHMENT_H__
#define __CMTFTESTACTIONADDLINKEDATTACHMENT_H__

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionAddLinkedAttachment,"AddLinkedAttachment");

class CMtfTestActionAddLinkedAttachment : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionAddLinkedAttachment(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionAddLinkedAttachment(CMtfTestCase& aTestCase);
	
	void RunTestL();
	};


#endif // __CMTFTESTACTIONADDLINKEDATTACHMENT_H__
