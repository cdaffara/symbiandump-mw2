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
// MSVSTD.H
//
/**
 * @file 
 * @publishedAll
 * @released
 */
#ifndef __MSVSTD_H__
#define __MSVSTD_H__

#include <e32base.h>
#include <s32file.h>
#include <msvstd.hrh>

/**
Specifies an entry in the Message Server index.
@publishedAll
@released
@see #KMsvNullIndexEntryId
@see #KMsvTempIndexEntryId
@see #KMsvRootIndexEntryId
@see #KMsvLocalServiceIndexEntryId
@see #KMsvGlobalInBoxIndexEntryId
@see #KMsvGlobalOutBoxIndexEntryId
@see #KMsvDraftEntryId
@see #KMsvSentEntryId
@see #KMsvDeletedEntryFolderEntryId
@see #KMsvUnknownServiceIndexEntryId
*/
typedef TInt32 TMsvId;

/** Identifies an asynchronous messaging operation. 

This ID is unique within a 
Message Server session. The ID of an operation can be obtained from CMsvOperation::Id(). */
typedef TInt32 TMsvOp;

/** Supplies the standard values for the message priority index entry field. 
@publishedAll
@released
*/
enum TMsvPriority {	
	/** High message priority */
					EMsvHighPriority,
	/** Medium message priority */
					EMsvMediumPriority,
	/** Low message priority */
					EMsvLowPriority};	

class CRichText;
class CMsvStore;
class CMsvEntry;
class CMsvMove;
class CMsvServer;
class CMsvServerEntry;

_LIT(KMsvDefaultFolder, "\\Private\\1000484b\\Mail\\");
_LIT(KMsvDefaultIndexFile, "\\Private\\1000484b\\Mail\\Index");
_LIT(KMsvDefaultFolder2, "\\Private\\1000484b\\Mail2\\");
_LIT(KMsvDefaultIndexFile2, "\\Private\\1000484b\\Mail2\\Index");
_LIT(KMsvDirectoryExt, "_S\\");
_LIT(KMsvBinaryFolderExt, "_F\\");

const TInt KFileNameFixedWidth=8;
const TInt KMaxSubjectSkipStringLength=8; // For subject based email thread sorting
_LIT(KSendAsRenderedImage, "Image");

// Server name and moving semaphore
_LIT(KMsvServerName, "!MsvServer");
_LIT(KMsvMovingSemaphore, "MsvMovingSemaphore");

_LIT_SECURE_ID(KMsvServerId, 0x1000484B);


