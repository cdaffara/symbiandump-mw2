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

#ifndef __CMFTESTACTIONUNLOADPHONEMODULE_H_
#define __CMFTESTACTIONUNLOADPHONEMODULE_H_
#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionUnloadPhoneModule,"UnloadPhoneModule");

/**
This test action unloads the phone module which is loaded during Initializing SimTsy.

@internalTechnology
*/
class CMtfTestActionUnloadPhoneModule : public CMtfSynchronousTestAction
	{
	public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionParameters);
	CMtfTestActionUnloadPhoneModule(CMtfTestCase& aTestCase);
	~CMtfTestActionUnloadPhoneModule();
	virtual void ExecuteActionL();

	private:
	void UnloadPhoneModuleL(HBufC* aTsyName);
	};
	
#endif /*CMFTESTACTIONUNLOADPHONEMODULE_H_*/
