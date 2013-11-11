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


#ifndef __CMTFTESTACTIONGETATTACHMENTINFOBYID_H__
#define __CMTFTESTACTIONGETATTACHMENTINFOBYID_H__

#include "CMtfSynchronousTestAction.h"

class CMsvAttachment;

_LIT(KTestActionGetAttachmentInfoById,"GetAttachmentInfoById");

class CMtfTestActionGetAttachmentInfoById : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionGetAttachmentInfoById(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionGetAttachmentInfoById(CMtfTestCase& aTestCase);
	
	void RunTestL();
	void PrintAttachmentInfo(CMsvAttachment& aInfo);
	};


#endif // __CMTFTESTACTIONGETATTACHMENTINFOBYID_H__
