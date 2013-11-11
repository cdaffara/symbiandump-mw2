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



#ifndef __CMTFTESTACTIONSENDEMSVGETCHILDIDS_H__
#define __CMTFTESTACTIONSENDEMSVGETCHILDIDS_H__


#include "CMtfSynchronousTestAction.h"
#include <msvipc.h>


_LIT( KTestActionSendEMsvGetChildIds,"SendEMsvGetChildIds");

// Need to duplicate the functionality of TMsvPackedEntryFilter
// since it's not exported from MSGS.DLL
class TMsvPackedEntryFilterTest
{
public:
	TMsvPackedEntryFilterTest(HBufC8*& aBuffer);
	TInt PackFilter(const CMsvEntryFilter& aFilter);
	void UnpackFilter(CMsvEntryFilter& aFilter);
private:
	HBufC8*& iBuffer;
};

class CMtfTestActionSendEMsvGetChildIds : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionSendEMsvGetChildIds();
	
private:
	CMtfTestActionSendEMsvGetChildIds(CMtfTestCase& aTestCase);
	};




#endif  // __CMTFTESTACTIONSENDEMSVGETCHILDIDS_H__
