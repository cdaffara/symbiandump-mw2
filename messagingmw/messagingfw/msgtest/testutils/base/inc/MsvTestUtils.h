// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSV_TEST_UTILS_H_
#define MSV_TEST_UTILS_H_

#include "msvtestutilsbase.h"
#include <mtclreg.h>
#include <msventry.h>
#include <msvids.h>

//	Forward Declarations

class CBaseServerMtm;
class CServerMtmDllRegistry;
class CMtmRegistryControl;
class TTaskInfo;

//	Constants

#define KMtmDir					_L("c:\\system\\mtm\\")

// used for writing to logs
#define KFileNameBodies			_L("Entry_RichTextBodies.txt")
#define KFileNameFiles			_L("Entry_Files.txt")
#define	KFileNameEntryStructure	_L("Entry_Structure.txt")

#define KRfc822Files			_L("c:\\mailtest\\rfc822\\Rfc_*.txt")

_LIT(KMsvTestUtilsNewLine, "\n");


/**
@internalComponent
*/
const TInt KMsvTestUidPhonePwrValue=0x100052C5;
const TUid KMsvTestUidPhonePwr={KMsvTestUidPhonePwrValue};

/**
@internalComponent
*/
enum TMsvTestPhoneStatus
	{
	EMsvTestPhoneOff = 0,
	EMsvTestPhoneOn
	};

/**
@internalComponent
*/
const TInt KMsvTestUidSIMStatusValue	=0x100052C6;
const TUid KMsvTestUidSIMStatus		={KMsvTestUidSIMStatusValue};

/**
@internalComponent
*/
enum TMsvTestSIMStatus
	{
	EMsvTestSimOk,
	EMsvTestSimNotPresent,
	EMsvTestSimRejected
	};

/**
@internalComponent
*/
const TInt KMsvTestUidNetworkStatusValue=0x100052C7;
const TUid KMsvTestUidNetworkStatus	={KMsvTestUidNetworkStatusValue};

/**
@internalComponent
*/
enum TMsvTestNetworkStatus
	{
	EMsvTestNetworkAvailable,
	EMsvTestNetworkUnAvailable
	};


//
//
//	TestMsvOperationTimer
//
//

class TestMsvOperationTimer : public CTimer
	{
public:
	IMPORT_C static TestMsvOperationTimer* NewLC(CConsoleBase* aConsole, CMsvOperation* aMsvOperation, RTest& aRTest);

	IMPORT_C void RunL();
	IMPORT_C void DoCancel();
	IMPORT_C void ConstructL();
	IMPORT_C void IssueRequest();
	
	CConsoleBase*				iConsole;
	CMsvOperation*				iMsvOperation;
	TTimeIntervalMicroSeconds32 iPeriod;

protected:
	TInt iCount;
	TBool iClosing;
	RTest iRTest;
	IMPORT_C TestMsvOperationTimer(CConsoleBase* aConsole, CMsvOperation* aMsvOperation, RTest& aRTest);
	};

//
//
//	TMsvTestDllInfo
//
//

class TMsvTestDllInfo
	{
	public:
		inline TMsvTestDllInfo(const TDesC& aFileName, TInt aOrdinal = 1, TVersion aVersion = TVersion(2,0,0));
		TFileName iFileName;
		TInt iOrdinal;
		TVersion iVersion;
	};

//
//
// enum TMsvTestUtilsCreationFlags
//
//

enum TMsvTestUtilsCreationFlags
	{
	ETuNone						= 0x0000,
	ETuCreateTestDirectories	= 0x0001,
	ETuCleanMessageFolder		= 0x0002,
	ETuDeleteMtmRegistry		= 0x0004,
	ETuCreateServerMtmReg		= 0x0008,
	ETuInstallMtmGroup			= 0x0010,
	ETuDeleteService			= 0x0020,
	ETuCreateService			= 0x0040, //NewL may leave if service already exists
	ETuGoClientSide				= 0x0080,
	ETuGoServerSide				= 0x0100,
	ETuAllClientSide			= ETuCleanMessageFolder
									| ETuDeleteMtmRegistry
									| ETuCreateTestDirectories
									| ETuDeleteService
									| ETuCreateService
									| ETuGoClientSide,
	ETuAllServerSide			= ETuAllClientSide
									& ~ETuGoClientSide
									| ETuGoServerSide,
	ETuMinimalClientSide		= ETuCreateTestDirectories
									| ETuDeleteService
									| ETuCreateService
									| ETuGoClientSide,
	ETuMinimalServerSide		= ETuMinimalClientSide
									& ~ETuGoClientSide
									| ETuGoServerSide,
	ETuKeepLogFile				= 0x10000
	};

//
//
//	CMsvTestUtils
//
//

