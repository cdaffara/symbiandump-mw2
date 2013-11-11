// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "Testsetup.h"
#include <msvruids.h>
#include "msvregip.h"
#include "T_REG.H"
#include "t_reg.hrh"
#include <s32mem.h>

// local variables//

RTest	test(_L("MSV Register Testrig"));

const TUid KDummyMtmTypeUid={268440244};
const TUid KDummyTechnologyTypeUid={268441126};
const TUid KUidMsgTypeText = {268439598};
const TUid KUidMsgTechnologyTypeText = {268441129};
const TUid KDummyMtm2TypeUid={0x10004847};

_LIT(KDefaultRegistryFileStoreName, "\\private\\1000484b\\Mtm Registry v2");
_LIT(KTestMtmResourceFileName, "t_msgs_registry.rsc");
_LIT(KTestComponent, "msgs");
_LIT(KDummyMtmFilename, "z:\\system\\libs\\mtm1.dll");
_LIT(KDummyMtm2Filename, "z:\\system\\libs\\mtm2.dll");
_LIT(KTextMtmFilename, "z:\\system\\libs\\textmtm.dll"); // this dll does not actually exist

class CMyMtmDllRegistry : public CMtmDllRegistry
	{
public:
	static CMyMtmDllRegistry* NewL(RFs& aFs,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32=KMsvDefaultTimeoutMicroSeconds32);
	~CMyMtmDllRegistry();

	TInt AddRegisteredMtmDll(TUid aMtmTypeUid,TUid aTechnologyTypeUid,const CMtmDllInfo& aMtmDllInfo,MRegisteredMtmDllObserver& aRegisteredMtmDllObserver);

	CMyMtmBase* NewMyMtmL(TUid aMtmTypeUid);

protected:
	CMyMtmDllRegistry(RFs& aFs,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32);
	};

class CDummyRegisterMtmDllObserver : public MRegisteredMtmDllObserver
	{
public:
	CDummyRegisterMtmDllObserver():iRefCount(0){}
	TInt UseMtmGroup(TUid /*aMtmTypeUid*/){iRefCount++;return KErrNone;}
	TInt ReleaseMtmGroup(TUid /*aMtmTypeUid*/){iRefCount--;return KErrNone;}
public:
	TInt iRefCount;
private:
	};

CMyMtmDllRegistry* CMyMtmDllRegistry::NewL(RFs& aFs,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32)
	{
	return new(ELeave) CMyMtmDllRegistry(aFs,aTimeoutMicroSeconds32);
	}

CMyMtmDllRegistry::~CMyMtmDllRegistry()
	{
	}

TInt CMyMtmDllRegistry::AddRegisteredMtmDll(TUid aMtmTypeUid,TUid aTechnologyTypeUid,const CMtmDllInfo& aMtmDllInfo,MRegisteredMtmDllObserver& aRegisteredMtmDllObserver)
	{
	return CMtmDllRegistry::AddRegisteredMtmDll(aMtmTypeUid,aTechnologyTypeUid,aMtmDllInfo,aRegisteredMtmDllObserver);
	}

CMyMtmBase* CMyMtmDllRegistry::NewMyMtmL(TUid aMtmTypeUid)
	{
	if(!IsPresent(aMtmTypeUid))
		User::Leave(KErrNotFound);
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	CRegisteredMtmDll* registeredmtmdll=iRegisteredMtmDllArray[index];
	RLibrary mtmdlllibrary;
	User::LeaveIfError(registeredmtmdll->GetLibrary(iFs,mtmdlllibrary));

	typedef CMyMtmBase* (*NewMyMtmL)(TUid aMtmTypeUid,CRegisteredMtmDll& aRegisteredMtmDll);

	TInt entrypointordinalnumber=registeredmtmdll->MtmDllInfo().iEntryPointOrdinalNumber;
	TLibraryFunction libFunc=mtmdlllibrary.Lookup(entrypointordinalnumber);
	if (libFunc==NULL)
		User::Leave(KErrBadLibraryEntryPoint) ;
	NewMyMtmL pFunc=(NewMyMtmL)libFunc;
	CMyMtmBase* mymtm=NULL;
	TInt refcount=registeredmtmdll->MtmDllRefCount();
	TRAPD(ret,mymtm=((*pFunc)(aMtmTypeUid,*registeredmtmdll)));
	if ((ret!=KErrNone) && (registeredmtmdll->MtmDllRefCount()==refcount))  //  Library not released in mtm destructor
		registeredmtmdll->ReleaseLibrary();
	User::LeaveIfError(ret);	
	return mymtm;
	}

