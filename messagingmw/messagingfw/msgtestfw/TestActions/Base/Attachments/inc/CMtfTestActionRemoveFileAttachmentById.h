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


#ifndef __CMTFTESTACTIONREMOVEFILEATTACHMENTBYID_H__
#define __CMTFTESTACTIONREMOVEFILEATTACHMENTBYID_H__

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionRemoveFileAttachmentById,"RemoveFileAttachmentById");

class CMtfTestActionRemoveFileAttachmentById : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionRemoveFileAttachmentById(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionRemoveFileAttachmentById(CMtfTestCase& aTestCase);
	
	void RunTestL();
	};


#endif // __CMTFTESTACTIONREMOVEFILEATTACHMENTBYID_H__
