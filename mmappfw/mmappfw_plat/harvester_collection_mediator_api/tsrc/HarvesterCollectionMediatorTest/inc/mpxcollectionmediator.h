/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Thin dll to handle special collection handling events
*
*/


#ifndef CMPXCOLLECTIONMEDIATOR_H
#define CMPXCOLLECTIONMEDIATOR_H

#include <e32base.h>
#include <f32file.h>
#include <barsc.h>

#include <mpxcollectionutility.h>

// FOWARD DECLARATIONS
class CMPXMedia;
class CMPXMediaArray;
class CMPXCollectionPath;
class CMPXCollectionType;
class CMPXMoveItemHelper;
class MMPXMediatorObserver;
class MMPXMoveItemObserver;

/**
 *
 *  Mediator class to handle Music Player specific requirements
 *  ie: files with genre podcast must be stored in podcast db.  
 *
 *  @lib mpxcollectionmediator.lib
 */
class CMPXCollectionMediator : public CBase
    {
public:

    /**
    * Two-phased constructor.
	 *  @since S60 v3.2.3
    * @param aColUtil collection utility.
    * @param aObs observer.
    * @return New instance of CMPXCollectionMediator
    */
    IMPORT_C static CMPXCollectionMediator* NewL( MMPXCollection& aColUtil, 
                                                  MMPXMediatorObserver* aObs = NULL );

    /**
    * Two-phased constructor.
    * Only use to execute "CheckItemL" not for adding/moving in collection.
	 *  @since S60 v3.2.3
    * @return New instance of CMPXCollectionMediator
    */
    IMPORT_C static CMPXCollectionMediator* NewL();
    
    /**
    * Two-phased constructor.
	 *  @since S60 v3.2.3
    * @param aColUtil.
    * @return New instance of CMPXCollectionMediator
    */
    IMPORT_C static CMPXCollectionMediator* NewLC( MMPXCollection& aColUtil,                   
                                                   MMPXMediatorObserver* aObs = NULL );

    /**
    * Virtual destructor.
	 *  @since S60 v3.2.3
    */
    virtual ~CMPXCollectionMediator();

public: // New functions

    /**
    * Add a media to the collection. Upon successfull addition,
    * its KMPXMediaGeneralId will be updated.
	 *  @since S60 v3.2.3
    * @param aMediaProp media item.
    * @leave KErrArgument leave if not a supported media type
    */
    IMPORT_C void AddItemL( CMPXMedia*& aMediaProp );
    
    /**
    * Add a list of files to the collection db.
	 *  @since S60 v3.2.3
    * @param aArray list of media item.
    * @leave KErrArgument leave if not a supported media type
    */
    IMPORT_C void AddItemL( CMPXMediaArray& aMPArray );
    
    /**
    * Updates the media properties of a file.
    * Used for media objects where the caller does not know
    * the collection for which the item should belong to.
	 *  @since S60 v3.2.3
    *
    * @param aMediaProp media to update
    */
    IMPORT_C void SetItemL( CMPXMedia*& aMediaProp );
    
    /**
    * Updates media for a list of items.
    * Used for media objects where the caller does not know
    * the collection for which the item should belong to.
	 *  @since S60 v3.2.3
    *
    * @param aMPArray list of media item
    */
    IMPORT_C  void SetItemL( CMPXMediaArray& aMPArray );
                   
    /**
    * Update media for items in the same collection depth. Not implemented.
	 *  @since S60 v3.2.3
    * @param aArray list of items.
    * @param aPath collection path of the item.
    * @param aIndices list of indices
    */
    IMPORT_C  void SetItemL( TArray<CMPXMedia*>& aArray,
                             CMPXCollectionPath& aPath, 
                             RArray<TInt>& aIndices );
    
    /**
    * Static item to check if the collection id is correct.
    * Updates the collection path object to podcast if it is podcast.
	 *  @since S60 v3.2.3
    * @param aMedia media object
    */
    IMPORT_C  void CheckItemL( CMPXMedia& aMedia );
    
    /**
    * Moves an item from one collection to another.
    * Updates the media properties "genre" to Unknown
    * if moving from podcast -> local audio.
	 *  @since S60 v3.2.3
    * @param aMedia, properties of the object.
    * @param aOldCollectionId old collection id.
    * @param aNewCollectionId new collection id
    */
    IMPORT_C  void MoveItemL( CMPXMedia*& aMedia,
                              TUid aOldCollectionId,
                              TUid aNewCollectionId );                
   
    /**
    * Moves an item from one collection to another.
    * Updates the media properties "genre" to Unknown
    * if moving from podcast -> local audio.
	 *  @since S60 v3.2.3
    * @param aMedia properties of the object.
    * @param aOldCollectionId old collection id.
    * @param aNewCollectionId new collection id.
    * @param aObs observer for event
    */
    IMPORT_C  void MoveItemL( CMPXMedia*& aMedia,
                              TUid aOldCollectionId,
                              TUid aNewCollectionId,
                              MMPXMoveItemObserver* aObs ); 
                          
private: // New functions
    
    /**
    * Is this item recognizable as podcast.
	 *  @since S60 v3.2.3
    * @param aMedia media object.
    * @return ETrue if the file is a podcast EFalse otherwise 
    */
    TBool IsPodcastL( CMPXMedia& aMedia );
    
    /**
    * Change a collection id to the podcast db.
	 *  @since S60 v3.2.3
    * @param aMedia media to update the collection id to podcast 
    */
    void UpdatePathToPodcastL( CMPXMedia& aMedia );
    
    /**
    * Move a file from one db to another.
	 *  @since S60 v3.2.3
    * @param aMedia media object.
    * @param aNewCollection new colection
    */
    void DoMoveItemL( CMPXMedia*& aMedia, 
                      TUid aNewCollection );
                      
    /**
    * Add a file to the collection.
	 *  @since S60 v3.2.3
    * @param aMedia media to add.
    * @param aCollectionID collection to add the item
    */                
    void DoAddToCollectionL( CMPXMedia* aMedia,
                             TUid aCollectionId );
    
    
    /**
    * Set a file to the collection.
	 *  @since S60 v3.2.3
    * @param aMedia media to set.
    * @param aCollectionID collection to set the item
    */
    void DoSetToCollectionL( CMPXMedia* aMedia,
                             TUid aCollectionId );
    
private:

    /**
    * Private constructor.
	 *  @since S60 v3.2.3
    * @param aColUtil collection utility to share with the owner.
    * @param aObs observer to mediator operations
    */
    CMPXCollectionMediator( MMPXCollection& aColUtil,
                            MMPXMediatorObserver* aObs );
    
    /**
    * Private constructor.
	 *  @since S60 v3.2.3
    */
    CMPXCollectionMediator();

    /**
    * 2nd phase constructor.
	 *  @since S60 v3.2.3
    */
    void ConstructL();


private: // data
    MMPXCollection*   iColUtil;      // collection util, not owned
    CMPXMoveItemHelper* iMoveHelper; // Move item helper
    MMPXMediatorObserver* iObserver; // Observer
    RFs               iFs;           // FsSession (owned)
    HBufC*            iPodCastBuf;   // Localized string
    HBufC*            iPodCastEngBuf;// Podcast in english
    HBufC*            iUnknownBuf;   // Unknown text 
    TUid              iMusicCollectionID;   // Music Collection ID
    TUid              iPodCastCollectionID; // PodCast Collection ID
    };


#endif // CMPXCOLLECTIONMEDIATOR_H
