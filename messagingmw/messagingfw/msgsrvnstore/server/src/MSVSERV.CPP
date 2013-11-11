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
// MSVSERV.CPP
//

#ifdef _DEBUG
#undef _NO_SERVER_LOGGING_
#endif

#include <e32std.h>

#include <barsc.h>
#include <barsread.h>
#pragma warning( disable : 4245 )
#include <bautils.h>
#pragma warning( default : 4245 )
#include <bafindf.h>
#include <u32std.h>
#include <msgs.rsg>

#include "MSVSTD.H"
#include "MSVIPC.H"
#include "MSVIDS.H"
#include "MSVUIDS.H"
#include "MSVRUIDS.H"
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
#include "TMsvServerStoreManager.h"
#include "msvsearchsortcachemanager.h"
#include "msvsearchsortdeltacache.h"

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "cinstalledmtmgroup.h"
#include "msvconsts.h"			  
#endif

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	#include "centralrepository.h"
	#include "cmsvdiskchangenotifier.h"
	
	const TInt KMaxDriveSupported=7;
	const TInt KUidConifgFileValue = 0x10286a26;
	const TUid KUidConfigFile = {KUidConifgFileValue};
	const TInt KCenRepCurrentDriveKey=10;
	
	#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		_LIT(KPreferredDriveListFilePath, "C:\\private\\1000484b\\msgprioritydrivelist.ini");
	#else
		_LIT(KPreferredDriveListFilePath, "z:\\private\\1000484b\\msgprioritydrivelist.ini");
	#endif		  // #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE) 
#endif	  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

const TInt KMsvMtmOpsQueueGranularity=8;
const TInt KMsvMtmOpsQueueArrayGranularity=4;
const TInt KMsvMtmOpsSessionIdArrayGranularity=4;

_LIT(KRomMtmDataFilePath, ":\\RESOURCE\\MESSAGING\\MTM\\");
_LIT(KMsvStoreInitFileName, "StoreInit.tmp");
_LIT(KMsvPostInitialisationExe, "z:\\sys\\bin\\MailInit.exe");

#ifdef MSG_SERV_AUTO_CLOSE
// the server closes after the last session closes
const TInt KMsvCloseTime=0x200000; // approx 2 seconds
const TInt KMsvInitCloseTime=0xA00000; // approx 10 seconds
#endif

// the time between the attempts at writing hidden streams and removing emntries
const TInt KMsvDelayTime=0x00A00000; // approx 10 seconds
const TUid KMsvMailInitExeUid={0x10004849};
const TUint msgServerPolicyRangeCount = 22;
const TInt msgServerPolicyRanges[msgServerPolicyRangeCount] =
	{
	0,  //  EAlwaysPass	 -> EMsvGetEntry				 -> ReadEntryData
//  1,	  EAlwaysPass	 -> EMsvGetChildren			  -> Filtered
//  2,	  EAlwaysPass	 -> EMsvGetRemainingChildren
	3,  //  Test			-> EMsvLockEntry
//  4,	  Test			-> EMsvReleaseEntry
	5,  //  PanicClient	 -> EMsvMakeFileDirectory
	6,  //  EAlwaysPass	 -> EMsvChangeEntry			  -> ModifyEntryData
//  7,	  EAlwaysPass	 -> EMsvCreateEntry			  -> CreateEntry
//  8,	  EAlwaysPass	 -> EMsvDeleteEntries			-> ModifyEntryData
//  9,	  EAlwaysPass	 -> EMsvMoveEntries			  -> MoveEntries
//  10,	 EAlwaysPass	 -> EMsvCopyEntries			  -> CopyEntries
//  11,	 EAlwaysPass	 -> EMsvNotifySessionEvent
//  12,	 EAlwaysPass	 -> EMsvCancelSessionEventNotification
//  13,	 EAlwaysPass	 -> EMsvReadStore				-> ReadEntryData
//  14,	 EAlwaysPass	 -> EMsvLockStore				-> ModifyEntryData
//  15,	 EAlwaysPass	 -> EMsvReleaseStore			 -> ModifyEntryData
	16, //  PanicClient	 -> EMsvCommittedStore
	17, //  Deprecated	  -> EMsvLocalStoreDeleted
	18, //  EAlwaysPass	 -> EMsvOperationData
//  19,	 EAlwaysPass	 -> EMsvCommandData
//  20,	 EAlwaysPass	 -> EMsvCancelOperation
//  21,	 EAlwaysPass	 -> EMsvOperationProgress
//  22,	 EAlwaysPass	 -> EMsvOperationCompletion
//  23,	 EAlwaysPass	 -> EMsvOperationMtm
//  24,	 EAlwaysPass	 -> EMsvMtmCommand			   -> MTMTransferCommand
//  25,	 EAlwaysPass	 -> EMsvFillRegisteredMtmDllArray
	26, //  WriteDeviceData -> EMsvInstallMtmGroup
//  25,	 WriteDeviceData -> EMsvDeInstallMtmGroup
	28, //  EAlwaysPass	 -> EMsvUseMtmGroup
//  29,	 EAlwaysPass	 -> EMsvReleaseMtmGroup
//  30,	 EAlwaysPass	 -> EMsvGetMtmGroupData
	31, //  WriteDeviceData -> EMsvCloseServer
	32, //  EAlwaysPass	 -> EMsvStopService			  -> StopService
//  33,	 EAlwaysPass	 -> EMsvServiceActive
//  34,	 EAlwaysPass	 -> EMsvServiceProgress
//  35,	 EAlwaysPass	 -> EMsvRemoveEntry			  -> ModifyEntryData
	36, //  PanicClient	 -> EMsvCreatedStore
	37, //  EAlwaysPass	 -> EMsvGetMessageDirectory
//  38,	 EAlwaysPass	 -> EMsvSlotAvailable
//  39,	 EAlwaysPass	 -> EMsvSetSessionAsObserver
	40, //  Test			-> EMsvSetFailure
	41, //  EWriteUserData  -> EMsvChangeAttributes
	42, //  EAlwaysPass	 -> EMsvGetChildIds
	43, //  WriteDeviceData -> EMsvChangeDrive
	44, //  EAlwaysPass	 -> EMsvOutstandingOperations
//  45,	 EAlwaysPass	 -> EMsvGetNotifySequence
	46, //  Deprecated	  -> EMsvGetMtmPath
//  47,	 Deprecated	  -> EMsvSetMtmPath
//  48,	 Deprecated	  -> EMsvDelMtmPath
	49, //  EAlwaysPass	 -> EMsvSetReceiveEntryEvents
//  50,	 EAlwaysPass	 -> EMsvDecStoreReaderCount	  -> ReadEntryData
//  51,	 EAlwaysPass	 -> EMsvGetMessageDrive
//  52,	 EAlwaysPass	 -> EMsvGetMtmRequiredCapabilities
//  53,	 EAlwaysPass	 -> EMsvCreateAttachmentForWrite -> ModifyEntryData
//  54,	 EAlwaysPass	 -> EMsvOpenAttachment		   -> ReadEntryData
//  55,	 EAlwaysPass	 -> EMsvOpenAttachmentForWrite   -> ModifyEntryData
//  56,	 EAlwaysPass	 -> EMsvDeleteAttachment		 -> ModifyEntryData
//  57,	 EAlwaysPass	 -> EMsvOpenFileStoreForRead	 -> ReadEntryData
//  58,	 EAlwaysPass	 -> EMsvOpenTempStoreFile		-> ModifyEntryData
//  59,	 EAlwaysPass	 -> EMsvReplaceFileStore		 -> ModifyEntryData
//  60,	 EAlwaysPass	 -> EMsvDeleteFileStore		  -> ModifyEntryData
//  61,	 EAlwaysPass	 -> EMsvFileStoreExists
//  62,	 EAlwaysPass	 -> EMsvGetAndClearIndexCorruptFlag
		63,	// EWriteDeviceData		  -> EMsvCopyStore
//	  64,	// EWriteDeviceData		  -> EMsvDeleteStore
	65	 // EAlwaysPass			 -> EMsvDriveContainsStore
//  66,	 EAlwaysPass	 -> EMsvMessageStoreDrivePresent
//  67,	 EAlwaysPass	 -> EMsvFileExists
//  68,	 EAlwaysPass	 -> EMsvRenameAttachment		 -> ModifyEntryData
//  69,	 EAlwaysPass	 -> EMsvReplaceAttachmentForWrite -> ModifyEntryData
//  70,	 EAlwaysPass	 -> EMsvGetAttachmentFilePath
//  71,	 EAlwaysPass	 -> EMsvOperationSystemProgress
//  72,	 EAlwaysPass	 -> EMsvGetNonOperationMtmData
	};

enum TMsvSecurityPolicy
	{
	EMsvSecurityWriteDeviceData	 = 0,
	EMsvSecurityWriteUserData	   = 1,
	EMsvSecurityPanicClient		 = 2,
	EMsvSecurityTestCapabilities	= 3
	};

const TInt KMsvDeprecationPolicy	= EMsvSecurityPanicClient;
const TInt KMsvTestPolicy		   = EMsvSecurityTestCapabilities;

const TUint8 msgServerPolicyElementsIndex[] =
	{
	CPolicyServer::EAlwaysPass,	 // applies to req 0-2
	KMsvTestPolicy,				 // applies to req 3-4
	EMsvSecurityPanicClient,		// applies to req 5
	CPolicyServer::EAlwaysPass,	 // applies to req 6-15
	EMsvSecurityPanicClient,		// applies to req 16
	KMsvDeprecationPolicy,		  // applies to req 17
	CPolicyServer::EAlwaysPass,	 // applies to req 18-25
	EMsvSecurityWriteDeviceData,	// applies to req 26-27
	CPolicyServer::EAlwaysPass,	 // applies to req 28-30
	EMsvSecurityWriteDeviceData,	// applies to req 31
	CPolicyServer::EAlwaysPass,	 // applies to req 32-35
	EMsvSecurityPanicClient,		// applies to req 36
	CPolicyServer::EAlwaysPass,	 // applies to req 37-39
	KMsvTestPolicy,				 // applies to req 40
	EMsvSecurityWriteUserData,	  // applies to req 41
	CPolicyServer::EAlwaysPass,	 // applies to req 42
	EMsvSecurityWriteDeviceData,	// applies to req 43
	CPolicyServer::EAlwaysPass,	 // applies to req 44-45
	KMsvDeprecationPolicy,		  // applies to req 46-48
	CPolicyServer::EAlwaysPass,	 // applies to req 49-62
	EMsvSecurityWriteDeviceData,			// applies to req 63-64
	CPolicyServer::EAlwaysPass	  // applies to req 65
	};

const CPolicyServer::TPolicyElement msgServerPolicyElements[] =
	{
	// Check Write Device Data	  -> EMsvSecurityWriteDeviceData  = 0
	{_INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData), CPolicyServer::EFailClient},
	// Check Write User Data		-> EMsvSecurityWriteUserData	= 1
	{_INIT_SECURITY_POLICY_C1(ECapabilityWriteUserData), CPolicyServer::EFailClient},
	// Panic the client			 -> EMsvSecurityPanicClient	  = 2
	{_INIT_SECURITY_POLICY_FAIL, CPolicyServer::EPanicClient},
	// Check for message server caps-> EMsvSecurityTestCapabilities = 3
	{_INIT_SECURITY_POLICY_C7(ECapabilityReadDeviceData, ECapabilityWriteDeviceData, ECapabilityProtServ, ECapabilityNetworkControl, ECapabilityNetworkServices, ECapabilityLocalServices, ECapabilityReadUserData), CPolicyServer::EFailClient}
	};

const CPolicyServer::TPolicy msgServerPolicy =
	{
	CPolicyServer::EAlwaysPass,
	msgServerPolicyRangeCount,
	msgServerPolicyRanges,
	msgServerPolicyElementsIndex,
	msgServerPolicyElements
	};


//**********************************
// CMsvMtmOperationList
//**********************************

CMsvMtmOperationQueue::CMsvMtmOperationQueue(TUid aMtmUid, TMsvId aServiceId)
: CArrayPtrFlat<CMsvMtmOperation>(KMsvMtmOpsQueueGranularity),
	iMtmUid(aMtmUid), iServiceId(aServiceId),
	iSessionIdArray(KMsvMtmOpsSessionIdArrayGranularity)
	{
	__DECLARE_NAME(_S("CMsvMtmOperationQueue"));
	}

CMsvMtmOperationQueue::~CMsvMtmOperationQueue()
	{
	delete iMtm;
	}


//**********************************
//CMsvServer
//**********************************


CMsvServer::CMsvServer(TInt aPriority, TBool aDebug)
:   CPolicyServer(aPriority, msgServerPolicy), iSessionNumber(1), iMtmOperationQueueArray(KMsvMtmOpsQueueArrayGranularity), iDebug(aDebug), iReleasedForRestore(EFalse)
//
//
//
	{
	}


CMsvServer::~CMsvServer()
//
//
//
	{
#ifndef _NO_SERVER_LOGGING_
	Log(_L("Message Server closing"));
#endif

	__ASSERT_DEBUG(iMtmOperationQueueArray.Count()==0, PanicServer(EMsvSomeMtmQueueActive));
	iMtmOperationQueueArray.ResetAndDestroy();

	delete iMtmRegControl;
	delete iServerMtmReg;
#ifdef MSG_SERV_AUTO_CLOSE
	delete iCloseTimer;
#endif
	delete iDelayTimer;
	delete iBackup;
	delete iContext;
	delete iNewContext;
	delete iBulkCreationSelection;
	delete iMailinitWaiter;
	delete iBulkChangeSelection;

#ifndef _NO_SERVER_LOGGING_
	if (iLog.LogValid())
		iLog.CloseLog();
	iLog.Close();
#endif


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Destroy the notification objects.
	TInt notifierCount = iNotifier.Count();
	delete iDriveList;
	
	for(TInt index = 0; index < notifierCount; ++index)
		{
		iNotifier[index]->Cancel();
		}

	iNotifier.ResetAndDestroy();
#else
	delete iNotify;
#endif		  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	delete iFreePoolInstance;

	//iManager and iDeltacache deltetion.
	delete iSearchSortCacheManager;

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	delete iMessageDBAdapter;
	
	CMsvConverterWaiter *waiter = CMsvConverterWaiter::Instance();
	if(waiter != NULL)
		delete waiter;
	
#endif
	iFs.Close();
	iProtectedFolders.Reset();
	}




EXPORT_C CMsvServer* CMsvServer::NewL()
	{
	return CMsvServer::NewL(ETrue);
	}



CMsvServer* CMsvServer::NewL(TBool aDebug)
	{
	CMsvServer *pS = new(ELeave) CMsvServer(EPriority, aDebug);
	CleanupStack::PushL(pS);
	pS->ConstructL();
	CleanupStack::Pop();
	return pS;
	}



void CMsvServer::ConstructL()
	{
	// First start the server
	StartL(KMsvServerName);

	// conect to the file system
	User::LeaveIfError(iFs.Connect());
	TChar driveChar= iFs.GetSystemDriveChar();

	iSystemDrive.Append(driveChar);
	iSystemDrive.Append(KDriveDelimiter);
#ifdef MSG_SERV_AUTO_CLOSE
	iCloseTimer = CMsvTimer::NewL(*this, ETrue);
	iCloseTimer->After(KMsvInitCloseTime);
#endif
	iDelayTimer = CMsvTimer::NewL(*this, EFalse);

	iBulkCreationSelection = new(ELeave)CMsvEntrySelection();
	iBulkChangeSelection = new(ELeave)CMsvEntrySelection();

	iMailinitWaiter=CMsvMailinitWaiter::NewL(*this);

// Code changes for 557.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Create the preferred drive list.
	CreatePreferredDriveListL();
	
	// Create notifiers for the appropriate drives.
	TInt driveCount = iDriveList->Count(); 
	TDriveUnit driveValue;
	
	// Create the notification objects for each drive in
	// the preferred drive list. These objects will notify
	// message server about the activity (media insertion/
	// removal) in their respective drive.
	for(TInt i = 0;i < driveCount; ++i)
		{
		driveValue = TDriveUnit((*iDriveList)[i].driveNum);
		CMsvDiskChangeNotifier* diskChangeNotifier = CMsvDiskChangeNotifier::NewL(driveValue, *this);
		User::LeaveIfError(iNotifier.Append(diskChangeNotifier));
		}
#else
	iNotify = new(ELeave)CMsvNotifyDiskChange(iFs, *this);
#endif	  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	
#ifndef _NO_SERVER_LOGGING_
	CreateLogL();
#endif

	// Populate the protected folders list - currently only has the outbox, and
	// also the remote services are protected.
	User::LeaveIfError(iProtectedFolders.Append(KMsvGlobalOutBoxIndexEntryId));
	iRemoteServicesProtected = ETrue;

	// create the registry
	CreateRegistriesL();

	// set the file session and CMsvServer objects in TMsvServerStoreManager
	iServerStoreManager.SetMsvServerAndFileSession(*this,iFs);

	// Code change for PREQ 1189.
	// 1. Create free pool
	iFreePoolInstance = CMsvEntryFreePool::CreateL();
	

	// construct the index
	// (it used to be done in CMsvServer::DoNewSessionL when the first session
	// was being opened but we want to perform the full initialisation of
	// the Message Server before starting the watchers)
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	CreateIndexL();
	// Search Sort cache manager
	iSearchSortCacheManager= CMSvSearchSortCacheManager::CreateManagerL(*this);
	// Construct Search Sort cache Table
	CreateSearchSortCacheL();
	UpdateRepositoryL();
#else
	// Search Sort cache manager
	iSearchSortCacheManager= CMSvSearchSortCacheManager::CreateManagerL(*this);
	CreateIndexL(iDebug);
#endif

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	// Creating message DB adapter to access message header and body.
	iMessageDBAdapter = CMsvMessageDBAdapter::NewL(IndexAdapter().GetDbAdapter());
#endif

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	iNotification = EMsvNullNotification;
#endif
	}

void CMsvServer::CreateSearchSortCacheL()
	{
	iContext->IndexAdapter()->GetDbAdapter()->ConstructSortTableL();
	iContext->IndexAdapter()->GetDbAdapter()->InitializeSortTableL();
	}
	
	
