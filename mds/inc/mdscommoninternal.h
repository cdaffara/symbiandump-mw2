/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Common message definitions and utilities for MDS internal use
*
*/


#ifndef MDSCOMMONINTERNAL_H
#define MDSCOMMONINTERNAL_H

#include <mdccommon.h>


//                           X:[<UID>]<database-name>.db
_LIT( KMdsSqlDbDefaultName, "C:[200009f3]metadata.sq" );
_LIT( KMdsSqlDbDefaultPath, "C:\\private\\10281E17\\[200009f3]metadata.sq" );

// default import metadata file
_LIT( KMdsDefaultImportFile, "C:\\private\\200009f3\\defaultimportfile.mde" );
_LIT( KMdsDefaultRomImportFile, "Z:\\private\\200009f3\\defaultimportfile.mde" );

// Universal image folder
_LIT( KDCIMFolder, ":\\DCIM");

// schema file version
const TInt KSchemaFileMajorVersion = 3;

const TDefId KDefaultNamespaceDefId = 1;

const TDefId KBaseObjectDefId = 1;

const TUint32 KBaseObjectBasicValueColumnOffset = 8;

const TItemId KSystemFavouritesAlbumId = 1;

_LIT( KSystemFavouritesAlbumUri, "defaultalbum_favourites" );
_LIT( KSystemCapturedAlbumUri, "defaultalbum_captured" );

const TInt KMaxUintValueLength   = 10;
const TInt KMaxUint64ValueLength = 20;

const TInt KMaxTitleFieldLength = 255;

const TInt KTriggerDbMaintenanceTreasholdValue( 3000 );

_LIT( KMdSServerName,"!MdSServer" ); // Server name
_LIT( KMdSServerNameIAD,"!__MdSServer" ); // Server name after shutdown
_LIT( KMdSServerProcessString,"!MdSServer*" ); 
_LIT( KSchemaChunkName, "MdSSchema" );

_LIT( KCMediaIdKey, "CDriveMediaId" );
_LIT( KMassStorageMediaIdKey, "MassStorageMediaId" );

const TInt KMmcEntryBufferSize = 100;

// The server version. A version must be specified when
// creating a session with the server.
const TUint KMdSServMajorVersionNumber=2;
const TUint KMdSServMinorVersionNumber=5;
const TUint KMdSServBuildVersionNumber=0;

const TUint32 KNokiaVendorId = 52487775;

const TInt64 KDiskFullThreshold = 1024*70; // 70 kB, SQLite default maximum journal size

// P&S stuff
static _LIT_SECURITY_POLICY_PASS(KAllowAllPolicy);
static _LIT_SECURITY_POLICY_C1(KPowerMgmtPolicy,ECapabilityPowerMgmt);

const TUid KMdSPSShutdown = { 0x20022E94 };
const TUid KHarvesterPSShutdown = { 0x200009F5 };
const TUid KWatchdogPSShutdown = { 0x20022E93 };

const TInt KShutdown = 0x00000002;

//Locktype for query
enum TMdCQueryLockType
	{
	EGet,
	ELock	
	};