/** Represents an entry in the Message Server index.

MTM implementations and message client applications use it to access and manipulate 
index entry values. Note that a TMsvEntry is effectively a local cache of 
the index entry, and changes to it are not automatically written into the 
index. To commit changes to the Message Server, you need to call CMsvEntry::ChangeL() 
(or CMsvServerEntry::ChangeEntry()).

To avoid repetition, the functions and data members are simply listed here, 
without detailed explanation of the fields. The public data members can be 
used to get and set the values of some index entry fields. 
@publishedAll
@released
*/
class TMsvEntry
	{
public: 
	IMPORT_C TMsvEntry(); 
	IMPORT_C TMsvEntry(const TMsvEntry& aEntry);
	IMPORT_C TMsvEntry& operator=(const TMsvEntry& aEntry);
	IMPORT_C TBool operator==(const TMsvEntry& aEntry) const;
	IMPORT_C TBool PermanentDataUnchanged(const TMsvEntry& aEntry) const;
	//
	IMPORT_C TMsvPriority Priority() const;
	IMPORT_C void SetPriority(TMsvPriority aPriority);
	// 
	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
	// Read only for clients
	inline TMsvId Id() const;
	inline TMsvId Parent() const;
	inline TBool Owner() const;
	inline TBool Deleted() const;
	// Read & write for clients
	inline TBool Complete() const;
	inline void SetComplete(TBool aComplete);
	inline TBool New() const;
	inline void SetNew(TBool aNew);
	inline TBool Unread() const;
	inline void SetUnread(TBool aUnread);
	inline TBool Failed() const;
	inline void SetFailed(TBool aFailed);
	inline TBool Operation() const;
	inline void SetOperation(TBool aOperation);
	inline TBool Visible() const;
	inline void SetVisible(TBool aVisble);
	inline TBool MultipleRecipients() const;
	inline void SetMultipleRecipients(TBool aMultipleRecipient);
	inline TBool ReadOnly() const;
	inline void SetReadOnly(TBool aReadOnly);
	inline TBool StandardFolder() const;
	inline void SetStandardFolder(TBool aStanardFolder);
	inline TBool Attachment() const;
	inline void SetAttachment(TBool aAttachmentFlag);
	inline TBool Connected() const;
	inline void SetConnected(TBool aConnected);
	inline TBool InPreparation() const;
	inline void SetInPreparation(TBool aInPreparation);
	//
	inline TInt PcSyncCount() const;
	inline void DecPcSyncCount();
	inline void IncPcSyncCount();
	//
	inline TBool OffPeak() const;
	inline void SetOffPeak(TBool aOffPeak);
	inline TBool Scheduled() const;
	inline void SetScheduled(TBool aScheduled);
	inline TUint SendingState() const;
	inline void SetSendingState(TUint aSendingState);
	inline TBool PendingDelete() const;
	inline void SetPendingDelete(TBool aPendingDelete);
	
	/** This method sets bit 24 of iMtmData1, if you are using this bit elsewhere don't call this method.*/
	inline void SetDeleteProtected(TBool aDeleteProtected);
	inline TBool DeleteProtected() const;
	/** This method sets bit 31 of iMtmData1, if you are using this bit elsewhere don't call this method.*/
	inline void SetForwarded(TBool aForwarded);
	inline TBool Forwarded() const;
	/** This method sets bit 32 of iMtmData1, if you are using this bit elsewhere don't call this method.*/
	inline void SetLocallyDeleted(TBool aLocallyDeleted);
	inline TBool LocallyDeleted() const;
	inline void SetVisibleFolderFlag(TBool aReadOnly);
	inline TBool VisibleFolderFlag() const;
	
	IMPORT_C TBool PendingConditions() const;
	IMPORT_C void SetPendingConditions(TBool aPendingConditions);
	//
	IMPORT_C TInt32 MtmData1() const;
	IMPORT_C TInt32 MtmData2() const;
	IMPORT_C TInt32 MtmData3() const;

	IMPORT_C void SetMtmData1(TInt32 aMtmData);
	IMPORT_C void SetMtmData2(TInt32 aMtmData);
	IMPORT_C void SetMtmData3(TInt32 aMtmData);
	//
protected:
	// only the server will be able to set the following
	inline void SetId(TMsvId aId);
	inline void SetParent(TMsvId aId);
	inline void SetOwner(TBool aAttachment);
	inline void SetDeleted(TBool aDeletedFlag);
	//
	inline TUint PersistedFlags() const;
	inline TUint TemporaryFlags() const;
	//
private:
	void Copy(const TMsvEntry& aEntry);
	//
private:
	TMsvId iId;
	TMsvId iParentId;  
	TInt32 iData;         
	TInt32 iPcSyncCount;
	TInt32 iReserved; // reserved for future proofing
public: 
	/** Service ID. */
	TMsvId iServiceId; 
	/** Related folder ID. */
	TMsvId iRelatedId; 
	/** Entry type */
	TUid   iType;        
	/** MTM */
	TUid   iMtm;     
	/** Time */
	TTime  iDate;       
	/** Size */
	TInt32 iSize;      
	/** Error */
	TInt32 iError;
	/** BIO message type */
	TInt32 iBioType;
	// following three can be used by MTM
	/** MTM data 1: this can be used for any purpose by an MTM. This member must 
	only be accessed through the predefined set and get functions. If you must 
	write data to a bit that has no meaning in the predefined derived classes 
	then you must derive your own class from TMsvEntry.
	*/
	TInt32 iMtmData1; 
	/** MTM data 2: this can be used for any purpose by an MTM. This member must 
	only be accessed through the predefined set and get functions. If you must 
	write data to a bit that has no meaning in the predefined derived classes 
	then you must derive your own class from TMsvEntry.
	*/
	TInt32 iMtmData2; 
	/** MTM data 3: this can be used for any purpose by an MTM. This member must 
	only be accessed through the predefined set and get functions. If you must 
	write data to a bit that has no meaning in the predefined derived classes 
	then you must derive your own class from TMsvEntry.
	*/
	TInt32 iMtmData3; 
public:
	// The TPtrC must be kept at end of class to simplify comparisons and initialisation
	/** Description */
	TPtrC iDescription;
	/** Details */
	TPtrC iDetails;    
	//
	//
private:
	//
	

friend class CMsvEntry;
friend class CMsvServer;
friend class CMsvServerEntry;
friend class CMsvClientEntry;
friend class CMsvServerSession;
friend class TMsvServerEntry; 
friend class CMsvServerIndex;
friend class RMsvServerSession;
friend class CMsvLocalCopyOperation;
friend class CMsvMove;
friend class CMsvIndexRebuild;
friend class CMsvCopyEntry;
friend class CMsvDelete;
friend class CMsvIndexContext;
friend class CMsvEntryArray;
friend class CMsvIndexAdapter;
friend class CMsvDBAdapter;
friend class TSearchSortDbWrapper;

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
friend class CTestIndexAdapter;
friend class CTestDbAdapter;
friend class CTestVisibleFolder;
friend class CTestIndexTableEntry;
friend class CTestEntryFreePool;
friend class CTestIndexContext;
friend class CTestMsvServer;
friend class CTestMsvEntry;
friend class CTestSearchSortCacheEntry;
friend class CTestSearchSortCacheManager;
friend class CTestSearchSortDeltaCache;
friend class CTestSearchSortOperation;
friend class CTestSearchSortDBadapter;
friend class CTestSearchSortCacheManager;
friend class CTestSearchSortDeltaCache;
friend class CTestHeaderSearchSortOperation;
friend class CTestHeaderEntry;
#endif

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	friend class CMsvVersion0Version1Converter;
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestConverterDBAdapter;
#endif
#endif
private:
	//
enum {	KMsvEntryPriorityMask=			0x00000003,
		KMsvEntryHighPriority=			0x00000002,
		KMsvEntryMediumPriority=		0x00000001,
		KMsvEntryLowPriority=			0x00000000,
		KMsvEntryClearFlag=				0x00000000,
		KMsvOffPeakFlag=				0x00000004,
		KMsvEntryNotCompleteFlag=		0x00000008,
		KMsvEntryNewFlag=				0x00000010,
		KMsvEntryUnreadFlag=			0x00000020,
		KMsvEntryFailedFlag=			0x00000040,
		KMsvEntryOperationFlag=			0x00000080,
		KMsvEntryOwnerFlag=				0x00000100,
		KMsvEntryInvisibleFlag=			0x00000200,
		KMsvEntryMultipleRecipientFlag=	0x00000400,
		KMsvScheduledFlag=				0x00000800,
		KMsvEntryReadOnlyFlag=			0x00001000,
		KMsvEntryDeletedFlag=			0x00002000,
		KMsvEntryStandardFolderFlag=	0x00004000,
		KMsvEntryAttachmentFlag=		0x00008000,
		KMsvEntryInPreparationFlag=		0x00010000,
		KMsvSendingStateFlags=			0x001E0000, // Note: Sending state is shifted KMsvSendingStateShift places left
		KMsvEntryPersistedFlags=		0x00FFFFFF,
		KMsvEntryPendingConditionsFlag=	0x00800000,		
		KMsvEntryTemporaryFlags=		0xFF000000,
		KMsvEntryConnectedFlag=			0x01000000,
		KMsvEntryPendingDeleteFlag=		0x02000000,
	    KMsvVisibleFolderNodeFlag=      0x04000000
	   };
	   
	};
