// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTFTESTACTIONSENDEMSVGETRESULTCOUNT_H__
#define __CMTFTESTACTIONSENDEMSVGETRESULTCOUNT_H__

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionSendEMsvGetResultCount,"SendEMsvGetResultCount");
_LIT(KTestActionSearchSortOperation,"SendEMsvSearchSortOperation");
_LIT(KTestActionSearchSortOnIndexEntry,"SendEMsvSearchSortOnIndexEntry");
_LIT(KTestActionGetResult,"SendEMsvGetResult");
_LIT(KTestActionGetIdsOrResult,"SendEMsvGetIdsOrResult");
_LIT(KTestActionGetResultCount,"SendEMsvGetResultCount");
_LIT(KTestActionGetNextId,"SendEMsvGetNextId");
_LIT(KTestActionGetNextEntry,"SendEMsvGetNextEntry");
_LIT(KTestActionGetQueryId,"SendEMsvGetQueryId");
_LIT(KTestActionUnmarQueryId,"SendEMsvUnmarQueryId");
_LIT(KTestActionIdWithSortFiled,"SendEMsvIdWithSortFiled");
_LIT(KTestActionSearchSortQueryId,"SendEMsvSearchSortQueryId");
_LIT(KTestActionQueryData,"SendEMsvQueryData");
_LIT(KTestActionGetSearchSortProgress,"SendEMsvGetSearchSortProgress");
_LIT(KTestActionCancelSearchSortOp,"SendEMsvCancelSearchSortOp");
_LIT(KTestActionUpdateAndSort,"SendEMsvUpdateAndSort");




class CMtfTestActionSendEMsvGetResultCount : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSendEMsvGetResultCount(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendEMsvGetResultCount(CMtfTestCase& aTestCase);
	};


#endif // __CMTFTESTACTIONSENDEMSVGETRESULTCOUNT_H__
