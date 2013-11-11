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


#ifndef __CMTFTESTACTIONSMTPGETATTACHMENTFILEFROMINDEX_H__
#define __CMTFTESTACTIONSMTPGETATTACHMENTFILEFROMINDEX_H__

#include "CMtfSynchronousTestAction.h"

#include <f32file.h>

_LIT(KTestActionSmtpGetAttachmentFileFromIndex,"SmtpGetAttachmentFileFromIndex");

class CMtfTestActionSmtpGetAttachmentFileFromIndex : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSmtpGetAttachmentFileFromIndex(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionSmtpGetAttachmentFileFromIndex(CMtfTestCase& aTestCase);
	
	void RunTestActionL();
	void CompareFileL(RFile& aFile, const TDesC& aDataFilePath);
	};


#endif // __CMTFTESTACTIONSMTPGETATTACHMENTFILEFROMINDEX_H__
