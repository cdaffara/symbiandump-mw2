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


#ifndef __CMTFTESTACTIONGETATTACHMENTINDEXFROMID_H__
#define __CMTFTESTACTIONGETATTACHMENTINDEXFROMID_H__

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionGetAttachmentIndexFromId,"GetAttachmentIndexFromId");

class CMtfTestActionGetAttachmentIndexFromId : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionGetAttachmentIndexFromId(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionGetAttachmentIndexFromId(CMtfTestCase& aTestCase);
	};


#endif // __CMTFTESTACTIONGETATTACHMENTINDEXFROMID_H__
