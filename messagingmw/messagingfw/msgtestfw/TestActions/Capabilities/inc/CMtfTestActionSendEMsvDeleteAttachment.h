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



#ifndef __CMTFTESTACTIONSENDEMSVDELETEATTACHMENT_H__
#define __CMTFTESTACTIONSENDEMSVDELETEATTACHMENT_H__


#include "CMtfSynchronousTestAction.h"



_LIT( KTestActionSendEMsvDeleteAttachment,"SendEMsvDeleteAttachment");

class CMtfTestActionSendEMsvDeleteAttachment : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionSendEMsvDeleteAttachment();
	
private:
	CMtfTestActionSendEMsvDeleteAttachment(CMtfTestCase& aTestCase);
	};




#endif  // __CMTFTESTACTIONSENDEMSVDELETEATTACHMENT_H__
