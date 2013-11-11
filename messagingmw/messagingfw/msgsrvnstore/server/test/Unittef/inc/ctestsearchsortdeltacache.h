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



#ifndef CTESTSEARCHSORTDELTACACHER_H
#define CTESTSEARCHSORTDELTACACHER_H

#include <test/tefunit.h>
#include "msvsearchsortdeltacache.h"

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


class CMsvDBAdapter;
class CMsvDBAdaption;
/**
@internalTechnology
@prototype
*/
class CTestSearchSortDeltaCache : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestSearchSortDeltaCache(); 
	virtual ~CTestSearchSortDeltaCache();
	void SetupL();
	void TearDownL();
	
	void TestHandelL();
	
	static CTestSuite* CreateSuiteL(const TDesC& aName);

	void AddEntry();
	void TestDeleteEntry();
	void TestChangeEntry();
	void DeleteEntry(TMsvId aId);
	void InitCacheMangerL();
private:
	CMsvSearchSortDeltaCache* iSearchSortDeltaCache;
	CMsvServer* iServer;
	CMsvIndexAdapter* iIndexAdapter;
	
	CMsvDBAdapter* iDBadapter;
	CMsvDBAdaption* iSearchDbAdapter;
	};
#endif // ctestsearchsortCacheManager_H