enum 
	{
		KMsvEntryDeleteProtected=		0x800000,
		KMsvEntryForwarded=				0x40000000,
		KMsvEntryLocallyDeleted=		0x80000000
	};

const TUint KMsvSendingStateShift=0x11; // Places to shift sending state

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	const TInt KDriveMask = 0x0fffffff;
	const TInt KCurrentDriveId = 1;
#endif

/** Defines sending state flags. 
@publishedAll
@released
*/
enum TMsvSendState
	{
	/** State unknown. */
	KMsvSendStateUnknown		= 0x0,
	/** Message will be sent on request. */
	KMsvSendStateUponRequest	= 0x1,
	/** Waiting to be sent.
	
	Messages in the outbox with this state can be sent automatically by the relevant 
	MTM once an appropriate connection is established. */
	KMsvSendStateWaiting		= 0x2,
	/** Sending in progress. */
	KMsvSendStateSending		= 0x3,
	/** Sending scheduled. */
	KMsvSendStateScheduled		= 0x4,
	/** Resending. */
	KMsvSendStateResend			= 0x5,
	/** Sending suspended. */
	KMsvSendStateSuspended		= 0x6,
	/** Sending failed. */
	KMsvSendStateFailed			= 0x7,
	/** Sending complete. */
	KMsvSendStateSent			= 0x8,
	/** Not applicable. */
	KMsvSendStateNotApplicable	= 0x9
	};

