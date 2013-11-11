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



#ifndef __CMTFTESTACTIONCORRUPTFILE_H__
#define __CMTFTESTACTIONCORRUPTFILE_H__

#include <f32file.h>

#include "CMtfSynchronousTestAction.h"


_LIT( KTestActionCorruptFile,"CorruptFile");

class CMtfTestActionCorruptFile : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionCorruptFile(); 

private:
	CMtfTestActionCorruptFile(CMtfTestCase& aTestCase);
	void RunTestL();
	void CorruptFileLC(RFs& aFs, const TDesC& aFilePath, const TInt& aCorruptionType);
	void UpdateCorruptionListLC(RFs& aFs, const TDesC& aFilePath, const TInt& aCorruptionType);
	};


#endif // __CMTFTESTACTIONCORRUPTFILE_H__