void CMsvServer::CreateRegistriesL()
//
// Create the MTM registry and control
//
	{
	iServerMtmReg = CServerMtmDllRegistry::NewL(iFs);
	iMtmRegControl = CMtmRegistryControl::NewL(iFs, *iServerMtmReg);
	
	TFileName filename;
	TChar driveLetter;

	TEntry entry;	   
	TDriveList driveList;
	User::LeaveIfError(iFs.DriveList(driveList));

	for(TInt drive=0;drive< KMaxDrives; ++drive)
		{
		if (driveList[drive] == 0)
			{
			continue;
			}
		User::LeaveIfError(iFs.DriveToChar(drive,driveLetter));
		filename.Zero();
		filename.Append(driveLetter);
		filename.Append(KRomMtmDataFilePath);
		
		// scan for any mtm data files and try to load them
		if (iFs.Entry(filename, entry)==KErrNone)  
			{
			CDir* filelist=NULL;
			if (iFs.GetDir(filename, KEntryAttNormal|KEntryAttAllowUid, ESortByName, filelist)==KErrNone)
				{
				CleanupStack::PushL(filelist);  
				TInt i=filelist->Count();
				TParse fileEntry;
				TPtrC entryName;
				//check we have files
				if(i>0)
					{
					//get the first entry to be dealt with and register it
					i--;
					entry=(*filelist)[i];
					fileEntry.Set(entry.iName,NULL,NULL);
					entryName.Set(fileEntry.Name());

					while(i>=0)
						{
						filename.Zero();
						filename.Append(driveLetter);
						filename.Append(KRomMtmDataFilePath);
						filename.Append(entry.iName);
						TUid mtmtypeuid;

						// Use the file most appropriate to the current language
						BaflUtils::NearestLanguageFile(iFs, filename);

	#if defined (_DEBUG)
						TInt err = iMtmRegControl->InstallMtmGroup(filename, mtmtypeuid);
						iFs.Entry(filename, entry);
						__ASSERT_DEBUG(err==KErrNone || err==KErrAlreadyExists || !((entry[0]==KPermanentFileStoreLayoutUid) && (entry[1]==KUidMsvDataComponent)), PanicServer(EMsvBadMtmDatFile));
	#else
						iMtmRegControl->InstallMtmGroup(filename, mtmtypeuid); // ignore the error
	#endif
						//search for the next entry with a different filename
						while(i--)
							{
							TPtrC prevEntry=fileEntry.Name();
							//get next entry
							entry=(*filelist)[i];
							TParse nextFileEntry;
							nextFileEntry.Set(entry.iName,NULL,NULL);
							entryName.Set(nextFileEntry.Name());
							if(entryName.Compare(prevEntry)!=0)
								{
								//different filename
								break;
								}
							}
						}
					}
				CleanupStack::PopAndDestroy();
				}
			}
		}
		
	}




// Code change for PREQ 557.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

/**
 * CreateIndexL()
 *
 * Creates the index context. This will create the index
 * cache and related objects.
 */
EXPORT_C void CMsvServer::CreateIndexL()
	{
	__ASSERT_DEBUG(iNewContext == NULL, PanicServer(EMsvNewContextExists));

	// Create the new context.
	iNewContext = CMsvIndexContext::NewL(*this, *this);
	iStartupState = EMsvNullNotification;

	// Creating the index cache.
	iNewContext->CreateIndexL();
	
	// Updating the current drive number.
	TUint curDriveIndex = iDriveList->CurrentDriveIndex();
	iIndexDrive = (*iDriveList)[curDriveIndex].driveNum;
	}




// The context has completed successfully or with failure
void CMsvServer::ContextComplete(TInt aError, TBool aRunMailInit)
	{
#ifndef _NO_SERVER_LOGGING_
	Log(_L("Context complete with error %d"), aError);
#endif

	if (!aError)
		{
		for(TInt index = 0; index < iNotifier.Count(); ++index)
			{
			iNotifier[index]->Cancel();
			}

		iContext = iNewContext;
		iNewContext = NULL;
		
#ifndef _NO_SERVER_LOGGING_
		Log(_L("Deleting iBackup"));
#endif

		if(iBackup == NULL)
			{
			// To Gracefully handle and to register with the backup server.
			TRAP_IGNORE(iBackup=CMsvBackupHandler::NewL(*this));
			}

		// Run mailinit.
		TUint unused;
		iFs.SetSessionToPrivate(iDriveList->CurrentDriveNumber());
		TBool initNotCompleted = (iFs.Att(KMsvStoreInitFileName, unused)!=KErrNotFound);
		if ((initNotCompleted || aRunMailInit))
			{
			RunMailInitExe(iDriveList->CurrentDriveNumber());
			}

		// Send index ready notification
		NotifyChanged(EMsvIndexLoaded);	 
		// Reset the index error state
		iContext->IndexAdapter()->SetErrorState(KErrNone);
		iStartupState = EMsvNullNotification;
	
		// Restart disk change notifications.
		if(iContext)
			{
			for(TInt index = 0; index < iNotifier.Count(); ++index)
				{
				iNotifier[index]->Start();
				}
			}
		}	   // if (!aError)

	// Remember the error
	iLoadError = aError;
	}




/**
 * ChangeDrive()
 * @param TInt: DriveNumber of the new current drive.
 * @param TRequestStatus: Request Status.
 * @return TInt: System wide error code.
 *
 * The function changes the message server current drive to the
 * drive specified as its first argument. The function implementation
 * is changed in PREQ 557. The function now assumes that the new 
 * current drive is already present in the preferred drive list. The 
 * client should ensure this before calling ChangeDrive().
 */
EXPORT_C TInt CMsvServer::ChangeDrive(TInt aDrive, TRequestStatus* aStatus)
	{
	__ASSERT_ALWAYS(iContext->State() == TMsvIndexLoadProgress::EIndexComplete, PanicServer(EMsvLoadingInProgress));

	TRAPD(err, DoChangeDriveL(aDrive, aStatus));
	return err;
	}
	
	
	
void CMsvServer::DoChangeDriveL(TInt aDrive, TRequestStatus* aStatus)
	{
	if(aDrive == iIndexDrive)
		{
		User::Leave(KErrNotSupported);
		}
	
	if(aStatus)
		{
		*aStatus = KRequestPending; 
		}
	
	// Check if the drive is present in the preferred drive list.
	TInt newCurrentDriveIndex = 0;
	TBool isDriveFound = EFalse;
	for(; newCurrentDriveIndex < CMsvPreferredDriveList::GetDriveList()->Count(); newCurrentDriveIndex++)
		{
		if(aDrive == (*iDriveList)[newCurrentDriveIndex].driveNum)
			{
			isDriveFound = ETrue;
			break;
			}
		}
	
	// The new drive is not already present in the preferred drive list.
	if(!isDriveFound)
		{
		// We should add the drive in the drive list.
		// Check if the drive list is full
		if(iDriveList->Count() < KMaxDriveSupported)
			{
			// If the drive list is NOT full, insert the new
			// current drive entry at the beginning of the list.

			TUint KFirstPriority = 1;
			AddDriveToListL((TDriveNumber) aDrive, KFirstPriority);
			}
		else
			{
			// If the drive list is full, check the last drive
			// in the drive list.

			// If the last drive is the current drive, remove the
			// second last drive from the preferred drive list.
			if(iDriveList->CurrentDriveIndex() == KMaxDriveSupported - 1)
				{
				RemoveDriveFromListL((*iDriveList)[KMaxDriveSupported - 2].driveNum);
				}
			else
				{
				// If the last drive is not the current drive, remove
				// it from the list. 
				RemoveDriveFromListL((*iDriveList)[KMaxDriveSupported - 1].driveNum);
				}

			// Now insert the new current drive entry at the 
			// beginning of the list.
			TUint KFirstPriority = 1;
			AddDriveToListL((TDriveNumber)aDrive, KFirstPriority);
			}
		
		// Complete the caller.
		if(aStatus)
			{
			User::RequestComplete(aStatus, KErrNone);
			}
		}
	// If the new drive is already present in the preferred drive list.
	else
		{
		// The drive status should be either 
		// EMsvMessageStoreAvailable or EMsvMessageStoreUnavailable.
		if( (EMsvMessageStoreAvailableStatus   != (*iDriveList)[newCurrentDriveIndex].status) &&
			(EMsvMessageStoreUnavailableStatus != (*iDriveList)[newCurrentDriveIndex].status)
		  )
			{
			User::Leave(KErrNotSupported);
			}
	
		// Check if server is busy.
		if (0 < OutstandingOperations())
			{
			User::Leave(KErrServerBusy);
			}
	
		iStartupState = EMsvMediaChanged;
		
		// Perform Change Drive.
		if(aStatus)
			{
			User::LeaveIfError(iContext->ChangeDrive(newCurrentDriveIndex, *aStatus));
			}
		else
			{
			User::LeaveIfError(iContext->ChangeDrive(newCurrentDriveIndex));
			}
				
		// Remove the drive 
		TMsvPreferredDrive driveEntry = (*iDriveList)[newCurrentDriveIndex];
		iDriveList->Remove(newCurrentDriveIndex);
		iDriveList->Insert(driveEntry, 0);
		iDriveList->SetCurrentDriveIndex(0);
		iIndexDrive = (*iDriveList)[0].driveNum;
	
		// Update the CenRep with the changes to the preferred drive list.
		UpdateRepositoryL();
	
		// Update the notifier list.
		CMsvDiskChangeNotifier* notifier = iNotifier[newCurrentDriveIndex];
		iNotifier.Remove(newCurrentDriveIndex);
		iNotifier.Insert(notifier, 0);
		}
	
	// Changes for 1667.
	if(iSearchSortCacheManager)
		{   
		iSearchSortCacheManager->ResetSearchSortCache();
		}
	
	iStartupState = EMsvNullNotification;
	}




// This function is called when ChangeDrive() 
// is completed in index context.
void CMsvServer::ChangeDriveComplete(TInt aError, TBool aRunMailInit, TDriveNumber aNewDrive)
	{
#ifndef _NO_SERVER_LOGGING_
	Log(_L("ChangeDrive() complete with error %d"), aError);
#endif

	if (!aError)
		{
		for(TInt index = 0; index < iNotifier.Count(); ++index)
			{
			iNotifier[index]->Cancel();
			}

#ifndef _NO_SERVER_LOGGING_
		Log(_L("Deleting iBackup"));
#endif

		delete iBackup;
		iBackup = NULL;
		// To Gracefully handle the missing backup server.
		TRAP_IGNORE(iBackup = CMsvBackupHandler::NewL(*this));

		// Run mailinit.
		TUint unused;
		iFs.SetSessionToPrivate(aNewDrive);
		TBool initNotCompleted = (iFs.Att(KMsvStoreInitFileName, unused)!=KErrNotFound);
		if ((initNotCompleted || aRunMailInit) && !(iDebug))
			{
			RunMailInitExe(aNewDrive);
			}

		// Send change drive notification.
		NotifyChanged(EMsvMediaChanged, KMsvNullIndexEntryId, iIndexDrive, aNewDrive);

		// Reset the index error state
		iContext->IndexAdapter()->SetErrorState(KErrNone);
		iStartupState = EMsvNullNotification;
	
		// Restart disk change notifications.
		for(TInt index = 0; index < iNotifier.Count(); ++index)
			{
			if(!iNotifier[index]->IsActive())
				{
				iNotifier[index]->Start();
				}
			}
		}	   // if (!aError)

	// Remember the error
	iLoadError = aError;
	}
	
	


/**
 * CreatePreferredDriveList()
 * @param None.
 * @return None.
 *
 * Creates a preferred drive list.
 * 1. Create an initial preferred drive list.
 * 2. Check if a CenRep has been created.
 *  2.1. CenRep has been created. Read from CenRep and fill the initial preferred
 *	   drive list.
 *  2.2. CenRep has not been created yet. Check if the config file exists.
 *	  2.2.1. Config file exists. Fill the initial preferred drive list from it.
 *	  2.2.2. Save this in the CenRep.
 */
void CMsvServer::CreatePreferredDriveListL()
	{
	_LIT(KDriveNameFormat, "PriorityDrive%d");
	
	
	// 1. Create an initial preferred drive list.
	iDriveList = CMsvPreferredDriveList::CreateL();
	
	// 2. Check if a CenRep has been created.
	// We use CRepository to open the CenRep and read the first key
	// from it. The CenRep is valid if the value returned is valid,
	// i.e. <= EDriveZ in this case.
	CRepository* repository = CRepository::NewL(KUidConfigFile);
	CleanupStack::PushL(repository);
	TUint keyIndex = 0;
	TInt driveIndex = 0;
	TInt driveNum;
	TInt err = repository->Get(keyIndex, driveNum);
		
	// 2.1. CenRep has been created. Read from CenRep and fill the initial preferred
	//	  drive list.
	// <= EDriveZ will be a valid drive number and will mean that CenRep has
	// not been initialized by us yet.
	if( (err == KErrNone) && (driveNum <= EDriveZ) )
		{
		for(; keyIndex < KMaxDriveSupported; ++keyIndex)
			{
			if(keyIndex)
				{
				User::LeaveIfError(repository->Get(keyIndex, driveNum));
				if(driveNum > EDriveZ)
					{
					break;
					}
				}
						
			// Insert this drive in the list at the appropriate index.
			TMsvPreferredDrive driveEntry;
			driveEntry.driveNum = (TDriveNumber) driveNum;
			driveEntry.driveId = KMsvInvalidDriveId;
			driveEntry.status = EMsvInvalidDriveStatus;
			iDriveList->Insert(driveEntry, driveIndex);
			++driveIndex;
			}
		}
	
	// 2.2. CenRep has not been created yet. Check if the config file exists.
	else
		{
		CMsvIniData* iniConfigFile = NULL;
		
		TRAPD(err, iniConfigFile = CMsvIniData::NewL(KPreferredDriveListFilePath));
		if(KErrNone == err)
			{
			CleanupStack::PushL(iniConfigFile);
			
			// List of drives currently present in the device.
			TBool systemDriveAdded = EFalse;
			TInt systemDrive = RFs::GetSystemDrive();
			TName driveNameMacro;
			TDriveList driveList;
			User::LeaveIfError(iFs.DriveList(driveList));	   
			
			// Add entries into preferred drive list.
			keyIndex = 0;
			driveIndex = 0;
			for(TInt index = 1; index <= KMaxDriveSupported; ++index)
				{
				// Read drive number from ini file.
				TInt driveNumber;
				TPtrC ptr(NULL, 0);
				driveNameMacro.Format(KDriveNameFormat, index);
				TBool found = iniConfigFile->FindVar(driveNameMacro, ptr);
				if( !found || 
					KErrNone != RFs::CharToDrive((TChar)ptr[0], driveNumber)
				  )
					{		   
					continue;
					}
				
				// Check if the drive is present in the device and
				// is not a ROM drive.
				if( (KDriveAbsent == driveList[driveNumber]) ||
					(driveNumber == EDriveZ)
				  )
					{
					continue;
					}				   
							
				// Check for duplicates before adding in the drive list.
				TBool entryFound = EFalse;
				for(TInt i = 0; i < driveIndex; i++)
					{
					if((*iDriveList)[i].driveNum == driveNumber)
						{
						entryFound = ETrue;
						break;
						}
					}
				
				if(entryFound)
					{
					continue;
					}

				// If it is a system drive...
				if(driveNumber == systemDrive)
					{
					systemDriveAdded = ETrue;
					}
						
				// Add drive entry into preferred drive list.
				TMsvPreferredDrive driveEntry;
				driveEntry.driveNum = (TDriveNumber) driveNumber;
				driveEntry.driveId = KMsvInvalidDriveId;
				driveEntry.status = EMsvInvalidDriveStatus;
				iDriveList->Insert(driveEntry, driveIndex);
				User::LeaveIfError(repository->Set(keyIndex, (TInt)((*iDriveList)[driveIndex].driveNum)));
				++driveIndex;
				++keyIndex;
				} //for(TInt index = 1; index <= KMaxDriveSupported; ++index)
				
			CleanupStack::PopAndDestroy();  // iniConfigFile
			
			// If the system drive is not already added.
			if(!systemDriveAdded)
				{
				// If the number of drives in the drive list is already 7,
				// Replace the last drive with the system drive.
				TInt index = (driveIndex < KMaxDriveSupported)? driveIndex: 6;
				
				TMsvPreferredDrive driveEntry;
				driveEntry.driveNum = (TDriveNumber) systemDrive;
				driveEntry.driveId = KMsvInvalidDriveId;
				driveEntry.status = EMsvInvalidDriveStatus;
				iDriveList->Insert(driveEntry, index);
				User::LeaveIfError(repository->Set(keyIndex, (TInt)((*iDriveList)[index].driveNum)));
				}
			}
		
		// Prefered drive list config file is not available. Add system drive to the drive list.
		else
			{
			TMsvPreferredDrive driveEntry;
			driveEntry.driveNum = RFs::GetSystemDrive();
			driveEntry.driveId = KMsvInvalidDriveId;
			driveEntry.status = EMsvInvalidDriveStatus;
			iDriveList->Insert(driveEntry, 0);
			keyIndex = 0;
			User::LeaveIfError(repository->Set(keyIndex, (TInt)((*iDriveList)[0].driveNum)));
			}
		}

	CleanupStack::PopAndDestroy(); //repository
	}



/**
 * PrepareDriveForDeletionL()
 * @param TDriveNumber: Drive number under consideration.
 *
 */
void CMsvServer::PrepareDriveForDeletionL(TDriveNumber aDrive)
	{
	// Drive should be present in the preferred drive list.
	TInt index = 0;
	TBool isDriveFound = EFalse;
	for(; index < iDriveList->Count(); index++)
		{
		if(aDrive == (*iDriveList)[index].driveNum)
			{
			isDriveFound = ETrue;
			break;
			}
		}
	if(!isDriveFound)
		{
		return;
		}
		
	// Following operations needs to be performed
	// only if the message store exists in the drive.
	if(EMsvMessageStoreAvailableStatus == (*iDriveList)[index].status)
		{
		// Cleanup cache and detach the corresponding DB.
		IndexAdapter().RemoveDriveL((*iDriveList)[index].driveId, index, EFalse);
		
		// Send notification to all registered clients to refresh their view of messaging.
		NotifyChanged(EMsvRefreshMessageView, KMsvNullIndexEntryId, (*iDriveList)[index].driveNum);	 
		}
	}
	
		
	

/**
 * RemoveDriveFromListL()
 * @param TDriveNumber: Drive number of the drive to be removed.
 * 
 */
