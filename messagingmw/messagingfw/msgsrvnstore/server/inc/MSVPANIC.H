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


#if !defined(__MSVPANIC_H__)
#define __MSVPANIC_H__

// reasons for server panic
/**
@internalComponent
@released
*/
enum TMsvServerPanic
	{
	EMainSchedulerError1=0,
	EMainSchedulerError2=1,
	ESvrCreateServer1=2,
	ESvrCreateServer2=3,
	ESvrCreateServer3=4,
	ESvrStartServer=5,
	ECreateTrapCleanup=6,
	ENotImplementedYet=7,
	EMsvChangeBufferEmpty=8,
	EMsvFileStoreNotPermanent=9,
	EMsvReleasingEntryNotLocked=10, //10
	EMsvEntryParentDoesNotExist1=11,
	EMsvEntryParentDoesNotExist2=12,
	EMsvEntryParentDoesNotExist3=13,
	EMsvEntryParentDoesNotExist4=14,
	EMsvEntryParentDoesNotExist5=15,
	EMsvEntryParentDoesNotExist6=16,
	EMsvEntryParentDoesNotExist7=17,
	EMsvEntryParentDoesNotExist8=18,
	EMsvEntryNotInParentsChildList1=19,
	EMsvEntryNotInParentsChildList2=20, //20
	EMsvEntryStreamIdNotInIndex=21,
	EMsvFileContainsOrphanEntry=22,
	EMsvEntryNotInMainList1=23,
	EMsvEntryNotInMainList2=24,
	EMsvNoEntryStreamsInFile=25,
	EMsvCleanupStreamNotInIndex=26,
	EMsvSelectionNotNull=27,
	EMsvSelectionNotInList=28,
	EMsvStringNotInStringList1=29,
	EMsvStringNotInStringList2=30, //30
	EMsvNoStringsLoaded=31,
	EMsvCannotRollBackDeletion1=32,
	EMsvCannotRollBackChanges1=33,
	EMsvStringBufferNotNull=34,
	EMsvDeletingCurrentContext=35,
	EMsvNotChangingCurrentContext=36,
	EMsvAlreadyLocked=37,
	EMsvNotLocked=38,
	EMsvReleasingStoreNotLocked=39, 
	EMsvOutstandingOperation=40,//40
	EMsvIsLocalRecursedToRoot=41,
	EMsvMovingNewParentNotExist=42,
	EMsvMovingEntryNotExist=43,
	EMsvMovingEntryParentDifferent=44,
	EMsvMovingEmptySelection=45,
	EMsvMovingUnlockedEntry=46,
	EMsvMovingWithinSameEntry=47,
	EMsvMovingLocalEntryIsRemote1=48, 
	EMsvMovingLocalEntryIsRemote2=49,
	EMsvDeletionEntryNotFound=50,//50
	EMsvDeletingUnlockedEntry=51,
	EMsvDeletionEntryHasChildren=52,
	EMsvEntryNotAnAttachment=53,
	EMsvRecursiveDeleteFailedNoError=54,
	EMsvDeletingLocalEntryIsRemote=55,
	EMsvDeletingAllLocalEntryIsRemote=56,
	EMsvMtmDeleteAllSelectionNotEmpty=57, 
	EMsvStoreInUseNotLocked=58,
	EMsvUpdatingFileFlagAlreadyTrue=59,	
	EMsvCopyingRemoteToRemote=60,//60
	EMsvUnknownMtmOpType=61,
	EMsvSelectionNotEmpty=62,
	EMsvDescendentArgumentsEqual=63,
	EMsvDescendentArgumentsRoot=64,
	EMsvMovingLocalEntryIsRemote=65,
	EMsvBadEntryContents=66,
	EMsvBufferExceeded=67,
	EMsvOperationQueueNotFound1=68,
	EMsvOperationQueueNotFound2=69,
	EMsvOperationQueueNotFound3=70,//70
	EMsvWrongOperationCompletion=71,
	EMsvSomeMtmQueueActive=72,
	EMsvOperationNotCompleted=73,
	EMsvEntryDataNotFound=74,
	EMsvProgressOnQueuedOp=75,
	EMsvCompletedNonRunningOp=76,
	EMsvCancellingNonRunningOp=77,
	EMsvOperationNotFound=78,
	EMsvDeletingMtmOperation1=79,
	EMsvDeletingMtmOperation2=80,//80
	EMsvDeletingMtmOperation3=81,
	EMsvChildrenDetailsNotEmpty1=82,
	EMsvChildrenDetailsNotEmpty2=83,
//	EMsvChildrenSelectionNotNull1=84, // Unused
	EMsvChildrenSelectionNotNull2=85,
	EMsvChildrenSelectionNull=86,
	EMsvOperationMtm1=87,
	EMsvOperationMtm2=88,
	EMsvOperationMtm3=89,
	EMsvDeletingReadOnlyEntry=90,//90
	EMsvChangingReadOnlyEntry=91,
	EMsvChangingReadOnlyContext=92,
	EMsvEditingStoreOfReadOnlyContext=93,
	EMsvStoreAlreadyOpen1=94,
	EMsvStoreAlreadyOpen2=95,
	EMsvStoreLeftOpenOnDestruction=96,
	EMsvUnknownStoreEvent2=97,
	EMsvUnknownStoreId=98,
	EMsvEntryStoreLeftOpen=99,
	EMsvChildAddedInSiblingList1=100,//100
	EMsvChildAddedInSiblingList2=101,
	EMsvEntryWithNoContext=102, // from MSVR 030 replaced by 146-156
	EMsvIncorrectStoreVersionNumber=103,
	EMsvIncorrectChangingTempData=104,
	EMsvChangingEntryToNewParent=105,
	EMsvChangingUnlockedEntry=106,
	EMsvDeletingSyncdEntry=107,
	EMsvMissingStore=108,
	EMsvChangingDeletedEntry=109,
	EMsvNewEntryHasStoreSet=110, //110
	EMsvBadMtmDatFile=111,
	EMsvCloseTimerNotAddeed=112,
	EMsvBadMessageDriveStreamVersion=113,
	EMsvIncorrectVariantIndexFile=114,
	EMsvrIndexPathNotEmpty=115,
	EMsvrMovingEntriesWithDifferentParents=116,
	EMsvrUnableToRemovePathDir=117,
	EMsvMtmMovingUnlockedStore=118,
	EMsvMtmMovingUnlockedEntry=119,
	EMsvMtmMovingToSameParent=120,//120
	EMsvCannotHaveRootBinaryFiles=121,
	EMsvIncorrectStateInFindNextStep=122,
	EMsvServiceFolderAlreadyExists=123,
	EMsvChangedEntryNotFound=124,
	EMsvMovingOperationInvariant=125,
	EMsvHiddenStreamEntryNotFound=126,
	EMsvCreatingServiceWithoutStoreFlag=127,
	EMsvCreatingLocalEntryWithoutStoreFlag=128,
	EMsvCreatingEntryWithIncorrectParent=129,
	EMsvHiddenStreamEntryNotLocal=130,//130
	EMsvHiddenStreamEntryNotFound2=131,
	EMsvHiddenOwnerNotFound=132,
	EMsvHiddenOwnerNotFound2=133,
	EMsvChangingString1WithAccessOverride=134,
	EMsvChangingString2WithAccessOverride=135,
	EMsvRebuildingServerWithNoServices=136,
	EMsvRebuildingServerWithNoLocalFolders=137,
	EMsvUsingRebuildingIndex=138,
	EMsvBadIndexState=139,
	EMsvLocalDeletionMisCount=140,//140
	EMsvCopyingOperationInvariant=141,
	EMsvActiveCopyEntryReset=142,
	EMsvCopyErrorButNoIds=143,
	EMsvCopyEntryFailureBadState=144,
	EMsvRemovingOrphanedEntries=145,
	EMsvEntryWithNoContext1=146,
	EMsvEntryWithNoContext2=147,
	EMsvEntryWithNoContext3=148,
	EMsvEntryWithNoContext4=149,
	EMsvEntryWithNoContext5=150,//150
	EMsvEntryWithNoContext6=151,
	EMsvEntryWithNoContext7=152,
	EMsvEntryWithNoContext8=153,
	EMsvEntryWithNoContext9=154,
	EMsvEntryWithNoContext10=155,
	EMsvEntryWithNoContext11=156,
	EMsvCancelledNonQueueOp=157,
	EMtsrRegisteredMtmDllNotPresent=158,
	EMtsrRegistryControlStillInUse=159,
	EMtsrInstalledMtmGroupStillInUse=160,//160
	EMtsrInstalledMtmGroupNotPresent=161,
	EMsvChangeAttEmtryNotFound=162,
	EMsvDeleteSelectionentryMissing=163,
	EMsvDeleteSelectionCorrupt=164,
	EMsvEntryDeletingService=165,
	EMsvMissingServiceInDeletion=166,
	EMsvMissingEntryInDeletion=167,
	EMsvDeleteBadState=168,
	EMsvChangeEntryBadState=169,
	EMsvMissingEntryInDeletion2=170,//170
	EMsvDeletionChangedEntryNotFound=171,
	EMsvMissingEntryInDeletion3=172,
	EMsvDeletionNoParent=173,
	EMsvDeleteBadState2=174,
	EMsvMovingentryAccrossServices=175,
	EMsvSetentryWhileMoving=176,
	EMsvMoveNotCompleted=177,
	EMsvCopyMoveCompletionMissing=178,
	EMsvMovingToSamePArent=179,
	EMsvCopyMoveCancelMissing=180,//180
	EMsvSetMovingInDtor=181,

	EMsvBufferNotEmpty=182,	// From here these used to be MENT panics
	EMsvHeapBufferNotNull=183,
	EMsvPointerOverrunBuffer=184,
	EMsvFailUnpackWithResizedBuffer=185,
	EMsvStoreBodyWhenUnlocked=186,
	EMsvCommittingWhenUnlocked=187,
	EMsvRemovingWhenUnlocked=188,
	EMsvRevertingWhenUnlocked=189,
	EMsvOperationUnpackSelectionNotEmpty=190,//190
	EMsvChangedUnpackSelectionNotEmpty=191,
	EMsvChangeSelectionTooLarge=192,
	EMsvUnknownChangeType=193,
	EMsvHumanReadableNameTooLong=194,
	EMsvRegisteredMtmDllStillInUse=195,
	EMsvRegisteredMtmDllRefCountZero=196,
	EMsvRegisteredMtmDllRefCountNonZero=197,
	EMsvRegisteredMtmDllIndexOutRange=198,
	EMsvRegisteredMtmDllNotFound=199,
	EMsvRegisteredMtmDllHasSameMtmTypeUid=200,//200
	EMsvRegisteredMtmDllArrayNotEmpty=201,
	EMsvUnknownMessageType=202,
	EMsvUnknownPriority=203,
	EMsvEntryOverrunBuffer=204,
	EMsvStoreHasBeenDeleted=205,
	EMsvDeletingWhenUnlocked=206,
	EMsvUnknownSortType1=207,
	EMsvUnknownSortType2=208,
	EMsvMtmDllInfoSecondUidIncorrect=209,
	EMsvUnknownMtm=210,//210
	EMsvMtmListNotDefined=211,
	EMsvToManyGroups=212,
	EMsvPathAsForNonService=213,
	EMsvAskingForRootFolderOrPath=214,
	EMsvAskingForRootFolder=215,
	EMsvCompactorIncorrectStart=216,
	EMsvCompactionProgressBeforeStart=217,
	EMsvCMsgActiveAlreadyActive=218,

	EMsvBufferNotEmpty2=219, // From now on these used to be MCLD panics
	EMsvHeapBufferNotNull2=220,//220
	EMsvPointerOverrunBuffer2=221,
	EMsvFailUnpackWithResizedBuffer2=222,
	EMsvChangingEntryNotContext=223,
	EMsvDeletingEntryNotChild=224,
	EMsvUnknownChangeType2=225,
	EMsvEntryNotAnAttachment2=226,
	EMsvSessionUnknownObserver=227,
	EMsvEntryUnknownObserver=228,
	EMsvOperationNotLocalIs=229,
	EMsvOperationDifferentMtms=230,//230
	EMsvChangingEntryType=231,
	EMsvEntryHasNullType=232,
	EMsvEntryHasNullMtm=233,
	EMsvSessionNoObservers=234,
	EMsvChildEntriesExist1=235,
	EMsvChildEntriesExist2=236,
	EMsvNonChildDeleted=237,
	EMsvEntryHasNullServiceId=238,
	EMsvEntryStateUndetermined=239,
	EMsvEntryAlreadyChangingContext=240,//240
	EMsvNonConsistentDeleteSelection=241,
	EMsvUnknownEntryType=242,
	EMsvEntryNewServiceNotUnderRoot=243,
	EMsvMulitpleMtmsForMoveCommand=244,
	EMsvMulitpleMtmsForCopyCommand=245,
	EMsvChangingReadOnlyEntry2=246,
	EMsvDeletingReadOnlyEntry2=247,
	EMsvEditingStoreOfReadOnlyEntry=248,
	EMsvCreatingInvalidEntry=249,
	EMsvChangingToInvalidEntry=250,//250
	EMsvStoreLeftOpenOnDestruction2=251,
	EMsvUnknownStoreEvent3=252,
	EMsvEntryArrayNotEmpty=253,
	EMsvChangedChildNotFound1=254,
	EMsvChangedChildNotFound2=255,
	EMsvNotLocalOperation=256,
	EMsvServerExeMissing=257,
	EMsvEmptySelection=258,
	EMsvStoreLeftOpen=259,
	EMsvMovingEntryToSameParent=260,//260
	EMsvStoreAlreadyOpen=261,
	EMsvEntriesStillOnCleanupList=262,
	EMsvDeletedChildNotInMainList=263,
	EMsvContextChangedOwningService=264,
	EMsvDeletingEntryDifferentOwningService=265,
	EMsvMovingEntryDifferentOwningService=266,
	EMsvCopyingEntryDifferentOwningService=267,
	EMsvNewChildDifferentOwningService=268,
	EMsvDeletedGrandChildDifferentOwningService=269,
	EMsvChildHasDifferentOwningService=270,//270
	EMsvChangedChildHasDifferentOwningService=271,
	EMsvAskingForRootFolder2=272,
	EMsvErrorNotification=273,
	EMsvBadMessageDriveStreamVersion2=274,
	EMtclMsvEntryNotSet=275,
	EMtclRegisteredMtmDllNotPresent=276,
	EMtclMsvEntryWrongType=277,
	EMtclSettingEntryHasIncorrectMtm=278,
	EMtclContextChangedByOwner=279,
	EMsvMissingMtmData=280,//280

	EMsvServerEntryNotIdle=281,// From now on these are general MSGS panics
	EMsvServerEntryIdle=282,
	EMsvUtilsHasDirFailed=283,
	EMsvSvrEntryHasDirFailed=284,
	EMsvActiveInFinalProgress=285,
	EMsvFinalProgressFailed=286,
	EMsvUtilsHasStoreFailed=287,
	EMsvDescriptionBufferNotNull=288,
	EMsvDetailsBufferNotNull=289,
	EMsvDescriptionBufferIsNull=290,//290
	EMsvDetailsBufferIsNull=291,
	EMsvBadRollBackCopy1=292,
	EMsvBadRollBackCopy2=293,
	EMsvNotLocalService=294,
	EMsvDeletedFlagNotSet=295,
	EMsvDeletingEmptySelection=296,
	EMsvDeleteAndMoveSelectionsNotNull=297,
	EMsvMoveSelectionNotNull=298,
	EMsvCopyingToSameParent=299,
	EMsvCopyNotCompleted=300,//300
	EMsvNoErrorSet=302,
	EMsvInvalidCurrentIndex=303,
	EMsvInvalidFindOperationEntry=304,
	EMsvNoFindTextSpecified=305,
	EMsvTooMuchFindTextSpecified=306,
	EMsvFindTextAlreadyActive=307,
	EMsvDateOutsideValidRange=308,
	EMsvDateNotSet=309,
	EMsvLoadingInProgress=310,
	EMsvChangingToExistingDrive=311,
	EMsvDiskChangeNotiferActive=312,
	EMsvNoIndex=313,
	EMsvInvalidDrive=314,
	EMsvNoServerContext=315,
	EMsvNewContextExists=316,
	EMsvIncorrectReclaimStatus=317,
	EMsvBackupObserverGotUnknownEvent=318,
	EMsvBackupHandlerInUnkownState=319,
	EMsvBackupReclaimLockFailed=320,
	EMsvMultipleCompletedEntryData=321,
	EMsvBadMtmVersionNumber=322,
	EMsvConstructWithNullDllInfoArray=323,
	EMsvAttemptToUseNullDllInfoArray=324,
	EMsvSecurityCapabilitySetResourceIsNotPresent=325,
	EMsvFileSessionNotValid=326,
	EMsvServerNotValid=327,
	EMsvCopyStoreReset=328,
	EMsvDeleteStoreReset=329,
	EMsvChildrenSelectionCountsMismatch=330,
	EMsvSystemProgressOnQueuedOp=331,
	EMsvInputBufferEmpty=332,
	EMsvFreePoolNotCreated=333,
 	EMsvSearchSortCacheMangerNotCreated=334,
	EMsvSearchSortDeltaCacheNotCreated=335,
	EMsvInvalidSearchSortResultType=336,
	EMsvIteratorCountIsMore=337,
	EMsvInvalidRequest=338
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	,EMsvPreferredDriveListNotCreated=339
#endif
	};

GLREF_C void PanicServer(TMsvServerPanic aPanic);

#endif
