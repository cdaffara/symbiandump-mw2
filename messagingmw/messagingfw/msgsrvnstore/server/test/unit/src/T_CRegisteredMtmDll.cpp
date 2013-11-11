// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// To build: bldmake bldfiles
// abld test build wins udeb
// To run from release\wins\udeb: T_CRegisteredMtmDll
// *** NOTE FOR HARDWARE TESTING ***
// This test harness makes use of RFs::SetErrorCondition to simulate
// errors in the file server.  This metod only works in debug mode, 
// so in order to test on hardware the debug version of EFSrv.dll
// needs to be used when the rom is built.
// To do this, modify epoc32/rom/include/base.iby and change the line:
// file=ABI_DIR\BUILD_DIR\efsrv.dll    System\Libs\EFSrv.dll   -to-
// file=ABI_DIR\DEBUG_DIR\efsrv.dll    System\Libs\EFSrv.dll  
// *** Read this if the test fails ***
// The test relies on using RFs::SetErrorCondition to force the 
// file server to fail after 2 requests.  The first call is at
// CRegisteredMtmDll::DoFindFullName (line offset = +7) [TInt ret=aFs.Drive(driveinfo,n);]
// the second is at CRegisteredMtmDll::DoFindFullName (line offset = +19) [ret=aFs.GetDir(...)]
// If the number of calls to RFs changes in the intermediate code, the 
// test will most likely fail and will require the hardcoded value (2)
// to be re-evaluated.
// 
//

#include <e32cons.h>
#include <e32test.h>
#include <s32file.h>
#include <e32uid.h>
#include <msvruids.h>
#include <msvreg.h>
#include "Testsetup.h"


_LIT(KDummyMtmFilename, "z:\\system\\libs\\mtm1.dll");
const TUid KDummyMtmTypeUid={268440244};
const TUid KDummyTechnologyTypeUid={268441126};

// Prototypes
LOCAL_C void TestDoFindFullName();

// Globals
_LIT(KTitle,"CRegisteredMtmDll");
RTest test(KTitle);


class CDummyRegisterMtmDllObserver : public MRegisteredMtmDllObserver
	{
public:
	CDummyRegisterMtmDllObserver():iRefCount(0){}
	TInt UseMtmGroup(TUid){iRefCount++;return KErrNone;}
	TInt ReleaseMtmGroup(TUid){iRefCount--;return KErrNone;}
public:
	TInt iRefCount;
private:
	};

LOCAL_C void TestDoFindFullName()
//
//	Test DoFindFullName method
//
	{
	CDummyRegisterMtmDllObserver* dummyRegisterMtmDllObserver=new(ELeave) CDummyRegisterMtmDllObserver();
	CleanupStack::PushL(dummyRegisterMtmDllObserver);

	CMtmDllInfo* mtmDllInfo=CMtmDllInfo::NewL(_L("My server dummy dll info"),TUidType(KDynamicLibraryUid,KUidMtmServerComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),KDummyMtmFilename,1,TVersion(2,0,0));
	CleanupStack::PushL(mtmDllInfo);
	CRegisteredMtmDll* registeredMtmDll=CRegisteredMtmDll::NewL(KDummyMtmTypeUid,KDummyTechnologyTypeUid,(const CMtmDllInfo&) *mtmDllInfo,KMsvDefaultTimeoutMicroSeconds32,*dummyRegisterMtmDllObserver);
	CleanupStack::PushL(registeredMtmDll);

	RLibrary mtmDllLibrary;
	TInt ret=KErrNone;

	// Test 1.
	// Set Rfs to fail with KErrGeneral on the second call.  
	//  This will be at the call to GetDir in DoFindFullName, ie when tring to GetDir on A:
	//  DoFindFullName will return KErrNotFound in this condition.
	theUtils->FileSession().SetErrorCondition(KErrCorrupt, 2);  
	ret = registeredMtmDll->GetLibrary(theUtils->FileSession(), mtmDllLibrary);
	// This will work with secure API
	test(ret == KErrNone || ret == KErrNotFound);
	// Release the library
	if (registeredMtmDll->MtmDllRefCount())  
		registeredMtmDll->ReleaseLibrary();	//  Library not released in mtm destructor

	// Test 2.
	// Set Rfs to fail with KErrNotSupported on the second call.  
	//  This will be at the call to GetDir in DoFindFullName, ie when tring to GetDir on A:
	// This test case is for defect DEF027335 - "WINS: Mailinit fails on clean baseline"
	//   Prior to the fix, DoFindFullName could return with KErrNotSupported before
	//   it had finished scanning the list of available drives.  It should now complete
	//   with KErrNotFound
	theUtils->FileSession().SetErrorCondition(KErrNotSupported, 2);  
	ret = registeredMtmDll->GetLibrary(theUtils->FileSession(), mtmDllLibrary);
	// This will work with secure API
	test(ret == KErrNone || ret == KErrNotFound);
	// Release the library
	if (registeredMtmDll->MtmDllRefCount())  
		registeredMtmDll->ReleaseLibrary();	//  Library not released in mtm destructor

	// Test 3.
	// Allow calls to RFs to complete
	//  Validreturn values for this case are: KErrNone if found and KErrNotFound if not found
	theUtils->FileSession().SetErrorCondition(KErrNone);  
	ret = registeredMtmDll->GetLibrary(theUtils->FileSession(), mtmDllLibrary);
	test(ret == KErrNone || ret == KErrNotFound);

	// Release the library
	if (registeredMtmDll->MtmDllRefCount())  
		registeredMtmDll->ReleaseLibrary();	//  Library not released in mtm destructor

	// Clean up
	CleanupStack::PopAndDestroy(registeredMtmDll);
	CleanupStack::PopAndDestroy(mtmDllInfo);
	CleanupStack::PopAndDestroy(dummyRegisterMtmDllObserver);
	}


LOCAL_C void doTestsL()
	{
	CMsgsTestUtils::Start(_L("DoFindFullName"));
	TestDoFindFullName();
	CMsgsTestUtils::Complete();
	}