CMyMtmDllRegistry::CMyMtmDllRegistry(RFs& aFs,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32):
	CMtmDllRegistry(aFs,KUidMtmServerComponent,aTimeoutMicroSeconds32)
	{
	__DECLARE_NAME(_S("CMyMtmDllRegistry"));
	}

LOCAL_C CMtmGroupData* CreateMtmGroupDataL()
	{
	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	CMtmDllInfo* mtmdllinfo1=CMtmDllInfo::NewL(_L("My server dll info"),TUidType(KDynamicLibraryUid,KUidMtmServerComponent,KDummyMtm2Version2Uid),KDummyMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);
	test(mtmdllinfo1->HumanReadableName()==_L("My server dll info"));

	CMtmDllInfo* mtmdllinfo2=CMtmDllInfo::NewL(_L("My client dll info"),TUidType(KDynamicLibraryUid,KUidMtmClientComponent,KDummyMtm2Version2Uid),KDummyMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo2);

	CMtmDllInfo* mtmdllinfo3=CMtmDllInfo::NewL(_L("My ui dll info"),TUidType(KDynamicLibraryUid,KUidMtmUiComponent,KDummyMtm2Version2Uid),KDummyMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo3);

	CMtmDllInfo* mtmdllinfo4=CMtmDllInfo::NewL(_L("My ui data dll info"),TUidType(KDynamicLibraryUid,KUidMtmUiDataComponent,KDummyMtm2Version2Uid),KDummyMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo4);

	TCapabilitySet capSet;
	capSet.SetEmpty();
	capSet.AddCapability(ECapabilityReadUserData);
	capSet.AddCapability(ECapabilityLocalServices);
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(KDummyMtmTypeUid, KDummyTechnologyTypeUid, mtmdllinfoarray, capSet);

	return mtmgroupdata;
	}


LOCAL_C void TestRegister1L()

//
// Test read file handling.
//
	{


// Open file


	CMtmGroupData* mtmgroupdata=CreateMtmGroupDataL();
	CleanupStack::PushL(mtmgroupdata);

	test(mtmgroupdata->MtmDllInfoArray().Count()==4);

	CMyMtmDllRegistry* mymtmdllregistry=CMyMtmDllRegistry::NewL(theUtils->FileSession());
	CleanupStack::PushL(mymtmdllregistry);

	CMtmDllInfo* mtmdllinfo=mtmgroupdata->MtmDllInfoArray()[0];

	CDummyRegisterMtmDllObserver* dummyregistermtmdllobserver=new(ELeave) CDummyRegisterMtmDllObserver();
	CleanupStack::PushL(dummyregistermtmdllobserver);

	User::LeaveIfError(mymtmdllregistry->AddRegisteredMtmDll(mtmgroupdata->MtmTypeUid(),mtmgroupdata->TechnologyTypeUid(),(const CMtmDllInfo&) *mtmdllinfo,*dummyregistermtmdllobserver));
	test(mymtmdllregistry->NumRegisteredMtmDlls()==1);

	test(!mymtmdllregistry->IsInUse());
	test(!mymtmdllregistry->IsInUse(mtmgroupdata->MtmTypeUid()));

	CMyMtmBase* mymtm=NULL;
	mymtm=mymtmdllregistry->NewMyMtmL(mtmgroupdata->MtmTypeUid());

	test(mymtm->Type()==mtmgroupdata->MtmTypeUid());
	test(mymtmdllregistry->IsInUse());
	test(mymtmdllregistry->IsInUse(mtmgroupdata->MtmTypeUid()));
	delete mymtm;

	test(!mymtmdllregistry->IsInUse());
	test(!mymtmdllregistry->IsInUse(mtmgroupdata->MtmTypeUid()));

	test(dummyregistermtmdllobserver->iRefCount==0);

	CleanupStack::PopAndDestroy();  // dummyregistermtmdllobserver

	CleanupStack::PopAndDestroy();  // mymtmdllregistry

	CleanupStack::PopAndDestroy();  // mtmgroupdata


	}