void CMsvServer::RemoveDriveFromListL(TDriveNumber aDriveNumber)
	{
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	iNotification = EMsvNullNotification;
#endif

	// System drive cannot be removed from the preferred
	// drive list.
	if(aDriveNumber == RFs::GetSystemDrive())
		{
		User::Leave(KErrArgument);
		}
	
	// Drive should be present in the preferred drive list.
	TInt index = 0;
	TBool isDriveFound = EFalse;
	for(; index < iDriveList->Count(); index++)
		{
		if(aDriveNumber == (*iDriveList)[index].driveNum)
			{
			isDriveFound = ETrue;
			break;
			}
		}
		
	if(!isDriveFound)
		{
		User::Leave(KErrNotFound);
		}
		
	// This is a request to remove current drive.
	TUint curDriveIndex = iDriveList->CurrentDriveIndex();
	if(curDriveIndex == index)
		{
		// Make sure there are no pending operations
		// with the server.
		if (0 < OutstandingOperations())
			{
			User::Leave(KErrServerBusy);
			}
			
		iStartupState = EMsvMediaChanged;
		
		// Check for the next available drive.
		// This assumes that there is always a drive available.
		TInt newDriveIndex = index + 1;
		for(; newDriveIndex < iDriveList->Count(); newDriveIndex++)
			{
			if( (EMsvMessageStoreAvailableStatus   == (*iDriveList)[newDriveIndex].status) ||
				(EMsvMessageStoreUnavailableStatus == (*iDriveList)[newDriveIndex].status)
			  )
				{
				break;
				}
			}
		
		// Perform ChangeDrive() operation.
		User::LeaveIfError(iContext->ChangeDrive(newDriveIndex, EFalse));
		
		// Update the current drive index.
		iDriveList->SetCurrentDriveIndex(newDriveIndex);
		
		iIndexDrive = (*iDriveList)[newDriveIndex].driveNum;
		iStartupState = EMsvNullNotification;
		
		// Remove drive entry from the preferred drive list.
		iDriveList->Remove(index);  
		}
	else
		// This handles removal of non-current drive.
		{
		// Following operations needs to be performed
		// only if the message store exists in the drive.
		if(EMsvMessageStoreAvailableStatus == (*iDriveList)[index].status)
			{
			// Cleanup cache and detach the corresponding DB.
			IndexAdapter().RemoveDriveL((*iDriveList)[index].driveId, index, EFalse);
				
			// Send notification to all registered clients to refresh their view of messaging.
			NotifyChanged(EMsvRefreshMessageView, KMsvNullIndexEntryId, (*iDriveList)[index].driveNum);
			}
		
		// Remove drive entry from the preferred drive list.
		iDriveList->Remove(index);
		}
		
	// Update the CenRep with the changes to the preferred drive list.
	UpdateRepositoryL();
	
	// Remove the notifier for this drive.
	delete iNotifier[index];
	iNotifier.Remove(index);	
	}
	
	


/**
 * AddDriveToListL()
 * @param TDriveNumber: Drive number of the drive to be added.
 * @param CMsvServerSession*: The calling session.
 * @param TUint: Priority of the new drive.
 * 
 */ 
void CMsvServer::AddDriveToListL(TDriveNumber aDriveNumber, TUint& aPriority, CMsvServerSession* aCurrentSession /*DEFAULT=NULL*/)
	{
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	iNotification = EMsvNullNotification;
#endif

	TInt driveIndex = aPriority - 1;
	// Check if the limit of allowable drives has been reached.
	if(KMaxDriveSupported == iDriveList->Count())
		{
		User::Leave(KErrNotSupported);
		}
			
	// Check if the drive is present in the device.
	TDriveList driveList;
	User::LeaveIfError(iFs.DriveList(driveList));
	if (0 == driveList[aDriveNumber])
		{
		User::Leave(KErrNotFound);
		}

	// Check if the priority is valid.
	if( (0 > driveIndex) || (KMaxDriveSupported - 1 < driveIndex) )
		{
		User::Leave(KErrArgument);
		}
	
	// Check if the drive already present in the preferred drive list.
	TInt index = 0;
	for(; index < iDriveList->Count(); index++)
		{
		if(aDriveNumber == (*iDriveList)[index].driveNum)
			{
			User::Leave(KErrAlreadyExists);
			break;
			}
		}
		
	// Create the drive object.
	TMsvPreferredDrive driveEntry;
	driveEntry.driveNum = aDriveNumber;
	driveEntry.driveId = KMsvInvalidDriveId;
	driveEntry.status = EMsvInvalidDriveStatus;
	
	// This is to avoid USER:131 panic.
	if(driveIndex > iDriveList->Count())
		{
		driveIndex = iDriveList->Count();
		aPriority = driveIndex + 1;
		}
	// Create the drive entry in preferred drive list.
	iDriveList->Insert(driveEntry, driveIndex);
		
	// Update the new drive status.
	TDriveState statusDuringUpdate = EMsvInvalidDriveStatus;
	TRAPD(err, iContext->UpdateDriveStatusL(driveIndex, statusDuringUpdate));

	// Undo the insertion if there is an error.
	if(KErrNone != err)
		{
		iDriveList->Remove(driveIndex);
		User::Leave(err);
		}
			
	// Send a message store corrupt notification first if it was wiped. This needs to be
	// sent before the drive change notification. Other notifications are sent below.
	if(EMsvMessageStoreCorruptStatus == statusDuringUpdate)
		{
		TMsvPackedChangeNotification package(iChange);
		package.Pack(EMsvMessageStoreCorrupt, KMsvNullIndexEntryId, driveEntry.driveNum, 0);
		if(aCurrentSession)
			{
			TRAP_IGNORE(aCurrentSession->NotifyChangedL(iChange, EFalse));
			}
		}

	TDriveState driveStatus = (*iDriveList)[driveIndex].status;
	// If the priority of the new drive is higher than the 
	// current drive, we need to change the current drive.
	TUint curDriveIndex = iDriveList->CurrentDriveIndex();
	if(driveIndex <= curDriveIndex)
		{
		// Change the drive only if a valid message store
		// exists or can be created.
		if( (KErrNone == err) &&
			( (EMsvMessageStoreAvailableStatus   == driveStatus) ||
			  (EMsvMessageStoreUnavailableStatus == driveStatus)
			)
		  )		
			{
			// Make sure there are no pending operations
			// with the server.
			if (0 < OutstandingOperations())
				{
				iDriveList->Remove(driveIndex);
				User::Leave(KErrServerBusy);
				}
				
			TBool isEntryRemovalRequired = EFalse;
			if(EMsvMessageStoreAvailableStatus == (*iDriveList)[driveIndex].status)
				{
				isEntryRemovalRequired = ETrue;
				}
				
			iStartupState = EMsvMediaChanged;
			err = iContext->ChangeDrive(driveIndex, ETrue);
			if(KErrNone == err)
				{
				TUint oldDriveNum = iDriveList->CurrentDriveNumber();
				iDriveList->SetCurrentDriveIndex(driveIndex);
				iIndexDrive = (*iDriveList)[driveIndex].driveNum;
				
				// Remove incomplete entries from the new drive.
				if(isEntryRemovalRequired)
					{
					iContext->GetInPreparationIds((*iDriveList)[driveIndex].driveId);		   
					if (iContext->Remove().Count())
						{
						RemoveEntry(KMsvNullIndexEntryId);
						}
					}
				}
			iStartupState = EMsvNullNotification;
			}
		
		// Undo the insertion if there is an error.
		if(KErrNone != err)
			{
			iDriveList->Remove(driveIndex);
			User::Leave(err);
			}
		}
	else
		// Priority is lower than the current drive.
		{
		// Update index cache and the database, only if
		// message store is available in the new drive.

		if(EMsvMessageStoreAvailableStatus == driveStatus)
			{
			TRAP(err, IndexAdapter().AddDriveL(driveIndex));
			if(KErrNone == err)
				{
				iContext->GetInPreparationIds((*iDriveList)[driveIndex].driveId);		   
				if (iContext->Remove().Count())
					{
					RemoveEntry(KMsvNullIndexEntryId);
					}
				
				// Send notification to all registered clients to refresh their view of messaging.
				NotifyChanged(EMsvRefreshMessageView, KMsvNullIndexEntryId, (*iDriveList)[driveIndex].driveNum);
				}
			else
				{
				iDriveList->Remove(driveIndex);
				User::Leave(err);
				}
			}
		}
	
	// Update the CenRep with the changes to the preferred drive list.
	UpdateRepositoryL();
	
	// Add a notifier for this drive.
	TDriveUnit driveValue = TDriveUnit(aDriveNumber);
	CMsvDiskChangeNotifier* diskChangeNotifier = CMsvDiskChangeNotifier::NewL(driveValue, *this);
	CleanupStack::PushL(diskChangeNotifier);
	User::LeaveIfError(iNotifier.Insert(diskChangeNotifier, driveIndex));
	diskChangeNotifier->Start();
	CleanupStack::Pop();
	
	// Send a notification to only current client if and as set above.
	if(aCurrentSession)
		{
		TMsvPackedChangeNotification package(iChange);
		switch(statusDuringUpdate)
			{
			case EMsvDriveDiskNotAvailableStatus:
				package.Pack(EMsvDiskNotAvailable, KMsvNullIndexEntryId, driveEntry.driveNum, 0);
				TRAP_IGNORE(aCurrentSession->NotifyChangedL(iChange, EFalse));
				break;
			
			case EMsvMessageStoreNotSupportedStatus:
				package.Pack(EMsvMessageStoreNotSupported, KMsvNullIndexEntryId, driveEntry.driveNum, 0);
				TRAP_IGNORE(aCurrentSession->NotifyChangedL(iChange, EFalse));
				break;
				
			default:
				break;
			}
		}
	}
	
	


/**
 * UpdateDrivePriorityL()
 * @param TDriveNumber: Drive number of the drive to be updated.
 * @param TUint: New priority of the drive.
 * 
 */ 
void CMsvServer::UpdateDrivePriorityL(TDriveNumber aDriveNumber, TUint& aNewPriority)
	{
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	iNotification = EMsvNullNotification;
#endif

	TInt updatedDriveIndex = aNewPriority - 1;

	// Check if the new priority is valid.
	if( (0 > updatedDriveIndex) || (6 < updatedDriveIndex) )
		{
		User::Leave(KErrArgument);
		}


	// Drive should be present in the preferred drive list.
	TInt index = 0;
	TBool isDriveFound = EFalse;
	for(; index<CMsvPreferredDriveList::GetDriveList()->Count(); index++)
		{
		if(aDriveNumber == (*iDriveList)[index].driveNum)
			{
			isDriveFound = ETrue;
			break;
			}
		}
		
	if(!isDriveFound)
		{
		User::Leave(KErrNotFound);
		}
		
	// If new priority is same as old.
	if(index == updatedDriveIndex)
		{
		return;
		}

	// Creating a copy of the drive entry.
	TMsvPreferredDrive driveEntry;
	driveEntry.driveNum = (*iDriveList)[index].driveNum;
	driveEntry.driveId = (*iDriveList)[index].driveId;
	driveEntry.status = (*iDriveList)[index].status;
	
	CMsvDiskChangeNotifier* notifier = NULL;
			
	// Check if the current drive priority needs to be updated.
	TUint curDriveIndex = iDriveList->CurrentDriveIndex();
	if(index == curDriveIndex)
		{
		// If the current drive remains current even after updation.
		if(updatedDriveIndex < curDriveIndex)
			{
			iDriveList->Remove(index);
			iDriveList->Insert(driveEntry, updatedDriveIndex);
			iDriveList->SetCurrentDriveIndex(updatedDriveIndex);
			
			// Update the CenRep with the changes to the preferred drive list.
			UpdateRepositoryL();
			
			// Update the notifier list.
			notifier = iNotifier[index];
			iNotifier.Remove(index);
			iNotifier.Insert(notifier, updatedDriveIndex);
			
			return;
			}
		
		// Ignoring the possibility that newDriveIndex 
		// is same as current drive priority.
		
		// Look for new current drive.
		if(updatedDriveIndex > curDriveIndex)
			{
			// Update the priority to avoid USER:131 panic.
			if(updatedDriveIndex >= CMsvPreferredDriveList::GetDriveList()->Count())
				{
				updatedDriveIndex = CMsvPreferredDriveList::GetDriveList()->Count() - 1;
				aNewPriority = updatedDriveIndex + 1;
				}
			
			// Add the current drive in its position.
			// Now current drive is present twice in the drive list.
			// And their drive index are: index and (updatedDriveIndex + 1).
			iDriveList->Insert(driveEntry, updatedDriveIndex + 1);
			TInt newCurrentIndex = curDriveIndex + 1;
			for(; newCurrentIndex<=(updatedDriveIndex+1); newCurrentIndex++)
				{
				if( (EMsvMessageStoreAvailableStatus   == (*iDriveList)[newCurrentIndex].status) ||
					(EMsvMessageStoreUnavailableStatus == (*iDriveList)[newCurrentIndex].status)
				  )
					{
					break;
					}
				}
			
			// If there is no change in the current drive.
			if(newCurrentIndex == updatedDriveIndex + 1)
				{
				iDriveList->Remove(index);
				iDriveList->SetCurrentDriveIndex(updatedDriveIndex);
				
				// Update the CenRep with the changes to the preferred drive list.
				UpdateRepositoryL();
				
				// Update the notifier list.
				notifier = iNotifier[index];
				iNotifier.Remove(index);
				iNotifier.Insert(notifier, updatedDriveIndex);
				
				return;
				}
			else
				{
				// newCurrentIndex is the position of new current drive.
				if (0 < OutstandingOperations())
					{
					User::Leave(KErrServerBusy);
					}

				iStartupState = EMsvMediaChanged;
									
				// Perform Change Drive.
				User::LeaveIfError(iContext->ChangeDrive(newCurrentIndex, ETrue));
			
				// The drive-id of the old current drive is updated.
				// This should be refelected in the duplicate driveEntry 
				// created in the preferred drive list.
				(*iDriveList)[updatedDriveIndex + 1].driveId = (*iDriveList)[curDriveIndex].driveId;
				// Remove the drive 
				iDriveList->Remove(curDriveIndex);
				iDriveList->SetCurrentDriveIndex(newCurrentIndex - 1);
				
				iIndexDrive = (*iDriveList)[newCurrentIndex - 1].driveNum;
				
				// Update the CenRep with the changes to the preferred drive list.
				UpdateRepositoryL();
				
				// Update the notifier list.
				notifier = iNotifier[index];
				iNotifier.Remove(index);
				iNotifier.Insert(notifier, updatedDriveIndex);

				iStartupState = EMsvNullNotification;
				return;
				}		   
			}
		}
	else
		{
		// Updating non-current drive.
		
		// The drive can be the new current drive.
		if(updatedDriveIndex <= curDriveIndex)
			{
			// Check if the drive can be made the current drive.
			if( (EMsvMessageStoreAvailableStatus   == (*iDriveList)[index].status) ||
				(EMsvMessageStoreUnavailableStatus == (*iDriveList)[index].status)
			  )
				{
				// updatedDriveIndex is the position of new current drive.
				if (0 < OutstandingOperations())
					{
					User::Leave(KErrServerBusy);
					}
				iStartupState = EMsvMediaChanged;
				
				// Perform Change Drive.
				User::LeaveIfError(iContext->ChangeDrive(index, ETrue));
			
				// Remove the drive 
				driveEntry = (*iDriveList)[index];
				iDriveList->Remove(index);
				iDriveList->Insert(driveEntry, updatedDriveIndex);
				iDriveList->SetCurrentDriveIndex(updatedDriveIndex);
								
				iIndexDrive = (*iDriveList)[updatedDriveIndex].driveNum;
				
				// Update the CenRep with the changes to the preferred drive list.
				UpdateRepositoryL();
				
				// Update the notifier list.
				notifier = iNotifier[index];
				iNotifier.Remove(index);
				iNotifier.Insert(notifier, updatedDriveIndex);
				
				iStartupState = EMsvNullNotification;
				return;			 
				}
			else
				{
				// A message store cannot be created in the drive.
				// Hence no change in the current drive.
				
				// Remove the drive 
				iDriveList->Remove(index);
				iDriveList->Insert(driveEntry, updatedDriveIndex);

				// Update the CenRep with the changes to the preferred drive list.
				UpdateRepositoryL();
				
				// Update the notifier list.
				notifier = iNotifier[index];
				iNotifier.Remove(index);
				iNotifier.Insert(notifier, updatedDriveIndex);

				return; 
				}		   
			}	   
		else
			{
			iDriveList->Remove(index);
			
			if(updatedDriveIndex >= CMsvPreferredDriveList::GetDriveList()->Count())
				{
				updatedDriveIndex = CMsvPreferredDriveList::GetDriveList()->Count();
				aNewPriority = updatedDriveIndex + 1;
				}	   
			iDriveList->Insert(driveEntry, updatedDriveIndex);

			// Update the CenRep with the changes to the preferred drive list.
			UpdateRepositoryL();
			
			// Update the notifier list.
			notifier = iNotifier[index];
			iNotifier.Remove(index);
			iNotifier.Insert(notifier, updatedDriveIndex);
			
			return;
			}
		}   
	}




/**
 * UpdateRepositoryL()
 * @param None.
 * @return None.
 * 
 * Updates CenRep with changes to the preferred drive list.
 */
void CMsvServer::UpdateRepositoryL()
	{
	CRepository* repository = CRepository::NewL(KUidConfigFile);
	CleanupStack::PushL(repository);
	
	// Reset everything in the repository. New drive list information
	// will be written.
	repository->Reset();
	
	TInt driveIndex = 0;	
	for(TUint keyIndex=0; keyIndex < CMsvPreferredDriveList::GetDriveList()->Count(); ++keyIndex, ++driveIndex)
		{
		User::LeaveIfError(repository->Set(keyIndex, (*iDriveList)[driveIndex].driveNum));
		}
		
	// Setting up the current drive information.
	User::LeaveIfError(repository->Set(KCenRepCurrentDriveKey, CMsvPreferredDriveList::GetDriveList()->CurrentDriveNumber()));
	CleanupStack::PopAndDestroy(); //repository
	}
	