const TUint KMsvSendStateLast = KMsvSendStateNotApplicable;
const TUint KMsvSendStateMax = 0x0F;
// Message Database Version.
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
const TUint KCurrentDatabaseVersion = 2;
#else
const TUint KCurrentDatabaseVersion = 1;
#endif

/** Supplies values for bitmasks that allows the rapid setting or clearing of a 
number of TMsvEntry fields.

The fields that can be manipulated are:

1. PC synchronisation

2. Visibility flag

3. Read flag

4. In-preparation flag

5. Connected flag

6. Pending deletion flag.

7. New flag

@see TMsvEntry
@see CMsvSession::ChangeAttributesL()
@see CMsvServerEntry::ChangeAttributes() 
@publishedAll
@released
*/
enum TMsvAttribute    { 
	/** PC synchronisation field. */
						KMsvPcSyncCountAttribute	=0x00000001,
	/** Visibility flag field. */
						KMsvVisibilityAttribute		=0x00000200,
	/** Read flag field. */
						KMsvUnreadAttribute			=0x00000020,
	/** In-preparation field. */
						KMsvInPreparationAttribute	=0x00010000,
	/** Connected flag field. */
						KMsvConnectedAttribute		=0x01000000,
	/** Pending deletion flag. */
						KMsvPendingDeleteAttribute	=0x02000000,
	/** New flag field. */
						KMsvNewAttribute			=0x00000010};

/** Defines the sort order options for a TMsvSelectionOrdering object. Options 
are set through TMsvSelectionOrdering::SetSorting().

@see TMsvSelectionOrdering 
@publishedAll
@released
*/
enum TMsvSorting 
	{
	/** Don't sort */
	EMsvSortByNone=0,				// Don't sort
	/** Date (earliest-latest) */
	EMsvSortByDate,					// Date (earliest-latest)
	/** Date (latest-earliest) */
	EMsvSortByDateReverse,			// Date (latest-earliest)
	/** Size (smallest-largest) */
	EMsvSortBySize,					// Size (smallest-largest)
	/** Size (largest-smallest) */
	EMsvSortBySizeReverse,			// Size (largest-smallest)
	/** To/From (A-Z folded) */
	EMsvSortByDetails,				// ToFrom (A-Z collated)
	/** To/From (Z-A folded) */
	EMsvSortByDetailsReverse,		// ToFrom (Z-A collated)
	/** Description (A-Z folded) */
	EMsvSortByDescription,			// Description (A-Z collated)
	/** Description (Z-A folded) */
	EMsvSortByDescriptionReverse,	// Description (Z-A collated)
	/** Sort by message ID. */
	EMsvSortById,			        // Id (lowest-highest)
	/** Sort in reverse order by message ID. */
	EMsvSortByIdReverse	            // Id (highest-lowest)
	};

