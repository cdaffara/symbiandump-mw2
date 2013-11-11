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


#ifndef __CMTFTESTACTIONREMOVEFILEATTACHMENTWITHDESTROY_H__
#define __CMTFTESTACTIONREMOVEFILEATTACHMENTWITHDESTROY_H__

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionRemoveFileAttachmentWithDestroy,"RemoveFileAttachmentWithDestroy");

class CMtfTestActionRemoveFileAttachmentWithDestroy : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionRemoveFileAttachmentWithDestroy(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionRemoveFileAttachmentWithDestroy(CMtfTestCase& aTestCase);
	};


#endif // __CMTFTESTACTIONREMOVEFILEATTACHMENTWITHDESTROY_H__