LOCAL_C CMtmGroupData* CreateMtm2GroupDataL()
	{
	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	CMtmDllInfo* mtmdllinfo1=CMtmDllInfo::NewL(_L("My server dll info"),TUidType(KDynamicLibraryUid,KUidMtmServerComponent,KDummyMtm1Version2Uid),KDummyMtm2Filename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);
	test(mtmdllinfo1->HumanReadableName()==_L("My server dll info"));

	CMtmDllInfo* mtmdllinfo2=CMtmDllInfo::NewL(_L("My client dll info"),TUidType(KDynamicLibraryUid,KUidMtmClientComponent,KDummyMtm1Version2Uid),KDummyMtm2Filename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo2);

	CMtmDllInfo* mtmdllinfo3=CMtmDllInfo::NewL(_L("My ui dll info"),TUidType(KDynamicLibraryUid,KUidMtmUiComponent,KDummyMtm1Version2Uid),KDummyMtm2Filename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo3);

	CMtmDllInfo* mtmdllinfo4=CMtmDllInfo::NewL(_L("My ui data dll info"),TUidType(KDynamicLibraryUid,KUidMtmUiDataComponent,KDummyMtm1Version2Uid),KDummyMtm2Filename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo4);

	TCapabilitySet capSet;
	capSet.SetEmpty();
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(KDummyMtm2TypeUid, KDummyTechnologyTypeUid, mtmdllinfoarray, capSet);

	return mtmgroupdata;
	}


LOCAL_C void TestRegister4L()

//
// Test read file handling.
//
	{


// Open file


	CMtmGroupData* mtmgroupdata=CreateMtm2GroupDataL();
	CleanupStack::PushL(mtmgroupdata);

	test(mtmgroupdata->MtmDllInfoArray().Count()==4);

	CMyMtmDllRegistry* mymtmdllregistry=CMyMtmDllRegistry::NewL(theUtils->FileSession());
	CleanupStack::PushL(mymtmdllregistry);

	CMtmDllInfo* mtmdllinfo=mtmgroupdata->MtmDllInfoArray()[0];

	CDummyRegisterMtmDllObserver* dummyregistermtmdllobserver=new(ELeave) CDummyRegisterMtmDllObserver();
	CleanupStack::PushL(dummyregistermtmdllobserver);

	User::LeaveIfError(mymtmdllregistry->AddRegisteredMtmDll(mtmgroupdata->MtmTypeUid(),mtmgroupdata->TechnologyTypeUid(),(const CMtmDllInfo&) *mtmdllinfo,*dummyregistermtmdllobserver));
	test(mymtmdllregistry->NumRegisteredMtmDlls()==1);

	test(!mymtmdllregistry->IsInUse());
	test(!mymtmdllregistry->IsInUse(mtmgroupdata->MtmTypeUid()));

	CMyMtmBase* mymtm=NULL;
	mymtm=mymtmdllregistry->NewMyMtmL(mtmgroupdata->MtmTypeUid());

	test(mymtm->Type()==mtmgroupdata->MtmTypeUid());
	test(mymtmdllregistry->IsInUse());
	test(mymtmdllregistry->IsInUse(mtmgroupdata->MtmTypeUid()));
	delete mymtm;

	test(!mymtmdllregistry->IsInUse());
	test(!mymtmdllregistry->IsInUse(mtmgroupdata->MtmTypeUid()));

	test(dummyregistermtmdllobserver->iRefCount==0);

	CleanupStack::PopAndDestroy();  // dummyregistermtmdllobserver

	CleanupStack::PopAndDestroy();  // mymtmdllregistry

	CleanupStack::PopAndDestroy();  // mtmgroupdata
	}

LOCAL_C void TestRegister2L()

//
// Test read file handling.
//
	{


// Open file


	CMtmGroupData* mtmgroupdata1=CreateMtmGroupDataL();
	CleanupStack::PushL(mtmgroupdata1);

	HBufC8* buffer=NULL;
	TInt ret=KErrNone;
	TInt size=1024;
	do 
		{
		buffer=HBufC8::NewLC(size);
		TPtr8 bufferPtr = buffer->Des();
		RDesWriteStream writeStream(bufferPtr);
		TRAP(ret,
			writeStream << *mtmgroupdata1;
			writeStream.CommitL();
			)
		if (ret==KErrOverflow)
			{
			size+=1024;
			CleanupStack::PopAndDestroy(buffer);
			}
		}
	while (ret==KErrOverflow);

	RDesReadStream readStream(*buffer);
	CMtmGroupData* mtmgroupdata2 = CMtmGroupData::NewL(readStream);

	CleanupStack::PushL(mtmgroupdata2);

	test(*mtmgroupdata1==*mtmgroupdata1);

	CleanupStack::PopAndDestroy(3);  // mtmgroupdata1, buffer, mtmgroupdata2
	}

