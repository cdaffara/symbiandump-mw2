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


#ifndef __CMTFTESTACTIONSMTPCREATEATTACHMENT_H__
#define __CMTFTESTACTIONSMTPCREATEATTACHMENT_H__

#include "CMtfSynchronousTestAction.h"

#include <f32file.h>

_LIT(KTestActionSmtpCreateAttachment,"SmtpCreateAttachment");

class CMtfTestActionSmtpCreateAttachment : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSmtpCreateAttachment(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionSmtpCreateAttachment(CMtfTestCase& aTestCase);
	
	void RunTestL();
	void PopulateFileL(RFile& aFile, const TDesC& aDataFilePath);
	};


#endif // __CMTFTESTACTIONSMTPCREATEATTACHMENT_H__