class CMsvTestUtils : public CTestUtils
	{
public:
	IMPORT_C void GoClientSideL();
	IMPORT_C void GoServerSideL();
	IMPORT_C virtual void Reset();
	IMPORT_C void CreateRegistryObjectAndControlL();
	IMPORT_C void CleanMessageFolderL();
	IMPORT_C void CreateAllTestDirectories();

	IMPORT_C void FindChildrenL(TMsvId aFolderToTraverse, TBool aReplace = ETrue, TBool aOtherFiles = ETrue);
	IMPORT_C void FindChildrenL(TMsvId aFolderToTraverse, const TFileName& aFilepath, TBool aReplace = ETrue, TBool aOtherFiles = ETrue);

	//CMsvEntry and CMsvServerEntry functions
	inline void CreateEntryL(TMsvEntry& aEntry);
	inline void CreateAndSetEntryL(TMsvEntry& aEntry);
	inline void DeleteEntryL(TMsvId aId);
	inline void ChangeEntryL(TMsvEntry& aEntry);
	inline void SetEntryL(TMsvId aId);
	inline TMsvEntry Entry();
	inline CMsvStore* EditStoreL();
	inline CMsvStore* ReadStoreL();
	inline CMsvEntrySelection* ChildrenWithTypeLC(TUid aType);
	inline CMsvEntrySelection* ChildrenWithMtmLC(TUid aMtm);
	inline void SetSortTypeL(TMsvSelectionOrdering aOrder);

	virtual void InstantiateClientMtmsL() = 0;
	virtual void InstantiateServerMtmsL() = 0;
	virtual void DeleteServicesL() = 0;
	virtual void CreateServicesL() = 0;
	virtual void FindExistingServicesL() = 0;
	virtual void InstallMtmGroupsL() = 0;
	virtual void CreateServerMtmRegsL() = 0;

	//Returns TMsvEntry.iDate for aEntryId, and the time it is scheduled for on the task scheduler
	IMPORT_C TInt ScheduleTime(TMsvId aEntryId, TTime& rEntryTime, TTime& rTaskSchedulerTime, TTaskInfo& rTaskInfo);
	IMPORT_C TInt AppendScheduleTimeL(TMsvId aEntryId, TDes& rOutput);
	IMPORT_C void SpecifyRfc822Dir(const TFileName& aFilePath);
	
	IMPORT_C void NavigateMessageStoreL(TMsvId aStart = KMsvRootIndexEntryId);

protected:
	IMPORT_C CMsvTestUtils(RTest& aRTest);
	IMPORT_C ~CMsvTestUtils();

	IMPORT_C virtual void ConstructL(TUint aCreationFlags = ETuNone);

	//MTM Functions
	IMPORT_C void InstallMtmGroupL(const TDesC& aDatFile);
	IMPORT_C void CreateServerMtmRegL(const TUid aMsgType, const TDesC& aHumanReadableName, const TMsvTestDllInfo& aServerMtm, const TMsvTestDllInfo& aClientMtm, const TMsvTestDllInfo& aUiMtm, const TMsvTestDllInfo& aUiDataMtm, const TUid aGroup, const TDesC& aDatFile);
	IMPORT_C CBaseMtm* InstantiateClientMtmL(TUid aMtmType, TMsvId aServiceId);
	IMPORT_C CBaseServerMtm* InstantiateServerMtmL(TUid aMtmType, TMsvId aServiceId);

	//Service Functions
	IMPORT_C TMsvId CreateServiceL(const TUid aMtm, TBool aReadOnly = EFalse, TBool aVisible = EFalse);
	IMPORT_C void DeleteServiceL(TUid aMtm);
	IMPORT_C void ServiceIdL(TUid aMtm, TMsvId& rFirstId, CMsvEntrySelection* rServiceIds = NULL);

	IMPORT_C void WriteBodyDataL(TMsvId aId, const TFileName& aFilepath, CMsvStore& fileStore, TBool aReplace);
	IMPORT_C void WriteFileDataL(TMsvId aId, const TFileName& aFileName, const TFileName& aLogFilepath, TBool aReplace);
	IMPORT_C virtual void ListChildrenL(RFile& aFile, CDir& rfcFileList, TInt& aCount, TInt& aEntryCount, TBool aReplace = ETrue, TBool aOtherFiles = ETrue);
	IMPORT_C void SpecifyLogsDir(const TFileName& aFilePath); 

	IMPORT_C virtual void SetFolderType(TMsvEntry& entry, TPtrC& type);

	virtual void DisplayChildDetailsL(const TMsvEntry& /*aEntry*/) {};
	IMPORT_C void DisplayChildL(const TMsvEntry& entry);

private:
	void ListChildrenL(TBool aReplace, TBool aOtherFiles, RFile& aFile, CDir& rfcFileList);
	void DoScheduleTimeL(TMsvId aEntryId, TTime& rEntryTime, TTime& rTaskSchedulerTime, TTaskInfo& rTaskInfo);

	void ShowChildrenL(const CMsvEntry& aEntry, TInt aStart, TInt& aMore, TInt aMaxCount = 10);
	TInt DoNavigateMessageStoreL(TMsvId& aParent, TInt& aStart);

public:
	CMsvSession*			iMsvSession;
	CMsvEntry*				iMsvEntry;
	CMsvServerEntry*		iServerEntry;
	CClientMtmRegistry*		iClientMtmRegistry;
	TBuf<2> 				iDriveName ;

protected:
	enum TClientOrServerSide
		{
		ENeither,
		EClientSide,
		EServerSide
		} iClientServer;

	TBool					iRegistryObjectAndControlCreated;
	CMsvServer*				iMsvServer;
	CServerMtmDllRegistry*	iServerMtmDllRegistry;
	CMtmRegistryControl*	iMtmRegistryControl;
	TMsvDummyObserver*		iDummyObserver;
	TFileName				iLogsDir;
	TFileName				iRfc822Dir;
	TInt					iFileCount;

	};

#include <msvtestutils.inl>

#endif
