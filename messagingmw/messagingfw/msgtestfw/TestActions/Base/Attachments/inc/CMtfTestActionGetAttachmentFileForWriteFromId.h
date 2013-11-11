// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTFTESTACTIONGETATTACHMENTFILEFORWRITEFROMID_H__
#define __CMTFTESTACTIONGETATTACHMENTFILEFORWRITEFROMID_H__

#include "CMtfSynchronousTestAction.h"

#include <f32file.h>

_LIT(KTestActionGetAttachmentFileForWriteFromId,"GetAttachmentFileForWriteFromId");

class CMtfTestActionGetAttachmentFileForWriteFromId : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionGetAttachmentFileForWriteFromId(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionGetAttachmentFileForWriteFromId(CMtfTestCase& aTestCase);
	};


#endif // __CMTFTESTACTIONGETATTACHMENTFILEFORWRITEFROMID_H__
