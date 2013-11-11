// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTFTESTACTIONCLASS0SMSNOTIFIER_H__
#define __CMTFTESTACTIONCLASS0SMSNOTIFIER_H__


#include "CMtfSynchronousTestAction.h"


_LIT(KTestActionClass0SmsNotifier,"CheckClass0SMS");

class CMtfTestActionClass0SmsNotifier : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters);
	virtual ~CMtfTestActionClass0SmsNotifier();
	
public:
	virtual void ExecuteActionL();
	
private:
	CMtfTestActionClass0SmsNotifier(CMtfTestCase& aTestCase);

	};

#endif	//__CMTFTESTACTIONCLASS0SMSNOTIFIER_H__
