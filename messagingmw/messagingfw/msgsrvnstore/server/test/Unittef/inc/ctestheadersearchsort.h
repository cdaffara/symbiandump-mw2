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

#ifndef CTESTHEADERSEARCHSORT_H
#define CTESTHEADERSEARCHSORT_H

#include <test/tefunit.h>
#include <msvsearchsortquery.h>
#include <msvsearchsortoperation.h>
#include "cactivewaiter.h"
#include <msvstd.h>
#include <e32std.h>
#include "msvdbadapter.h"
#include "emailtestutils.h"
#include "msvtestutils.h"

/**
@internalTechnology
@prototype
*/
class CTestHeaderSearchSortOperation : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestHeaderSearchSortOperation(); 
	virtual ~CTestHeaderSearchSortOperation();
	void SetupL();
	void TearDownL();
	void SaveMessageL(CMsvSession& aSession, TMsvEntry aEntry, TBool aRichText = EFalse);
	void CreateMessagesL(TUid aMtm);
	void TestCreateMsgL();
	void CreateEntryL(TMsvId aId, TMsvId aParentId, TUid aUid, TMsvId& aVisibleFolder, TBool aIsVisible=EFalse, TBool isPreparationFlag=EFalse, TBool iTestValue=EFalse);
	void TestHeaderBodySearchL();
	void TestHeaderSearchWithSortL();
	void TestHeaderSortToL();
	void TestHeaderSortFromL();
	void TestHeaderSortCcL();
	void TestHeaderSortBccL();
	void TestHeaderSortSubjectL();
	void TestHeaderSearchToL();
	void TestHeaderSearchCcL();
	void TestHeaderSearchBccL();
	void TestHeaderSearchFromL();
	void TestHeaderSearchSubjectL();
	void TestHeaderSearchBodyL();
	void TestHeaderSearchTwoLevelL();
	void TestHeaderSearchTwoLevelWithSortL();
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	
private:
	CActiveWaiter* iActiveWaiter;
	CMsvSearchSortQuery* iQuery;
	CMsvDBAdapter* iDBadapter;
	RArray<TMsvId> iSelectionArray;
	RArray<TMsvId> iResultArray;
	RArray<TMsvIdWithSortField> iResultIdSortFieldArray;
	RArray<TMsvEntry> iEntryArray;
	};

#endif // CTESTHEADERSEARCHSORTOPERATION_H