#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
void CMsvServer::DiskRemoved(const TDriveUnit& aDrive, TBool aTestRemoval/*DEFAULT = EFalse*/)
#else
void CMsvServer::DiskRemoved(const TDriveUnit& aDrive)
#endif
	{
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	TRAPD(err, DoDiskRemovedL(aDrive, aTestRemoval));
#else
	TRAPD(err, DoDiskRemovedL(aDrive));
#endif
	
	TRAP_IGNORE(UpdateRepositoryL());
	if(KErrNone != err)
		{
		NotifyChanged(EMsvUnableToProcessDiskNotification, KMsvNullIndexEntryId, TInt(aDrive), 0);	  
		}
	}



#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
void CMsvServer::DoDiskRemovedL(const TDriveUnit& aDrive, TBool aTestRemoval)
#else
void CMsvServer::DoDiskRemovedL(const TDriveUnit& aDrive)
#endif
	{
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	iNotification = EMsvNullNotification;
#endif

	__ASSERT_DEBUG(iContext->IndexAdapter(), PanicServer(EMsvNoIndex));

#ifndef _NO_SERVER_LOGGING_
	Log(_L("Disk %d removed"), TInt(aDrive));
#endif

	//Find the index of the drive that has been removed.
	TInt driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	TInt index = 0;
	for(index=0; index < driveCount; ++index)
		{
		if((TInt)(*iDriveList)[index].driveNum == (TInt)aDrive)
			{
			break;
			}
		}
	
	//Record previous drive status before we attempt to update drive's status.
	TDriveState previousDriveStatus = (*iDriveList)[index].status;
	
	//Update the drive's status.
	TDriveState statusDuringUpdate = EMsvInvalidDriveStatus;
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	if(aTestRemoval)
		(*iDriveList)[index].status = EMsvDriveDiskNotAvailableStatus;
	else
#endif
		iContext->UpdateDriveStatusL(index, statusDuringUpdate);
		
	//Check if the disk had a message store in the first place.
	//If yes, then perform removal operations.
	//Note that we do not send a notification for a disk which did
	//not have a message store available.
	if(EMsvMessageStoreAvailableStatus == previousDriveStatus)
		{
		//Check if the disk was removed from the current drive.
		//If yes, then we need to change the drive to the next drive in
		//the list.
		TUint curDriveIndex = iDriveList->CurrentDriveIndex();
		if(index == curDriveIndex)
			{
			//Find the next drive index to change the drive to.
			TInt nextDriveIndex = curDriveIndex + 1;
			for(; nextDriveIndex < driveCount; ++nextDriveIndex)
				{
				if(EMsvMessageStoreAvailableStatus == (*iDriveList)[nextDriveIndex].status ||
				   EMsvMessageStoreUnavailableStatus == (*iDriveList)[nextDriveIndex].status )
					{
					break;
					}
				}
			
			iStartupState = EMsvMediaUnavailable;

			
			//Change the drive to the above drive and set current drive index.
			User::LeaveIfError(iContext->ChangeDrive(nextDriveIndex, EFalse));
			iDriveList->SetCurrentDriveIndex(nextDriveIndex);
			
			iStartupState = EMsvMediaChanged;
			iIndexDrive = (*iDriveList)[nextDriveIndex].driveNum;
			}
		//If not, we only need to remove the drive from the list.
		else
			{
			iContext->IndexAdapter()->RemoveDriveL((*iDriveList)[index].driveId, index, EFalse);
			
			iStartupState = EMsvNullNotification;
			
			//Send the notification to all registered clients that they need to 
			//refresh their view of messaging.
			NotifyChanged(EMsvRefreshMessageView, KMsvNullIndexEntryId, TInt(aDrive));
			} //if(index == iCurrentDriveIndex)
		} //if(EMsvMessageStoreAvailable == previousDriveStatus)
	
	iStartupState = EMsvNullNotification;
	iContext->IndexAdapter()->SetErrorState(KErrNone);
	}




#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
void CMsvServer::DiskInserted(const TDriveUnit& aDrive, TBool aTestInsert/*DEFAULT = EFalse*/)
#else
void CMsvServer::DiskInserted(const TDriveUnit& aDrive)
#endif
	{
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	TRAPD(err, DoDiskInsertedL(aDrive, aTestInsert));
#else
	TRAPD(err, DoDiskInsertedL(aDrive));
#endif
	
	TRAP_IGNORE(UpdateRepositoryL());
	if(KErrNone != err)
		{
		NotifyChanged(EMsvUnableToProcessDiskNotification, KMsvNullIndexEntryId, TInt(aDrive), 0);	  
		}
	}



#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
void CMsvServer::DoDiskInsertedL(const TDriveUnit& aDrive, TBool aTestInsert)
#else
void CMsvServer::DoDiskInsertedL(const TDriveUnit& aDrive)
#endif
//
// Disk reinserted - Message store available again
//
	{
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	iNotification = EMsvNullNotification;
#endif

	__ASSERT_DEBUG(iContext->IndexAdapter(), PanicServer(EMsvNoIndex));

#ifndef _NO_SERVER_LOGGING_
	Log(_L("Disk %d inserted"), TInt(aDrive));
#endif

	//Find the drive index of this drive
	TInt driveCount = iDriveList->Count();
	TInt index = 0;
	for(index=0; index < driveCount; ++index)
		{
		if((TInt)(*iDriveList)[index].driveNum == (TInt)aDrive)
			{
			break;
			}
		}
	
	//Update the drive's status.
	TDriveState statusDuringUpdate = EMsvInvalidDriveStatus;

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	if(aTestInsert)
		{
		statusDuringUpdate = EMsvMessageStoreCorruptStatus;
		(*iDriveList)[index].status = EMsvMessageStoreUnavailableStatus;
		}
	else
		{
#endif
		iContext->UpdateDriveStatusL(index, statusDuringUpdate);
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		}
#endif
		
	TDriveState driveState = (*iDriveList)[index].status;
	
	//Check if message store was corrupt.
	if(EMsvMessageStoreCorruptStatus == statusDuringUpdate)
		{
		//Send notifications to all registered clients that 
		// media has a corrupt message store.
		NotifyChanged(EMsvMessageStoreCorrupt, KMsvNullIndexEntryId, TInt(aDrive));	 
		}
		
	if(EMsvMessageStoreNotSupportedStatus == driveState)
		{
		// Send notifications to all registered clients that 
		// media has an unsupported version of message store.
		NotifyChanged(EMsvMessageStoreNotSupported, KMsvNullIndexEntryId, TInt(aDrive));		
		}
	
	//Check if this new disk is in a higher priority drive.
	//If yes, then change drive to this higher priority drive.
	TUint curDriveIndex = iDriveList->CurrentDriveIndex();
	if(index < curDriveIndex)
		{	   
		//Check if message store is available on this drive.
		if( EMsvMessageStoreAvailableStatus   == driveState ||
			EMsvMessageStoreUnavailableStatus == driveState
		  )
			{
			TBool isEntryRemovalRequired = EFalse;
			if(EMsvMessageStoreAvailableStatus == (*iDriveList)[index].status)
				{
				isEntryRemovalRequired = ETrue;
				}

			iStartupState = EMsvMediaChanged;
			
			//Change the drive to this drive.
			User::LeaveIfError(iContext->ChangeDrive(index));
			
			iIndexDrive = (*iDriveList)[index].driveNum;
			
			//Set the current drive index as the above drive.
			iDriveList->SetCurrentDriveIndex(index);
			
			// Remove incomplete entries from the new drive.
			if(isEntryRemovalRequired)
				{
				iContext->GetInPreparationIds((*iDriveList)[index].driveId);			
				if (iContext->Remove().Count())
					{
					RemoveEntry(KMsvNullIndexEntryId);
					}
				}
			}
		}
	//If not, then we need to send a notification only if the disk
	//has a valid message store.
	else
		{
		//Check if message store is available on this drive.
		if(EMsvMessageStoreAvailableStatus == driveState)
			{
			//Attach the database in the drive.
			iContext->IndexAdapter()->AddDriveL(index);
			
			iContext->GetInPreparationIds((*iDriveList)[index].driveId);			
			if (iContext->Remove().Count())
				{
				RemoveEntry(KMsvNullIndexEntryId);
				}
					
			//Send the notification to all registered clients that they need to 
			//refresh their view of messaging.
			NotifyChanged(EMsvRefreshMessageView, KMsvNullIndexEntryId, TInt(aDrive));
			}
		}
	
	iStartupState = EMsvNullNotification;
	iContext->IndexAdapter()->SetErrorState(KErrNone);  
	}




void CMsvServer::DiskChanged(const TDriveUnit& aDrive)
	{
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	iNotification = EMsvNullNotification;
#endif

	__ASSERT_DEBUG(iContext->IndexAdapter(), PanicServer(EMsvNoIndex));

#ifndef _NO_SERVER_LOGGING_
	Log(_L("Disk %d changed"), TInt(aDrive));
#endif

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	TRAPD(err, DoDiskRemovedL(aDrive, EFalse));
#else
	TRAPD(err, DoDiskRemovedL(aDrive));
#endif
	TRAP_IGNORE(UpdateRepositoryL());
	if(KErrNone != err)
		{
		// Send change drive notification.
		NotifyChanged(EMsvUnableToProcessDiskNotification, KMsvNullIndexEntryId, TInt(aDrive), 0);
		}
	iContext->IndexAdapter()->SetErrorState(KErrNone);
	}



/*
 * UpdateDriveStatus()
 * @param TDriveUnit& : Drive information.
 * @param TDriveStatus: New status of the drive.
 *
 * The function is called by CMsvCopyStoreOperation and 
 * CMsvDeleteStoreOperation to update the target drive status 
 * after completion of copying/deleting message store.
 */
void CMsvServer::UpdateDriveStatusL(const TDriveUnit& aDrive, TDriveState aStatus)
	{
	TUint driveIndex;
	// If the drive is not present in the preferred drive list.
	if(KErrNotFound == iDriveList->GetDriveIndex(TDriveNumber(TInt(aDrive)), driveIndex))
		{
		return;
		}
	
	// Update the drive status.
	iDriveList->UpdateDriveStatusL(driveIndex, aStatus);		
		
	// Drive should be available for viewing.
	if(EMsvMessageStoreAvailableStatus == aStatus)
		{	   
		TUint curDriveIndex = iDriveList->CurrentDriveIndex();
		
		// Check if the new drive can be the current drive.
		if(driveIndex < curDriveIndex)
			{
			//Change the drive to this drive.
			User::LeaveIfError(iContext->ChangeDrive(driveIndex));
			
			iIndexDrive = (*iDriveList)[driveIndex].driveNum;
			
			//Set the current drive index as the above drive.
			iDriveList->SetCurrentDriveIndex(driveIndex);
			
			// Current drive changed, update the cenrep.
			UpdateRepositoryL();
			}
		else
			{
			iContext->IndexAdapter()->AddDriveL(driveIndex);
			
			//Send the notification to all registered clients that they need to 
			//refresh their view of messaging.
			NotifyChanged(EMsvRefreshMessageView, KMsvNullIndexEntryId, TInt(aDrive));
			}

		// Remove incomplete entries from the new drive.
		iContext->GetInPreparationIds((*iDriveList)[driveIndex].driveId);		   
		if (iContext->Remove().Count())
			{
			RemoveEntry(KMsvNullIndexEntryId);
			}
		}
	}
	
		
	
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
void CMsvServer::ResetRepositoryL()
	{
	CRepository* repository = CRepository::NewL(KUidConfigFile);
	
	// Reset everything in the repository. Default values supplied will be
	// filled in.
	User::LeaveIfError(repository->Reset());
	delete repository;
	}

#endif	  // #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)



#else	   // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)



EXPORT_C void CMsvServer::CreateIndexL(TBool aSync)
//
// Creates the index, either loading it from file or creating a new file/index
//
	{
	__ASSERT_DEBUG(iNewContext == NULL, PanicServer(EMsvNewContextExists));

	TDriveUnit drive(iSystemDrive);

	// Create the new context
	DeleteNewContext();
	iNewContext = CMsvIndexContext::NewL(*this, *this);

	// Get the Message Server configuration
	TInt error = iNewContext->LoadStoreConfig(ETrue);
	iStartupState = EMsvNullNotification;

	if (!error)
		{
		drive = iNewContext->Config().iDrive;
		iIndexDrive = drive;

		// Check the drive is available and has correct Id
		TVolumeInfo volume;
		if (iFs.Volume(volume, TInt(drive)) != KErrNone)
			{
#ifndef _NO_SERVER_LOGGING_
			Log(_L("The media is not available - using default"));
#endif
			// The media is not available - use default
			iStartupState = EMsvMediaUnavailable;
			drive = iSystemDrive;
			}
		iNewContext->CreateIndexL(drive, aSync);
		return;
		}
	else if (error == KErrInUse)
		{
		iIndexDrive = drive;
		iNewContext->CreateIndexL(drive, aSync);
		return;
		}

	User::Leave(error);
	}
	
	

	
EXPORT_C TInt CMsvServer::ChangeDrive(TInt aDrive, TRequestStatus* aStatus)
//
// Reload the Message Store from the specified drive
// If aStatus is null the index is loaded synchronously
// On error the index is unchanged
//
	{
	TRAPD(error, DoChangeDriveL(aDrive, aStatus));
	if (error)
		DeleteNewContext();

	return error;
	}
	
	
	

void CMsvServer::DoChangeDriveL(TInt aDrive, TRequestStatus* aStatus)
	{
	__ASSERT_ALWAYS(iContext->State() == TMsvIndexLoadProgress::EIndexComplete, PanicServer(EMsvLoadingInProgress));
	__ASSERT_ALWAYS(aDrive != iContext->Config().iDrive, User::Leave(KErrNotSupported));
	__ASSERT_DEBUG(iNewContext == NULL, PanicServer(EMsvNewContextExists));

	if(iSearchSortCacheManager)
		{   
		iSearchSortCacheManager->ResetSearchSortCache();
		}
	iContext->IndexAdapter()->GetDbAdapter()->ClearDBContentsL();

	// Delete any existing context
	DeleteNewContext();
	iNewContext = CMsvIndexContext::NewL(*this, *this);
	iNewContext->LoadStoreConfig(ETrue);
	iIndexDrive = aDrive;

	if (aStatus)
		{
#ifndef _NO_SERVER_LOGGING_
		Log(_L("Changing drive asynchronously %d"), aDrive);
#endif
		// Load index asynchronously - will signal observer when complete
		iNewContext->CreateIndexL(aDrive, *aStatus);
		}
	else
		{
#ifndef _NO_SERVER_LOGGING_
		Log(_L("Changing drive synchronously %d"), aDrive);
#endif
		// Load index synchronously
		iNewContext->CreateIndexL(aDrive, ETrue);

		// Delete the context - only relevant if an error occurs
		DeleteNewContext();
		User::LeaveIfError(iLoadError);
		}
	}
	



void CMsvServer::BuildDefaultIniFileL(TMsvConfig& aConfig)
	{
	RFs fileSession;
	User::LeaveIfError(fileSession.Connect());

	TInt error;
	TVolumeInfo volume;
	TUint uniqueId = 0;
	TChar driveChar= fileSession.GetSystemDriveChar();
	
	TBuf<2> systemDrive;
	systemDrive.Append(driveChar);
	systemDrive.Append(KDriveDelimiter);
	TDriveUnit driveUnit(systemDrive);

	error = fileSession.Volume(volume, TInt(driveUnit));
	if (!error)
		{
		uniqueId = volume.iUniqueID;
		}
	else
		{
		User::Leave(error);
		}

	TPath pathName(systemDrive);
	pathName.Append(KServerINIFile);
	CDictionaryFileStore *store=CDictionaryFileStore::OpenLC(fileSession, pathName, KNullUid);

	RDictionaryWriteStream stream;
	stream.AssignLC(*store, KUidMsvMessageDriveStream);

	//Assign the default values
	stream.WriteUint8L(KMsvMessageDriveStreamVersionNumber); // version number
	stream << systemDrive;
	stream.WriteUint32L(uniqueId);
	stream.WriteInt8L(EFalse);

	stream.CommitL();
	store->CommitL();
	CleanupStack::PopAndDestroy(2,store); // stream, store

	// update the values in aConfig
	aConfig.iDrive = driveUnit;
	aConfig.iUniqueID = uniqueId;
	aConfig.iDebug = EFalse;
	return; // there's no need to try to read further
			// values since we just made them up and
			// stored them here to replace the corrupt
			// ini file
	}




void CMsvServer::CurrentConfigL(RFs& aFs, TMsvConfig& aConfig)
	{
	TChar driveChar= aFs.GetSystemDriveChar();
	TBuf<2> systemDrive;
	systemDrive.Append(driveChar);
	systemDrive.Append(KDriveDelimiter);
	
	TDriveUnit driveUnit(systemDrive);  
	aConfig.iDrive = driveUnit;
	aConfig.iUniqueID = 0;
	aConfig.iDebug = EFalse;

	TPath pathName(driveUnit.Name());
	pathName.Append(KServerINIFile);
	// try to open system ini
	CDictionaryFileStore* store = NULL;
	TUint unused;
	TInt err=aFs.Att(pathName, unused);
	if(err==KErrNone)
		{
		TRAPD(openIniErr,store = CDictionaryFileStore::OpenL(aFs, pathName, KNullUid));
		if (openIniErr == KErrCorrupt)
			{
			// The ini file has been corrupted. Delete it and create a new one
			aFs.Delete(pathName);
			RFile newIniFile;
			User::LeaveIfError(newIniFile.Create(aFs, pathName, EFileWrite));
			newIniFile.Close();
			BuildDefaultIniFileL(aConfig);
			return;
			}
		User::LeaveIfError(openIniErr);
		CleanupStack::PushL(store);

		// get configuration from system ini
		if (store->IsPresentL(KUidMsvMessageDriveStream))
			{
			RDictionaryReadStream readStream;
			readStream.OpenLC(*store, KUidMsvMessageDriveStream);
			TInt version = readStream.ReadInt8L();
			// Check the drive stream version number. If invalid,
			// rebuild the ini file
			if (version > KMsvMessageDriveStreamVersionNumber)
				{
				//cleanup the corrupted info
				CleanupStack::PopAndDestroy(2, store);

				//create a new ini file
				RFs fileSession;
				User::LeaveIfError(fileSession.Connect());

				TInt error;
				TVolumeInfo volume;
				TUint uniqueId = 0;
				TDriveUnit driveUnit(systemDrive);
				error = fileSession.Volume(volume, TInt(driveUnit));
				if (!error)
					{
					uniqueId = volume.iUniqueID;
					}
				else
					{
					User::Leave(error);
					}

				CDictionaryFileStore *store=CDictionaryFileStore::OpenLC(fileSession, pathName, KNullUid);
				RDictionaryWriteStream stream;
				stream.AssignLC(*store, KUidMsvMessageDriveStream);

				//Assign the default values
				stream.WriteUint8L(KMsvMessageDriveStreamVersionNumber); // version number
				stream << systemDrive;
				stream.WriteUint32L(uniqueId);
				stream.WriteInt8L(EFalse);

				stream.CommitL();
				store->CommitL();
				CleanupStack::PopAndDestroy(2,store); // stream, store

				// update the values in aConfig
				aConfig.iDrive = driveUnit;
				aConfig.iUniqueID = uniqueId;
				aConfig.iDebug = EFalse;
				return; // there's no need to try to read further
						// values since we just made them up and
						// stored them here to replace the corrupt
						// ini file
				}

			TBuf<2> drive;
			readStream >> drive;
			aConfig.iDrive = drive;

			// The drive Id didn't exist in earlier versions
			if (version > 1)
				aConfig.iUniqueID = readStream.ReadUint32L();

			// This debug frig wasn't in earlier versions
			if (version > 2)
				aConfig.iDebug = readStream.ReadInt8L();

			CleanupStack::PopAndDestroy(); // readStream
			}

		CleanupStack::PopAndDestroy(); // store
		}
	// remember what we loaded, so we don't have to save it if it doesn't change.
	// also means we don't have the defaults as they never change.
	aConfig.iDriveAsLoaded=aConfig.iDrive;
	aConfig.iDebugAsLoaded=aConfig.iDebug;
	aConfig.iUniqueIDAsLoaded=aConfig.iUniqueID;
	}




