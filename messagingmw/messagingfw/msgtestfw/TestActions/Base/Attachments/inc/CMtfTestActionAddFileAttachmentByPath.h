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


#ifndef __CMTFTESTACTIONADDFILEATTACHMENTBYPATH_H__
#define __CMTFTESTACTIONADDFILEATTACHMENTBYPATH_H__

#include "CMtfTestAction.h"

_LIT(KTestActionAddFileAttachmentByPath,"AddFileAttachmentByPath");

#include <cmsvattachment.h>

class CMsvEntry;
class CMsvStore;
class CMsvAttachment;

class CMtfTestActionAddFileAttachmentByPath : public CMtfTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionAddFileAttachmentByPath(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionAddFileAttachmentByPath(CMtfTestCase& aTestCase);
	
	void RunTestL();
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	
private:
	CMsvEntry* iEntry;
	CMsvStore* iStore;
	CMsvAttachment* iAttachment;
	TMsvAttachmentId iAttachmentId;
	};


#endif // __CMTFTESTACTIONADDFILEATTACHMENTBYPATH_H__
