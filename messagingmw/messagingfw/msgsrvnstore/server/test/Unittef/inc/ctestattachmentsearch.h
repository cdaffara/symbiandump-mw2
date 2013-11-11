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

#ifndef CTESTATTACHMENTSEARCH_H
#define CTESTATTACHMENTSEARCH_H

#include <test/tefunit.h>
#include <msvsearchsortquery.h>
#include <msvsearchsortoperation.h>
#include "cactivewaiter.h"
#include <msvstd.h>
#include <e32std.h>
#include "msvdbadapter.h"

#include "emailtestutils.h"
#include <miutlog.h>
#include <miutmsg.h>
#include <txtrich.h>



#include <apgaplst.h>
#include <apaflrec.h>
#include <apgcli.h>
#include <apacmdln.h>

#include <apasvst.h> // To manually start the app list server

#include <apfrec.h>
#include <apgicnfl.h>
#include <ecom/ecom.h>

#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>
#include <cemailaccounts.h>



/**
@internalTechnology
@prototype
*/
class CTestAttachmentSearch : public  CActiveTestFixture, public RMsvServerSession
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestAttachmentSearch(); 
	virtual ~CTestAttachmentSearch();
	void SetupL();
	void TearDownL();
	void TestLinkAttachmentTypeSearchL();
	void TestCreateAttachmentL(TInt aAttachmentType,TInt aIndex, TMsvId aId);
	void CreateNewPlaintextMessageL();
	void TestAddMessageAsAttachmentL(TMsvId aMessageId);
	CMsvEntry* CreateTestEntryLC();
	void TestAddAttachmentL();
	void TestAddAttachmentAsLinkL();
	void TestAddFileAttachmentL();
	void TestAttachmentFileTypeSearchL();
	void TestAttachmentMessageTypeSearchL();
	TInt PopulateFileL(RFile& aFile, const TDesC& aDataFilePath);
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	
private:
	CActiveWaiter* iActiveWaiter;
	CMsvSearchSortQuery* iQuery;
	CMsvDBAdapter* iDBadapter;
	RArray<TMsvId> iSelectionArray;
	RArray<TMsvId> iResultArray;
	RArray<TMsvIdWithSortField> iResultIdSortFieldArray;
	RArray<TMsvEntry> iEntryArray;
	TMsvId newMessageId;
	TMsvId attachmentMessageId;
	TMsvId messageId;
	};

#endif // CTESTATTACHMENTSEARCH_H
