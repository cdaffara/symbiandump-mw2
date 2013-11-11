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


#ifndef __CMTFTESTACTIONSMTPREMOVEFILEATTACHMENTBYINDEX_H__
#define __CMTFTESTACTIONSMTPREMOVEFILEATTACHMENTBYINDEX_H__

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionSmtpRemoveFileAttachmentByIndex,"SmtpRemoveFileAttachmentByIndex");

class CMtfTestActionSmtpRemoveFileAttachmentByIndex : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSmtpRemoveFileAttachmentByIndex(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionSmtpRemoveFileAttachmentByIndex(CMtfTestCase& aTestCase);
	
	void RunTestL();
	};


#endif // __CMTFTESTACTIONSMTPREMOVEFILEATTACHMENTBYINDEX_H__