LOCAL_C void TestRegister3L()

//
// Test read file handling.
//
	{


// Open file


	CRegisteredMtmDllArray* registeredmtmdllarray=new(ELeave) CRegisteredMtmDllArray();
	CleanupStack::PushL(registeredmtmdllarray);

	CDummyRegisterMtmDllObserver* dummyregistermtmdllobserver=new(ELeave) CDummyRegisterMtmDllObserver();
	CleanupStack::PushL(dummyregistermtmdllobserver);

	CMtmDllInfo* mtmdllinfo=CMtmDllInfo::NewL(_L("My server dummy dll info"),TUidType(KDynamicLibraryUid,KUidMtmServerComponent,KDummyMtm3Version2Uid),KDummyMtmFilename,1,TVersion(2,0,0));
	CleanupStack::PushL(mtmdllinfo);
	CRegisteredMtmDll* registeredmtmdll=CRegisteredMtmDll::NewL(KDummyMtmTypeUid,KDummyTechnologyTypeUid,(const CMtmDllInfo&) *mtmdllinfo,KMsvDefaultTimeoutMicroSeconds32,*dummyregistermtmdllobserver);
	registeredmtmdllarray->AddRegisteredMtmDllL(registeredmtmdll);
	test(registeredmtmdll->MtmDllInfo().HumanReadableName()==_L("My server dummy dll info"));
	CleanupStack::PopAndDestroy();


	mtmdllinfo=CMtmDllInfo::NewL(_L("My server text dll info"),TUidType(KDynamicLibraryUid,KUidMtmServerComponent,KDummyMtm3Version2Uid),KTextMtmFilename,1,TVersion(2,0,0));
	CleanupStack::PushL(mtmdllinfo);
	registeredmtmdll=CRegisteredMtmDll::NewL(KUidMsgTypeText,KUidMsgTechnologyTypeText,(const CMtmDllInfo&) *mtmdllinfo,KMsvDefaultTimeoutMicroSeconds32,*dummyregistermtmdllobserver);
	registeredmtmdllarray->AddRegisteredMtmDllL(registeredmtmdll);
	CleanupStack::PopAndDestroy();

	test(registeredmtmdllarray->Count()==2);

	HBufC8* buffer=NULL;
	TInt ret=KErrNone;
	TInt size=1024;
	do 
		{
		buffer=HBufC8::NewLC(size);
		TPtr8 bufferPtr = buffer->Des();
		RDesWriteStream writeStream(bufferPtr);
		TRAP(ret,
			TMsvPackedRegisteredMtmDllArray::PackRegisteredMtmDllArrayL(writeStream, *registeredmtmdllarray);
			writeStream.CommitL();
			);

		if (ret==KErrOverflow)
			{
			size+=1024;
			CleanupStack::PopAndDestroy(buffer);
			}
		}
	while (ret==KErrOverflow);
	User::LeaveIfError(ret);

	registeredmtmdllarray->ResetAndDestroy();

	TInt count=registeredmtmdllarray->Count();
	test(count==0);

	RDesReadStream readStream(*buffer);
	TMsvPackedRegisteredMtmDllArray::UnpackRegisteredMtmDllArrayL(readStream, *registeredmtmdllarray, KMsvDefaultTimeoutMicroSeconds32, *dummyregistermtmdllobserver);

	count=registeredmtmdllarray->Count();
	test(count==2);

	test((*registeredmtmdllarray)[0]->MtmDllInfo().HumanReadableName()==_L("My server dummy dll info"));
	test((*registeredmtmdllarray)[1]->MtmDllInfo().HumanReadableName()==_L("My server text dll info"));

	test(dummyregistermtmdllobserver->iRefCount==0);

	CleanupStack::PopAndDestroy();  // dummyregistermtmdllobserver
	CleanupStack::PopAndDestroy();  // mymtmregisteredmtmdllarray
	CleanupStack::PopAndDestroy();  // buffer
	}

LOCAL_C void TestRegister5L()

