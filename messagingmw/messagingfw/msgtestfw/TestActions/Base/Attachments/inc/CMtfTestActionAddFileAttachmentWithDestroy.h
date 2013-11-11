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


#ifndef __CMTFTESTACTIONADDFILEATTACHMENTWITHDESTROY_H__
#define __CMTFTESTACTIONADDFILEATTACHMENTWITHDESTROY_H__

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionAddFileAttachmentWithDestroy,"AddFileAttachmentWithDestroy");

class CMtfTestActionAddFileAttachmentWithDestroy : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionAddFileAttachmentWithDestroy(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionAddFileAttachmentWithDestroy(CMtfTestCase& aTestCase);
	};


#endif // __CMTFTESTACTIONADDFILEATTACHMENTWITHDESTROY_H__
