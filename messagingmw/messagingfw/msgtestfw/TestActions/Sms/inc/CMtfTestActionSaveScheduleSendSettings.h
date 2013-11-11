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



#ifndef __CMTFTESTACTIONSaveScheduleSendSettings_H__
#define __CMTFTESTACTIONSaveScheduleSendSettings_H__


#include "CMtfSynchronousTestAction.h"


_LIT( KTestActionSaveScheduleSendSettings,"SaveScheduleSendSettings");

class CMtfTestActionSaveScheduleSendSettings : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionSaveScheduleSendSettings();
	
private:
	CMtfTestActionSaveScheduleSendSettings(CMtfTestCase& aTestCase);
	};


#endif