TBool CMsvServer::DiskRemoved(const TDriveUnit& aDrive)
	{
	__ASSERT_DEBUG(iContext->IndexAdapter(), PanicServer(EMsvNoIndex));
#ifndef _NO_SERVER_LOGGING_
	Log(_L("Disk %d removed"), TInt(aDrive));
#endif
	if(iContext->IndexAdapter()->ErrorState() != KMsvIndexRestore)
		{
		NotifyChanged(EMsvMediaUnavailable, KMsvNullIndexEntryId, TInt(aDrive));
		}
	else
		{
		iReleasedForRestore = ETrue;
		}
	iContext->IndexAdapter()->SetErrorState(KMsvMediaUnavailable);
	iContext->IndexAdapter()->DeleteDbAdapter();

	iStartupState = EMsvMediaUnavailable;
	return ETrue;
	}




TBool CMsvServer::DiskInserted(const TDriveUnit& aDrive)
	{
	__ASSERT_DEBUG(iContext->IndexAdapter(), PanicServer(EMsvNoIndex));

#ifndef _NO_SERVER_LOGGING_
	Log(_L("Disk %d inserted"), TInt(aDrive));
#endif
	if(!iReleasedForRestore)
		{
		NotifyChanged(EMsvMediaAvailable, KMsvNullIndexEntryId, TInt(aDrive));
		}
	else
		{
		iReleasedForRestore = EFalse;
		}

	iContext->IndexAdapter()->SetErrorState(KErrNone);

	//Need to re-open the DB when media is available (EMsvMediaAvailable).
	//Get current drive
	TInt currentDrive = 0;
	TRAPD(err, currentDrive = MessageServer::CurrentDriveL(iFs));
	if (err != KErrNone)
		{
		return EFalse;
		}
	
	//Get system drive
	TChar charDrive = FileSession().GetSystemDriveChar();
	TInt defaultDrive = 0;
	iFs.CharToDrive(charDrive, defaultDrive);
	TChar driveToOpen;
	iFs.DriveToChar(currentDrive, driveToOpen);
		
	if(currentDrive != defaultDrive)
		{
		TBuf<2> systemDrive;
		systemDrive.Append(driveToOpen);
		systemDrive.Append(KDriveDelimiter);
		TPath pathName(systemDrive);
		pathName.Append(KMsvDbFile);

		//Re-open the DB
		TRAPD(error, iContext->IndexAdapter()->OpenclosedL(pathName));
	
		if(error)
			{
			#ifndef _NO_SERVER_LOGGING_
				Log(_L("Unable to re-open DB  "));
			#endif  
			} 
		}

	iStartupState = EMsvNullNotification;
	return ETrue;
	}




TBool CMsvServer::DiskChanged(const TDriveUnit& aDrive, TUint)
	{
	__ASSERT_DEBUG(iContext->IndexAdapter(), PanicServer(EMsvNoIndex));

#ifndef _NO_SERVER_LOGGING_
	Log(_L("Disk %d changed"), TInt(aDrive));
#endif

	NotifyChanged(EMsvMediaIncorrect, KMsvNullIndexEntryId, TInt(aDrive));
	iContext->IndexAdapter()->SetErrorState(KMsvMediaIncorrect);
	iStartupState = EMsvMediaIncorrect;
	return ETrue;
	}



void CMsvServer::ContextComplete(TInt aError, TBool aRunMailInit)
//
// The context has completed successfully or with failure
//
	{
#ifndef _NO_SERVER_LOGGING_
	Log(_L("Context complete with error %d"), aError);
#endif

	if (!aError)
		{
		// Stop disk change notifications
		iNotify->Cancel();

		// Save the new configuration
		iNewContext->LoadStoreConfig(EFalse); // Ignore the error
		TBool startup = iContext == NULL;

		CMsvIndexContext* oldContext = iContext;
		iContext = iNewContext;
		iNewContext = NULL;

#ifndef _NO_SERVER_LOGGING_
		Log(_L("Deleting iBackup"));
#endif
		if(iBackup == NULL)
			{
			// To Gracefully handle and to register with the backup server.
			TRAP_IGNORE(iBackup=CMsvBackupHandler::NewL(*this));
			}

		// Check for presence of MailInit not completed flag
		iFs.SetSessionToPrivate(iContext->Config().iDrive);
		TUint unused;
		TBool initNotCompleted = (iFs.Att(KMsvStoreInitFileName, unused)!=KErrNotFound);
		if ((initNotCompleted || aRunMailInit) && !(iDebug || iContext->Config().iDebug))
			RunMailInitExe();


		// Send index ready notification
		NotifyChanged(EMsvIndexLoaded);

		if (!startup)
			{
			// Send disk change notification
			NotifyChanged(EMsvMediaChanged, KMsvNullIndexEntryId, TInt(oldContext->Config().iDrive), TInt(iContext->Config().iDrive));
			}
		else
			{
			// Send disk change notifications if drive changed
			if (iStartupState != EMsvNullNotification && TInt(iIndexDrive) != TInt(iContext->Config().iDrive))
				{
				NotifyChanged(iStartupState, KMsvNullIndexEntryId, TInt(iIndexDrive));
				NotifyChanged(EMsvMediaChanged, KMsvNullIndexEntryId, TInt(iIndexDrive), TInt(iContext->Config().iDrive));
				}

			}

		// Reset the index error state
		iContext->IndexAdapter()->SetErrorState(KErrNone);
		iStartupState = EMsvNullNotification;
		delete oldContext;
		}

	if (iContext && iContext->Config().iDrive.Name().CompareF(iSystemDrive) != 0 && !iNotify->IsActive())
		{
		// Restart disk change notifications
		iNotify->Start(iContext->Config().iDrive, iContext->Config().iUniqueID);
		}

	// Remember the error
	iLoadError = aError;
	}

#endif		  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)




#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
void CMsvServer::RunMailInitExe(TDriveNumber aDriveNum)
#else
void CMsvServer::RunMailInitExe()
#endif
//
//
//
	{
#ifndef _NO_SERVER_LOGGING_
	Log(_L("Running mailinit"));
#endif
	_LIT8(KMsvStoreInitTestData,"StoreInit");
	RFile storeInitFile;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT) 
	iFs.SetSessionToPrivate(aDriveNum);
#else
	iFs.SetSessionToPrivate(iContext->Config().iDrive);
#endif

	if (storeInitFile.Replace(iFs, KMsvStoreInitFileName, EFileShareAny|EFileWrite)==KErrNone)
		storeInitFile.Write(KMsvStoreInitTestData);
	storeInitFile.Close();
	// NOTE! MailInit.exe MUST delete this file otherwise MailInit will be run each time the
	//	   message server is started.
	// this note is only true for releases before v9.0

	_LIT(KMsvPostInitialisationSearchPattern, "*MAILINIT*");
	_LIT(KMsvPostInitialisationProcessPattern, "*");
	RProcess p;
	TFindProcess finder;
#define CREATE_PROCESS_OR_THREAD() p.Create(KMsvPostInitialisationExe, TPtr(NULL,0), TUidType(KNullUid, KNullUid, KMsvMailInitExeUid))


#ifndef _NO_SERVER_LOGGING_
	Log(_L("Checking for mailinit process/thread"));
#endif

	TFullName fullName;
	TBool found = EFalse;
	finder.Find(KMsvPostInitialisationProcessPattern);
	while (finder.Next(fullName) == KErrNone && !found)
		{
		fullName.UpperCase();
		if (fullName.Match(KMsvPostInitialisationSearchPattern) != KErrNotFound)
			{
#ifndef _NO_SERVER_LOGGING_
			Log(_L("Mailinit (%S) found"), &fullName);
#endif
			found = ETrue;
			break;
			}
		}

	if (!found)
		{
#ifndef _NO_SERVER_LOGGING_
		Log(_L("Mailinit not detected, running"));
#endif
		if (CREATE_PROCESS_OR_THREAD() == KErrNone)
			{
			p.Resume();
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT) 
			iMailinitWaiter->WaitFor(p, aDriveNum);
#else
			iMailinitWaiter->WaitFor(p, (TDriveNumber) (TInt)(iContext->Config().iDrive));
#endif		  

#ifndef _NO_SERVER_LOGGING_
			Log(_L("Mailinit run"));
#endif
			}
		}
	}


CSession2* CMsvServer::DoNewSessionL(const RMessage2 &aMessage)
//
//
//
	{
	if (iCloseServer)
		{
#ifndef _NO_SERVER_LOGGING_
		Log(_L("New session requested whilst waiting to close!"));
#endif
		User::Leave(KErrServerBusy);
		}

	// create the session
	CMsvServerSession* ses = CMsvServerSession::NewL(*this, aMessage);

#ifndef _NO_SERVER_LOGGING_
	Log(_L("New session created for process %S"), &ses->ProcessName());
#endif

#ifdef MSG_SERV_AUTO_CLOSE
	// cancel the close down timer
	iCloseTimer->Cancel();
#endif

	return ses;
	}


CSession2 *CMsvServer::NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const
	{
	// check we're the right version
	TVersion v(KMsvServerMajorVersionNumber,KMsvServerMinorVersionNumber,KMsvServerBuildVersionNumber);
	if (!User::QueryVersionSupported(v,aVersion))
		User::Leave(KErrNotSupported);

	// create the index if not already done
	return ((CMsvServer*)this)->DoNewSessionL(aMessage);
	}

void CMsvServer::CompleteBulkTransaction(void)
	{
	iContext->IndexAdapter()->CommitNonCommitedEntries();
	TRAP_IGNORE(NotifyChangedBulkL());
	}

void CMsvServer::NotifyChangedBulkL()
	{
	// send notifications for the created entries
	DoNotifyChangedBulkL(EMsvEntriesCreated, *iBulkCreationSelection);


	// send notifications for the changed entries
	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection();
	CleanupStack::PushL(selection);
	TInt bulkChangeCount = iBulkChangeSelection->Count();
	selection->SetReserveL(bulkChangeCount);

	// no need to notify entries which have already been notified (by the create notification)
	TMsvId changedId = 0;
	for (TInt ii=0; ii<bulkChangeCount; ++ii)
		{
		changedId = iBulkChangeSelection->At(ii);
		if (iBulkCreationSelection->Find(changedId) == KErrNotFound)
			{
			selection->AppendL(changedId);
			}
		}

	DoNotifyChangedBulkL(EMsvEntriesChanged, *selection);

	iBulkCreationSelection->Reset();
	iBulkChangeSelection->Reset();
	CleanupStack::PopAndDestroy(selection);
	}

void CMsvServer::DoNotifyChangedBulkL(TMsvServerChangeNotificationType aChangeType, const CMsvEntrySelection& aEntriesToNotify)
	{
	TInt entriesToNotifyCount = aEntriesToNotify.Count();
	if( entriesToNotifyCount > 0 )
		{
		//Check whether all entries has the same parent id
		//If not send individual notifications
		CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection();
		CleanupStack::PushL(selection);

		// Reserve the maximum space required - avoid leaves later on.
		selection->SetReserveL(entriesToNotifyCount);

		TMsvId id;
		TMsvId parentId = KMsvNullIndexEntryId;
		TMsvId tempParentId = KMsvNullIndexEntryId;
		TMsvEntry* entryPtr;
		TInt error;
		TBool foundOne(EFalse);

		for (TInt i = 0; i < entriesToNotifyCount; ++i)
			{
			id = aEntriesToNotify.At(i);
			error = IndexAdapter().GetEntry(id, entryPtr);
			if (error == KErrNone)
				{
				tempParentId = entryPtr->Parent();

				if (foundOne)
					{
					// We have found a previously valid entry. If this
					// new entry has a different parent to the previous one,
					// send the notifications now using the old parent and
					// start again with the new parent.
					if (tempParentId != parentId)
						{
						NotifyChanged(aChangeType, *selection, parentId);
						selection->Reset();
						parentId = tempParentId;
						}
					}
				else
					{
					// This is the first valid entry we have found. Store its parent
					// as we need to see if the next entry has the same parent as well
					foundOne = ETrue;
					parentId = tempParentId;
					}

				selection->AppendL(id);
				}
			}

		if (selection->Count() != 0)
			{
			NotifyChanged(aChangeType, *selection, parentId);
			}

		CleanupStack::PopAndDestroy(selection);
		}
	}

void CMsvServer::PoliceReadEntryL(const RMessage2& aMsg, TSecureId aOwnerId, const char* aContextText)
	{
	// The client can read the entry under following circumstances.
	// 1. The client has Read User Data capability.
	// 2. The client does not have Read User Data but owns the entry.

	// First check if the client is trusted with Read User Data.
	if( !aMsg.HasCapability(ECapabilityReadUserData) )
		{
		// Client not trusted with Read User data - check that it owns the entry.
		if( aMsg.SecureId() != aOwnerId )
			{
			// Client missing capabilities - emit diagnostics and leave...
			User::LeaveIfError(PlatSec::CapabilityCheckFail(aMsg, ECapabilityReadUserData, aContextText));
			}
		}
	}

void CMsvServer::PoliceReadEntryL(const RMessage2& aMsg, TMsvId aId, const char* aContextText)
	{
	// The client can read the entry under following circumstances.
	// 1. The client has Read User Data capability.
	// 2. The client does not have Read User Data but owns the entry.

	// First check if the client is trusted with Read User Data.
	if( !aMsg.HasCapability(ECapabilityReadUserData) )
		{
		// Client not trusted with Read User data - check that it owns the entry.
		TMsvEntry* entryPtrNotUsed;
		TSecureId ownerId;
		TInt error = IndexAdapter().GetEntry(aId, entryPtrNotUsed, ownerId);
		if( aMsg.SecureId() != ownerId )
			{
			// Client missing capabilities - emit diagnostics and leave...
			User::LeaveIfError(PlatSec::CapabilityCheckFail(aMsg, ECapabilityReadUserData, aContextText));
			}
		}
	}

void CMsvServer::PoliceCreateEntryL(const RMessage2& aMsg, TMsvEntry aEntry, TBool& aIsLocal, const char* aContextText)
	{
	// Get the parent entry info...
	CMsvIndexAdapter::TMsvServerEntryInfo info;
	User::LeaveIfError(iContext->IndexAdapter()->EntryTreeInfo(aEntry.Parent(), info));
	// Change the MTM and type to that specified in aEntry...
	info.iType  = aEntry.iType;
	info.iMtm   = aEntry.iMtm;

	PoliceCreateModifyEntryL(aMsg, info, ETrue, aIsLocal, aContextText);
	}

void CMsvServer::PoliceModifyEntryL(const RMessage2& aMsg, TMsvEntry aEntry, TBool& aIsLocal, const char* aContextText)
	{
	// Get the entry info...
	CMsvIndexAdapter::TMsvServerEntryInfo info;
	User::LeaveIfError(iContext->IndexAdapter()->EntryTreeInfo(aEntry.Id(), info));
	// Change the MTM and type to that specified in aEntry...
	info.iType  = aEntry.iType;
	info.iMtm   = aEntry.iMtm;

	PoliceCreateModifyEntryL(aMsg, info, EFalse, aIsLocal, aContextText);
	}

void CMsvServer::PoliceModifyEntryL(const RMessage2& aMsg, TMsvId aId, TBool& aIsLocal, const char* aContextText)
	{
	// Get the entry info...
	CMsvIndexAdapter::TMsvServerEntryInfo info;
	User::LeaveIfError(iContext->IndexAdapter()->EntryTreeInfo(aId, info));
	PoliceCreateModifyEntryL(aMsg, info, EFalse, aIsLocal, aContextText);
	}

void CMsvServer::PoliceModifyEntryL(const RMessage2& aMsg, TMsvId aId, const char* aContextText)
	{
	TBool notUsed;
	PoliceModifyEntryL(aMsg, aId, notUsed, aContextText);
	}


void CMsvServer::PoliceCreateModifyEntryL(const RMessage2& aMsg, CMsvIndexAdapter::TMsvServerEntryInfo aEntryInfo, TBool aCreate, TBool& aIsLocal, const char* aContextText)
	{
	TCapabilitySet caps;

	// The entry is local if the service is the local service.
	aIsLocal = aEntryInfo.iService == KMsvLocalServiceIndexEntryId;

	// Check if a service entry or non-service entry is being created.
	if( aEntryInfo.iType != KUidMsvServiceEntry && aEntryInfo.iType != KUidMsvRootEntry )
		{
		// When creating/modifying an under local service, use the entry MTM
		// but under a remote service, use the remote service MTM.
		TUid mtm = aIsLocal ? aEntryInfo.iMtm : aEntryInfo.iServiceMtm;

		// If creating an entry, check against the parent owner ID, but modifying
		// an entry, check against the entry owner ID.
		TSecureId ownerId = aCreate ? aEntryInfo.iParentOwnerId : aEntryInfo.iEntryOwnerId;

		// If creating an entry, the create flag depends on whether the entry
		// is being created under an existing message.
		TBool create = aCreate ? !aEntryInfo.iPartOfMessage : EFalse;

		// Given all the info, go get the required capabilities.
		GetCapsEntryCreateModifyL(aMsg, aEntryInfo.iTopFolder, mtm, ownerId, create, aIsLocal, caps);
		}
	else
		{
		// Client requires write device data to create/modify a service entry.
		caps.SetEmpty();
		caps.AddCapability(ECapabilityWriteDeviceData);
		}
	// Ok, have the required capabilities - check the client capabilities
	TSecurityInfo clientInfo(aMsg);

	if( !clientInfo.iCaps.HasCapabilities(caps) )
		{
		// Client missing capabilities - emit diagnostics and leave...
		caps.Remove(clientInfo.iCaps);
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMsg, caps, aContextText));
		}
	}

