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

#if !defined(__MSVIPC_H__)
#define __MSVIPC_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MSVSTD_H__)
#include <msvstd.h>
#endif

#if !defined(__MSVSEARCHSORTQUERY_H__)
#include <msvsearchsortquery.h>
#endif

//
const TInt KMsvNullOperation=0;
const TInt KMsvFirstOperation=1;

//
const TInt KMsvSessionBufferLength=0x1000;
const TInt KMsvProgressBufferLength=256;

struct TMsvIdWithSortField;

// ***** NOTE!!!!! TMsvPackedChangeNotification::UnpackL() MUST be	****
// ***** updated when adding new types								*****
/**
@internalComponent
@released
*/
enum TMsvServerChangeNotificationType 
	{
	EMsvNullNotification=-1,
	EMsvEntriesNoChange=0,
	EMsvEntriesCreated=1,
	EMsvEntriesChanged=2,
	EMsvEntriesDeleted=3,
	EMsvEntriesMoved=4,
	EMsvMtmGroupInstalled=5,
	EMsvMtmGroupDeInstalled=6,
	EMsvStoreDeleted=7,  
	EMsvCloseSession=8,
	EMsvIndexLoaded=9,
	EMsvIndexFailedToLoad=10,
	EMsvMediaChanged=12, // Note: I assume the following four follow each other in order
	EMsvMediaUnavailable=13,
	EMsvMediaAvailable=14,
	EMsvMediaIncorrect=15,
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	EMsvMessageStoreNotSupported=16,
	EMsvMessageStoreCorrupt=17,	
	EMsvRefreshMessageView=18,
	EMsvDiskNotAvailable=19,
	EMsvUnableToProcessDiskNotification=20
#endif
	};


//**********************************
// TMsvPackedEntry
//**********************************
//
// Packs a TMsvEntry and the two associated descriptors into a buffer for
// sending across IPC. Also unpacks the data into a TMsvEntry and two heap 
// descriptors
//
//
class TMsvEntry;

class TMsvPackedEntry
/**
@internalComponent
@released
*/
	{
public:
	IMPORT_C TMsvPackedEntry(HBufC8*& aBuffer);
	IMPORT_C TInt PackEntry(const TMsvEntry& aEntry);
	IMPORT_C void UnpackEntry(TMsvEntry& aEntry);
    TInt PackEntryAndService(const TMsvEntry& aEntry, const TMsvId& aServiceId);
    void UnpackEntryAndService(TMsvEntry& aEntry, TMsvId& aServiceId);
private:
	HBufC8*& iBuffer;
	};


//**********************************
// TMsvPackedEntryArray
//**********************************
//
// Packs 1+ TMsvEntry's and the associated descriptors into a buffer for
// sending across IPC. Also unpacks the data into a TMsvEntry and two heap 
// descriptors using indexed access
//
//

class TMsvPackedEntryArray
/**
@internalComponent
@released
*/
	{
public:
	IMPORT_C TMsvPackedEntryArray(HBufC8*& aBuffer, TInt aCount=0);
	IMPORT_C TInt PackEntry(const TMsvEntry& aEntry);
	IMPORT_C TInt UnpackEntry(TInt aEntryIndex, TMsvEntry& aEntry);
	IMPORT_C void Reset();
private:
	TInt FindEntryInArray(TInt aIndex);
private:
	TInt	iIndex;
	TInt	iCount;
	const TUint8* iPos;
	HBufC8*& iBuffer;
	};


//**********************************
// TMsvChildrenDetails
//**********************************
//
// Contains the information about the number of children of an entry and how many
// have been sent in the main buffer
//

class TMsvChildrenDetails
/**
@internalComponent
@released
*/
	{
public:
	IMPORT_C TMsvChildrenDetails();
public:
	TMsvId	iParentId;
	TInt	iTotalNumberChildren;
	TInt	iNumberChildrenInArray;
	TInt	iLastEntryInArray;
	};


//**********************************
// TMsvLocalOperationProgress
//**********************************
//
// Contains the information about a local operation
//

