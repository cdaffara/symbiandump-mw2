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


#ifndef T_MSV_INDEXADAPTER_STEP_H
#define T_MSV_INDEXADAPTER_STEP_H

#include <test/tefunit.h>
#include "msvindexadapter.h"
#include "msvdbadapter.h"
#include "MSVSERV.H"
#include <msvids.h>
#include <msvstd.h>
#include <e32std.h>

/***********************************/
// Extra includes from CMsvServer.cpp


#include <e32std.h>

#include <barsc.h>
#include <barsread.h>
#pragma warning( disable : 4245 ) 
#include <bautils.h>
#pragma warning( default : 4245 ) 
#include <bafindf.h>
#include <u32std.h>

#include "MSVSTD.H"
#include "MSVIPC.H"
#include "MSVIDS.H"
#include "MSVUIDS.H"
#include "MSVRUIDS.H"

#include <msgs.rsg>

#include "MTSR.H"
#include "MSVENTRY.H"
#include "MSERVER.H"
#include "MSVRBLD.H"
#include "MSVSERV.H"
#include "MSVDELET.H"
#include "MSVMOVE.H"
#include "MSVUTILS.H"

#include "MSVPANIC.H"
#include "MSVAPI.H"
#include "indexcontext.h"
#include "CMsvCachedStore.h"
#include "CMsvBackupHandler.h"
#include "cmsvmailinitwaiter.h"
#include "msvcacheindextableentry.h"
#include "msventryfreepool.h"


/**
@internalTechnology
@prototype
*/
class CTestIndexAdapter : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestIndexAdapter();
	virtual ~CTestIndexAdapter();
	void SetupL();
	void TearDownL();

	void TestAddEntry();
	void TestAddEntryNoCommit();
	void TestDeleteEntry();
	void TestDeleteSelection();
	void TestExpandSelectionRecursively();
	void TestChangeEntry();
	void TestChangeEntryNoCommit();
	void TestGetEntry();
	void TestGetChildren();
	void TestAddEntryToCache();
	void TestGetChildrenId();
	void TestGetVisibleFolderId();
	void TestEntryLocking();
	void TestStoreLocking();
	void TestEntryAndStoreLocking();
	void TestMoveEntry();
	void TestIsDescendent();
	void TestEntryExists();
	void TestEntryTreeInfo();
	void TestChangeEntryInternal();
	void CreateFileNames();
	void DeleteEntry(TMsvId aId);
	void DeleteSelection(const CMsvEntrySelection& aIdList);
	void TestAdapterCreation();

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Changes for PREQ 557.
	void TestGetNextAvailableDriveId();
	void ValidateIndexAdapterCreation();
	void TestGetChildrenIdAll();
	void TestChangeDrive();
	void TestReloadCache();
	void TestSetLocalServiceComplete();
	void TestToImproveCoverage();
	void TestDoRemoveDeletedEntries();
	void TestDoForceDeleteEntry();	
	void AddEntryToCacheOnly(TMsvId aId, TMsvId aParentId, TMsvId aVisibleFolderId, TBool aIsVisible=EFalse);
	void AddEntry(TBool aCommit, TBool aAutoAssigned, TMsvId aId = 0, TMsvId aParentId = KMsvLocalServiceIndexEntryId);
#else
	void AddEntry(TBool aCommit, TBool aAutoAssigned, TMsvId aId = 0, TMsvId aParentId = KMsvRootIndexEntryIdValue);
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	static CTestSuite* CreateSuiteL(const TDesC& aName);
	CMsvCacheEntry* Entry();
private:
	RFs iFileHandle;
	CMsvServer* iServer;
	CMsvIndexAdapter* iIndexAdapter;
	};
	 
#endif // T_MSV_INDEXADAPTER_STEP_H