void CMsvServer::PoliceMoveEntriesL(const RMessage2& aMsg, const CMsvEntrySelection& aSelection, TMsvId aTarget, TMsvId aSource, TBool& aTargetIsLocal, TBool& aSourceIsLocal, const char* aContextText)
	{
	// To police a move entries request, the client must be able to modify both
	// the target entry and the source entry. In the case of the target entry,
	// the client is essentially creating entries under the target entry.

	// Checking the target is the same as for a copy...

#if (defined SYMBIAN_USER_PROMPT_SERVICE)
	TRAPD(err, PoliceCopyEntriesL(aMsg, aSelection, aTarget, aTargetIsLocal, aContextText));
	if(err != KErrPermissionDenied)
		{
		User::LeaveIfError(err);
		}
#else
	PoliceCopyEntriesL(aMsg, aSelection, aTarget, aTargetIsLocal, aContextText);
#endif
	
	// Get the entry into the source entry...
	CMsvIndexAdapter::TMsvServerEntryInfo sourceInfo;
	User::LeaveIfError(iContext->IndexAdapter()->EntryTreeInfo(aSource, sourceInfo));
	// Police the modification of source entry.
	PoliceCreateModifyEntryL(aMsg, sourceInfo, EFalse, aSourceIsLocal, aContextText);
#if (defined SYMBIAN_USER_PROMPT_SERVICE)
	if(err != KErrNone && aSourceIsLocal && aTargetIsLocal)
		{
		User::LeaveIfError(err);
		}
#endif
	}

void CMsvServer::PoliceCopyEntriesL(const RMessage2& aMsg, const CMsvEntrySelection& aSelection, TMsvId aTarget, TBool& aTargetIsLocal, const char* aContextText)
	{
	// To police a copy entries request, the client must be able to modify the
	// target entry - essentially it is creating entries under the target entry.

	// Get the entry info for target entry...
	CMsvIndexAdapter::TMsvServerEntryInfo targetInfo;
	User::LeaveIfError(iContext->IndexAdapter()->EntryTreeInfo(aTarget, targetInfo));
	// The policing strategy changes slightly if the target is the outbox or a
	// folder under the outbox that is not part of a message.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId topFolderId = UnmaskTMsvId(targetInfo.iTopFolder);
#else
	TMsvId topFolderId = targetInfo.iTopFolder;
#endif		  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	
	if( topFolderId == KMsvGlobalOutBoxIndexEntryId &&
		targetInfo.iType == KUidMsvFolderEntry && !targetInfo.iPartOfMessage )
		{
		aTargetIsLocal = ETrue;
		PoliceCopyMoveUnderOutboxL(aMsg, aSelection, aContextText);
		}
	else
		{
		// Police the modification of target entry. NOTE - check client can
		// create entries under target.
		PoliceCreateModifyEntryL(aMsg, targetInfo, ETrue, aTargetIsLocal, aContextText);
		}
	}

void CMsvServer::PoliceCopyMoveUnderOutboxL(const RMessage2& aMsg, const CMsvEntrySelection& aSelection, const char* aContextText)
	{
	// The target is either the outbox itself or a folder under the outbox
	// that is not part of message. In this case need to police the request
	// against the capabilities specified by the MTM of each entry in the
	// seleciton as well as read/write user data.
	TCapabilitySet caps;
	caps.SetEmpty();

	TInt count = aSelection.Count();
	for( TInt i=0; i<count; ++i )
		{
		TMsvEntry* entryPtr;
		User::LeaveIfError(iContext->IndexAdapter()->GetEntry(aSelection.At(i), entryPtr));
		// Get the capabilities specified by the MTM of the entry and add them
		// to the total required set.
		TCapabilitySet mtmCaps;
		GetCapsForMtmL(entryPtr->iMtm, mtmCaps);
		caps.Union(mtmCaps);
		}
	if( ProtectedFolder(KMsvGlobalOutBoxIndexEntryId) )
		{
		// The outbox is protected - need read/write user data capabilities.
		caps.AddCapability(ECapabilityReadUserData);
		caps.AddCapability(ECapabilityWriteUserData);
		}
	// Ok, have the required capabilities - check the client capabilities
	TSecurityInfo clientInfo(aMsg);

	if( !clientInfo.iCaps.HasCapabilities(caps) )
		{
		// Client missing capabilities - emit diagnostics and leave...
		caps.Remove(clientInfo.iCaps);
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMsg, caps, aContextText));
		}
	}

void CMsvServer::PoliceMtmTransferCommandL(const RMessage2& aMsg, TUid aMtm, const char* aContextText)
	{
	// To access the functionality of a server MTM, a client is required to be
	// trusted with read and write user data as well as the capabilities
	// specified by the server MTM.
	TCapabilitySet caps;
	GetMtmRequiredCapabilitiesL(aMtm, caps);
	caps.AddCapability(ECapabilityReadUserData);
	caps.AddCapability(ECapabilityWriteUserData);

	// Ok, have the required capabilities - check the client capabilities
	TSecurityInfo clientInfo(aMsg);

	if( !clientInfo.iCaps.HasCapabilities(caps) )
		{
		// Client missing capabilities - emit diagnostics and leave...
		caps.Remove(clientInfo.iCaps);
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMsg, caps, aContextText));
		}
	}

void CMsvServer::PoliceStopServiceL(const RMessage2& aMsg, TUid aMtm, const char* aContextText)
	{
	// To stop a service, a client is required to be trusted with the capabilities
	// specified by the server MTM.
	TCapabilitySet caps;
	GetMtmRequiredCapabilitiesL(aMtm, caps);

	// Ok, have the required capabilities - check the client capabilities
	TSecurityInfo clientInfo(aMsg);

	if( !clientInfo.iCaps.HasCapabilities(caps) )
		{
		// Client missing capabilities - emit diagnostics and leave...
		caps.Remove(clientInfo.iCaps);
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMsg, caps, aContextText));
		}
	}

TBool CMsvServer::ProtectedFolder(TMsvId aFolder) const
	{
	TInt index = iProtectedFolders.Count();
	while( index-- > 0 )
		{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		if( UnmaskTMsvId(aFolder) == iProtectedFolders[index] )
#else
		if( aFolder == iProtectedFolders[index] )
#endif
			{
			return ETrue;
			}
		}
	return EFalse;
	}

TBool CMsvServer::ProtectedRemoteServices() const
	{
	return iRemoteServicesProtected;
	}

void CMsvServer::GetCapsEntryCreateModifyL(const RMessage2& aMsg, TMsvId aFolder, TUid aMtm, TSecureId aOwnerId, TBool aCreateEntry, TBool aLocal, TCapabilitySet& aCaps)
	{
	// Checks what capabilities are required to create/modify an entry under
	// the local for a given type of MTM under the specified folder or under a
	// remote service of given type of MTM.
	//
	// 1. Protected	 - read/write user data.
	// 2. Unprotected   - for creating an entry or modifying an entry that is
	// owned by the client, no capabilities are required, but if modifying an
	// entry not owned by the client then read/write user data is required.
	//
	// Additional capabilities required whether entry under local service or a
	// remote service -
	// 1. Local service - if the entry is under the outbox then the capabilities
	// specified by the MTM are required.
	// 2  Remote service - the capabilities specified by the MTM of the remote
	// entry are required.
	aCaps.SetEmpty();

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	if( !aLocal || UnmaskTMsvId(aFolder) == KMsvGlobalOutBoxIndexEntryId )
#else
	if( !aLocal || aFolder == KMsvGlobalOutBoxIndexEntryId )
#endif

		{
		__ASSERT_DEBUG( aMtm != KUidMsvNullEntry, User::Invariant() );
		if( aMtm == KUidMsvNullEntry )
			User::Leave(KErrNotSupported);

		// Entry being created/modified under the outbox or under a remote service
		// - need the capabilities specified by the MTM.
		GetCapsForMtmL(aMtm, aCaps);
		}
	if( !aLocal && ProtectedRemoteServices() ||
		ProtectedFolder(aFolder) || (!aCreateEntry && aMsg.SecureId()!=aOwnerId) )
		{
		// An entry being created/modified under a protected folder or an entry
		// that is not owned by the client and under an unprotected folder is
		// being modified - need read/write user data capabilities.
		//
		// NOTE - the remote servive is considered protected if the Outbox is
		// protected.
		aCaps.AddCapability(ECapabilityReadUserData);
		aCaps.AddCapability(ECapabilityWriteUserData);
		}
	}

void CMsvServer::GetCapsForMtmL(TUid aMtm, TCapabilitySet& aCaps)
	{
	// Entry being created/modified under the outbox or under a remote service
	// - need the capabilities specified by the MTM. Check the following
	// 1. MTM is present - if not then by specify the default capabilties of
	// Network Services and Local Services.
	// 2. MTM is not the local service MTM - query the capabilities from the
	// MTM registry.
	// 3. MTM is the local service MTM - no added capabilities required.
	if( aMtm != KUidMsvLocalServiceMtm )
		{
		if( !iServerMtmReg->IsPresent(aMtm) )
			{
			// The MTM is not present - default to Network Services and Local
			// Services.
			aCaps.SetEmpty();
			aCaps.AddCapability(ECapabilityNetworkServices);
			aCaps.AddCapability(ECapabilityLocalServices);
			}
		else
			{
			// The MTM is present and not the local service MTM - query the MTM
			// registry for the required capabilities.
			GetMtmRequiredCapabilitiesL(aMtm, aCaps);
			}
		}
	}



void CMsvServer::NotifyChanged(TMsvServerChangeNotificationType aChangeType, const CMsvEntrySelection& aSelection, TInt aParameter1, TInt aParameter2)
	{
	TMsvPackedChangeNotification package(iChange);
	TInt finish=0;
	do
		{
		TInt start=finish;
		finish = Min(finish + TMsvPackedChangeNotification::KMsvPackedChangeLimit, aSelection.Count());
		package.Pack(aChangeType, aSelection, aParameter1, aParameter2, start, finish-1);
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		iNotification = aChangeType;
#endif
		DoNotify(aChangeType);
		}
	while (finish < aSelection.Count());
	}


void CMsvServer::NotifyChanged(TMsvServerChangeNotificationType aChangeType, TMsvId aId, TInt aParameter1, TInt aParameter2)
	{
	TMsvPackedChangeNotification package(iChange);
	package.Pack(aChangeType, aId, aParameter1, aParameter2);
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	iNotification = aChangeType;
#endif
	DoNotify(aChangeType);
	}

void CMsvServer::NotifyChanged(TMsvServerChangeNotificationType aChangeType, TUid aMtmTypeUid)
	{
	TMsvPackedChangeNotification package(iChange);
	package.Pack(aChangeType, 0, aMtmTypeUid.iUid, 0);
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	iNotification = aChangeType;
#endif
	DoNotify(aChangeType);
	}

void CMsvServer::QueueNotifyChanged(TMsvServerChangeNotificationType aChangeType)
	{
	TMsvPackedChangeNotification package(iChange);
	package.Pack(aChangeType, 0, 0, 0);
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	iNotification = aChangeType;
#endif
	DoNotify(aChangeType,ETrue);
	}

void CMsvServer::DoNotify(TMsvServerChangeNotificationType aChangeType,TBool aQueue)
//
// pass the information to each session
//
	{
	iSessionIter.SetToFirst();
	CMsvServerSession* ses = (CMsvServerSession*)&(*iSessionIter++);
	while (ses!=NULL)
		{
#ifndef _NO_SERVER_LOGGING_
		Log(_L("Notifying session, process %S"), &ses->ProcessName());
#endif
		// will only leave if the write to the client fails - so no problem ignoring
		// the leave as the client will have been panic'd anyway
		if(ses->ReceiveEntryEvents() ||
			(aChangeType!=EMsvEntriesCreated &&
			 aChangeType!=EMsvEntriesChanged &&
			 aChangeType!=EMsvEntriesDeleted &&
			 aChangeType!=EMsvEntriesMoved))
			{
			TRAP_IGNORE(ses->NotifyChangedL(iChange, aQueue));
			}

		ses = (CMsvServerSession*)&(*(iSessionIter++));
		}
	}

TInt CMsvServer::GetEntryName(TMsvId aId, TDes& aFileName, TBool aFolderRequired)
//
// Construct the entry filename
//
	{
	return GetEntryName(*iContext, aId, aFileName, aFolderRequired);
	}

TInt CMsvServer::GetEntryName(const CMsvIndexContext& aContext, TMsvId aId, TDes& aFileName, TBool aFolderRequired)
//
// Construct the entry filename with the given context
//
	{
	// set up the basic path
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	aContext.MessageFolder(GetDriveId(aId), aFileName);
#else
	aFileName = aContext.MessageFolder();
#endif		  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	TInt error=KErrNone;
	TMsvId service=KMsvRootIndexEntryId;
	if (aId!=KMsvRootIndexEntryId)
		{
		error = aContext.IndexAdapter()->OwningService(aId, service);
		}

	if (error==KErrNone)
		MsvUtils::ConstructEntryName(service, aId, aFileName, (aFolderRequired ? MsvUtils::EFolder : MsvUtils::EStore));
	return error;
	}


TInt CMsvServer::GetAndMakeFileDirectory(TMsvId aId, TDes& aDirectory)
//
// Constructs the name of the directory for files associated with the entry id,
// and makes the directory if it doesn't exist
//
	{
	TInt error = GetEntryName(aId, aDirectory, ETrue);
	if (error==KErrNone)
		error = iFs.MkDirAll(aDirectory);
	return (error==KErrAlreadyExists ? KErrNone : error);
	}

TInt CMsvServer::GetFileDirectoryListing(TMsvId aId, TDes& aDirectory, CDir*& aDir)
//
//
//
	{
	TInt error = GetEntryName(aId, aDirectory, ETrue);
	if (error==KErrNone)
		error =  iFs.GetDir(aDirectory, KEntryAttNormal, ESortNone, aDir);
	return error;
	}

TInt CMsvServer::AddEntry(TMsvEntry& aEntry, TSecureId aOwnerId, TBool aAutoAssignId, TBool aBulk)
	{

#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TDriveUnit defaultDrive(iSystemDrive);

	// Don't attempt to store the message if the Message Store is being copied
	// from the internal disk to the MMC
	if (iStartupState == EMsvMediaUnavailable)
		{
#ifndef _NO_SERVER_LOGGING_
		Log(_L("CMsvServer::AddEntry - Media is unavailable"));
#endif

		TInt currentDrive = 0;
		TRAPD(err, currentDrive = MessageServer::CurrentDriveL(iFs));

		if (err != KErrNone)
			{
			return err;
			}

		if (currentDrive == defaultDrive)
			{
#ifndef _NO_SERVER_LOGGING_
			Log(_L("Msg store Unavailable, sending KErrNotReady response"));
#endif
			return KErrNotReady;
			}
		}

	// If we're trying to add something to the Inbox and the disk containing the
	// Message Store is missing, change drive to internal disk
	TInt err = iContext->IndexAdapter()->ErrorState();
	// Assuming the current drive will always be available.
	if (aEntry.iParentId == KMsvGlobalInBoxIndexEntryId && ( err != KErrNone || iStartupState == EMsvMediaUnavailable ) && !iNewContext)
		{
		if(err !=KMsvIndexRestore &&
			err !=KMsvIndexBackup)
			{
#ifndef _NO_SERVER_LOGGING_
			Log(_L("Automatically changing drive to %d"), TInt(defaultDrive));
#endif
			ChangeDrive(TInt(defaultDrive), NULL); // Ignore error?
			}
		}
#endif	  // #if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	return AddEntry(iContext, aEntry, aOwnerId, aAutoAssignId, aBulk);
	}

TInt CMsvServer::AddEntry(CMsvIndexContext* const& aContext, TMsvEntry& aEntry, TSecureId aOwnerId, TBool aAutoAssignId, TBool aBulk)
//
// Add a new entry to the Message Store
// Uses the given context
// If aBulk is ETrue,
//	 add entry to bulk selection and update count
//	 based on count, decide whether to commit entry and generate notification
//
	{
	// Return if the message store is not currently available
	TInt err = KErrNone;
	err = aContext->IndexAdapter()->ErrorState();
	if (err != KErrNone)
		return err;

	// get a unique id for the entry
	if (aAutoAssignId)
		{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		aEntry.SetId(aContext->IndexAdapter()->NextId(GetDriveId(aEntry.Parent())));
#else
		aEntry.SetId(aContext->IndexAdapter()->NextId());
#endif
		}

	TInt error = KErrNone;

	if (aBulk)
		{
		TRAP(error, iBulkCreationSelection->AppendL(aEntry.Id()));
		}

#ifndef _NO_SERVER_LOGGING_
	Log(_L("Adding entry with id %x"), aEntry.Id());
#endif

	TFileName fileName;

	// create the path for a service
	if (aEntry.iType==KUidMsvServiceEntry)
		{
		fileName = aContext->MessageFolder();
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		MsvUtils::ConstructEntryName(UnmaskTMsvId(aEntry.Id()), aEntry.Id(), fileName, MsvUtils::EPath);
#else
		MsvUtils::ConstructEntryName(aEntry.Id(), aEntry.Id(), fileName, MsvUtils::EPath);
#endif

#ifndef _NO_SERVER_LOGGING_
		Log(_L("Creating path for service %S"), &fileName);
#endif
		error = iFs.MkDir(fileName);
		}

	// add the entry to the index
	if (error==KErrNone || error==KErrAlreadyExists)
		{
		if (!aBulk)
			{
			error = aContext->IndexAdapter()->AddEntry(aEntry, aOwnerId, EFalse);
			// We may also have a bulk procedure occuring at this time.
			// This call to AddEntry will commit those entries to the index file,
			// so we should let any observers know about the bulk entries now.
			// With the parallel message processing, we will have to forego some of
			// the efficiencies of bulk processing.
			if ((iBulkCreationSelection->Count() != 0) || (iBulkChangeSelection->Count() != 0))
				{
				TRAP(error, NotifyChangedBulkL());
				}
			}
		else
			{
			if (iBulkCreationSelection->Count() == KBulkCommitInterval)
				{
				error = aContext->IndexAdapter()->AddEntry(aEntry, aOwnerId, EFalse);
				// We have done a bulk commit to the index file.
				// Let observers know about the entries
				TRAP(error, NotifyChangedBulkL());
				}
			else
				{
				error = aContext->IndexAdapter()->AddEntryNoCommit(aEntry, aOwnerId, EFalse);
				}
			}
		if (error==KErrNone)
			{
			}
		// Delete the service folder if we created one
		else if ((fileName.Length() > 0) && error!=KErrAlreadyExists)
			iFs.RmDir(fileName);
		}

	return error;
	}