//
// Common enumerations in client/server messages
//
// Server requests
enum TMdEServRqst
    {
    EShutdown = 0,         	// shut down the session
    EAdd,					// add items
    ERemove,				// remove items
    EFind,              	// finds in sync
    EAsyncFind,         	// finds asynchronously
    EContinueAsyncFind, 	// continues existing asynchronous find, fetches next set
    ECancelFind,        	// cancels asynchronous find
    ECancelObject,      	// cancels opened object
    ECheckObject,			// get object's "base" values
    EUpdate,		      	// update items
    ERegister,          	// registers an object to be notified
    EListen,            	// listens for events for a registered profile
    EUnregister,        	// unregisters for no more notifications
    EGetData,           	// (transfers data server->client)
    EImportMetadata,    	// starts metadata importing
    EAsyncImportMetadata,	// starts asynchronous metadata importing
    EExportMetadata,		// starts metadata exporting
    EAsyncExportMetadata,	// starts asynchronous metadata exporting
    EImportSchema,      	// starts schema importing
    EAddRelationDef,		// Add new relation
    EAddEventDef,			// Add new event
    EAddMemoryCard,			// Add new memory card's media ID
    EGetMemoryCard,			// Get latest memory card's media ID
    ECheckMemoryCard,		// Check if there is already  memory card with given media ID
    ESetMedia,				// Set media's media ID, drive and present state to DB
    EGetMedia,				// Get media's drive and present state from DB
    EGetPresentMedias,		// Get present medias' media IDs, drives and present states from DB
    ESetFileToPresent,		// Set file to present state
    ESetFilesToPresent,		// Set files to present state
    ESetFilesToNotPresent,	// Set all files, which media ID is given, to not present state
    ERemoveFilesNotPresent,	// Remove all objects, which are in not present state, with given media ID
    EGetSchemaVersion,		// Get schema version's major and minor version
    ESetObjectToPresentByGuid,					// Set object to "present" state by GUID
    ESetHarvestingPrioritizationChunk,			// Set global harvester prioritization chunk
    EAddHarvestingPrioritizationObserver,		// Add harvester prioritization observer
    ECancelHarvestingPrioritizationObserver,	// Cancel harvester prioritization observer
    EChangePath,            // Change path of objects
    ESetPending,    		// Set pending flag to objects
    EResetPending,			// Reset pending flag to objects
    EGetPendingCount,		// Get count of objects with pending flag
    EGetPending,			// Get objects with pending flag
    EResetDB,              	// Reset database (works only in DEBUG build)
    EChangeMediaId,			// Change C-drive media id and update objects
    EChangeMassStorageMediaId,         // Change internal mass storage media id and update objects
    EUnknownMdEServRqst     // handle for unknown requests
    };

// Server request arguments for 
// EAddRelationDef and EAddEventDef
enum TMdEServRqstArgsAddDef
	{
	EAddDefArgNamespaceId = 0,
	EAddDefArgDefName     = 1
	};
	
// Server request arguments for 
// EFind, EAsyncFind, EContinueAsyncFind and ECancelFind
enum TMdEServRqstArgsFind
	{
	EFindArgQueryId         = 0,
	EFindArgConditions      = 1,
	EFindArgResulBufferSize = 2,
	EFindArgNotifyCount     = 3
	};

enum TMdEServRqstArgsCheckObject
	{
	ECheckObjectArgType           = 0,
	ECheckObjectArgTypeValue      = 1,
	ECheckObjectArgNamespaceDefId = 2,
	ECheckObjectArgObject         = 3
	};

enum TMdECheckObjectType
	{
	ECheckObjectByUri,
	ECheckObjectById,
	ECheckObjectByIds
	};

// Server request arguments for 
// ESetObjectToPresentByGuid
enum TMdEServRqstArgsSetObjectToPresentByGuid
	{
	ESetObjectToPresentByGuidArgGuidHigh = 0,
	ESetObjectToPresentByGuidArgGuidLow  = 1
	};
	
// Server request responses
enum TMdEServRqstComplete
	{
    EAsyncFindSetReady  = 1000, // set complete
    EAsyncFindComplete,         // whole find complete
    ELoadSchema,        		// fetches schema to client

    /*ENotifyAdded,               // notification: item added
    ENotifyRemoved,             // notification: item removed
    ENotifyModified,            // notification: item modified
    ENotifyObjectPresent,       // notification: object set to present
    ENotifyObjectNotPresent,    // notification: object set to not present
    ENotifyRelationPresent,     // notification: relation set to present
    ENotifyRelationNotPresent,  // notification: relation set to not present
    ENotifySchemaAdded,         // notification: schema added
    */
	};
		
	
// MdE user levels
enum TUserLevel
    {
    EUserLevelNone,
    EUserLevelNormal,
    EUserLevelDeviceAccess
    };