class TMsvLocalOperationProgress
/** Encapsulates progress information returned by a local operation. A local operation 
is a CMsvOperation-derived object returned by a function that affects only 
local entries. For example, such a function would be a CMsvEntry::ChangeL() 
call on a local entry. You can check if the operation is local from CMsvOperation::Mtm(): 
a local operation returns a value of KUidMsvLocalServiceMtm. 

Progress information is obtained, in a packaged format, by calling CMsvOperation::ProgressL(). 
Understanding the TMsvLocalOperationProgress structure enables an application 
to use that information for a local operation.  
@publishedAll
@released
*/
	{
public:
/** Defines the possible values for the type of operation progress report. 
@publishedAll
@released
*/
	enum TLocalOperation
						{	
	/** Default value for new TMsvLocalOperationProgress */
							ELocalNone=0,
	/** Move operation */
							ELocalMove,
	/** Delete operation */
							ELocalDelete,
	/** Copy operation */
							ELocalCopy,
	/** Create operation */
							ELocalNew,
	/** Change operation */
							ELocalChanged
							};
public:
	IMPORT_C TMsvLocalOperationProgress();
public:
	/** The type of operation on which the progress is reporting */
	TLocalOperation iType;
	/** The number of entries on which the operation was or will be attempted */
	TInt iTotalNumberOfEntries;
	/** The number of entries on which the operation has been successful */
	TInt iNumberCompleted;	
	/** The number of entries on which the operation has failed */
	TInt iNumberFailed;	
	/** The number of entries on which the operation has yet to be attempted */
	TInt iNumberRemaining;	
	/** The error code for the latest failed operation */
	TInt iError;
	/** This contains different information for different operation types:
	
	1. copying a single entry: ID of the new entry
	
	2. moving an entry: ID of the entry moved
	
	3. copying or moving multiple entries: ID of the first entry in the selection */
	TMsvId	iId;
	};

//**********************************
// TMsvServerOperationProgress
//**********************************
/** Encapsulates progress information returned by a non-MTM specific Message Server 
operation. 

You can check if an operation has this type of progress information by calling 
CMsvOperation::Mtm(): a suitable operation returns a value of KUidMsvServerMtm. 

Progress information is obtained, in a packaged format, by calling CMsvOperation::ProgressL(). 
Understanding the TMsvServerOperationProgress class enables an application 
to use that information for a Message Server operation. 

@see TMsvLocalOperationProgress 
@see CMsvSession::CopyStoreL() 
@see CMsvSession::DeleteStoreL()
@publishedAll
@released
*/

class TMsvServerOperationProgress
	{
public:

	enum TMsvServerOperationType 
		{ 
		/** No operation in progress. */
		EMsvNoOperation = 0, 
		/** Change drive operation in progress.
	
		@see CMsvSession::ChangeDriveL() */
		EMsvChangeDriveOperation
		
		,
 		/** Copy Store operation in progress.
 		@see CMsvSession::CopyStoreL() */
 		EMsvCopyOperation,
 		
 		/** Delete Store operation in progress.
 		@see CMsvSession::DeleteStoreL() */
 		EMsvDeleteOperation
 		
 				
 		 };
	//
public:
	IMPORT_C TMsvServerOperationProgress();
	TMsvServerOperationProgress(TMsvServerOperationType aType);
	//
public:
	/** The type of operation in progress. */
	TMsvServerOperationType iOperationType;
	};

		
//**********************************
// TMsvIndexProgress
//**********************************

class TMsvIndexProgress
/** Provides details of index loading progress. 

@see TMsvIndexLoadProgress 
@publishedAll
@released
*/
	{
public:
	IMPORT_C TMsvIndexProgress();
	//
public:
	/** Total number of entries in the index. */
	TInt iTotal;
	/** Number of entries created so far. */
	TInt iCompleted;
	/** Number of entries remaining. */
	TInt iRemaining;
	/** ID of entry last created. */
	TMsvId iId;
	};


//**********************************
// TMsvIndexLoadProgress
//**********************************