/** Defines grouping order options for a TMsvEntrySelection object. Grouping options 
can be combined in a bitmask and set through the TMsvEntrySelection constructor 
@publishedAll
@released
*/
enum TMsvGrouping
	{
	/** No grouping */
	KMsvNoGrouping=0,				// No grouping
	/** Group by entry type, in order, services, folders, messages, attachments */
	KMsvGroupByType=0x2,			// Folder, Message, Attachment
	/** Place standard folders first, in the order they were created */
	KMsvGroupByStandardFolders=0x6,	// Standrad folders first (must have sorting by type set aswell)
	/** Group by message priority, in order, high, medium, then low */
	KMsvGroupByPriority=0x8,		// High, Mediem, Low
	/** Group by MTM, either in the order set by CMsvEntry::SetMtmListL() or by MTM 
	Uid value */
	KMsvGroupByMtm=0x10,			// By increasing uid value
	};

/** Defines the sorting and grouping rules used to prepare a selection of entries.
 
It is used in the interfaces of CMsvEntry and CMsvServerEntry to set the ordering 
of an entry's children.

Firstly, the entries will be grouped, if the appropriate grouping option is 
set, by:

1. Standard folders: any entries marked as standard folders are placed first, 
in the order they were created.

2. Entry type: in the order services, folders, messages, attachments, unknown

3. Priority: in the order high, medium, then low

4. MTM: either in the MTM order set by CMsvEntry::SetMtmListL() or just in MTM 
Uid value

Grouping options are simply Boolean flags that can be set on or off by functions 
of this class.

Secondly, the order of entries within these groups is defined by a sorting 
order option.

@see CMsvEntry
@see CMsvServerEntry 
@publishedAll
@released
*/
class TMsvSelectionOrdering
	{
public:
	IMPORT_C TMsvSelectionOrdering();
	IMPORT_C TMsvSelectionOrdering(TInt aGroupingKey, TMsvSorting aSorting, TBool aShowInvisible=EFalse);
	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
	IMPORT_C TBool operator==(const TMsvSelectionOrdering& aOrdering) const;
	IMPORT_C void SetSubjectSkipString(const TDesC& aSubjectSkipString);
	IMPORT_C const TDesC& SubjectSkipString() const;

	//
	inline TMsvSorting Sorting() const;
	inline TBool GroupByType() const;
	inline TBool GroupStandardFolders() const;
	inline TBool GroupByPriority() const;
	inline TBool GroupByMtm() const;
	inline TBool ShowInvisibleEntries() const;
	inline TBool GroupingOn() const;
	// 
	inline void SetGroupByType(TBool aFlag);
	inline void SetGroupStandardFolders(TBool aFlag);
	inline void SetGroupByPriority(TBool aFlag);
	inline void SetGroupByMtm(TBool aFlag);
	inline void SetShowInvisibleEntries(TBool aFlag);
	inline void SetSorting(TMsvSorting aSortType);

	//
	//
private:

	enum TMsvGroupingPrivate
		{
		KMsvInvisibleFlag=0x1,
		KMsvStandardFolders=0x4,
		KMsvAllGroupingFlags=0x1e
		};
	TInt iGrouping;
	TMsvSorting iSortType;
 	TBuf<KMaxSubjectSkipStringLength>	iSubjectSkipString;
	};

const TInt KMsvInvisibleFlag=1;

/** Encapsulates an array of entry IDs. 

An object of this type is commonly passed to a function to indicate the set 
of entries on which the function should operate. The array base class (CArrayFixFlat<TMsvId>) 
provide methods to access, sort, and manipulate members of the array.
 
@publishedAll
@released
*/
class CMsvEntrySelection : public CArrayFixFlat<TMsvId>
	{ 
public: 
	IMPORT_C CMsvEntrySelection();
	IMPORT_C CMsvEntrySelection* CopyL()  const;
	IMPORT_C CMsvEntrySelection* CopyLC() const;
	IMPORT_C TInt Find(TMsvId aId) const;
	};

/** Specifies a default service for a MTM.

@publishedAll
@released
*/
class TMsvDefaultService
	{
public:
	/** MTM for which the default service is specified. */
	TUid iMtm;
	/** ID of the default service. */
	TMsvId iService;
	};