enum TMdEObjectFlags
	{
	EMdEObjectFlagNone                 = 0x00000000, // no flags set
	EMdEObjectFlagNotPresent           = 0x00000001, // object is marked as not present
	EMdEObjectFlagRemoved              = 0x00000002, // object is marked as removed
	EMdEObjectFlagFreetexts            = 0x00000004, // object has freetext(s)
	EMdEObjectFlagConfidential         = 0x00000008, // object is confidential
	EMdEObjectFlagPlaceholder          = 0x00000010, // object is placeholder
	EMdEObjectFlagStartUpNotPresent    = 0x00000020, // object is marked as start up not present
	EMdEObjectFlagContext              = 0x00000040, // object is context object
	EMdEObjectFlagGarbage              = 0x00000080, // object is marked to be removed by garbage collector
	
	EMdeObjectFlagPending			   = 0x00000100, // composing to binary is pending for this object
	
	EMdEObjectFlagModObject            = 0x00010000, // object modifications
	EMdEObjectFlagModProperty          = 0x00020000, // one of properties modifications
	EMdEObjectFlagModFreeText          = 0x00040000, // one of freetext modifications
	EMdEObjectFlagModOpen              = 0x00080000, // open for modifications
	
	EMdEObjectFlagAutoLock             = 0x00100000, // open for modifications
	};

enum TMdERelationFlags
	{
	EMdERelationFlagNone           = 0x00000000, // no flags set
	EMdERelationFlagNotPresent     = 0x00000001, // relation's object is not present
	EMdERelationFlagDeleted        = 0x00000002, // relation's object is marked as removed
	EMdERelationFlagGarbageDeleted = 0x00080000, // relation's object is marked to be removed by garbage collector
	};

enum TMdEPropertyFlags
	{
	EPropertyReadOnly   = 0x01,
	EPropertyMandatory  = 0x02,
	};

enum TMdEPropertyModFlags
	{
	EMdEPropertyModNone   = 0x00,
	EMdEPropertyModChange = 0x01,
	EMdEPropertyModRemove = 0x02,
	};
	
enum TMdSObserverNotificationType
	{
	EObjectNotifyAdd          = 0x00000001,
	EObjectNotifyModify       = 0x00000002,
	EObjectNotifyRemove       = 0x00000004,

	EObjectNotifyPresent      = 0x00000010,
	EObjectNotifyNotPresent   = 0x00000020,
	
	ERelationNotifyAdd        = 0x00000100,
	ERelationNotifyModify     = 0x00000200,
	ERelationNotifyRemove     = 0x00000400,

	ERelationNotifyPresent    = 0x00001000,
	ERelationNotifyNotPresent = 0x00002000,

	//ERelationItemNotifyAdd    = 0x00010000,
	//ERelationItemNotifyModify = 0x00020000,
	ERelationItemNotifyRemove = 0x00040000,
	
	
	EEventNotifyAdd           = 0x00100000,
	EEventNotifyRemove        = 0x00200000,
	
	ESchemaModify             = 0x01000000,
	
	EObjectNotifyAddWithUri        = 0x10000000,
    EObjectNotifyModifyWithUri     = 0x20000000,
    EObjectNotifyRemoveWithUri     = 0x40000000,	
	
	// 0x80000000 is not allowed (signed number is negative)
	};

// File info
struct TMdSFileInfo
	{
	/** Last modification time. */
	TInt64 iModifiedTime;

	/** File size in bytes. */
	TUint32 iSize;
	};

/** Storage state flags.  */
enum TStoredFlags
	{
	EStoredFlagsNone  = 0x0000,
	EStoredFlagsDB    = 0x0001, // data stored in DB
	EStoredFlagsTable = 0x0002, // table is stored in DB
	EStoredFlagsC2P   = 0x0004, // data is stored in Col2Prop table
	};

/**
 * Query's internal optimization flags
 */
enum TInternalOptimizationFlags
	{
	EContainsObjectCondition     = 0x00000001,
	EContainsRelationCondition   = 0x00000002,
	EContainsEventCondition      = 0x00000004,
	EContainsFreetextCondition   = 0x00000008,
	EContainsNotPresentCondition = 0x00000010,
	EContainsObjectLocking       = 0x00000020,
	EContainsPlaceholdersOnly    = 0x00000040
	};

struct TMdEMediaInfo
	{
	TUint32 iMediaId;
	TChar iDrive;
	};
	
/**
 * File present states.
 * Values must be between 0 and 255.
 */
enum TFilePresentStates
	{
	EMdsNotFound = 0,
	EMdsNormal,
	EMdsPlaceholder,
	EMdsModified
	};
	
struct TMdSMediaIdAndCount
	{
	TUint32 iMediaId;
	TUint32 iCount;
	};	
	
#endif //MDSCOMMONINTERNAL_H
