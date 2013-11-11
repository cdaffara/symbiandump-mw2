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

#ifndef __MSERVER_H__
#define __MSERVER_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MSVSTD_H__)
#include <msvstd.h>
#endif



// Server name, semaphore, and startup exe
_LIT(KMsvServerSemaphore, "MsvStartupSemaphore");

// A version must be specifyed when creating a session with the server
const TUint KMsvServerMajorVersionNumber=1;
const TUint KMsvServerMinorVersionNumber=4;
const TUint KMsvServerBuildVersionNumber=0;


// server request slots
const TInt KMsvNumberOfSlots=14;
const TInt KMsvMaxSlotsAvailable=KMsvNumberOfSlots-1; // reserved for change notification

// Opcodes used in message passing between client and server
/**
@internalComponent
@released
*/
enum TMsvServerRqst 
	{
	// Entry related (Sync)
	EMsvGetEntry=0,
	EMsvGetChildren=1,
	EMsvGetRemainingChildren=2,
	EMsvLockEntry=3, 
	EMsvReleaseEntry=4,
//	EMsvMakeFileDirectory=5,  // Not used
	// Entry related (Async)
	EMsvChangeEntry=6,
	EMsvCreateEntry=7,
	// Entry/Entries related (Async)
	EMsvDeleteEntries=8,
	EMsvMoveEntries=9,
	EMsvCopyEntries=10,
	// Observer related (Async)
	EMsvNotifySessionEvent=11,
	// Observer related (Sync)
	EMsvCancelSessionEventNotification=12,
	// Store related (Sync)
	EMsvReadStore=13,
	EMsvLockStore=14,
	EMsvReleaseStore=15,
//	EMsvCommittedStore=16, // Not used
	// Operation related (Sync)
	EMsvOperationData=18,
	EMsvCommandData=19,
	EMsvCancelOperation=20,
	EMsvOperationProgress=21,
	EMsvOperationCompletion=22,
	EMsvOperationMtm=23,
	EMsvMtmCommand=24,
	// MTM related
	EMsvFillRegisteredMtmDllArray=25,
	EMsvInstallMtmGroup=26,
	EMsvDeInstallMtmGroup=27,
	EMsvUseMtmGroup=28,
	EMsvReleaseMtmGroup=29,
	EMsvGetMtmGroupData=30,
	// closes the server
	EMsvCloseServer=31,
	// service related
	EMsvStopService=32,
	EMsvServiceActive=33,
	EMsvServiceProgress=34,
	// misc
	EMsvRemoveEntry=35,
	// added after functional freeze
//	EMsvCreatedStore=36, // Not used
	EMsvGetMessageDirectory=37,
	EMsvSlotAvailable=38,
	// ER5U+
	EMsvSetSessionAsObserver=39,
	EMsvSetFailure=40,
	EMsvChangeAttributes=41,
	EMsvGetChildIds=42,
	EMsvChangeDrive=43,
	EMsvOutstandingOperations=44,
	EMsvGetNotifySequence=45,
	// added in 6.1, allows client to say that it does/doesn't want entry events
	EMsvSetReceiveEntyEvents=49,
	EMsvDecStoreReaderCount=50,
	EMsvGetMessageDrive=51,
	EMsvGetMtmRequiredCapabilities=52,
	EMsvCreateAttachmentForWrite=53,
	EMsvOpenAttachment=54,
	EMsvOpenAttachmentForWrite=55,
	EMsvDeleteAttachment=56,
	EMsvOpenFileStoreForRead=57,
	EMsvOpenTempStoreFile=58,
	EMsvReplaceFileStore=59,
	EMsvDeleteFileStore=60,
	EMsvFileStoreExists=61,
	EMsvGetAndClearIndexCorruptFlag=62,
	EMsvCopyStore=63,
	EMsvDeleteStore=64,
	EMsvDriveContainsStore=65,
	EMsvMessageStoreDrivePresent=66,
	EMsvFileExists=67,
	EMsvRenameAttachment=68,
	EMsvReplaceAttachmentForWrite=69,
	EMsvGetAttachmentFilePath=70,
	EMsvOperationSystemProgress=71,
	EMsvGetBodyTextFilePath=72,
	EMsvOpenTextFileForRead=73,
	EMsvCreatePlainTextFile=74,
	EMsvDeletePlainTextFile=75,
	EMsvReplacePlainTextFile=76,
	EMsvGetNonOperationMtmData=77,
	EMsvSearchSortOperation=78,
	EMsvQueryData=79,
	EMsvGetResult=80,
	EMsvSearchSortOnIndexEntry=81,
	EMsvGetQueryId=82,
	EMsvUnmarQueryId=83,
	EMsvSearchSortQueryId=84,
	EMsvGetIdsOrResult=85,
	EMsvGetResultCount=86,
	EMsvGetNextId=87,
	EMsvGetNextEntry=88,
	EMsvIdWithSortFiled=89,
	EMsvGetSearchSortProgress=90,
	EMsvCancelSearchSortOp=91,
	EMsvUpdateAndSort=92,
	// Code added for PREQ 557.
	// Macros for preferred drive list APIs.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
    ,EMsvGetCurrentDriveInfo=94,
    EMsvGetDriveList=95,
    EMsvGetAvailableDriveList=96,
    EMsvAddDriveToDriveList=97,
    EMsvRemoveDriveFromDriveList=98,
    EMsvUpdateDrivePriority=99,
    EMsvGetChildrenALL=100,
    EMsvGetChildIdsALL=101
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	,EMsvResetRepository=102,
	EMsvPrintCache=103,
#endif
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	,EMsvCreateStore=104,
	EMsvCheckStoreExists=105,
	EMsvLastErrorMessage=106,
	EMsvCreateHeaderEntry=107,
	EMsvLoadHeaderEntry=108,
	EMsvUpdateHeaderEntry=109,
	EMsvDeleteHeaderEntry=110,
	EMsvCheckAnyStoreExists=111,
	EMsvCheckHeaderTableExist=112,

	EMsvConvertMessageStore=113,
	EMsvCancelConversionRequest=114,
	EMsvGetConvertibleDriveList=115,
	EMsvGetConversionStatus=116,
#endif
	EMsvChangeEntries = 117
	};


#endif

