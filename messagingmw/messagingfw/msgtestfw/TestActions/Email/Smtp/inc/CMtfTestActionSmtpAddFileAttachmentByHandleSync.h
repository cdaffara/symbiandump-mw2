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


#ifndef __CMTFTESTACTIONSMTPADDFILEATTACHMENTBYHANDLESYNC_H__
#define __CMTFTESTACTIONSMTPADDFILEATTACHMENTBYHANDLESYNC_H__

#include <f32file.h>

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionSmtpAddFileAttachmentByHandleSync,"SmtpAddFileAttachmentByHandleSync");

class CMtfTestActionSmtpAddFileAttachmentByHandleSync : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSmtpAddFileAttachmentByHandleSync(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionSmtpAddFileAttachmentByHandleSync(CMtfTestCase& aTestCase);
	
	void RunTestL();
	RFile OpenFileLC(RFs& aFs, const TDesC& aFilePath);
	};


#endif // __CMTFTESTACTIONSMTPADDFILEATTACHMENTBYHANDLESYNC_H__