TInt CMsvServer::ChangeEntry(const TMsvEntry& aEntry, TSecureId aOwnerId, TBool aForcedUpdate, TBool aBulk)
//
//
//
	{
#ifndef _NO_SERVER_LOGGING_
	Log(_L("Changing entry with id %x"), aEntry.Id());
#endif

	TInt error;
	if (!aBulk)
		{
		error = iContext->IndexAdapter()->ChangeEntry(aEntry, aOwnerId, aForcedUpdate);
		// We may also have a bulk procedure occuring at this time.
		// This call to ChangeEntry will commit those entries to the index file,
		// so we should let any observers know about the bulk entries now.
		if ((iBulkCreationSelection->Count() != 0) || (iBulkChangeSelection->Count() != 0))
			{
			TRAP(error, NotifyChangedBulkL());
			}
		}
	else
		{
		TRAP(error, iBulkChangeSelection->AppendL(aEntry.Id()));
		if (error == KErrNone)
			{
			if (iBulkChangeSelection->Count() == KBulkCommitInterval)
				{
				error = iContext->IndexAdapter()->ChangeEntry(aEntry, aOwnerId, aForcedUpdate);
				// We have done a bulk commit to the index file. Let observers know about the entries.
				TRAP(error, NotifyChangedBulkL());
				}
			else
				{
				error = iContext->IndexAdapter()->ChangeEntryNoCommit(aEntry, aOwnerId, aForcedUpdate);
				}
			}
		}

	return error;
	}



TInt CMsvServer::CheckMtmStatus(TMsvId aServiceId, TBool& aRunning)
//
// Returns
//  KErrNotFound if the mtm is not loaded
//  >=0 if the mtm is loaded and this is the position in the queue array
//  aRunning is true if an operation is currently running
//
	{
	aRunning = EFalse;
	TInt count=iMtmOperationQueueArray.Count();
	while (count--)
		{
		// check if the service has a loaded mtm
		if (iMtmOperationQueueArray.At(count)->ServiceId() == aServiceId)
			{
			aRunning = iMtmOperationQueueArray.At(count)->Count()!=0;
			return count;
			}
		}
	return KErrNotFound;
	}

#ifdef MSG_SERV_AUTO_CLOSE
void CMsvServer::CheckAndClose()
	{
	// check if any session remaining are just observers - then shut down

	iSessionIter.SetToFirst();

	if(iBackup && iBackup->Locked()!=EFalse)
		{
#ifndef _NO_SERVER_LOGGING_
		Log(_L("Not closing sever because we have a backup lock to retake."));
#endif
		}
	else if(iMailinitWaiter->IsActive())
		{
#ifndef _NO_SERVER_LOGGING_
		Log(_L("Not closing sever because we are waiting for mailinit to finish"));
#endif  //_NO_SERVER_LOGGING_
		}
	else if (iSessionIter == NULL)
		{

		if (iCloseServer)
			{
			CActiveScheduler::Stop();
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			MailinitFinished(KErrNone, iDriveList->CurrentDriveNumber());
#else
			MailinitFinished(KErrNone, (TDriveNumber) (TInt)(iContext->Config().iDrive));
#endif
			}
		else
			{
			iCloseTimer->Cancel();
			iCloseTimer->After(KMsvCloseTime);
			}
		}
	else if (!iCloseServer)
		{
		TBool shouldClose=ETrue;

		iSessionIter.SetToFirst();
		CMsvServerSession* session = (CMsvServerSession*)&(*iSessionIter++);
		while(session)
			{
			if (!session->IsAnObserver())
				{
				shouldClose=EFalse;
				break;
				}
			session = (CMsvServerSession*)&(*iSessionIter++);
			}

		if (shouldClose)
			{
#ifndef _NO_SERVER_LOGGING_
			Log(_L("Telling clients to close - CheckAndClose"));
#endif
			NotifyChanged(EMsvCloseSession, KMsvNullIndexEntryId);
			iCloseServer=ETrue;
			}
		}
	}
#endif	  // MSG_SERV_AUTO_CLOSE


void CMsvServer::ClosingSession(TInt aSessionId)
//
// Ensures that no mtm are left running when a session is closed
// NOTE Cannot leave - called by CMsvServerSession d'tor
//
	{
	TInt ii = iMtmOperationQueueArray.Count();
	// check each queue
	while (ii--)
		{
		// check if the session used this queue
		CMsvMtmOperationQueue* queue = iMtmOperationQueueArray.At(ii);
		TInt jj = queue->iSessionIdArray.Count();
		while (jj--)
			{
			if (queue != NULL && queue->iSessionIdArray.At(jj)==aSessionId)
				{
				// the session used this queue - so remove the reference to it
				queue->iSessionIdArray.Delete(jj);

#if defined(_DEBUG)
				// check all session operations have been removed
				TInt kk = queue->Count();
				while (kk--)
					__ASSERT_DEBUG(queue->At(kk)->SessionId()!=aSessionId, PanicServer(EMsvOutstandingOperation));
#endif
				// check if anyone else is using the queue
				if (queue->iSessionIdArray.Count()==0)
					{
					// remove the mtm and queue
					delete queue;
					queue = NULL;
					iMtmOperationQueueArray.Delete(ii);
					}
				}
			}
		}

	iSessionIter.SetToFirst();
	iSessionIter++;

	if (iSessionIter == NULL)
		{
		if (iCloseServer)
			{
			CActiveScheduler::Stop();
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			MailinitFinished(KErrNone, iDriveList->CurrentDriveNumber());
#else
			MailinitFinished(KErrNone, (TDriveNumber) (TInt)(iContext->Config().iDrive));
#endif
			}

#ifdef MSG_SERV_AUTO_CLOSE
		else
			{
			iCloseTimer->Cancel();
			iCloseTimer->After(KMsvCloseTime);
			}

		}
	else if (!iCloseServer)
		{
		// check if any session remaining are just observers - then shut down
		TBool shouldClose=ETrue;

		iSessionIter.SetToFirst();
		CMsvServerSession* session = (CMsvServerSession*)&(*iSessionIter++);
		while(session)
			{
			if (!session->IsAnObserver() && session->SessionId() != aSessionId)
				{
				shouldClose=EFalse;
				break;
				}
			session = (CMsvServerSession*)&(*iSessionIter++);
			}

		if (shouldClose)
			{
#ifndef _NO_SERVER_LOGGING_
			Log(_L("Telling clients to close - Closing session"));
#endif
			NotifyChanged(EMsvCloseSession, KMsvNullIndexEntryId);
			iCloseServer=ETrue;
			}
#endif

		}

	}


TBool CMsvServer::SessionIdInQueue(CMsvMtmOperationQueue& aQueue, TInt aSessionId) const
//
// Returns true if the session id is already in the queue session id array
//
	{
	TInt count=aQueue.iSessionIdArray.Count();
	while (count--)
		{
		if (aQueue.iSessionIdArray.At(count)==aSessionId)
			return ETrue;
		}
	return EFalse;
	}

void CMsvServer::QueueOperationL(CMsvMtmOperation& aOperation, TInt aSessionId)
//
// Appends an operation to the correct mtm queue
//
	{
	// append the operation on the right queue
	TInt count=iMtmOperationQueueArray.Count();
	while (count--)
		{
		if (iMtmOperationQueueArray.At(count)->ServiceId() == aOperation.ServiceId())
			{
			AddSessionIdToQueueL(*iMtmOperationQueueArray.At(count), aSessionId);
			iMtmOperationQueueArray.At(count)->AppendL(&aOperation);
			aOperation.SetState(EMsvOperationQueued);

			}
		}
	__ASSERT_DEBUG(aOperation.State() == EMsvOperationQueued, PanicServer(EMsvOperationQueueNotFound1));
	}


void CMsvServer::StartOperationL(CMsvMtmOperation& aOperation, TInt aSessionId, const RMessage2 &aMessage, TBool hasCapability)
//
// Either starts an operation or queues it for later execution
//
	{
	// check the mtm exists
	if (!iServerMtmReg->IsPresent(aOperation.MtmUid()))
		User::Leave(KErrNotFound);

	// get the status of the service mtm
	TBool opRunning;
	TInt index = CheckMtmStatus(aOperation.ServiceId(), opRunning);

	// if an operation is already running, queue this one
	if (opRunning)
		{
#if (defined SYMBIAN_USER_PROMPT_SERVICE)        
		aOperation.SetCapability(hasCapability);
#endif
		QueueOperationL(aOperation, aSessionId);
		return;
		}

	// create the new mtm if required
	CMsvMtmOperationQueue* queue;
	if (index==KErrNotFound)
		{
		queue = new(ELeave) CMsvMtmOperationQueue(aOperation.MtmUid(), aOperation.ServiceId());
		CleanupStack::PushL(queue);

		// load the mtm and assign to the queue
		queue->iMtm = LoadMtmL(aOperation);

		// append the operation to the queue
		queue->AppendL(&aOperation);

		iMtmOperationQueueArray.AppendL(queue);
		CleanupStack::Pop(); // queue
		}
	else
		{
		queue = iMtmOperationQueueArray.At(index);
		queue->AppendL(&aOperation);
		}
	AddSessionIdToQueueL(*queue, aSessionId);
#if (defined SYMBIAN_USER_PROMPT_SERVICE)   
	RThread thread;
	User::LeaveIfError(aMessage.Client(thread));
	TThreadId threadId = thread.Id();
	aOperation.SetThreadId(threadId);
	thread.Close();
#else
	if(aMessage.IsNull()){} // To avoid compiler warnings
#endif
	// start the operation
	aOperation.Start(*queue->iMtm, hasCapability);
	aOperation.SetState(EMsvOperationRunning);
	}



CBaseServerMtm* CMsvServer::LoadMtmL(const CMsvMtmOperation& aOperation)
//
// Returns the server mtm instance required for the operation
//
	{
	CBaseServerMtm* mtm = iServerMtmReg->NewServerMtmL(aOperation.MtmUid(), CMsvServerEntry::NewL(*this, aOperation.ServiceId()));
	return mtm;
	}



#if defined(_DEBUG)
void CMsvServer::OperationCompleted(TMsvId aServiceId, TMsvOp aOpId)
#else
void CMsvServer::OperationCompleted(TMsvId aServiceId, TMsvOp /*aOpId*/)
#endif
//
// An operation has completed, so start the next operation if there is one or remove the Mtm
//
	{
	// find the correct operation queue
	CMsvMtmOperationQueue* queue=NULL;
	TInt count=iMtmOperationQueueArray.Count();
	while (count--)
		{
		if (iMtmOperationQueueArray.At(count)->ServiceId() == aServiceId)
			{
			queue = iMtmOperationQueueArray.At(count);
			break;
			}
		}
	__ASSERT_DEBUG(count>=0, PanicServer(EMsvOperationQueueNotFound2));

	if ( queue != NULL )
		{
		__ASSERT_DEBUG(queue->At(0)->Id()==aOpId, PanicServer(EMsvWrongOperationCompletion));

		// mark the operation as completed and remove from the queue
		queue->At(0)->SetState(EMsvOperationCompleted);
		queue->Delete(0);

		// start the next operation
		StartNextOperation(queue, count);
		}
	}

TInt CMsvServer::OutstandingOperations()
	{
	TInt count = 0;
	iSessionIter.SetToFirst();
	CMsvServerSession* ses = (CMsvServerSession*)&(*iSessionIter++);
	while (ses!=NULL)
		{
		count += ses->HaveOutstandingOperations();
		ses = (CMsvServerSession*)&(*(iSessionIter++));
		}
	return count;
	}

void CMsvServer::OperationCancelled(const CMsvMtmOperation& aOperation)
//
// An operation has cancelled
//
	{
	const TMsvId aServiceId = aOperation.ServiceId();

	// find the correct operation queue
	CMsvMtmOperationQueue* queue=NULL;
	TInt qCount=iMtmOperationQueueArray.Count();
	while (qCount--)
		{
		if (iMtmOperationQueueArray.At(qCount)->ServiceId() == aServiceId)
			{
			queue = iMtmOperationQueueArray.At(qCount);
			break;
			}
		}
	__ASSERT_DEBUG(qCount>=0, PanicServer(EMsvOperationQueueNotFound3));

	// find the correct operation
	if (queue != NULL)
		{
		TInt oCount=queue->Count();
		while (oCount--)
			{
			if (queue->At(oCount) == &aOperation)
				break;
			}
		__ASSERT_DEBUG(oCount>=0, PanicServer(EMsvOperationNotFound));
		__ASSERT_DEBUG(queue->At(oCount)->State()==EMsvOperationQueued, PanicServer(EMsvCancelledNonQueueOp));
		if (oCount>=0)
			queue->Delete(oCount);
		}
	}



void CMsvServer::StartNextOperation(CMsvMtmOperationQueue* aQueue, TInt aQueueIndex)
//
//
//
	{
	// if there are no more operations
	if (aQueue->Count()==0)
		{
		if (!aQueue->iMtm->CommandExpected())
			{
			// the mtm is no longer needed
			delete iMtmOperationQueueArray.At(aQueueIndex);
			iMtmOperationQueueArray.Delete(aQueueIndex);
			}
		return;
		}


	// if the Mtm cannot accept multiple commands, create a new one
	if (!aQueue->iMtm->CommandExpected())
		{
		delete aQueue->iMtm;
		aQueue->iMtm=NULL;
		TRAPD(leave, {aQueue->iMtm = LoadMtmL(*aQueue->At(0));});
		if (leave)
			{
			// fail all queued operations
			TInt count=aQueue->Count();
			while (count--)
				{
				aQueue->At(count)->Failed(leave);
				aQueue->Delete(count);
				}
			// delete the mtm queue
			delete iMtmOperationQueueArray.At(aQueueIndex);
			iMtmOperationQueueArray.Delete(aQueueIndex);
			return;
			}
		}
	
#if (defined SYMBIAN_USER_PROMPT_SERVICE)
	aQueue->At(0)->Start(*aQueue->iMtm, aQueue->At(0)->Capability());
#else
	aQueue->At(0)->Start(*aQueue->iMtm, EFalse);
#endif
	aQueue->At(0)->SetState(EMsvOperationRunning);
	}


TInt CMsvServer::FillRegisteredMtmDllArray(TUid aMtmDllTypeUid,CRegisteredMtmDllArray& aRegisteredMtmDllArray)
	{
	return iMtmRegControl->FillRegisteredMtmDllArray(aMtmDllTypeUid,aRegisteredMtmDllArray);
	}


TInt CMsvServer::InstallMtmGroup(const TDesC& aFullName,TUid& aMtmTypeUid)
	{
	TInt error=iMtmRegControl->InstallMtmGroup(aFullName,aMtmTypeUid);
	return error;
	}


TInt CMsvServer::DeInstallMtmGroup(const TDesC& aFullName,TUid& aMtmTypeUid)
	{
	TInt error=iMtmRegControl->FullNameToMtmTypeUid(aFullName,aMtmTypeUid);
	if (error==KErrNone)
		{
		TBool isinuse=iMtmRegControl->IsInUse(aMtmTypeUid);
		if (!isinuse)
			{
			iSessionIter.SetToFirst();
			CMsvServerSession* ses = (CMsvServerSession*)&(*iSessionIter++);
			while ((ses!=NULL) && !isinuse)
				{
				isinuse=ses->IsInUse(aMtmTypeUid);
				ses = (CMsvServerSession*)&(*(iSessionIter++));
				}
			}
		if (isinuse)
			error=KErrInUse;
		}
	if (error==KErrNone)
		error=iMtmRegControl->DeInstallMtmGroup(aMtmTypeUid);
	return error;
	}


CMtmGroupData* CMsvServer::GetMtmGroupDataL(TUid aMtmTypeUid) const
	{
	return iMtmRegControl->GetMtmGroupDataL(aMtmTypeUid);
	}


void CMsvServer::GetMtmRequiredCapabilitiesL(TUid aMtmTypeUid, TCapabilitySet& aCaps) const
	{
	aCaps=iMtmRegControl->GetMtmGroupDataReferenceL(aMtmTypeUid).GetMtmRequiredCapabilities();
	}


