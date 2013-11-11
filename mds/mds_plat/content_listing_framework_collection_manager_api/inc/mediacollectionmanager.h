/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*  Description :
*
*/


#ifndef MMEDIACOLLECTIONMANAGER_H
#define MMEDIACOLLECTIONMANAGER_H


#include <badesca.h>

/// Propertie flags for media collections
enum TMediaCollectionProperties
    {
    /// Collection items are in Phone memory
    EMCPhoneMemoryCollection = 0x00000001,
    /// Collection items are in MMC
    EMCMmcCollection = 0x00000002
    };

class MMediaCollectionManager;
class MMediaCollectionInfoExt;
class MMediaCollectionManagerExt;
class MMediaCollectionObserverExt;

/**
 *  Factory for Media Collection Manager
 *
 *  @lib MediaCollectionManager.lib
 *  @since S60 3.2
 */
class MediaCollectionManagerFactory
    {
public: // New functions

	/**
	 * Create new instance of Media Collection Manager
	 * @return New instance of Media Collection Manager.
	 *         Ownership is transferred to the client application.
	 */
	IMPORT_C static MMediaCollectionManager* CreateCollectionManagerL();

private:
	// Prohibit C++ default constructor.
	MediaCollectionManagerFactory();
	// Prohibit Destructor.
	~MediaCollectionManagerFactory();
    };

/**
 *  Media Collection Info interface,
 *   use MMediaCollectionManager to get collections
 *
 *  @lib N/A
 *  @since S60 3.2
 */
class MMediaCollectionInfo
    {
public:
	virtual ~MMediaCollectionInfo() {}

public:
	/**
	* Returns the name of the collection
	* @since S60 3.2
	* @return Name of the collection
	*/
	virtual const TDesC& Name() const = 0;

	/**
	* Returns the id of the album
	* @since S60 3.2
	* @return The id of the album
	*/
	virtual TInt Id() const = 0;

	/**
	* Returns list of media types that are defined for the collection.
	* See media types from CLFContentListing.hrh TCLFMediaType
	* @since S60 3.2
	* @return List of media types that are defined for the collection.
	*/
	virtual const RArray<TInt>& CollectionItemTypes() const = 0;
	
	/**
	* Returns collection propertie flags
	* Properties are defined in TMediaCollectionProperties
	* Should not be used in 5.0 or later!
	* @since S60 3.2, depricated in 5.0
	* @return Properties flags
	*/
	virtual TInt32 Properties() const = 0;

private: // Extension interface

	/**
	* This member is internal and not intended for use.
	*/
	virtual MMediaCollectionInfoExt* Extension()
						{ return NULL; }
	virtual const MMediaCollectionInfoExt* Extension() const
						{ return NULL; }
    };


/**
 *  Observer interface to get notification about changes in
 *   Media Collection Manager
 *
 *  @lib N/A
 *  @since S60 3.2
 */
class MMediaCollectionObserver
    {
public:
	/**
	* Collection manager uses this method to notify about
	*  created/deleted/modified collections.
	* @since 3.2
	* @param aCollectionIdArray
	*/
	virtual void CollectionChangedL(
						const TArray<TInt>& aCollectionIdArray ) = 0;

	/**
	* Collection manager uses this method to notify about
	*  possible errors.
	* @since 3.2
	* @param aError Collection manager error status
	*               KErrCorrupt if collection manager is corrupted and
	*               possible some data is lost
	*/
	virtual void CollectionManagerErrorL(
						TInt aError ) = 0;

protected:

	/**
	* Destructor.
	*/
	virtual ~MMediaCollectionObserver() {}

private: // Extension interface

	/**
	* This member is internal and not intended for use.
	*/
	virtual MMediaCollectionObserverExt* Extension()
						{ return NULL; }
	virtual const MMediaCollectionObserverExt* Extension() const
						{ return NULL; }
    };

/**
 *  Media Collection Manager
 *
 *  Use this class to get Media Collections, create new collections,
 *   delete collections, rename collections, add files to collections,
 *   remove files from collections.
 *
 *  @lib N/A
 *  @since S60 3.2
 */
