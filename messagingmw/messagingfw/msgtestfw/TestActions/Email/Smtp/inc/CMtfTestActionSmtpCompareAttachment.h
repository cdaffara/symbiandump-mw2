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


#ifndef __CMTFTESTACTIONSMTPCOMPAREATTACHMENT_H__
#define __CMTFTESTACTIONSMTPCOMPAREATTACHMENT_H__

#include "CMtfSynchronousTestAction.h"

#include <f32file.h>

_LIT(KTestActionSmtpCompareAttachment,"SmtpCompareAttachment");

class CMtfTestActionSmtpCompareAttachment : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSmtpCompareAttachment(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionSmtpCompareAttachment(CMtfTestCase& aTestCase);
	
	void CompareFileL(RFile& aFile, const TDesC& aDataFilePath);
	};


#endif // __CMTFTESTACTIONSMTPCOMPAREATTACHMENT_H__
