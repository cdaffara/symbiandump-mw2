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

#ifndef CTESTSEARCHSORTOPERATION_H
#define CTESTSEARCHSORTOPERATION_H

#include <test/tefunit.h>
#include <msvsearchsortquery.h>
#include <msvsearchsortoperation.h>
#include "cactivewaiter.h"
#include <msvstd.h>
#include <e32std.h>
#include "msvdbadapter.h"
#include "MSVSERV.H"
/**
@internalTechnology
@prototype
*/
class CTestSearchSortOperation : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestSearchSortOperation(); 
	virtual ~CTestSearchSortOperation();
	void SetupL();
	void TearDownL();
	void CreateValidQueryOneL();
	void CreateValidQueryTwoL();
	void CreateValidQueryThreeL();
	void CreateSearchQueryWithMaxLevelsL();
	void CreateSearchSortQueryWithMaxLevels();
	void CreateQueryWithInvalidDateL();
	void CreateQueryWithInvalidSizeL();
	void CreateTooBigQueryL();
	void CreateQueryForMultilevelSortL();
	void CreateSortQueryWithSubfolderOptionL();
	void CreateQueryWithAttachmentTypeL();
	void CreateQueryUsingNewLcL();
	void CreateSearchQueryWithInvalidQueryOptionL();
	
	void TestHeaderSearchBodyL();
	void CreateMessagesL(TUid aMtm);
	void SeachSortRequestL();
	void CreateEntryL(TMsvId aId, TMsvId aParentId, TUid aUid, TMsvId& aVisibleFolder, TBool aIsVisible=EFalse, TBool isPreparationFlag=EFalse, TBool iTestValue=EFalse, TBool aService=EFalse);
	
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	
private:
	CActiveWaiter* iActiveWaiter;
	RArray<TMsvId> aSelectionArray;
	RArray<TMsvId> aResultArray;
	RArray<TMsvIdWithSortField> aResultIdSortFieldArray;
	HBufC8* iBuffer;
	};

#endif // CTESTSEARCHSORTOPERATION_H