class TMsvIndexLoadProgress : public TMsvServerOperationProgress
/** Progress information for an operation controlling the Message Server loading 
its index. 

Such an operation can occur when the Message Server changes drive.

@see CMsvOperation
@see CMsvSession::ChangeDriveL() 
@publishedAll
@released
*/
	{
public:
	/** States for the index loading process. 
@publishedAll
@released
*/
	enum TIndexState 
		{
		/** Index loading has not begun. */
		EIndexNotLoaded = 0, 
		/** Index loading is in progress. */
		EIndexLoading, 
		/** Index is being built. */
		EIndexRebuilding, 
		/** Index loading is complete. */
		EIndexComplete};
	//
public:
	IMPORT_C TMsvIndexLoadProgress();
	//
public:
	/** Stores any errors that prevented the index being loaded. */
	TInt iError;
	/** Current state of the index loading process. */
	TIndexState iState;
	/** Details of the index loading progress. */
	TMsvIndexProgress iIndex;
	};



//**********************************
// TMsvCopyProgress
//**********************************

/** Progress information for the copy operation. 

@see CMsvOperation
@see CMsvSession::CopyStoreL() 
@publishedAll
@released
*/
class TMsvCopyProgress : public TMsvServerOperationProgress
	{
public:
/** States for the copy operation. 
@publishedAll
@released
*/
	enum TCopyState 
		{
		/** Operation not yet started */
		ENotYetStarted = 0,
		/** Mail Store is being locked */
		ELock, 
		/** Initialising Copy operation */
		EInitCopy,
		/** Mail Store is being copied */
		ECopyStore, 
		/** Mail Store is being unlocked. */
		EUnlock,
		/** Operation completed */
		ECompleted
		};
		
	/** Stores any errors that the operation encountered. */
	TInt iError;
	TInt iTotal;      // Total number of items to be copied
	TInt iCurrent;    // Current item number being copied
 	TCopyState iState;
	
public:
    IMPORT_C TMsvCopyProgress();
	};


//**********************************
// TMsvDeleteProgress
//**********************************

/** Progress information for the delete operation.
 
@see CMsvOperation
@see CMsvSession::DeleteStoreL() 
@publishedAll
@released
*/
class TMsvDeleteProgress : public TMsvServerOperationProgress
	{
public:
/** States for the delete operation. 
@publishedAll
@released
*/
	enum TDeleteState 
		{
		/** Operation not yet started */
		ENotYetStarted = 0,
		/** Mail Store is being deleted */
		EDeleteStore, 
		/** Operation is complete */
		ECompleted
		};
		
	/** Stores any errors that the operation encountered. */
	TInt iError;
	TDeleteState iState;
	
public:
    IMPORT_C TMsvDeleteProgress();
	};



//**********************************
// TMsvPackedOperation
//**********************************
//
// Packs an operation, the selection, and some associated parameters (TInt) into the
// IPC buffer
//
//

class TMsvPackedOperation
/**
@internalComponent
@released
*/
	{
public:
	IMPORT_C TMsvPackedOperation(HBufC8*& aBuffer);
	IMPORT_C TInt Pack(const CMsvEntrySelection& aSelection, TInt aParameter1=0, TInt aParameter2=0);
	IMPORT_C void UnpackL(CMsvEntrySelection& aSelection, TInt& aParameter1, TInt& aParameter2);
private:
	HBufC8*& iBuffer;
	};


//**********************************
// TMsvPackedChangeNotification
//**********************************
//
// Packs the CMsvEntrySelection of index entries that have been changed, and also 
// the integer describing the type of change. There is a limit of KMsvPackedChangeLimit
// top the number of entry ids that can be sent at one time
//
//

typedef TBuf8<128> TMsvNotifBuffer;

class TMsvPackedChangeNotification
/**
@internalComponent
@released
*/
	{
public:
	enum {	KMsvChangeNotificationNumberOfTInts=4,
			KMsvPackedChangeLimit=28, // 32-KMsvChangeNotificationNumberOfTInts
			}; 
public:
	IMPORT_C TMsvPackedChangeNotification(TMsvNotifBuffer& aBuffer);
	inline void Pack(TMsvServerChangeNotificationType aChangeType, const CMsvEntrySelection& aSelection, TInt aParameter1, TInt aParameter2);
	IMPORT_C void Pack(TMsvServerChangeNotificationType aChangeType, const CMsvEntrySelection& aSelection, TInt aParameter1, TInt aParameter2, TInt aStartIndex, TInt aFinishIndex);
	IMPORT_C void Pack(TMsvServerChangeNotificationType aChangeType, TMsvId aId, TInt aParameter1, TInt aParameter2);
	IMPORT_C void UnpackL(TMsvServerChangeNotificationType& aChangeType, CMsvEntrySelection& aSelection, TInt& aParameter1, TInt& aParameter2);
private:
	TMsvNotifBuffer& iBuffer;
	};

