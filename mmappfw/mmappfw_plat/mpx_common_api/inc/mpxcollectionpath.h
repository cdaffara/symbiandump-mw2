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
* Description:  Encapsulates a path into a collection
*
*/



#ifndef CMPXCOLLECTIONPATH_H
#define CMPXCOLLECTIONPATH_H

#include <mpxcommonframeworkdefs.h>

class RWriteStream;
class RReadStream;
class CMPXCollectionPathNode;
class TMPXAttribute;

/**
*  Encapsulates a 'bookmark' to a specific collection entry, i.e. encapsulates
*  a specific path in a collection hierarchy; you can navigate through the
*  items at this level.
*
*  @lib mpxcommon.lib
*/
class CMPXCollectionPath : public CBase
    {
public:
    /**
    *  Convenience enumeration to identify levels
    *  within a collection path.
    */
    enum TMPXCollectionPathLevel
        {
        ECollectionUid=0,
        ECollectionRoot=0 // Use ECollectionRoot+1, +2, etc as for other levels
        };

    enum TMPXPathChangeResult
        {
        EPathUnchanged=0,
        EPathModified,
        EPathClipped
        };

    /**
    * Collection path change type.
    *
    */
     enum TMPXCollectionPathChange
        {
        EAdded,         // Item changed
        EDeleted,       // Item deleted
        EModified,      // Item modified
        EGroupModified  // A "Group" modified, e.g. a playlist, an artist, etc
        };
public:
    /**
    *  Two-phase constructor.
    *
    *  @since S60 3.2.3
    *  @param aPath reference to an existing collection path
    *  @return object created
    */
    IMPORT_C static CMPXCollectionPath* NewL(const CMPXCollectionPath& aPath);

    /**
    *  Two-phase constructor.
    *
    *  @since S60 3.2.3
    *  @param aStream reference to a stream
    *  @return object created
    */
    IMPORT_C static CMPXCollectionPath* NewL(RReadStream& aStream);

    /**
    *  Two-phase constructor.
    *
    *  @since S60 3.2.3
    *  @return object created
    */
    IMPORT_C static CMPXCollectionPath* NewL();

    /**
    *  Destructor.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C virtual ~CMPXCollectionPath();

public: // Navigation through items at top level
    /**
    *  Go to next item.
    *
    *  @since S60 3.2.3
    *  @return ETrue if exists
    */
    IMPORT_C TBool operator++();

    /**
    *  Go to previous item.
    *
    *  @since S60 3.2.3
    *  @return ETrue if exists
    */
    IMPORT_C TBool operator--();

    /**
    *  Go to the first item.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C void SetToFirst();

    /**
    *  Go to the last item.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C void SetToLast();

    /**
    *  Go to specific item at top level.
    *
    *  @since S60 3.2.3
    *  @param aIndex index of the item
    *  @panic USER 130, if aIndex is out of bound
    *  @panic USER 0, if collection path is invalid(zero level)
    */
    IMPORT_C void Set(TInt aIndex);

    /**
    *  Go to specific item at top level.
    *
    *  @since S60 3.2.3
    *  @param aId id of the item
    *  @panic USER 0, if aId is invalid or invalid collection path
    */
    IMPORT_C void Set(const TMPXItemId& aId);

    /**
    *  Sets the open mode with which to navigate to the next level.
    *
    *  @since S60 3.2.3
    *  @param aMode the open mode
    *  @panic USER 0, if collection path is invalid(zero level)
    */
    IMPORT_C void Set(TMPXOpenMode aMode);

    /** 
    *  Sets the open attributes with which to navigate to the next level.
    *
    *  @since S60 3.2.3
    *  @param aAttrs the open attributes
    *  @panic USER 0, if collection path is invalid(zero level)
    *  @deprecated Use method CMPXCollectionPath::SetL(
    *    const TArray<TMPXAttribute>& aAttrs) instead.
    */
    IMPORT_C void Set(const TArray<TMPXAttribute>& aAttrs);

    /**
     *  Sets the open attributes with which to navigate to the next level.
     *
     *  @since S60 3.2.3
     *  @param aAttrs the open attributes
     *  @leave system error code
     *  @panic USER 0, if collection path is invalid(zero level)
     */
    IMPORT_C void SetL(const TArray<TMPXAttribute>& aAttrs);

public: // Selection of items at top level

    /**
    *  Select an item with the id in the path.
    *  If the id appears more than once in the path, Select(TInt aIndex) can
    *  be used.
    *
    *  @since S60 3.2.3
    *  @param aId ID of the item
    *  @panic USER 0 if aId is not found
    */
    IMPORT_C void SelectL(const TMPXItemId& aId);

    /**
    *  Select an item in the path.
    *
    *  @since S60 3.2.3
    *  @param aIndex index of the item
    *  @leave The function leaves with one of the system wide error codes,
    *         if the operation fails
    *  @panic USER 130, if aIndex is out of bound
    */
    IMPORT_C void SelectL(TInt aIndex);

    /**
     *  Select all of items at top level in the path.
     *
     *  @since S60 3.2.3
     *  @leave The function leaves with one of the system wide error codes,
     *         if the operation fails
     */
    IMPORT_C void SelectAllL();

    /**
    *  Deselects an item in the path.
    *
    *  @since S60 3.2.3
    *  @param aId ID of the item
    *  @panic USER 0, if aId was not selected
    */
    IMPORT_C void Deselect(const TMPXItemId& aId);

    /**
    *  Deselects an item in the path.
    *
    *  @since S60 3.2.3
    *  @param aIndex index of the item
    *  @panic USER 130, if aIndex is out of bound
    *  @panic USER 0, if aIndex was not selected
    */
    IMPORT_C void Deselect(TInt aIndex);

    /**
     *  Deselect all of items at top level in the path.
     *
     *  @since S60 3.2.3
     */
    IMPORT_C void DeselectAll();

    /**
    *  Removes an item in the path and adjust selection indices accordingly.
    *
    *  @since S60 3.2.3
    *  @param aId ID of the item
    *  @panic USER 0, if aId was not selected
    */
    IMPORT_C void Remove(const TMPXItemId& aId);

    /**
    *  Removes an item in the path and adjust selection indices accordingly.
    *
    *  @since S60 3.2.3
    *  @param aIndex index of the item
    *  @panic USER 0, if aIndex was not selected
    */
    IMPORT_C void Remove(TInt aIndex);

    /**
    *  Queries if item is selected.
    *
    *  @since S60 3.2.3
    *  @param aId ID of item
    *  @return whether selected
    */
    IMPORT_C TBool IsSelected(const TMPXItemId& aId) const;

    /**
    *  Queries if item is selected.
    *
    *  @since S60 3.2.3
    *  @param aIndex index of item
    *  @return whether selected
    */
    IMPORT_C TBool IsSelected(TInt aIndex) const;

    /**
    * Clears selection.
    *
    * @since S60 3.2.3
    */
    IMPORT_C void ClearSelection();

    /**
    * Array of currently selected indices.
    *
    * @since S60 3.2.3
    * @return current selected indices
    */
    IMPORT_C TArray<TInt> Selection() const;

    /**
    * Current selected ids.
    *
    * @since S60 3.2.3
    * @param aIds array of ids returned, id may be duplicated. e.g. music
    *        playlist can contains an item more than once.
    * @leave The function leaves with one of the system wide error codes,
    *        if the operation fails.
    */
    IMPORT_C void SelectionL(RArray<TMPXItemId>& aIds) const;

    /**
    * Update the item ID for a particular item.
    *
    * @since S60 3.2.3
    * @param aIndex index to update
    * @param aNewId the item id to set to the index
    */
    IMPORT_C void Update( TInt aIndex, TMPXItemId& aNewId );

public: // Information about top level

    /**
    * Current index.
    *
    * @since S60 3.2.3
    * @return current index
    */
    IMPORT_C TInt Index() const;

    /**
    * Current ID.
    *
    * @since S60 3.2.3
    * @return current ID
    */
    IMPORT_C const TMPXItemId& Id() const;

    /**
    * Number of items at the top level.
    *
    * @since S60 3.2.3
    * @return the number of items
    */
    IMPORT_C TInt Count() const;

    /**
    * The open mode with which to navigate to the next level.
    *
    * @since S60 3.2.3
    * @return the open mode for next level
    */
    IMPORT_C TMPXOpenMode OpenNextMode() const;

    /**
    * The open mode with which to navigate to the previous level.
    *
    * @since S60 3.2.3
    * @return the open mode for previous level
    */
    IMPORT_C TMPXOpenMode OpenPreviousMode() const;

    /**
    * Index from item id at the top level.
    *
    * @since S60 3.2.3
    * @param aId item id
    * @return index to the item with aId at the top level
    */
    IMPORT_C TInt IndexOfId(const TMPXItemId& aId) const;

    /**
    * Return the item id for the item at aIndex at the top level.
    *
    * @since S60 3.2.3
    * @return TMPXItemId if aIndex is valid
              KMPXInvalidItemId if invalid
    */
    IMPORT_C const TMPXItemId& IdOfIndex( TInt aIndex ) const;

    /**
    * Open attributes for next level.
    *
    * @since S60 3.2.3
    * @panic USER 0 if no level in the path
    */
    IMPORT_C const TArray<TMPXAttribute> OpenAttributes() const;

    /**
    * Retrieve a TArray of the top level items.
    *
    * @since S60 3.2.3
    * @panic USER 0 if no level in the path
    * @return TArray,
    */
    IMPORT_C const TArray<TMPXItemId> Items() const;

public: // Information about other levels

    /**
    * Return item index at a specific level.
    *
    * @since S60 3.2.3
    * @param aLevel level of depth
    * @return the index of item
    */
    IMPORT_C TInt Index(TInt aLevel) const;

    /**
    * Return item id at a specific level.
    *
    * @since S60 3.2.3
    * @param aLevel level of depth
    * @return the id of item
    */
    IMPORT_C const TMPXItemId& Id(TInt aLevel) const;

public: // functions about levels

    /**
    * Number of levels into the collection.
    *
    * @since S60 3.2.3
    * @return the levels count
    */
    IMPORT_C TInt Levels() const;

    /**
    * Back a level.
    *
    * @since S60 3.2.3
    * @panic USER 0 if no more level in the path
    */
    IMPORT_C void Back();

    /**
    * Append a level.
    *
    * @since S60 3.2.3
    * @param aIds IDs of items at this level
    */
    IMPORT_C void AppendL(const TArray<TMPXItemId>& aIds);

    /**
    * Append a level, where only single ID exists or is known.
    *
    * @since S60 3.2.3
    * @param aId id of item at this level
    */
    IMPORT_C void AppendL(const TMPXItemId& aId);

    /**
    * Insert an id at top level.
    *
    * @since S60 3.2.3
    * @param aId the id to be inserted
    * @param aPos the position where the id to be inserted. The position is
    *        relative to zero.
    * @leave Leave with system errror code
    * @panic USER 131 if a pos is negative or is greater than the number of ids
    *        at top top level
    */
    IMPORT_C void InsertL(const TMPXItemId& aId, TInt aPos);

    /**
    * Resets the collection path object.
    * removes all node arrays.
    * removes all top level ids.
    * removes all selection ids.
    *
    * @since S60 3.2.3
    */
    IMPORT_C void Reset();

    /*
    * Returns a collection path pointing to the container of this path.
    *
    * @since S60 3.2.3
    * @return CMPXCollectionPath* ownership transferred
    */
    IMPORT_C CMPXCollectionPath* ContainerPathL() const;

public:  // Handle update
    /**
    * Handle collection change.
    *
    * @since S60 3.2.3
    * @param aCollectionId collection id of changes
    * @param aId Id of item change
    * @param aDeprecatedId old Id of the item changed
    * @param aChange change type
    * @param aSelection, on return contains the valid selection
    *                    if the path is invalidated
    * @return TMPXPathChangeResult
    *
    */
    IMPORT_C TInt HandleChange(
                         const TUid& aCollectionId,
                         const TMPXItemId& aId,
                         const TMPXItemId& aDeprecatedId,
                         CMPXCollectionPath::TMPXCollectionPathChange aChange,
                         TInt& aSelection);

public:

    /**
    * Externalize a object of this class to steam.
    *
    * @since S60 3.2.3
    * @param aStream write stream
    */
    IMPORT_C void ExternalizeL(RWriteStream& aStream) const;

    /**
    * Internalize a object of this class from steam.
    *
    * @since S60 3.2.3
    * @param aStream read stream
    */
    IMPORT_C void InternalizeL(RReadStream& aStream);

private:

    /**
    * C++ default constructor.
    *
    * @since S60 3.2.3
    */
    CMPXCollectionPath();

    /**
    * 2nd phase constructor.
    *
    * @since S60 3.2.3
    * @param aPath collection path
    */
    void ConstructL(const CMPXCollectionPath& aPath);

    /**
    * 2nd phase constructor.
    *
    * @since S60 3.2.3
    */
    void ConstructL();

    /**
    * 2nd phase constructor.
    *
    * @since S60 3.2.3
    * @param aStream stream
    */
    void ConstructL(RReadStream& aStream);

    /**
    * The top level node of the path.
    *
    * @since S60 3.2.3
    * @panic USER 0 if no level in the path
    * @return the top level
    */
    CMPXCollectionPathNode& TopLevel();

    /**
    * The top level node of the path.
    *
    * @since S60 3.2.3
    * @panic USER 0 if no level in the path
    * @return the top level
    */
    const CMPXCollectionPathNode& TopLevel() const;

private:
    /// Node of path levels
    RPointerArray<CMPXCollectionPathNode> iNodeArray;
    /// Additional info of Top Level
    RArray<TMPXItemId> iIds; // item ids of top level
    RArray<TInt> iSelection; // selected item indices of top level, sorted array
    TMPXItemId   iInvalidId;
    };

#endif // CMPXCOLLECTIONPATH_H
