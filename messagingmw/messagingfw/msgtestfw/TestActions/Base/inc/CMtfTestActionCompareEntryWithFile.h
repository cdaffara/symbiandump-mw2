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
// CMtfTestActionCreateEmailOverSmsMessage.h
// 
//



#ifndef __CMTFTESTACTIONCOMPAREENTRYWITHFILE_H__
#define __CMTFTESTACTIONCOMPAREENTRYWITHFILE_H__


#include <e32def.h>
#include "CMtfSynchronousTestAction.h"


class CMsvEntry;
class TMsvEntry;

_LIT(KTestActionCompareEntryWithFile,"CompareEntryWithFile");

/**
This action compares an entry with contents of a file.

This action compares an entry with contents of a file. It is intended to be used to compare text such as the description, details,
and the body of the text.This is useful to check that what we get sent is correct.

@internalTechnology
*/
class CMtfTestActionCompareEntryWithFile : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters);

public:
	virtual void ExecuteActionL();
	
private:
	CMtfTestActionCompareEntryWithFile(CMtfTestCase& aTestCase);
	TBool EqualL(CMsvEntry& aEntry, const TPtrC& smsMessageFileName);
	};


#endif
