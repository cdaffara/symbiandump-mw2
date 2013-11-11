/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class to help with moving items between collections
*
*/


#ifndef CMPXMOVEITEMHELPER_H
#define CMPXMOVEITEMHELPER_H

#include <e32base.h>
#include <mpxcollectionobserver.h>

class CMPXMedia;
class MMPXCollectionUtility;
class MMPXMoveItemObserver;

/**
 *  Class to help move items between collections
 *
 *  @lib mpxcollectionmediator.lib
 *  @since S60 3.0
 */
class CMPXMoveItemHelper  : public CBase,
                            public MMPXCollectionFindObserver
    {

public:

    /**
    * Two-phase constructor
    * @param aObs observer to the move item helper
    */
    IMPORT_C static CMPXMoveItemHelper* NewL( MMPXMoveItemObserver* aObs );

    /**
    * Two-phase construcotr
    * @param aObs observer to the move item helper
    */
    IMPORT_C static CMPXMoveItemHelper* NewLC( MMPXMoveItemObserver* aObs );

    /**
    * Virtual destructor
    */
    virtual ~CMPXMoveItemHelper();

public: // New Functions
    
    /**
    * Move an item from one collection to another, asynchronous
    * @param aMedia Source item, must contain item id or uri and original collection
    * @param aNewCollection Destinaton collection to move the item to
    */
    IMPORT_C void MoveItemL( CMPXMedia& aMedia, TUid aNewCollection );

    /**
    * Gets the full details of an item then
    * move an item from one collection to another, asynchronous
    * @param aMedia Source item, must contain item id or uri and original collection
    * @param aNewCollection Destinaton collection to move the item to
    */
    IMPORT_C void FetchAndMoveItemL( const CMPXMedia& aMedia, TUid aNewCollection );
    
    /**
    * Gets the full details of an item then
    * move an item from one collection to another, synchronous
    * @param aMedia Source item, must contain item id or uri and original collection
    * @param aNewCollection Destinaton collection to move the item to
    */
    IMPORT_C void FetchAndMoveItemSyncL( const CMPXMedia& aMedia, TUid aNewCollection );   

private: // New functions

    /**
    * Updates a media with any special move requirements
    * ie: move from podcast -> music will set genre to "unknown"
    * @param aMedia, media to update
    * @param aOldCollection, old collection
    * @param aNewCollection, new collection
    */
    void UpdateMediaForMoveL( CMPXMedia& aMedia, 
                              TUid& aOldCollection,
                              TUid& aNewCollection );
    
    /**
    * Handles the completion of a FindAllL() 
    * @param aResult result of the find
    */
    void DoHandleFindAllL( const CMPXMedia& aResult );
    
    /**
    * Add an item to a collection
    * @param aMedia, media to add
    * @param aCollectionId, collection UID
    */
    void DoAddL( CMPXMedia* aMedia, TUid aCollectionId );
    
    /**
    * Remove an item to a collection
    * @param aMedia, media to add
    * @param aCollectionId, collection UID
    */
    void DoRemoveL( CMPXMedia* aMedia, TUid aCollectionId );
    
protected:
     
     /**
     * From MMPXCollectionFindObserver
     */
     void HandleFindAllL(const CMPXMedia& aResults,
                         TBool aComplete,TInt aError);

private: 
    /* State representation for the move event */
    enum TMoveState
        {
        EIdle, // Not moving
        EFind  // Fetching full details
        };
                
private:

    /**
    * Private constructor
    * @param aObs observer to the move helper
    */
    CMPXMoveItemHelper( MMPXMoveItemObserver* aObs );

    /**
    * 2nd phase constructor
    */
    void ConstructL();

private: // data
    MMPXMoveItemObserver* iObs;
    MMPXCollectionUtility* iCollection;
    
    // Cache the async state
    TMoveState iMoveState;
    TUid       iMoveTarget;
    TUid       iMusicCollectionID;   // Music Collection ID
    TUid       iPodCastCollectionID; // PodCast Collection ID
    };

#endif // CMPXMOVEITEMHELPER_H
