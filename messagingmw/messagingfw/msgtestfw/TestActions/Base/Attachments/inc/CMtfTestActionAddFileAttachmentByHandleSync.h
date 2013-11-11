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


#ifndef __CMTFTESTACTIONADDFILEATTACHMENTBYHANDLESYNC_H__
#define __CMTFTESTACTIONADDFILEATTACHMENTBYHANDLESYNC_H__

#include <f32file.h>

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionAddFileAttachmentByHandleSync,"AddFileAttachmentByHandleSync");

class CMtfTestActionAddFileAttachmentByHandleSync : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionAddFileAttachmentByHandleSync(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionAddFileAttachmentByHandleSync(CMtfTestCase& aTestCase);
	
	void RunTestL();
	RFile OpenFileLC(RFs& aFs, const TDesC& aFilePath);
	};


#endif // __CMTFTESTACTIONADDFILEATTACHMENTBYHANDLESYNC_H__
