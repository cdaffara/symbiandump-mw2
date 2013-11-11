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



#ifndef __CMTFTESTACTIONSENDEMSVOPENATTACHMENTFORWRITE_H__
#define __CMTFTESTACTIONSENDEMSVOPENATTACHMENTFORWRITE_H__


#include "CMtfSynchronousTestAction.h"



_LIT( KTestActionSendEMsvOpenAttachmentForWrite,"SendEMsvOpenAttachmentForWrite");

class CMtfTestActionSendEMsvOpenAttachmentForWrite : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionSendEMsvOpenAttachmentForWrite();
	
private:
	CMtfTestActionSendEMsvOpenAttachmentForWrite(CMtfTestCase& aTestCase);
	};




#endif  // __CMTFTESTACTIONSENDEMSVOPENATTACHMENTFORWRITE_H__