/** Used to query the Message Server for entries that match the specification defined 
in the filter. 

It allows a query to be performed on service, MTM, type and last change date. 
The filter also enables the specification of the order in which the entries 
are returned. 

@see CMsvSession::GetChildIdsL() 
@publishedAll
@released
*/
class CMsvEntryFilter : public CBase
	{
public:
	IMPORT_C static CMsvEntryFilter* NewLC();
	IMPORT_C static CMsvEntryFilter* NewL();
	//
public:
	inline TMsvId Service() const;
	inline void SetService(TMsvId aServiceId);
	//
	inline TUid Mtm() const;
	inline void SetMtm(TUid aMtm);
	//
	inline TUid Type() const;
	inline void SetType(TUid aType);
	//
	inline const TTime& LastChangeDate() const;
	inline void SetLastChangeDate(const TTime& aLastChange);
	//
	inline const TMsvSelectionOrdering& Order() const;
	inline void SetOrder(const TMsvSelectionOrdering& aOrder);
	//
	inline TUid SortMtm() const;
	inline void SetSortMtm(TUid aSortMtm);
	//
private:
	CMsvEntryFilter();
	//
private:
	TMsvId iServiceId;
	TUid iMtm;
	TUid iType;
	TTime iLastChange;
	TMsvSelectionOrdering iOrdering;
	TUid iSortMtm;
	};

// Error resolver leave codes -7000 to -7127
/** Defines error codes that relate to the Message Server.

@see MMsvEntryObserver::EMsvContextInvalid 
@publishedAll
@released
*/
enum TMsvResolverLeave
	{
	/** There is no disk in the drive containing the Message Server index. */
	KMsvMediaUnavailable = -7000,
	/** There is a disk in the drive containing the Message Server index, but it is 
	the wrong one. */
	KMsvMediaIncorrect = -7001,
	/** A CMsvEntry context is no longer valid because the location of the Message Server's 
	index has changed.
	
	Such contexts will need refreshing by the client.
	
	@see MMsvEntryObserver::EMsvContextInvalid */
	KMsvMediaChanged = -7002,
	KMsvIndexBackup      = -7003,
	KMsvIndexRestore     = -7004
	};

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

enum EFieldType
/**
Allowed field types.
This class is also used at the server side.
 
@publishedAll
@released
*/
	{
	EIntegerField,
	ETextField,
	EDateField
	};



// Common class used by both client and server.
class CFieldPair: public CBase
/**
Allowed field types.
This class is also used at the server side.
 
@publishedAll
@released
*/
	{
public:
	~CFieldPair()
		{
		if(iFieldName)
			{
			delete iFieldName;
			}
		if(iFieldTextValue)
			{
			delete iFieldTextValue;
			}
		}
	HBufC* iFieldName;			// Column name
	EFieldType iFieldType;		// Column type
	TInt64 iFieldNumValue;		// Column value in number
	HBufC* iFieldTextValue;		// Column value in text
	};
	


class CHeaderFields: public CBase
	{
public:
	~CHeaderFields()
		{
		iFieldPairList.ResetAndDestroy();
		}
	RPointerArray<CFieldPair> iFieldPairList;
	TUid iUid;
	};
	

/*
// Used for CMsvEntry::ReadDBStoreL() and CMsvEntry::ReadDBStoreL()
// Used for CMsvServerEntry::ReadDBStoreL() and CMsvServerEntry::ReadDBStoreL()

@publishedAll
@released
*/

enum TEntryStoreType
	{
	EHeaderEntry,
	EBodyEntry,
	EUnknownEntry,
	EHeaderAndBodyEntry		
	};


#endif 				// #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

/**
Temporary ID used for testing purpose
@publishedAll
@released
@see TMsvId
@see KMsvTempIndexEntryIdValue
*/
const TMsvId KMsvTempIndexEntryId = KMsvTempIndexEntryIdValue;//1

/**
@publishedAll
@released
@see TMsvId
*/
const TMsvId KFirstFreeEntryId=0x100000;
		

#include <msvstd.inl>

#endif
