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


#ifndef __CMTFTESTACTIONSMTPGETATTACHMENTFILEFROMID_H__
#define __CMTFTESTACTIONSMTPGETATTACHMENTFILEFROMID_H__

#include "CMtfSynchronousTestAction.h"

#include <f32file.h>

_LIT(KTestActionSmtpGetAttachmentFileFromId,"SmtpGetAttachmentFileFromId");

class CMtfTestActionSmtpGetAttachmentFileFromId : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSmtpGetAttachmentFileFromId(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionSmtpGetAttachmentFileFromId(CMtfTestCase& aTestCase);
	
	void RunTestActionL();
	void CompareFileL(RFile& aFile, const TDesC& aDataFilePath);
	};


#endif // __CMTFTESTACTIONSMTPGETATTACHMENTFILEFROMID_H__