//**********************************
// TMsvPackedEntryFilter
//**********************************

class TMsvPackedEntryFilter
/**
@internalComponent
@released
*/
	{
public:
	TMsvPackedEntryFilter(HBufC8*& aBuffer);
	TInt PackFilter(const CMsvEntryFilter& aFilter);
	void UnpackFilter(CMsvEntryFilter& aFilter);
private:
	HBufC8*& iBuffer;
	};

/**
Packs TMsvSearchSortQuery object into a buffer for sending across IPC. 
Also unpacks the buffer to TMsvSearchSortQuery
@internalComponent
@released
*/
class TMsvPackQuery
	{
public:
	IMPORT_C TMsvPackQuery(HBufC8*& aBuffer);
	IMPORT_C TInt PackQuery(const CMsvSearchSortQuery* aQuery);
	IMPORT_C void UnpackQuery(CMsvSearchSortQuery* aQuery);
private:
	TInt DoPackQuery(const TUint8*& aPtrStart, const TUint8* aPtrEnd, const CMsvSearchSortQuery* aQuery);
	void DoUnpackQuery(TUint8*& aPtr, CMsvSearchSortQuery* aQuery);
private:
	HBufC8*& iBuffer;
	};


/**
Packs RArray of TMsvId into a buffer for sending across IPC. 
Also unpacks the TMsvId's to RArray
@internalComponent
@released
*/
class TMsvPackedIdOperation
	{
public:
	IMPORT_C TMsvPackedIdOperation(HBufC8*& aBuffer);
	IMPORT_C TInt Pack(const RArray<TMsvId>& aId);
	IMPORT_C void UnpackL(RArray<TMsvId>& aId);
private:
	HBufC8*& iBuffer;
	};

/**
Packs RArray of TMsvId with Sort field into a buffer for sending across IPC. 
Also unpacks the TMsvId and Sort filed to RArray
@internalComponent
@released
*/
class TMsvPackedIdAndMessagePart
	{
public:
	IMPORT_C TMsvPackedIdAndMessagePart();
	IMPORT_C TInt Size(const RArray<TMsvIdWithSortField>& aData);
	IMPORT_C void ExternalizeL(RWriteStream& aWriteStream, RArray<TMsvIdWithSortField>& aData) const;
	IMPORT_C void InternalizeL(RReadStream& aReadStream, RArray<TMsvIdWithSortField>& aData);
	};


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
/**
@internalComponent
@released
*/
class TMsvPackedDriveIdOperation
	{
public:
	IMPORT_C TMsvPackedDriveIdOperation(HBufC8*& aBuffer);
	IMPORT_C TInt Pack(const RArray<TDriveNumber>& aDriveNumber);
	IMPORT_C void UnpackL(RArray<TDriveNumber>& aDriveNumber);
private:
	HBufC8*& iBuffer;
	};
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)



#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

/**
 * Pack unpack the header structure from a buffer
 * @internalComponent
 * @released
 */
class TMsvPackedHeaderStructure
	{
public:
	IMPORT_C TMsvPackedHeaderStructure(HBufC8*& aBuffer);
	IMPORT_C TInt Pack(const RPointerArray<CFieldPair>& aFieldDetails);
	IMPORT_C void UnpackL(RPointerArray<CFieldPair>& aFieldDetails);
private:
	HBufC8*& iBuffer;
	};


/**
 * Pack unpack the header data from a buffer
 * @internalComponent
 * @released
 */
class TMsvPackedHeaderData
	{
public:
	IMPORT_C TMsvPackedHeaderData(HBufC8*& aBuffer);
	IMPORT_C TInt Pack(const RPointerArray<CHeaderFields>& aFieldDetails);
	IMPORT_C void UnpackL(RPointerArray<CHeaderFields>& aFieldDetails);
private:
	HBufC8*& iBuffer;
	};

#endif		// #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
#include <msvipc.inl>

#endif

