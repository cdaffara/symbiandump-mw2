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
* Description:  implementation of collection path
*
*/

#include <s32strm.h>
#include <e32panic.h>
#include <mpxuser.h>
#include <mpxcmn.h>
#include <mpxattribute.h>
#include <mpxdata.h>
#include "mpxcollectionpath.h"

/**
* Encapsulates single node in collection hierarchy level. A node represents a
* level of the collection path
*/
NONSHARABLE_CLASS(CMPXCollectionPathNode) : public CBase
    {
public:
    static CMPXCollectionPathNode* NewL();
    static CMPXCollectionPathNode* NewLC();
    static CMPXCollectionPathNode* NewLC(const CMPXCollectionPathNode& aNode);
    virtual ~CMPXCollectionPathNode();
public:
    /**
    * Sets current item at this level
    */
    void Set(const TMPXItemId& aId, TInt aIndex);

    /**
    * Sets the next level open mode
    */
    void Set(TMPXOpenMode aRequest);

    /**
    * Sets the next level open attributes
    */
    void SetL(const TArray<TMPXAttribute>& aAttrs);

    /**
    * Current index
    */
    TInt Index() const;

    /**
    * Current Id
    */
    const TMPXItemId& Id() const;

    /**
    * Open mode for next level
    */
    TMPXOpenMode OpenMode() const;

    /**
    * Open attributes for next level
    */
    const TArray<TMPXAttribute> OpenAttributes() const;

public:
    void ExternalizeL(RWriteStream& aStream) const;
    void InternalizeL(RReadStream& aStream);

private:
    CMPXCollectionPathNode();
    void ConstructL(const CMPXCollectionPathNode& aNode);
    void ConstructL();

private:
    TMPXItemId iId;
    TInt iIndex;
    TMPXOpenMode iOpenMode;
    RArray<TMPXAttribute> iAttrs; // Open attributes for next level
    };


// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::NewL
// -----------------------------------------------------------------------------
//
CMPXCollectionPathNode* CMPXCollectionPathNode::NewL()
    {
    CMPXCollectionPathNode* self = NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::NewLC
// -----------------------------------------------------------------------------
//
CMPXCollectionPathNode* CMPXCollectionPathNode::NewLC()
    {
    CMPXCollectionPathNode* self = new(ELeave) CMPXCollectionPathNode();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::NewLC
// -----------------------------------------------------------------------------
//
CMPXCollectionPathNode* CMPXCollectionPathNode::NewLC(
    const CMPXCollectionPathNode& aNode)
    {
    CMPXCollectionPathNode* self = new(ELeave) CMPXCollectionPathNode();
    CleanupStack::PushL(self);
    self->ConstructL(aNode);
    return self;
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::ConstructL
// -----------------------------------------------------------------------------
//
void CMPXCollectionPathNode::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::ConstructL
// -----------------------------------------------------------------------------
//
void CMPXCollectionPathNode::ConstructL(
    const CMPXCollectionPathNode& aNode)
    {
    iId=aNode.iId;
    iIndex=aNode.iIndex;
    iOpenMode=aNode.iOpenMode;
    ::CopyArrayL<TMPXAttribute>(aNode.iAttrs.Array(), iAttrs);
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::CMPXCollectionPathNode
// -----------------------------------------------------------------------------
//
CMPXCollectionPathNode::CMPXCollectionPathNode()
:   iId(KMPXInvalidItemId),
    iIndex(KErrNotFound),
    iOpenMode(EMPXOpenGroupOrPlaylist)
    {
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::~CMPXCollectionPathNode
// -----------------------------------------------------------------------------
//
CMPXCollectionPathNode::~CMPXCollectionPathNode()
    {
    iAttrs.Close();
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::Set
// -----------------------------------------------------------------------------
//
void CMPXCollectionPathNode::Set(const TMPXItemId& aId, TInt aIndex)
    {
    iId=aId;
    iIndex=aIndex;
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::Set
// -----------------------------------------------------------------------------
//
void CMPXCollectionPathNode::Set(TMPXOpenMode aMode)
    {
    iOpenMode=aMode;
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::Set
// -----------------------------------------------------------------------------
//
void CMPXCollectionPathNode::SetL(const TArray<TMPXAttribute>& aAttrs)
    {
    ::CopyArrayL(aAttrs, iAttrs);
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::Index
// -----------------------------------------------------------------------------
//
TInt CMPXCollectionPathNode::Index() const
    {
    return iIndex;
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::Id
// -----------------------------------------------------------------------------
//
const TMPXItemId& CMPXCollectionPathNode::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::OpenMode
// -----------------------------------------------------------------------------
//
TMPXOpenMode CMPXCollectionPathNode::OpenMode() const
    {
    return iOpenMode;
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPathNode::OpenAttributes
// -----------------------------------------------------------------------------
//
const TArray<TMPXAttribute> CMPXCollectionPathNode::OpenAttributes() const
    {
    return iAttrs.Array();
    }

// -----------------------------------------------------------------------------
// Externalize object
// -----------------------------------------------------------------------------
//
void CMPXCollectionPathNode::ExternalizeL(RWriteStream& aStream) const
    {
    aStream.WriteUint32L(iId.iId1);
    aStream.WriteUint32L(iId.iId2);
    aStream.WriteInt32L(iIndex);
    aStream.WriteInt32L(iOpenMode);
    ::ExternalizeL(iAttrs.Array(), aStream);
    }

// -----------------------------------------------------------------------------
// Internalize object
// -----------------------------------------------------------------------------
//
void CMPXCollectionPathNode::InternalizeL(RReadStream& aStream)
    {
    TUint32 id1 = aStream.ReadUint32L();
    TUint32 id2 = aStream.ReadUint32L();
    iId=TMPXItemId( id1, id2 );
    iIndex=aStream.ReadInt32L();
    iOpenMode=static_cast<TMPXOpenMode>(aStream.ReadInt32L());
    iAttrs.Reset();
    ::InternalizeL(iAttrs, aStream);
    }

// ============================== MEMBER FUNCTIONS =============================

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPath* CMPXCollectionPath::NewL()
    {
    CMPXCollectionPath* p = new(ELeave) CMPXCollectionPath();
    CleanupStack::PushL(p);
    p->ConstructL();
    CleanupStack::Pop(p);
    return p;
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPath* CMPXCollectionPath::NewL(
    const CMPXCollectionPath& aPath)
    {
    CMPXCollectionPath* p = new(ELeave) CMPXCollectionPath();
    CleanupStack::PushL(p);
    p->ConstructL(aPath);
    CleanupStack::Pop(p);
    return p;
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPath* CMPXCollectionPath::NewL(
    RReadStream& aStream)
    {
    CMPXCollectionPath* p = new(ELeave) CMPXCollectionPath();
    CleanupStack::PushL(p);
    p->ConstructL(aStream);
    CleanupStack::Pop(p);
    return p;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPath::~CMPXCollectionPath()
    {
    iNodeArray.ResetAndDestroy();
    iNodeArray.Close();
    iIds.Close();
    iSelection.Close();
    }

// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
CMPXCollectionPath::CMPXCollectionPath() : iInvalidId(KMPXInvalidItemId)
    {
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXCollectionPath::ConstructL(const CMPXCollectionPath& aPath)
    {
    iNodeArray.ResetAndDestroy();
    TInt count = aPath.iNodeArray.Count();
    for (TInt i=0; i<count; ++i)
        {
        CMPXCollectionPathNode* node =
            CMPXCollectionPathNode::NewLC(*(aPath.iNodeArray[i]));
        iNodeArray.AppendL(node);
        CleanupStack::Pop(node);
        }
    ::CopyArrayL(aPath.iIds.Array(),iIds);
    ::CopyArrayL(aPath.iSelection.Array(),iSelection);
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXCollectionPath::ConstructL()
    {
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXCollectionPath::ConstructL(RReadStream& aStream)
    {
    InternalizeL(aStream);
    }

// -----------------------------------------------------------------------------
// Advances path to next item
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXCollectionPath::operator++()
    {
    TBool ret(EFalse);
    TInt index=Index();
    if (index < Count()-1)
        {
        Set(++index);
        ret=ETrue;
        } // else the last one, no more to go and return false
    return ret;
    }

// -----------------------------------------------------------------------------
// Advances path to previous item
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXCollectionPath::operator--()
    {
    TBool ret=EFalse;
    TInt index=Index();
    if (index>0)
        {
        Set(--index);
        ret=ETrue;
        } // else the first one, no more to go and return false
    return ret;
    }

// -----------------------------------------------------------------------------
// Sets path to first item
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::SetToFirst()
    {
    Set(0);
    }

// -----------------------------------------------------------------------------
// Sets path to last item
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::SetToLast()
    {
    Set(Count()-1);
    }

// -----------------------------------------------------------------------------
// Change current item by index at top level
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::Set(TInt aIndex)
    {
    MPX_ASSERT(Levels());
    CMPXCollectionPathNode& topLevel = TopLevel();
    // RArray will panic if aIndex out of bound
    topLevel.Set(iIds[aIndex], aIndex);
    }

// -----------------------------------------------------------------------------
// Change current item by id at top level
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::Set(const TMPXItemId& aId)
    {
    TInt index=IndexOfId(aId);
    MPX_ASSERT(KErrNotFound!=index && Levels());
    CMPXCollectionPathNode& topLevel = TopLevel();
    topLevel.Set(aId, index);
    }

// -----------------------------------------------------------------------------
// Set open mode for next level
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::Set(TMPXOpenMode aMode)
    {
    MPX_ASSERT(Levels());
    TopLevel().Set(aMode);
    }

// -----------------------------------------------------------------------------
// Set open attrobutes for next level
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::Set(const TArray<TMPXAttribute>& aAttrs)
    { // DEPRECATED
    TRAP_IGNORE(SetL(aAttrs));
    }

// -----------------------------------------------------------------------------
// Set open attrobutes for next level
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::SetL(const TArray<TMPXAttribute>& aAttrs)
    {
    MPX_ASSERT(Levels());
    TopLevel().SetL(aAttrs);
    }

// -----------------------------------------------------------------------------
//  Select an item by id in the path
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::SelectL(const TMPXItemId& aId)
    {
    TInt index=IndexOfId(aId);
    MPX_ASSERT(KErrNotFound != index);
    SelectL(index);
    }

// -----------------------------------------------------------------------------
//  Select an item by index in the path
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::SelectL(TInt aIndex)
    {
    MPX_ASSERT_EX(aIndex>=0 && aIndex<iIds.Count(), EBadArrayIndex);
    TInt err = iSelection.InsertInOrder(aIndex);
    if (KErrNone!=err && KErrAlreadyExists!=err)
        { // ignore duplicated items
        User::Leave(err);
        }
    }

// -----------------------------------------------------------------------------
//  Select an item by index in the path
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::SelectAllL()
    {
    iSelection.Reset();
    for (TInt i=0; i < iIds.Count(); ++i)
        {
        iSelection.AppendL(i);
        }
    }

// -----------------------------------------------------------------------------
//  Deselects an item by id in the path
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::Deselect(const TMPXItemId& aId)
    {
    TInt index=IndexOfId(aId);
    MPX_ASSERT(KErrNotFound != index);
    Deselect(index);
    }

// -----------------------------------------------------------------------------
//  Deselects an item by index in the path
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::Deselect(TInt aIndex)
    {
    MPX_ASSERT_EX(aIndex>=0 && aIndex<iIds.Count(), EBadArrayIndex);
    TInt index(iSelection.FindInOrder(aIndex));
    MPX_ASSERT(KErrNotFound !=index);
    iSelection.Remove(index);
    }

// -----------------------------------------------------------------------------
//  Select an item by index in the path
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::DeselectAll()
    {
    iSelection.Reset();
    }

// -----------------------------------------------------------------------------
//  Removes an item in the path
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::Remove(const TMPXItemId& aId)
    {
    TInt index=IndexOfId(aId);
    MPX_ASSERT(KErrNotFound != index);
    Remove(index);
    }

// -----------------------------------------------------------------------------
//  Removes an item in the path
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::Remove(TInt aIndex)
    {
    iIds.Remove(aIndex);
    TInt ret = iSelection.FindInOrder(aIndex);
    if (KErrNotFound != ret)
        {
        iSelection.Remove(ret);
        TInt count = iSelection.Count();
        for (TInt i=ret; i<count; i++)
            {
            --iSelection[i];
            }
        }
    }

// -----------------------------------------------------------------------------
//  Query selection
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXCollectionPath::IsSelected(const TMPXItemId& aId) const
    {
    TBool ret(EFalse);
    TInt index=IndexOfId(aId);
    if (KErrNotFound != index)
        {
        ret = IsSelected(index);
        }
    return ret;
    }

// -----------------------------------------------------------------------------
//  Query selection
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXCollectionPath::IsSelected(TInt aIndex) const
    {
    return iSelection.FindInOrder(aIndex) != KErrNotFound;
    }

// -----------------------------------------------------------------------------
//  Clears selection
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::ClearSelection()
    {
    iSelection.Reset();
    }

// -----------------------------------------------------------------------------
// Array of currently selected indices
// -----------------------------------------------------------------------------
//
EXPORT_C TArray<TInt> CMPXCollectionPath::Selection() const
    {
    return iSelection.Array();
    }

// -----------------------------------------------------------------------------
// Current selected IDs
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::SelectionL(RArray<TMPXItemId>& aIds) const
    {
    CleanupClosePushL(aIds);
    aIds.Reset();
    TInt idCount(iIds.Count());
    TInt selCount(iSelection.Count());
    for  (TInt i=0; i<selCount; ++i)
        {
        TInt idIndex(iSelection[i]);
        if (idIndex < idCount)
            {
            aIds.AppendL(iIds[idIndex]);
            }
        }
    CleanupStack::Pop();
    }

// -----------------------------------------------------------------------------
// Update the item id at a particular index
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::Update( TInt aIndex, TMPXItemId& aNewId )
    {
    MPX_ASSERT( aIndex >= 0 && aIndex < iIds.Count() );
    iIds[aIndex] = aNewId;
    }

// -----------------------------------------------------------------------------
// Index of current item at top level
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXCollectionPath::Index() const
    {
    TInt ret(KErrNotFound);
    if (Levels())
        {
        ret = TopLevel().Index();
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// Return current item id at top level
// -----------------------------------------------------------------------------
//
EXPORT_C const TMPXItemId& CMPXCollectionPath::Id() const
    {
    if (Levels())
        {
        return TopLevel().Id();
        }
    else
        {
        return iInvalidId;
        }
    }

// -----------------------------------------------------------------------------
// Returns the number of items at top level
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXCollectionPath::Count() const
    {
    return iIds.Count();
    }

// -----------------------------------------------------------------------------
// Returns the open mode for the next level
// -----------------------------------------------------------------------------
//
EXPORT_C TMPXOpenMode CMPXCollectionPath::OpenNextMode() const
    {
    return Levels() ? TopLevel().OpenMode():EMPXOpenGroupOrPlaylist;
    }

// -----------------------------------------------------------------------------
// Returns the open mode for the previous level
// -----------------------------------------------------------------------------
//
EXPORT_C TMPXOpenMode CMPXCollectionPath::OpenPreviousMode() const
    {
    TInt n=Levels();
    return n>1?iNodeArray[n-2]->OpenMode():EMPXOpenGroupOrPlaylist;
    }

// -----------------------------------------------------------------------------
// Index from id
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXCollectionPath::IndexOfId(const TMPXItemId& aId) const
    {
    TInt ret(KErrNotFound);
    for (TInt i=iIds.Count(); --i>=0; )
        {
        if (aId == iIds[i])
            {
            ret = i;
            break;
            }
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// Return item id at top specific level for a specific item
// -----------------------------------------------------------------------------
//
EXPORT_C const TMPXItemId& CMPXCollectionPath::IdOfIndex( TInt aIndex ) const
    {
    if( aIndex >=0 && aIndex < iIds.Count() )
        {
        return iIds[aIndex];
        }
    else
        {
        return iInvalidId;
        }
    }

// -----------------------------------------------------------------------------
// Return item index at a specific level
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXCollectionPath::Index(TInt aLevel) const
    {
    MPX_ASSERT(aLevel>=0 && aLevel < Levels());
    return iNodeArray[aLevel]->Index();
    }

// -----------------------------------------------------------------------------
// Return item id at a specific level
// -----------------------------------------------------------------------------
//
EXPORT_C const TMPXItemId& CMPXCollectionPath::Id(TInt aLevel) const
    {
    if( aLevel < iNodeArray.Count() && aLevel >= 0 )
        {
        return iNodeArray[aLevel]->Id();
        }
    else
        {
        return iInvalidId;
        }
    }

// -----------------------------------------------------------------------------
// Returns the depth into the collection
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXCollectionPath::Levels() const
    {
    return iNodeArray.Count();
    }

// -----------------------------------------------------------------------------
// Remove a level from path
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::Back()
    {
    TInt levels = Levels();
    MPX_ASSERT(levels > 0);
    CMPXCollectionPathNode* node = iNodeArray[levels-1];
    iNodeArray.Remove(levels-1);
    delete node;
    iIds.Reset();
    iSelection.Reset();
    }

// -----------------------------------------------------------------------------
// Append a new level
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::AppendL(const TArray<TMPXItemId>& aIds)
    {
    iIds.Reset();
    iSelection.Reset();
    CMPXCollectionPathNode* node(NULL);
    if (aIds.Count())
        {
        ::CopyArrayL(aIds, iIds);
        node = CMPXCollectionPathNode::NewLC();
        node->Set(iIds[0], 0);
        iNodeArray.AppendL(node);
        CleanupStack::Pop(node);
        }
    else
        {// add a level in order to support back
        node = CMPXCollectionPathNode::NewLC();
        node->Set(iInvalidId, KErrNotFound);
        iNodeArray.AppendL(node);
        CleanupStack::Pop(node);
        }
    }

// -----------------------------------------------------------------------------
// Append a new level
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::AppendL(const TMPXItemId& aId)
    {
    RArray<TMPXItemId> ids;
    CleanupClosePushL(ids);
    ids.AppendL(aId);
    AppendL(ids.Array());
    CleanupStack::PopAndDestroy(&ids);
    }

// -----------------------------------------------------------------------------
// Append an id
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::InsertL(const TMPXItemId& aId, TInt aPos)
    {
    MPX_ASSERT(iIds.Count());
    iIds.InsertL(aId, aPos);
    // update the selection
    for (TInt i=0; i<iSelection.Count(); ++i)
        {
        TInt& sel = iSelection[i];
        if (sel >=aPos)
            {
            ++sel;
            }
        }
    }

// -----------------------------------------------------------------------------
// Resets the collection path object
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::Reset()
    {
    iNodeArray.ResetAndDestroy();
    iIds.Reset();
    iSelection.Reset();
    }

// -----------------------------------------------------------------------------
// Creates a collection path pointing to the container
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPath* CMPXCollectionPath::ContainerPathL() const
    {
    CMPXCollectionPath* p = CMPXCollectionPath::NewL();
    CleanupStack::PushL( p );

    TInt count = iNodeArray.Count() - 1;  // Copy up to container level
    for (TInt i=0; i<count; ++i)
        {
        CMPXCollectionPathNode* node =
            CMPXCollectionPathNode::NewLC(*iNodeArray[i]);
        p->iNodeArray.AppendL(node);
        CleanupStack::Pop(node);
        }

    CleanupStack::Pop( p );
    return p;
    }

// -----------------------------------------------------------------------------
// Update collection path. When a item changed in the collection plugin,
// collection plugin calls back collection context, which in turn updates
// its collection paths by calling this function
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXCollectionPath::HandleChange(
    const TUid& aCollectionId,
    const TMPXItemId& aId,
    const TMPXItemId& aDeprecatedId,
    CMPXCollectionPath::TMPXCollectionPathChange aChange,
    TInt& aSelection )
    {
    TBool updated(EPathUnchanged);
    aSelection = KErrNotFound;

    if (aChange == CMPXCollectionPath::EAdded)
        {
        updated = EPathModified;
        }
    else if (aChange == CMPXCollectionPath::EGroupModified )
        {
        TInt levels( Levels() );
        if( levels > 0 )
            {
            // Check if the collection id is correct
            if (iNodeArray[ECollectionUid]->Id() ==
                TMPXItemId(aCollectionId.iUid) && levels > 1)
                {
                // Check if the container level is the one modified
                if( aId == Id( levels-2 ) )
                    {
                    updated = EPathModified;
                    }
                }
            }
        }
    else if (aChange == CMPXCollectionPath::EDeleted ||
             aChange == CMPXCollectionPath::EModified)
        {
        TInt levels = Levels();
        if (levels > 0)
            {
            // check collection id level, aka root level
            if (iNodeArray[ECollectionUid]->Id() ==
                TMPXItemId(aCollectionId.iUid) && levels > 1)
                {
                // check node array except top level and root level.
                for (TInt i=ECollectionRoot+1; i<levels-1; ++i)
                    {
                    if (iNodeArray[i]->Id() == aId ||
                        (aDeprecatedId != 0 && iNodeArray[i]->Id() == aDeprecatedId))
                        {
                        if( aChange == CMPXCollectionPath::EModified &&
                            aDeprecatedId != 0 )
                            {
                            // If the item is modified and the item id is updated
                            // we simply replace the node level item id with the new
                            // item id
                            //
                            // Continue and check other levels if any other
                            // levels are using this ID. ie: all songs of an artist
                            // The item IDs need to be unique across all levels, but
                            // can have duplicates. This is in the case of a playlist with
                            // more than 1 instance of a song
                            //
                            CMPXCollectionPathNode* node = iNodeArray[i];
                            node->Set( aId, Index(i) );
                            updated = EPathModified;
                            }
                        else
                            {
                            // Trim the path and break out of the loop
                            aSelection = Index(i);
                            for (TInt j=Levels()-1; j>i; --j)
                                {
                                CMPXCollectionPathNode* node = iNodeArray[j];
                                iNodeArray.Remove(j);
                                delete node;
                                }
                            updated = EPathClipped;
                            break;
                            }
                        }
                    }
                 // Check the top level
                 if (!updated)
                    {
                    TMPXItemId search = aDeprecatedId != 0? aDeprecatedId : aId;
                    TInt temp(KErrNotFound);

                    // Unmatched ids in the item ID,
                    // try to look for an exact match first
                    //
                    if( search.iId1 != search.iId2 )
                        {
                        for (TInt i=iIds.Count(); --i>=0; )
                            {
                            if (search == iIds[i] )
                                {
                                temp = i;
                                break;
                                }
                            }
                        }
                    // Still not found, check for approximate equal
                    //
                    if( KErrNotFound == temp )
                        {
                        for (TInt i=iIds.Count(); --i>=0; )
                            {
                            if (search.ApproxEqual(iIds[i]))
                                {
                                temp = i;
                                break;
                                }
                            }
                        }
                    if (KErrNotFound != temp)
                        { // Improvement: only remove item deleted
                        updated = EPathModified;
                        aSelection = temp;
                        }
                    }

                // Only clean up level if the path has been clipped
                if (updated == EPathClipped )
                    {
                    // Reset top level
                    iIds.Reset();
                    iSelection.Reset();
                    }
                }
            }
        }
    return updated;
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPath::OpenAttributes
// -----------------------------------------------------------------------------
//
EXPORT_C const TArray<TMPXAttribute> CMPXCollectionPath::OpenAttributes() const
    {
    MPX_ASSERT(Levels()>0);
    return TopLevel().OpenAttributes();
    }

// -----------------------------------------------------------------------------
// Get top level items
// -----------------------------------------------------------------------------
//
EXPORT_C const TArray<TMPXItemId> CMPXCollectionPath::Items() const
    {
    MPX_ASSERT(Levels()>0);
    return iIds.Array();
    }

// -----------------------------------------------------------------------------
// Internalize object
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::InternalizeL(RReadStream& aStream)
    {
    (void)aStream.ReadInt32L(); // Discard object type
    iNodeArray.ResetAndDestroy();
    ::InternalizeCObjectArrayL(iNodeArray, aStream);
    iIds.Reset();
    MPXUser::InternalizeL(iIds, aStream);
    iSelection.Reset();
    ::InternalizeL(iSelection, aStream);
    }

// -----------------------------------------------------------------------------
// Externalize object
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPath::ExternalizeL(RWriteStream& aStream) const
    {
    aStream.WriteInt32L(MMPXData::EPath);
    ::ExternalizeL(iNodeArray.Array(), aStream);
    MPXUser::ExternalizeL(iIds.Array(), aStream);
    ::ExternalizeL(iSelection.Array(), aStream);
    }

// -----------------------------------------------------------------------------
// Top level
// -----------------------------------------------------------------------------
//
CMPXCollectionPathNode& CMPXCollectionPath::TopLevel()
    {
    MPX_ASSERT(Levels() > 0);
    return *iNodeArray[Levels()-1];
    }

// -----------------------------------------------------------------------------
// Top level
// -----------------------------------------------------------------------------
//
const CMPXCollectionPathNode& CMPXCollectionPath::TopLevel() const
    {
    MPX_ASSERT(Levels() > 0);
    return *iNodeArray[Levels()-1];
    }
