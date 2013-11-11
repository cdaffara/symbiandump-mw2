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


#ifndef __CMTFTESTACTIONADDFILEATTACHMENTBYHANDLE_H__
#define __CMTFTESTACTIONADDFILEATTACHMENTBYHANDLE_H__

#include "CMtfTestAction.h"

_LIT(KTestActionAddFileAttachmentByHandle,"AddFileAttachmentByHandle");

#include <cmsvattachment.h>

class CMsvEntry;
class CMsvStore;
class CMsvAttachment;

class CMtfTestActionAddFileAttachmentByHandle : public CMtfTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionAddFileAttachmentByHandle(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionAddFileAttachmentByHandle(CMtfTestCase& aTestCase);
	
	void RunTestL();
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	RFile OpenFileL(RFs& aFs, const TDesC& aFilePath);
	
private:
	CMsvEntry* iEntry;
	CMsvStore* iStore;
	CMsvAttachment* iAttachment;
	TMsvAttachmentId iAttachmentId;
	};


#endif // __CMTFTESTACTIONADDFILEATTACHMENTBYHANDLE_H__