class MMediaCollectionManager
    {
public:
	virtual ~MMediaCollectionManager() {}

public: // New functions

	/**
	* Return collection info by index.
	* @since 3.2
	* @param aCollectionInfoArray Collection infos are added to this array
	*/
	virtual void GetCollectionInfosL(
						RPointerArray<MMediaCollectionInfo>&
											aCollectionInfoArray ) const = 0;

	/**
	* Return collection info by ID.
	* @since 3.2
	* @param aId Collection ID
	* @return Pointer to a MMediaCollectionInfo object
	*         Ownership is transferred to client
	*/
	virtual MMediaCollectionInfo* CollectionInfoByIdLC(
						TInt aId ) const = 0;

	/**
	* Create new collection.
	* @since 3.2
	* @param aName Name of the collection
	* @param aCollectionItemTypes List of media types that are
	*         in the collection.
	*        Use media types from CLFContentListing.hrh TCLFMediaType.
	*        NOTE! that collection manager doesn't check is added item defined
	*              mediatype. This is only for client(s) for grouping different
	*              type folders.
	* @param aStorage In 3.2 See TMediaCollectionProperties
	*        NOTE! If given propertie set is not supported then
	*              function leaves with KErrNotSupported
	         NOTE! In 5.0 Drive number where the collection is to be created!
	* @return ID of the creaded collection
	*/
	virtual TInt CreateCollectionL(
						const TDesC& aName,
						const TArray<TInt>& aCollectionItemTypes,
						TInt32 aStorage) = 0;

	/**
	* Delete collection.
	* NOTE! this doesn't delete items that are in collection.
	* @since 3.2
	* @param aId ID of the collection
	*/
	virtual void DeleteCollectionL(
						TInt aId ) = 0;

	/**
	* Rename collection.
	* @since 3.2
	* @param aId ID of the collection.
	*            If collection doesn't found then
	*            function leaves with KErrNotFound
	* @param aNewName new name of the collection
	*/
	virtual void RenameCollectionL(
						TInt aId,
						const TDesC& aNewName ) = 0;

	/**
	* Add item(s) to collection.
	* @since 3.2
	* @param aId Collection ID
	*            If collection doesn't found then
	*            function leaves with KErrNotFound
	* @param aItemArray
	* @param aRemoveFromOtherCollections Is item removed from other
	*                                    available collections.
	*/
	virtual void AddToCollectionL(
						TInt aId,
						const MDesCArray& aItemArray,
						TBool aRemoveFromOtherCollections = ETrue ) = 0;

	/**
	* Remove item(s) from collection.
	* @since 3.2
	* @param aId Collection ID
	*            If collection doesn't found then
	*            function leaves with KErrNotFound
	* @param aItemArray
	*/
	virtual void RemoveFromCollectionL(
						TInt aId,
						const MDesCArray& aItemArray ) = 0;

	/**
	* Remove item(s) from all collections.
	* @since 3.2
	* @param aItemArray
	*/
	virtual void RemoveFromAllCollectionsL(
						const MDesCArray& aItemArray ) = 0;


	/**
	* Get collection items.
	* @since 3.2
	* @param aId Collection ID
	*            If collection doesn't found then
	*            function leaves with KErrNotFound
	* @param aItemArray Item are added to the array
	*/
	virtual void GetCollectionItemArrayL(
						TInt aId,
						CDesCArray& aItemArray ) const = 0;

	/**
	* Find collections IDs by item.
	* @since 3.2
	* @param aItem
	* @param aIdArray Possible collection IDs are added to the array
	*/
	virtual void GetCollectionIdByItemL(
						const TDesC& aItem,
						RArray<TInt>& aIdArray ) const = 0;

	/**
	* Add collection observer.
	* @since 3.2
	* @param aObserver
	*/
	virtual void AddCollectionObserverL(
						MMediaCollectionObserver& aObserver ) = 0;

	/**
	* Remove collection observer.
	* @since 3.2
	* @param aObserver
	*/
	virtual void RemoveCollectionObserverL(
						MMediaCollectionObserver& aObserver ) = 0;

	/**
	* Get the drivenumber of the drive where collection is located by collection id
	* @since 5.0
	* @param aId id of the collection
	* @return DriveNumber of the drive where collection is located
	*/						
	virtual TInt32 GetDriveByIdL( TInt aId ) = 0;

private: // Extension interface

	/**
	* This member is internal and not intended for use.
	*/
	virtual MMediaCollectionManagerExt* Extension()
						{ return NULL; }
	virtual const MMediaCollectionManagerExt* Extension() const
						{ return NULL; }

    };

#endif // MMEDIACOLLECTIONMANAGER_H
