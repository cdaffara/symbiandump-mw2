/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Metadata engine Harverster Server client session*
*/

#ifndef __MDEHARVESTERSESSION_H__
#define __MDEHARVESTERSESSION_H__


#include <e32base.h>
#include <badesca.h>

#include <mdesession.h>

#include "mdscommoninternal.h"

class CMdESessionImpl;
class CMdCSerializationBuffer;

NONSHARABLE_CLASS(TGetPendingPgckWrapper)
	{
private:
	TDefId iObjectDefId;
	
public:

	TPckgC<TDefId> iObjectDefIdPckg;
	TPckg<TInt> iIdCountPckg;
	
	TGetPendingPgckWrapper(TDefId aObjectDefId, TInt& aIdCount) : 
		iObjectDefId(aObjectDefId), iObjectDefIdPckg(iObjectDefId), 
		iIdCountPckg(aIdCount)
		{
		}
	
	TInt GetIdCount()
		{
		return iIdCountPckg();
		}
	};

/**
 * Metadata Engine Harvester Server session.
 */
NONSHARABLE_CLASS(CMdEHarvesterSession) : public CBase
	{
public:
	/* Constructors and destructor. */

	/**
	 * Opens a new metadata engine harvester server session.
	 *
	 * @param aObserver  observer to notify when opening the session has been 
	 *                   completed
	 *
	 * @return  metadata engine session
	 */
	IMPORT_C static CMdEHarvesterSession* NewL(CMdESession& aSession);

	/**
	 * Opens a new metadata engine harvester server session and leaves the 
	 * session in the cleanup stack.
	 *
	 * @param aObserver  observer to notify when opening the session has been 
	 *                   completed
     *
	 * @return  metadata engine session
	 */
	IMPORT_C static CMdEHarvesterSession* NewLC(CMdESession& aSession);

	/**
	 * Destructor.
	 */
	~CMdEHarvesterSession();

	IMPORT_C void SetSession( CMdESession& aSession );
	
	IMPORT_C CMdESession& SessionRef();

	// DEPRICATED
	IMPORT_C CMdESession* Session();
	
	/**
	 * Add memory card with given media ID and current time. If memory card 
	 * already exist only time is updated to current time.
	 * 
	 * @param memory card's media ID
	 */
	IMPORT_C void AddMemoryCard(TUint32 aMediaId);

	/**
	 * Get latest memory card's media ID.
	 * @param returned memory card's media ID
	 * @return EFalse if there is no any memory cards added
	 */
	IMPORT_C TBool GetMemoryCard(TUint32& aMediaId);

    /**
	 * Check if there is memory card with given media ID already in DB.
	 * 
	 * @param memory card's media ID
	 * @param returned boolean if memory card exists or not
	 * 
	 * @return EFalse if there was error
	 * 
     * @depricated
	 */
	IMPORT_C TBool CheckMemoryCard(TUint32 aMediaId, TBool& aExist);
    
	/**
	 * Set media's media ID, drive and present state.
	 * 
	 * @param aMediaId media's media ID
	 * @param aDrive media's drive
	 * @param aPresentState media's present state
	 */
	IMPORT_C void SetMediaL(TUint32 aMediaId, TChar aDrive, 
			TBool aPresentState);

	/**
	 * Get media's drive and present state by media ID.
	 * 
	 * @param aMediaId media's media ID
	 * @param aDrive returned media's drive
	 * @param aPresentState returned media's present state
	 * @return EFalse if there is no media with matching media ID in DB
	 */
	IMPORT_C TBool GetMediaL(TUint32 aMediaId, TChar& aDrive, 
			TBool& aPresentState);

    /**
	 * Get present medias' media IDs and drives.
	 * 
	 * @param aMedias returned present medias' media IDs and drives
	 */
	IMPORT_C void GetPresentMediasL(RArray<TMdEMediaInfo>& aMedias);
	
    /**
     * Set file to present state.
     * 
     * @param aMediaId file's media ID
     * @param aUri file's URI
     * @param aFileInfo file's modified date and byte size
     *
     * @return ETrue if file existed and changing to present state was 
     *               successful
     * 
     * @depricated
     */
	IMPORT_C TBool SetFileToPresent(TUint32 aMediaId, const TDesC& aUri, 
	                               TMdSFileInfo& aFileInfo);
    
	/**
	 * Set files to present state.
	 * 
	 * @param aMediaId file's media ID
	 * @param aUris files' URIs
	 * @param aFileInfos files' modified dates and byte sizes
	 * @param aResults Values from TFilePresentStates
	 */
	IMPORT_C void SetFilesToPresentL(TUint32 aMediaId, 
			const RArray<TPtrC>& aUris,
			const RArray<TMdSFileInfo>& aFileInfos, 
			RArray<TFilePresentStates>& aResults);
	
    /**
     * Set all files, which media ID is given, to not present or start up not 
     * present state.
     *
     * @param aMediaId memory card's media ID
     * @deprecated
     */
	IMPORT_C void SetFilesToNotPresent(TUint32 aMediaId);

    /**
     * Remove all objects, which are in not present state, with given media ID
     * 
     * @param aMediaId memory card's media ID
     * @deprecated
     */
	IMPORT_C void RemoveFilesNotPresent(TUint32 aMediaId);
	
	/**
     * Set all files, which media ID is given, to not present state.
     * If aMediaId is 0, call is silently ignored.
     *
     * @param aMediaId memory card's media ID
     * @param aStartUp is start up or normal mount
     */
	IMPORT_C void SetFilesToNotPresent(TUint32 aMediaId, TBool aStartUp);

    /**
     * Remove all objects, which are in not present state, with given media ID.
     * 
     * @param aMediaId memory card's media ID
     * @param aStartUp is start up or normal mount
     */
	IMPORT_C void RemoveFilesNotPresent(TUint32 aMediaId, TBool aStartUp);

	/**
	 * Create global harvester prioritization chunk and set it to MdE Server. 
	 * There can be only one chunk and observer at once. If MdE Server already 
	 * contains active observer, old observer completes with KErrAbort, chunk 
	 * is always changed to this new one. Observer is added using 
	 * DoAddHarvestingPrioritizationObserver.
	 *
     * @param aSize   the size of global chunk that will receive object 
     *                URIs which need to be prioritize
	 */
	IMPORT_C void SetHarvestingPrioritizationChunkL( TInt aSize );

	/**
	 * Add harvester prioritization observer to MdE Server. There can be 
	 * only one chunk and observer at once. If MdE Server already contains 
	 * active observer, aStatus completes with KErrAlreadyExists. When MdE 
	 * Server needs prioritize object, aStatus completes with KErrNone.
	 *
     * @param aStatus the asynchronous response status which is changed 
     *                when there is object URIs to be prioritize
	 */
	IMPORT_C void AddHarvestingPrioritizationObserver(TRequestStatus& aStatus);

	/**
	 * Cancel harvester prioritization observer from MdE Server. 
	 *
	 * @return If MdE Server doesn't have outstanding observer method 
	 *         returns KErrNotFound, otherwise KErrNone is returned.
	 */
	IMPORT_C TInt CancelHarvestingPrioritizationObserver();

	/**
	 * Return harvester prioritization URI count. Data is valid only after 
	 * AddHarvestingPrioritizationObserver aStatus is completed.
	 *
	 * @return harvester prioritization URI count
	 */
	IMPORT_C TInt HarvestingPrioritizationUriCountL();

	/**
	 * Return harvester prioritization URI from index aIndex. Data is valid 
	 * only after AddHarvestingPrioritizationObserver aStatus is completed.
	 *
	 * @return harvester prioritization URI, ownership is transfered to caller
	 */
	IMPORT_C HBufC* HarvestingPrioritizationUriL( TInt aIndex );

	/**
	 * Objects are left locked automatically after those are added to DB. Only 
	 * properties and freetexts can be modified after those objects are added 
	 * to DB. Objects' locking must be removed by commiting those to DB or 
	 * canceling object.
	 *
	 * @param aObjects Array of object to be auto locked after adding
	 */
	IMPORT_C void AutoLockL( RPointerArray<CMdEObject>& aObjects );
	
	/**
	 * Change path of objects.
	 * 
	 * @param aOldPath old path
	 * @param aNewPath new path
     * @param aStatus the asynchronous response status which is changed when 
     *                URIs are changed
	 */
	IMPORT_C void ChangePath( const TDesC& aOldPath, const TDesC& aNewPath,
			TRequestStatus& aStatus );
	
	/**
	 * Update current C-drive media id to all objects in DB that have the old media id.
	 */
	IMPORT_C void ChangeCDriveMediaId();

	/**
	 * Set "binary composing to file" pending flag to objects
	 *
	 * @param aObjectId object IDs
	 */
	IMPORT_C void SetPendingL( const RArray<TItemId>& aObjectIds );

	/**
	 * Reset "binary composing to file" pending flag from objects
	 *
	 * @param aObjectId object IDs
	 */
	IMPORT_C void ResetPendingL( const RArray<TItemId>& aObjectIds );

	/**
	 * Get count of objects with "binary composing to file" pending flag
	 *
	 * @param aObjectDef object definition which limits what objects are
	 *                   returned or NULL when all objects are returned
	 * 
	 * @return count of objects
	 */
	IMPORT_C TInt GetPendingCount( const CMdEObjectDef* aObjectDef );

	/**
	 * Get objects with "binary composing to file" pending flag
	 *
	 * @param aObjectDef object definition which limits what objects are
	 *                   returned or NULL when all objects are returned
	 * @param aObjectIdCount count of returned object IDs
	 * @param aObjectIdBuffer serialized buffer of returned object IDs
	 *
	 * @return If returned value is > 0, buffer is not large enough for 
	 * 		    objects ID from DB, otherwise error code is returned.
	 */
	IMPORT_C TInt GetPending( 
			const CMdEObjectDef* aObjectDef, TInt& aObjectIdCount, 
			CMdCSerializationBuffer& aObjectIdBuffer );

	/**
	 * Reset DB
	 * only for internal use (works in DEBUG ONLY!!)
	 */
	IMPORT_C void ResetDBL();

	/**
	 * Update DB with current internal mass storage media id
	 */
	IMPORT_C void CheckMassStorageMediaId( const TUint32 aMediaId );

private:
	CMdESessionImpl* iSession;

	RChunk iHarvestingPrioritizationChunk;

	CMdCSerializationBuffer* iHarvestingPrioritizationSerializationBuffer;
	
protected:

	/* Constructors. */

	/**
	 * Constructor.
	 */
	CMdEHarvesterSession(CMdESession& aSession);

	/**
	 * Second-phase constructor.
	 */
	void ConstructL();
	};

#endif  // __MDEHARVESTERSESSION_H__