//
// Test corrupt Mtm Registry file handling
//
	{
	TChar driveChar= theUtils->FileSession().GetSystemDriveChar();
	TBuf<2> systemDrive;
	systemDrive.Append(driveChar);
	systemDrive.Append(KDriveDelimiter);
	TPath pathName(systemDrive);
	pathName.Append(KDefaultRegistryFileStoreName);
	// Make sure that the old registry file is not read-only
	theUtils->FileSession().SetAtt(pathName,0,KEntryAttReadOnly|KEntryAttHidden|KEntryAttSystem);
	
	// Create a corrupt registry file and make it read-only
	RFileWriteStream file;
	User::LeaveIfError(file.Replace(theUtils->FileSession(), pathName, EFileWrite));
	CleanupClosePushL(file);
	file << _L8("This file is corrupt");
	CleanupStack::PopAndDestroy(); // CleanupClose file
	theUtils->FileSession().SetAtt(pathName, KEntryAttReadOnly, 0);

	// Attempt to restore data from the registry file
	CServerMtmDllRegistry* servermtmdllregistry;
	servermtmdllregistry = CServerMtmDllRegistry::NewL(theUtils->FileSession());
	CleanupStack::PushL(servermtmdllregistry);
	CMtmRegistryControl* mtmregistrycontrol;
	mtmregistrycontrol = CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
	CleanupStack::PushL(mtmregistrycontrol);

	CleanupStack::PopAndDestroy(mtmregistrycontrol);
	CleanupStack::PopAndDestroy(servermtmdllregistry);
	}
	
// This forces the preprocessor to make a 2nd pass and expand the #define'd string
#define _LIT2(name,s) _LIT(name,s) 

LOCAL_C void TestRegister6L()
//
// Test basic mtm info is correctly loaded from resource file
//
	{	
	// Create a registry control for installing an MTM
	CServerMtmDllRegistry* servermtmdllregistry;
	servermtmdllregistry = CServerMtmDllRegistry::NewL(theUtils->FileSession());
	CleanupStack::PushL(servermtmdllregistry);
	CMtmRegistryControl* mtmregistrycontrol;
	mtmregistrycontrol = CMtmRegistryControl::NewL(theUtils->FileSession(),*servermtmdllregistry);
	CleanupStack::PushL(mtmregistrycontrol);

	// Install the MTM group
	TParse parse;
	User::LeaveIfError(theUtils->ResolveFile(KTestComponent, KTestMtmResourceFileName, parse));
	TUid mtmtypeuid;
	User::LeaveIfError(mtmregistrycontrol->InstallMtmGroup(parse.FullName(), mtmtypeuid));
	
	// Test the MTM group data matches the resource file
	test(mtmtypeuid.iUid == KTestMtmTypeUid);
	test(servermtmdllregistry->IsPresent(mtmtypeuid));
	test(servermtmdllregistry->TechnologyTypeUid(mtmtypeuid).iUid == KTestMtmTechnologyTypeUid);
	
	const CMtmDllInfo& mtmdllinfo = servermtmdllregistry->RegisteredMtmDllInfo(mtmtypeuid);
	_LIT2(KTestServerMtmHumanReadableNameDes, KTestServerMtmHumanReadableName);
	test(mtmdllinfo.HumanReadableName().Compare(KTestServerMtmHumanReadableNameDes) == 0);
	test(mtmdllinfo.iUidType.MostDerived().iUid == KTestServerMtmSpecificUid);
	test(mtmdllinfo.iEntryPointOrdinalNumber == KTestServerMtmOrdinalNumber);
	test(mtmdllinfo.iVersion.iMajor == KTestServerMtmMajorVersionNumber);
	test(mtmdllinfo.iVersion.iMinor == KTestServerMtmMinorVersionNumber);
	test(mtmdllinfo.iVersion.iBuild == KTestServerMtmBuildVersionNumber);

	// Deinstall the MTM group
	User::LeaveIfError(mtmregistrycontrol->DeInstallMtmGroup(mtmtypeuid));

	// Cleanup
	CleanupStack::PopAndDestroy(mtmregistrycontrol);
	CleanupStack::PopAndDestroy(servermtmdllregistry);	
	}

void doTestsL()
    {
	CMsgsTestUtils::Start(_L("Create CMtmGroupData"));
	TestRegister1L();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Packing data"));
	TestRegister2L();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Create register mtm dll array"));
	TestRegister3L();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Mutiple mtm components"));
	TestRegister4L();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Corrupt mtm registry file handling"));
	TestRegister5L();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("MTM resource file reading"));
	TestRegister6L();
	CMsgsTestUtils::Complete();
    }