void CMsvServer::StopServiceL(const RMessage2& aMessage)
//
// Stops a service by cancelling all operations and deleting the server MTM
//
	{
	TMsvId serviceId = aMessage.Int0();
	TBool active;
	TInt index = CheckMtmStatus(serviceId, active);

	// service MTM is not loaded
	if (index==KErrNotFound)
		{
		aMessage.Complete(KErrNotFound);
		return;
		}

	// The service MTM is loaded - police if the client can stop it.
	PoliceStopServiceL(aMessage, iMtmOperationQueueArray.At(index)->MtmUid(), __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServer::StopService"));

	// get all sessions to cancel/stop the operations
	iSessionIter.SetToFirst();
	CMsvServerSession* ses = (CMsvServerSession*)&(*iSessionIter++);
	while (ses!=NULL)
		{
		ses->StopOperations(serviceId);
		ses = (CMsvServerSession*)&(*(iSessionIter++));
		}

	// check if the server MTM has been persisted
	index = CheckMtmStatus(serviceId, active);
	// remove the queue - irrespective of request of the Server MTM
	if (index!=KErrNotFound)
		{
		delete iMtmOperationQueueArray.At(index);
		iMtmOperationQueueArray.Delete(index);
		}
	aMessage.Complete(KErrNone);
	}



void CMsvServer::ServiceActive(const RMessage2& aMessage)
//
// Completes ETrue if the server MTM is running an operation
//
	{
	TMsvId serviceId = aMessage.Int0();
	TBool active;
	CheckMtmStatus(serviceId, active); // error ignored as "active" is always set correctly
	aMessage.Complete(active);
	}



const TDesC8& CMsvServer::ServiceProgressL(TMsvId aServcieId)
//
// Returns the progress from the server MTM
//
	{
	TBool active;
	TInt index = CheckMtmStatus(aServcieId, active);

	// service MTM is not loaded
	if (index==KErrNotFound)
		User::Leave(KErrNotFound);

	// get the progress from the server MTM
	CMsvMtmOperationQueue* queue = iMtmOperationQueueArray.At(index);
	return queue->iMtm->Progress();
	}


void CMsvServer::AttemptDelayedUpdating()
//
//
//
	{
	if (iContext)
		{
		if(iContext->Initialized()==EFalse)
			{
			iContext->Initialized(ETrue);
			TRAP_IGNORE({iContext->LocalizeStandardFoldersL();
							SuspendSendingMessagesL();});
			}

		if (iContext->Remove().Count())
			RemoveEntry(KMsvNullIndexEntryId);

		if (iContext->Update().Count() && !iDelayTimer->IsActive())
			iDelayTimer->After(KMsvDelayTime);
		}
	}




// this cleans up messages left sending when the machine was
// rebooted, as they would otherwise be displayed to the user
// as sending when they aren't
// A better fix would be to allow the server mtms to clean up
// on startup, then they could attempt to continue sending the
// message.
// This also only copes with mtms that either use the KMsvSendStateSending and KMsvSendStateWaiting
// or the fax mtm because I know about those.

// copied from fxut.h included so that we can set things with fax sending states
// to suspended on message server start
// horrible but we don't want to have to have fax available to build the message
// server.
enum TFaxSendingState
	{
	EFaxSendingStateCalling = (KMsvSendStateLast + 1),
		EFaxSendingStateNegotiating,
		EFaxSendingStatePreparing
	};
// can't find a header that defines this (it is defined in FXSENDOP.CPP
// and FAXC.CPP but I don't want to move it into a header now.
const TUid KUidMsgTypeFax = {0x1000102B};

void CMsvServer::SuspendSendingMessagesL()
	{
	CMsvEntrySelection *selection=new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	CMsvEntryFilter *filter=CMsvEntryFilter::NewLC();
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		User::LeaveIfError(iContext->IndexAdapter()->GetChildrenIdAll(KMsvGlobalOutBoxIndexEntryId,*filter,*selection));
#else
		User::LeaveIfError(iContext->IndexAdapter()->GetChildrenId(KMsvGlobalOutBoxIndexEntryId,*filter,*selection));
#endif

	TInt count=selection->Count();
	while(count--)
		{
		TMsvEntry *entry;
		TInt err = KErrNone;
		err = iContext->IndexAdapter()->GetEntry(selection->At(count),entry);
		if(err == KErrNone)
			{
			const TUint sendingState=entry->SendingState();

			if(sendingState==KMsvSendStateSending ||
			   sendingState==KMsvSendStateWaiting ||
				(entry->iMtm==KUidMsgTypeFax && (sendingState== EFaxSendingStateCalling ||
												 sendingState== EFaxSendingStateNegotiating ||
												 sendingState== EFaxSendingStatePreparing)))
				{
				entry->SetSendingState(KMsvSendStateSuspended);
				// ignore error as we can't report it and want to try fixing
				// the next entry in the list.
				iContext->IndexAdapter()->ChangeEntry(*entry, KMsvServerId, EFalse);
				}
			}
		}
	CleanupStack::PopAndDestroy(2,selection); //filter, selection
	}

void CMsvServer::RemoveEntry(TMsvId aId)
//
// Removes the removed entries from the index. Client were not interested in result.
//
	{
#ifndef _NO_SERVER_LOGGING_
	TRAPD(leave, DoRemoveEntriesL(aId));
	Log(_L("Remove entry %x, error %d, list size %d"), aId, leave, iContext->Remove().Count());
#else
	TRAP_IGNORE(DoRemoveEntriesL(aId));
#endif

	// start the timer if entries are stil requiring removal and the timer isn't running
	if (iContext->Remove().Count() && !iDelayTimer->IsActive())
		iDelayTimer->After(KMsvDelayTime);
	}

void CMsvServer::DoRemoveEntriesL(TMsvId aId)
//
// Deletes the removed entries from the index. Client were not interested in result.
//
	{
	if (aId!=KMsvNullIndexEntryId)
		{
		iContext->Remove().AppendL(aId);
		iContext->Remove().SetReserveL(iContext->Remove().Count()+1);
		}
	else if (iContext->Remove().Count()==0)
		return;

	CMsvEntrySelection* deletedSelection = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(deletedSelection);
	CMsvEntrySelection* movedSelection = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(movedSelection);
	CMsvDelete* deleteEntry = CMsvDelete::NewL(*this);
	CleanupStack::PushL(deleteEntry);

	TInt count=iContext->Remove().Count();
	while (count--)
		{
		// find the parent
		TMsvEntry* entry;
		TInt error = KErrNone;
		error = iContext->IndexAdapter()->GetEntry(iContext->Remove().At(count), entry);
		if (error)
			continue;

		// delete the entry
		TMsvId parent = entry->Parent();

		// Note: It should be possible to get rid of this trap - but this is safer for now
		TRAP(error, deleteEntry->StartL(entry->Id(), *deletedSelection, *movedSelection));
		if (error == KErrNone)
			{
			// notify client of a deletion
			iContext->Remove().Delete(count);
			if (deletedSelection->Count())
				NotifyChanged(EMsvEntriesDeleted, *deletedSelection, parent);
			if (movedSelection->Count())
				{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				NotifyChanged(EMsvEntriesMoved, *movedSelection, MaskTMsvId(GetDriveId(aId), KMsvDeletedEntryFolderEntryId), parent);
#else
				NotifyChanged(EMsvEntriesMoved, *movedSelection, KMsvDeletedEntryFolderEntryId, parent);
#endif
				}
			}

		deletedSelection->Reset();
		movedSelection->Reset();
		}

	CleanupStack::PopAndDestroy(3); // moveSelection, deletedSelection, deleteentry
	}


void CMsvServer::CloseServer(const RMessage2& aMessage)
//
// Tells alls sessions to close, and when all are closed the server is shut down immediately
//
	{
#ifndef _NO_SERVER_LOGGING_
	Log(_L("Telling clients to close - close server function"));
#endif
	aMessage.Complete(KErrNone);
	NotifyChanged(EMsvCloseSession, KMsvNullIndexEntryId);
	iCloseServer=ETrue;
	}


TInt CMsvServer::CheckEntries(const CMsvEntrySelection& aEntries, TInt aStartIndex, TInt aEndIndex)
//
// Checks that all entries in the selection can be deleted, copied, moved etc
//
	{
	for (TInt index=aStartIndex; index<=aEndIndex; index++)
		{
		TMsvId id = aEntries.At(index);
		TBool locked;
		TInt error = KErrNone;
		error = iContext->IndexAdapter()->IsEntryOrStoreLocked(id, locked);
		if (error)
			return error;
		if (locked)
			return KErrAccessDenied;

		// get the entry
		TMsvEntry* entry;
		iContext->IndexAdapter()->GetEntry(id, entry); // error ignored, as will not fail
		// check the store
		TFileName filename;
		GetEntryName(id, filename, EFalse);
		TBool open;
		error = iFs.IsFileOpen(filename, open);
		if (error)
			{
			if (error!=KErrNotFound && error!=KErrPathNotFound)
				return error;
			}
		else if (open)
			return KErrInUse;

		// check any files
		CDir* dir;
		error = GetFileDirectoryListing(id, filename, dir);
		if (error == KErrNone)
			{
			error = iFs.SetSessionPath(filename);
			if (error==KErrNone)
				{
				TInt fCount = dir->Count();
				while (fCount--)
					{
					TBool open;
					error = iFs.IsFileOpen((*dir)[fCount].iName, open);
					if (error)
						{
						if (error!=KErrNotFound && error!=KErrPathNotFound)
							{
							delete dir;
							return error;
							}
						}
					else if (open)
						{
						delete dir;
						return KErrInUse;
						}
					}
				}
			delete dir;
			if (error)
				return error;
			}
		else if (error != KErrPathNotFound)
			return error;
		}
	return KErrNone;
	}


#ifndef _NO_SERVER_LOGGING_
void CMsvServer::CreateLogL()
	{
	// Connect to flogger
	if (iLog.Connect() == KErrNone)
		{
		iLog.CreateLog(_L("msgs"), _L("server.txt"), EFileLoggingModeOverwrite);
		iLog.SetDateAndTime(EFalse, ETrue);

		TTime date;
		date.UniversalTime();

		TBuf<32> dateString;
		date.FormatL(dateString,(_L("%D%M%Y%/0%1%/1%2%/2%3%/3")));

		TPtrC ver(MessageServer::Version().Name());
		Log(_L("Started message server, date %S, version %S"), &dateString, &ver);
		}
	}

void CMsvServer::Log(TRefByValue<const TDesC> aFmt, ...)
	{
	if (!iLog.LogValid())
		return;

	VA_LIST list;
	VA_START(list, aFmt);

	TBuf<256> buf;
	buf.FormatList(aFmt, list);

	// Write to log
	iLog.Write(buf);

#ifndef _NO_SERVER_LOGGING_SERIAL_
	// Write to serial
	_LIT(KFormatSerial, "MSGS: %S");
	RDebug::Print(KFormatSerial, &buf);
#endif
	}
#endif	  // #ifndef _NO_SERVER_LOGGING_



void CMsvServer::SetFailure(TMsvFailure aType, TInt aArg1, TInt aArg2, TInt)
	{
	switch (aType)
		{
		case EHeapFailure:
			User::__DbgSetAllocFail(RHeap::EUser, (RHeap::TAllocFail)aArg1, aArg2);
			break;
		case EDiskFailure:
#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			iNotify->SetDiskMissing(aArg1);
			iNotify->SetWrongId(aArg2);
#endif
			break;
		}
	}




void CMsvServer::SetStartupState(TMsvServerChangeNotificationType aState)
	{
	iStartupState=aState;
	}

const TDriveUnit &CMsvServer::Drive() const
	{
	return(iIndexDrive);
	}



void CMsvServer::MailinitFinished(TInt aError, TDriveNumber aDriveNum)
	{
	// if mailinit exited sucessfully try and delete the
	// flag file.
	if(aError==KErrNone)
		{
		// remove the mailinit flag file, ignore errors it will just mean
		// we run mailinit next time the server starts and we require that
		// mailinit.exe handles this gracefully.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		if(iFs.SetSessionToPrivate(aDriveNum) == KErrNone)
#else
		// Just to ignore the warning.
		aDriveNum = aDriveNum;
		if(iFs.SetSessionToPrivate(iContext->Config().iDrive) == KErrNone)
#endif	  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

			{
			iFs.Delete(KMsvStoreInitFileName);
			}
		}
#ifdef MSG_SERV_AUTO_CLOSE
	// we may have delayed shutdown to allow mailinit to finish, so
	// let the server shutdown if it wants to.
	CheckAndClose();
#endif
	}


TMsvServerStoreManager& CMsvServer::ServerStoreManager()
	{
	return iServerStoreManager;
	}

void CMsvServer::GetNonOperationMtmDataL(TMsvId aServiceId, TNonOperationMtmDataType aMtmDataType, TPtrC8& aResultBuffer)
	{
	TBool active;
	TInt index = CheckMtmStatus(aServiceId, active);

	if (index == KErrNotFound)
		{
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(iMtmOperationQueueArray.At(index)->iMtm->GetNonOperationMtmData(aMtmDataType, aResultBuffer));
	}


/**
Checks client capabilty for readuserdata for performing search sort operation
*/
void CMsvServer::PoliceSearchSortQueryReadRequestL(const RMessage2& aMsg, const char* aContextText)
	{
	// The client can read the entry under following circumstances.
	// 1. The client has Read User Data capability.
	// 2. The client does not have Read User Data but owns the entry.

	// First check if the client is trusted with Read User Data.
	if( !aMsg.HasCapability(ECapabilityReadUserData) )
		{
		// Client not trusted with Read User data 
			{
			// Client missing capabilities - emit diagnostics and leave...
			User::LeaveIfError(PlatSec::CapabilityCheckFail(aMsg, ECapabilityReadUserData, aContextText));
			}
		}
	}


//**********************************
// CMsvTimer
//**********************************

CMsvTimer::CMsvTimer(CMsvServer& aServer, TBool aCloseServer)
: CTimer(EPriorityLow), iServer(aServer), iCloseServer(aCloseServer)
	{}

void CMsvTimer::RunL()
	{
	iServer.AttemptDelayedUpdating();
	if (iCloseServer)
		{
		CActiveScheduler::Stop();
		}
	}

CMsvTimer* CMsvTimer::NewL(CMsvServer& aServer, TBool aCloseServer)
	{
	CMsvTimer* self = new(ELeave) CMsvTimer(aServer, aCloseServer);
	CleanupStack::PushL(self);
	self->ConstructL(); // CTimer
	CActiveScheduler::Add(self);
	CleanupStack::Pop();
	return self;
	}


//**********************************
// MessageServer
//**********************************

EXPORT_C TVersion MessageServer::Version()
/** Returns the Message Server version number.

@return Message server version number */
	{
	return TVersion(KMsvServerMajorVersionNumber,KMsvServerMinorVersionNumber,KMsvServerBuildVersionNumber);
	}




EXPORT_C TMsvId MessageServer::NullUidValue()
/** Returns the predefined message entry ID null value.

@return Message entry ID null value */
	{
	return KMsvNullIndexEntryId;
	}




EXPORT_C TBool MessageServer::DriveContainsStore(RFs& aFs, TInt aDrive)
/** Checks if the specified drive has a Message Server index on it.

@capability AllFiles If not within the message server process
@capability None If within the message server process

@param aFs File server session
@param aDrive Drive to check, specified by a TDriveNumber value
@return True if aDrive has a Message Server index, otherwise false
@see TDriveNumber */
	{
	TParse parse1, parse2;
	TDriveUnit driveUnit(aDrive);
	TPtrC driveName(driveUnit.Name());
	
	// Check the existence of index file.
	parse1.Set(KMsvDefaultIndexFile,  &driveName, NULL);
	parse2.Set(KMsvDefaultIndexFile2, &driveName, NULL);
	TBool isMessageStoreExists =  BaflUtils::FileExists(aFs, parse1.FullName())
							  ||  BaflUtils::FileExists(aFs, parse2.FullName());
	
	// If index file is not present,
	// check the existence of index DB.
	if(!isMessageStoreExists)
		{
		parse1.Set(KMsvDbFile, &driveName, NULL);
		TFileName dBFile = parse1.FullName();   
		RSqlDatabase db;
		TRAPD(err, db.OpenL(dBFile));
		isMessageStoreExists = (KErrNone == err)? ETrue: EFalse;
		db.Close();
		}
	
	return isMessageStoreExists;
	}
	
	


EXPORT_C TInt MessageServer::CurrentDriveL(RFs& aFs)
/** Returns the drive that the Message Server is using to store messages.

@capability AllFiles If not within the message server process
@capability None If within the message server process
@param aFs File server session
@return Drive used, specified by a TDriveNumber value
@see TDriveNumber */
	{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	aFs = aFs;		  // This avoids warning.
	
	// Get the current drive info from cenrep.
	CRepository* repository = CRepository::NewL(KUidConfigFile);
	TInt driveNum;
	TInt err = repository->Get(KCenRepCurrentDriveKey, driveNum);
	delete repository;
	
	if( (err != KErrNone) || (driveNum > EDriveZ) )
		{
		User::Leave(KErrNotFound);  
		}
	return driveNum;

#else
	TMsvConfig config;
	CMsvServer::CurrentConfigL(aFs, config);
	return config.iDrive;
#endif	  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	}
	
	
	

/** Checks to see if the currently selected drive contains the correct mail store.

If the message store is present on an external drive, we have to check if the same drive is
mounted or not before storing the mail.

@return ETrue if the same drive is mounted. otherwise returns EFalse
@capability None
*/
EXPORT_C TBool MessageServer::IsMessageStoreDrivePresentL(RFs& aFs)
	{   
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	aFs = aFs;
	return ETrue;   
#else
	TUint uniqueId = 0;
	TMsvConfig config;
	CMsvServer::CurrentConfigL(aFs, config);
	//Check whether the drive and uid mentioned in the msgs.ini are mounted
	TVolumeInfo volumeInfo;
	uniqueId = config.iUniqueID;

	User::LeaveIfError(aFs.Volume(volumeInfo, config.iDrive.operator TInt()));
	//Check whether the uid stored in msgs.ini and mounted drive's uid are same
	if(volumeInfo.iUniqueID == uniqueId)
		{
		return ETrue;
		}
	return EFalse;
#endif		  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	}





#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
/**
 * IsMessageStoreSupported()
 *
 * @param TDriveNumber: The drive number of the drive.
 * @return TBool: ETrue: If the drive has a valid version of message store.
 *				EFalse: Otherwise.
 */
EXPORT_C TBool MessageServer::IsMessageStoreSupported(TDriveNumber aDrive)
	{
	TParse parse;
	TPtrC driveName(TDriveUnit(aDrive).Name());
	
	// Check if the message index database 
	// exists and it has a valid version.

	parse.Set(KMsvDbFile, &driveName, NULL);
	TFileName dBFile = parse.FullName();	
	RSqlDatabase db;
	TRAPD(err, db.OpenL(dBFile));
	if(KErrNone == err)
		{
		TInt version = 0;
		TSqlScalarFullSelectQuery query(db);
		_LIT8(KSelectVersionQuery, "SELECT version FROM VersionTable;");
		
		TRAP(err, version = query.SelectIntL(KSelectVersionQuery));	 
		if( (KErrNone == err) && (version == KCurrentDatabaseVersion) )
			{
			db.Close();
			return ETrue;
			}
		}
	// If the DB is already opened by message server, return ETrue.
	if(KErrInUse == err)
		{
		return ETrue;
		}
	db.Close();
	return EFalse;
	}

#endif	  //#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

