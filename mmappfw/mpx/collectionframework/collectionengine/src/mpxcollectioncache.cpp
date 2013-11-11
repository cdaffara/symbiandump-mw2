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
* Description:  Implements the collection cache
*
*/


// INCLUDES
#include <e32base.h>
#include <mpxitemid.h>
#include <mpxcmn.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxcollectionpath.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxcollectionopenlresultdef.h>
#include <mpxlog.h>
#include "mpxcollectioncache.h"
#include <mmf/common/mmfcontrollerpluginresolver.h>

// CONSTANTS
const TInt KMPXRootNodeId = 0;

// ---------------------------------------------------------------------------
// static function to compare two datablock items
// used for sorting datablocks
// ---------------------------------------------------------------------------
//
static TInt CompareOpenDataBlock( const TMPXOpenDataBlock& aFirst,
                                  const TMPXOpenDataBlock& aSecond )
    {
    return aFirst.iOffset > aSecond.iOffset;
    }


/**
* Encapsulates a node in the cache tree.
*/
NONSHARABLE_CLASS(CMPXCollectionCacheNode) :
    public CBase
    {
public:
    /**
    * Two phase constructor.
    */
    static CMPXCollectionCacheNode* NewL( const TMPXItemId aId );

    /**
    * Destructor.
    */
    virtual ~CMPXCollectionCacheNode();

    /**
    * Adds a child to the current node.
    * Ownership of the node is transferred
    */
    void AddChildL(CMPXCollectionCacheNode* aChild);

    /**
    * Removes a child from the current node.
    * Ownership of the node is not transferred
    */
    void RemoveChild(CMPXCollectionCacheNode& aChild);

    /**
    * Retrieves a child by ID. Returns NULL if not found.
    * Ownership of the node is not transferred.
    */
    CMPXCollectionCacheNode* GetChild(TMPXItemId aId);

    /**
    * Sets the results and attribute set for the current node.
    * @param aAttrs list of open attributes
    * @param aResults results to set in
    * @param aMediaFromOpen is the media from common
    */
    void SetResultsL(
        const TArray<TMPXAttribute>& aAttrs,
        CMPXMedia& aResults,
        TBool aMediaFromOpen );

    /**
    * Checks if the specified attribute set exists (is included) in the current
    * node attribute set.
    */
    TBool AttributesExist(const TArray<TMPXAttribute>& aAttrs);

    /**
    * Checks if the specified attribute set exists (is included) in the current
    * node attribute set.
    */
    TBool ContainerAttributesExist(const TArray<TMPXAttribute>& aAttrs);
    
    /**
    * Checks if the specified attribute exists (is included) in the current
    * node attribute set.
    * @param aAttribute attribute to look for
    * @param aArray array to check
    * @return ETrue if found, EFalse if not found
    */
    TBool AttributeExists(const TMPXAttribute& aAttribute, 
                          RArray<TMPXAttribute>& aArray );
    
    /**
    * Checks this node and all child nodes for invalidated data
    * @param aChangeItemId ID of the item modified
    * @param aArray array to store the list of affected nodes
    */
    void HandleChangeL( TMPXItemId aChangeItemId, 
                        RPointerArray<CMPXCollectionCacheNode>& aArray );
                        
#ifdef _DEBUG
    /**
    * Prints the node.
    */
    void PrintNode(TBool aPrintChildren, TBool aPrintAtts);
#endif

private:
    /**
    * C++ constructor
    */
    CMPXCollectionCacheNode(TMPXItemId aId,
        CMPXCollectionCache::TCachePriority aPriority);

    /**
    * Second phase constructor.
    */
    void ConstructL();

    /**
    * Returns the child index for the specified ID or KErrNotFound if not found.
    */
    TInt IndexOfId(TMPXItemId aId);

    /**
    * Compares two media instances. If the instances refer to the same collection
    * item (they have the same general ID) returns ETrue, otherwise EFalse.
    */
    TBool CompareMediaItemsL(
        const CMPXMedia& aMedia1, 
        const CMPXMedia& aMedia2 );

    /**
    *  Comparison method for node IDs. Used when searching child nodes by ID.
    *  Ownership of nodes is not transferred.
    *  This method is used with the Find method of RPointerArray and 
    *  must be declared as follow:
    *  TBool *(aFunction)(const K *k, const T &t);
    */
    static TBool CompareIDs(
        const CMPXCollectionCacheNode* aNode1,
        const CMPXCollectionCacheNode& aNode2 );

    /**
    * Merge the data from two arrays
    * @param aExistingArray Old array with current data
    * @param aNewArray New arrary with data to merge
    */
    TBool DoMergeMediaArraysL( const CMPXMediaArray& aExistingArray, 
                               const CMPXMediaArray& aNewArray );
    
    /**
    * Merge the data from two arrays
    * @param aExistingArray Old array with current data
    * @param aNewArray New arrary with data to insert
    */
    void DoInsertMediaArraysL( CMPXMediaArray& aExistingArray, 
                               const CMPXMediaArray& aNewArray, 
                               const TMPXOpenDataBlock& aBlock );

    /**
    * Compact a list of open datablocks into larger chunks
    * @param aArray Array to compact
    */
    void DoMergeArrayL( RArray<TMPXOpenDataBlock>& aArray );

public:
    TMPXItemId iId;                                 // node ID (same as path node ID)
    RArray<TMPXAttribute> iAttrs;                   // attribute set corresponding to the results
    RArray<TMPXAttribute> iContainerAttrs;          // attribute set corresponding to the container 
                                                    // of a result set
    CMPXMedia* iResults;                            // results
    CMPXCollectionCache::TCachePriority iPriority;  // node priority
    TTime iTime;                                    // last time the node was updated
    TInt iLevel;                                    // level of the node in the original path

    CMPXCollectionCacheNode* iParent;               // parent of the node (NULL for root)
    RPointerArray<CMPXCollectionCacheNode> iChildren;   // node children
    TBool iMediaFromOpenL;                          // whether or not this media was from an openl
    };

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::NewL
// ----------------------------------------------------------------------------
//
CMPXCollectionCacheNode* CMPXCollectionCacheNode::NewL(
    const TMPXItemId aId)
    {
    MPX_FUNC("CMPXCollectionCacheNode::NewL");

    CMPXCollectionCacheNode* self = new(ELeave)CMPXCollectionCacheNode(aId,
        CMPXCollectionCache::EPriorityNormal);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::~CMPXCollectionCacheNode
// ----------------------------------------------------------------------------
//
CMPXCollectionCacheNode::~CMPXCollectionCacheNode()
    {
    MPX_FUNC("CMPXCollectionCacheNode::~CMPXCollectionCacheNode");

    iAttrs.Close();
    iContainerAttrs.Close();
    delete iResults;

    // delete all children
    iChildren.ResetAndDestroy();
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::CMPXCollectionCacheNode
// ----------------------------------------------------------------------------
//
CMPXCollectionCacheNode::CMPXCollectionCacheNode(
    TMPXItemId aId,
    CMPXCollectionCache::TCachePriority aPriority) :
    iId(aId),
    iPriority(aPriority)
    {
    MPX_FUNC("CMPXCollectionCacheNode::CMPXCollectionCacheNode");
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::ConstructL
// ----------------------------------------------------------------------------
//
void CMPXCollectionCacheNode::ConstructL()
    {
    MPX_FUNC("CMPXCollectionCacheNode::ConstructL");
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::AddChildL
// ----------------------------------------------------------------------------
//
void CMPXCollectionCacheNode::AddChildL(
    CMPXCollectionCacheNode* aChild)
    {
    MPX_FUNC("CMPXCollectionCacheNode::AddChildL");
    aChild->iParent = this;
    iChildren.AppendL(aChild);
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::RemoveChild
// ----------------------------------------------------------------------------
//
void CMPXCollectionCacheNode::RemoveChild(
    CMPXCollectionCacheNode& aChild)
    {
    MPX_FUNC("CMPXCollectionCacheNode::RemoveChild");

    TInt index = IndexOfId(aChild.iId);
    if (index != KErrNotFound)
        {
        // remove the child
        iChildren.Remove(index);
        }
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::GetChild
// ----------------------------------------------------------------------------
//
CMPXCollectionCacheNode* CMPXCollectionCacheNode::GetChild(
    TMPXItemId aId)
    {
    MPX_FUNC("CMPXCollectionCacheNode::GetChild");

    CMPXCollectionCacheNode* child(NULL);

    TInt index = IndexOfId(aId);
    if (index != KErrNotFound)
        {
        child = iChildren[index];
        }

    return child;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::SetResultsL
// If results are already stored in the node try to merge the new results,
// otherwise overwrite them.
// ----------------------------------------------------------------------------
//
void CMPXCollectionCacheNode::SetResultsL(
    const TArray<TMPXAttribute>& aAttrs,
    CMPXMedia& aResults,
    TBool aMediaFromOpen )
    {
    MPX_FUNC("CMPXCollectionCacheNode::SetResultsL");

    // Problem if we did not have results from before, but
    // want to merge in new data. Plugins returning all results will
    // not contain KMPXCollectionOpenLResultRange in the media
    // therefore we do manual merging. If it does contain the attribute
    // That means it is from a subset, with a particular offset and block size
    //
    TMPXOpenDataBlock block;
    block.iOffset = KErrNotFound;
    if( aResults.IsSupported( KMPXCollectionOpenLResultRange ) )
        {
        block = aResults.ValueTObjectL<TMPXOpenDataBlock>( 
                                            KMPXCollectionOpenLResultRange );
        
        // If the results have not been cached before, 
        // we have nothing to merge
        //
        if( !iResults && block.iOffset != KErrNotFound )
            {
            User::Leave( KErrArgument );
            }
        }
            
    TBool overwrite(ETrue);
    
    // Merge
    if ( iResults )
        {
        // extract the arrays from the two media instances
        CMPXMediaArray* existingArray(NULL);
        if ( iResults->IsSupported(KMPXMediaArrayContents) )
            {
            existingArray = 
                        iResults->Value<CMPXMediaArray>(KMPXMediaArrayContents);
            User::LeaveIfNull(existingArray);
            }

        const CMPXMediaArray* newArray(NULL);
        if ( aResults.IsSupported( KMPXMediaArrayContents ))
            {
            newArray = aResults.Value<CMPXMediaArray>(KMPXMediaArrayContents);
            User::LeaveIfNull(const_cast<CMPXMediaArray*>(newArray));
            }
            
        // Only need to merge the two arrays if they exist, else it will just overwrite
        if ( existingArray && newArray && existingArray->Count())
            {
            if( block.iOffset == KErrNotFound )
                {
                overwrite = DoMergeMediaArraysL( *existingArray, 
                                                 *newArray );
                }
            else // aOffset != KErrNotFound 
                {
                DoInsertMediaArraysL( *existingArray, 
                                      *newArray, 
                                      block );
                
                // These 2 attributes already merged
                //
                aResults.Delete( KMPXMediaArrayContents );
                aResults.Delete( KMPXMediaArrayCount );
                
                // Merge container media
                //
                iResults->MergeMediaL( aResults );
                
                // Merge the root media
                //
                overwrite = EFalse;
                }
            }
        else
            {
            iResults->MergeMediaL( aResults );
            overwrite = EFalse;
            }
        }

    if (overwrite)
        {
        // List of supported attributes is 
        // seperated to media w/ container from OpenL()
        // and media only from MediaL()
        //
        if( aMediaFromOpen )
            {
            iAttrs.Reset();
            ::CopyArrayL(aAttrs, iAttrs);
            }
        else
            {
            iContainerAttrs.Reset();
            ::CopyArrayL(aAttrs, iContainerAttrs);
            }
                
        delete iResults;
        iResults = NULL;
        iResults = CMPXMedia::NewL( aResults );
        }
    else
        {
        // Update either the array list or the root media list
        //
        if( aMediaFromOpen )
            {
            // Add the attributes into the list
            for ( TInt attrIndex = 0; attrIndex < aAttrs.Count(); attrIndex++ )
                {
                const TMPXAttribute& attr = aAttrs[attrIndex];
                if ( !AttributeExists( attr, iAttrs ))
                    {
                    iAttrs.AppendL( attr );
                    }
                }    
            }
        else
            {
            // Add the attributes into the container list
            for ( TInt attrIndex = 0; attrIndex < aAttrs.Count(); attrIndex++ )
                {
                const TMPXAttribute& attr = aAttrs[attrIndex];
                if ( !AttributeExists( attr, iContainerAttrs ))
                    {
                    iContainerAttrs.AppendL( attr );
                    }
                }
            }
        }
#ifdef _DEBUG
    MPX_DEBUG1("CMPXCollectionCacheNode::SetResultsL(): Attributes cached");
    for ( TInt k = 0; k < iAttrs.Count(); k++ )
        {
        MPX_DEBUG4("CMPXCollectionCacheNode::SetResultsL(): attr[%d] = {0x%x,0x%x}", 
                   k, iAttrs[k].ContentId(), iAttrs[k].AttributeId());
        }
#endif
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::AttributesExist
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionCacheNode::AttributesExist(
    const TArray<TMPXAttribute>& aAttrs)
    {
    MPX_FUNC("CMPXCollectionCacheNode::AttributesExist");

    TBool exist(ETrue);
    if (0 == aAttrs.Count())
        {
        // no attributes specified, always exist
        exist = ETrue;
        }
    else if (0 == iAttrs.Count())
        {
        // no existing attributes, always do not exist
        exist = EFalse;
        }
    else
        {
        // both have valid number of attributes
        TInt count(aAttrs.Count());
        for (TInt index = 0; index < count; ++index)
            {
            if (!AttributeExists(aAttrs[index], iAttrs))
                {
                exist = EFalse;
                break;
                }
            }
        }

    return exist;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::AttributesExist
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionCacheNode::ContainerAttributesExist(
    const TArray<TMPXAttribute>& aAttrs)
    {
    MPX_FUNC("CMPXCollectionCacheNode::ContainerAttributesExist");

    TBool exist(ETrue);
    if (0 == aAttrs.Count())
        {
        // no attributes specified, always exist
        exist = ETrue;
        }
    else if (0 == iContainerAttrs.Count())
        {
        // no existing attributes, always do not exist
        exist = EFalse;
        }
    else
        {
        // both have valid number of attributes
        TInt count(aAttrs.Count());
        for (TInt index = 0; index < count; ++index)
            {
            if (!AttributeExists(aAttrs[index], 
                                 iContainerAttrs))
                {
                exist = EFalse;
                break;
                }
            }
        }

    return exist;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::AttributeExists
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionCacheNode::AttributeExists(
    const TMPXAttribute& aAttribute, 
    RArray<TMPXAttribute>& aArray)
    {
    MPX_FUNC("CMPXCollectionCacheNode::AttributeExists");

    TBool ret(ETrue);

    TUint content(aAttribute.ContentId());
    TUint attribute(aAttribute.AttributeId());
    TInt count(aArray.Count());
    
    // Have to do an exhaustive search of every bit
    // This is because clients can compress the content IDs
    //
    TUint32 mask(1); // 0x00000001
    while( mask != 0 )
        {
        TUint32 val(attribute&mask);
        if( val )
            {
            TInt i;
            for (i = 0; i < count; ++i)
                {
                if (aArray[i].ContentId() == content &&
                    (aArray[i].AttributeId() & val) )
                    {
                    // break out of for loop
                    break;
                    }
                }
            if( i==count )
                {
                // break out of while loop if
                // the particular bit is not found
                ret = EFalse;
                break; 
                }
            }
        mask = mask<<1; // Check next bit
        }
    
    return ret;
    }

// ----------------------------------------------------------------------------
// Handles any changes to the collection cache
// ----------------------------------------------------------------------------
//
void CMPXCollectionCacheNode::HandleChangeL( TMPXItemId aChangeItemId, 
                                RPointerArray<CMPXCollectionCacheNode>& aArray )
    {
    CleanupResetAndDestroyPushL(aArray); 
    // This node is affected
    // All child nodes are invalid!
    //
    TBool aOpen(ETrue);
    if( iId == aChangeItemId )
        {
        aArray.AppendL( this );
        aOpen=EFalse; // wiping out this node already
        }
    else if( iChildren.Count() )
        {
        TInt c( iChildren.Count() );
        for( TInt i=0; i<c; ++i )
            {
            // Next level maybe all MediaL() data 
            // in which case we do want to check this level's array
            //
            iChildren[i]->HandleChangeL( aChangeItemId, aArray );
            }
        }
        
    // Check the contents as well
    //
    if( aOpen )
        {
        // This is a leaf node, so we check the array
        //
        if( iResults && 
            iResults->IsSupported(KMPXMediaArrayContents) )
            {
            // Array not owned
            const CMPXMediaArray& ary =
                          *iResults->Value<CMPXMediaArray>(KMPXMediaArrayContents);
            User::LeaveIfNull(const_cast<CMPXMediaArray*>(&ary));
            TInt c(ary.Count() );
            
            // Check every media item, this *MAYBE* slow. May want to consider
            // caching all item IDs within each node for performance
            for( TInt i=0; i<c; ++i )
                {
                TMPXItemId id=KMPXInvalidItemId;
                const CMPXMedia& m = *(ary.AtL(i));
                
                // Just to be safe
                if( m.IsSupported( KMPXMediaGeneralId ) )
                    {
                    id=m.ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId);
                    }
                
                // If we match, add this as affected and return
                if( id == aChangeItemId )
                    {
                    aArray.AppendL( this );
                    break;
                    }
                } // for
            } // if iResults
        } // if no children
    CleanupStack::Pop(&aArray); 
    }
    
// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::IndexOfId
// ----------------------------------------------------------------------------
//
TInt CMPXCollectionCacheNode::IndexOfId(
    TMPXItemId aId)
    {
    MPX_FUNC("CMPXCollectionCacheNode::IndexOfId");
    TInt index( KErrNotFound );

    CMPXCollectionCacheNode* node( NULL );
    MPX_TRAPD(err, node = CMPXCollectionCacheNode::NewL( aId ));
    if ( KErrNone == err )
        {
        index = iChildren.Find(*node, CompareIDs);
        delete node;
        }

    return index;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::CompareMediaItems
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionCacheNode::CompareMediaItemsL(
    const CMPXMedia& aItem1,
    const CMPXMedia& aItem2)
    {
    return aItem1.IsSupported(KMPXMediaGeneralId) &&
        aItem2.IsSupported(KMPXMediaGeneralId) &&
        (aItem1.ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId) ==
         aItem2.ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId));
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::CompareIDs
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionCacheNode::CompareIDs(
    const CMPXCollectionCacheNode* aNode1,
    const CMPXCollectionCacheNode& aNode2)
    {
    MPX_FUNC("CMPXCollectionCacheNode::CompareIDs");
    return aNode1->iId == aNode2.iId;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::DoMergeMediaArraysL
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionCacheNode::DoMergeMediaArraysL( const CMPXMediaArray& aExistingArray,
                                                    const CMPXMediaArray& aNewArray )
    {
    TBool overwrite(ETrue);
    TInt newCount( aNewArray.Count() );
    for ( TInt arrayIndex = 0; arrayIndex < newCount; arrayIndex++ )
        {
        const CMPXMedia* newItem( (aNewArray).AtL(arrayIndex) );
        CMPXMedia* existingItem( NULL );
        TInt existingCount( aExistingArray.Count() );
        if ( arrayIndex < existingCount )
            {
            existingItem = aExistingArray.AtL(arrayIndex);
            }
        else
            {
            existingItem = aExistingArray.AtL(0);
            }

        MPX_ASSERT(newItem->IsSupported(KMPXMediaGeneralId));
        MPX_ASSERT(existingItem->IsSupported(KMPXMediaGeneralId));

        // try to match the items at the same index
        if ( CompareMediaItemsL( *newItem, *existingItem ))
            {
            existingItem->MergeMediaL( *newItem );
            overwrite = EFalse;
            }
        else
            {
            // items do not match, try linear search
            TBool found(EFalse);
            for ( TInt i = 0; i < existingCount && !found; ++i )
                {
                CMPXMedia* item( aExistingArray.AtL(i) );
                // Already compared existing item so skip it
                if ( item != existingItem )
                    {
                    if ( CompareMediaItemsL( *newItem, *item ))
                        {
                        // found the item
                        item->MergeMediaL( *newItem );
                        overwrite = EFalse;
                        found = ETrue;
                        }
                    }
                }
            }
        }
    return overwrite;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::DoInsertMediaArraysL
// ----------------------------------------------------------------------------
//
void CMPXCollectionCacheNode::DoInsertMediaArraysL( CMPXMediaArray& aExistingArray, 
                                                    const CMPXMediaArray& aNewArray, 
                                                    const TMPXOpenDataBlock& aBlock )
    {
    // Overflow checking. Make sure array indexes are correct
    //
    TInt newCount( aNewArray.Count() );
    if( aBlock.iOffset + newCount > aExistingArray.Count() )
        {
        User::Leave( KErrArgument );
        }
    
    // Merge all results by inserting into the existing array
    //
    for( TInt i=0; i<newCount; ++i )
        {
        aExistingArray.Set(*aNewArray.AtL(i),aBlock.iOffset+i);
        }


    // Update the container information about what has been read
    RArray<TMPXOpenDataBlock> datablocks;
    CleanupClosePushL( datablocks );
    if( iResults && 
        iResults->IsSupported(KMPXCollectionOpenLAllResultRange) )
        {
        // De-serialize from global data, would be good to have global arrays
        //
        const TDesC& buf = iResults->ValueText(KMPXCollectionOpenLAllResultRange);
        CBufBase* buffer(NULL);
        MPXUser::CreateBufferL( buf, buffer );
        CleanupStack::PushL( buffer );
        ::CreateFromBufferL( *buffer, datablocks );
        CleanupStack::PopAndDestroy( buffer );
        }
    
    // Append, sort and merge the array for more effient access
    //
    datablocks.AppendL( aBlock );
    
    // Sort and merge the data array
    //
    datablocks.Sort( CompareOpenDataBlock );
    DoMergeArrayL( datablocks );
    
    // Package it and store in media
    // Store the block list
    //
    CBufBase* buffer(NULL);
    ::CreateBufferL( datablocks.Array(), buffer );
    CleanupStack::PushL( buffer );
    TPtr8 ptr8 = buffer->Ptr(0);
    TPtrC ptr = MPXUser::Ptr( ptr8 );
    if(iResults)
    	{
    	iResults->SetTextValueL( KMPXCollectionOpenLAllResultRange, ptr );
  		}
    CleanupStack::PopAndDestroy( buffer );
    
    CleanupStack::PopAndDestroy( &datablocks );
    }

// ----------------------------------------------------------------------------
// Compact the datablocks
// ----------------------------------------------------------------------------
//
void CMPXCollectionCacheNode::DoMergeArrayL( RArray<TMPXOpenDataBlock>& aArray )
    {
    // O(n^2) complexity here is OK, Merging keeps the number of items 
    // very small. Less than 3 items are alive at a time
    //
    CleanupClosePushL( aArray );
    RArray<TMPXOpenDataBlock> newArray;
    CleanupClosePushL( newArray );
    
    TInt c1 = aArray.Count();
    for( TInt i=0; i<c1; ++i )
        {
        TBool updated(EFalse);
        TMPXOpenDataBlock& curBlock = aArray[i];
        
        TInt c2( newArray.Count() );
        for( TInt j=0; j<c2; ++j )
            {
            TMPXOpenDataBlock& mergedBlock = newArray[j];
            if( mergedBlock.iOffset == curBlock.iOffset + curBlock.iSize )
                {
                mergedBlock.iOffset = curBlock.iOffset;
                mergedBlock.iSize += curBlock.iSize;
                updated = ETrue;
                break;
                }
            else if( mergedBlock.iOffset + mergedBlock.iSize == curBlock.iOffset )
                {
                mergedBlock.iSize += curBlock.iSize;
                updated = ETrue;
                break;
                }
            }
        
        if( !updated )
            {
            newArray.AppendL( curBlock );
            }
        }
        
    aArray.Reset();
    TInt c3( newArray.Count() );
    for( TInt i=0; i<c3; ++i )
        {
        aArray.AppendL( newArray[i] );
        MPX_DEBUG3("Order: %i %i", newArray[i].iOffset, newArray[i].iSize );
        }
    CleanupStack::PopAndDestroy( &newArray );
    CleanupStack::Pop(); 
    }
                                 
#ifdef _DEBUG

// ----------------------------------------------------------------------------
// CMPXCollectionCacheNode::PrintNode
// ----------------------------------------------------------------------------
//
void CMPXCollectionCacheNode::PrintNode(
    TBool aPrintChildren,
    TBool aPrintAtts )
    {
    TBuf<30> buf;
    _LIT(KFormatString, "%-B%:0%J%:1%T%:2%S%:3%+B");
    TRAP_IGNORE( iTime.FormatL( buf, KFormatString ));
    // use RDebug directly since there is no MPX_DEBUG with 6 parameters.
    RDebug::Print(_L("+Id: %08X, Level: %d, Priority: %d, Time: %S"),
        iId.iId1, iLevel, iPriority, &buf);
    MPX_DEBUG2("CMPXCollectionCacheNode::PrintNode(): iMediaFromOpenL = %d", 
                iMediaFromOpenL);
    
    if ( aPrintAtts )
        {
        for ( TInt j = 0; j < iAttrs.Count(); j++ )
            {
            TMPXAttribute att( iAttrs[j] );
            MPX_DEBUG4("CMPXCollectionCacheNode::PrintNode(): attr[%d] = {0x%x,0x%x}", 
                                    j, att.ContentId(), att.AttributeId());
            TMPXAttributeType type( iResults->Type( att ));
            if ( EMPXTypeText == type )
                {
                MPX_DEBUG2("CMPXCollectionCacheNode::PrintNode(): Text = %S", 
                                                    &(iResults->ValueText( att )));
                }
            else if ( EMPXTypeTInt == type )
                {
                TInt val(0);
                TRAP_IGNORE(val = (iResults->ValueTObjectL<TInt>( att )));
                MPX_DEBUG2("CMPXCollectionCacheNode::PrintNode(): TInt = %d", 
                           val);
                }
            }
        }

    if ( aPrintChildren )
        {
        TInt children( iChildren.Count() );
        for ( TInt index = 0; index < children; ++index )
            {
            iChildren[index]->PrintNode( ETrue, aPrintAtts );
            }
        }
    }

#endif

// ----------------------------------------------------------------------------
// CMPXCollectionCache::NewL
// ----------------------------------------------------------------------------
//
CMPXCollectionCache* CMPXCollectionCache::NewL(
    TInt aMaximumSizeRatio /* = -1 */)
    {
    MPX_FUNC("CMPXCollectionCache::NewL");

    CMPXCollectionCache* self = new(ELeave) CMPXCollectionCache( aMaximumSizeRatio );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCache::~CMPXCollectionCache
// ----------------------------------------------------------------------------
//
CMPXCollectionCache::~CMPXCollectionCache()
    {
    MPX_FUNC("CMPXCollectionCache::~CMPXCollectionCache");

    // delete the entire cache
    delete iRoot;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCache::Add
// ----------------------------------------------------------------------------
//
CMPXMedia* CMPXCollectionCache::AddL(
    const CMPXCollectionPath& aPath,
    const TArray<TMPXAttribute>& aAttrs,
    CMPXMedia& aResults,
    TBool aMediaFromOpenL, 
    TCachePriority aPriority)
    {
    MPX_FUNC("CMPXCollectionCache::DoAddL");
    MPX_DEBUG_PATH(aPath);
    
    CMPXMedia* ret( &aResults );

    //if OOM then clear current cache and don't add to cache
    TBool toCache = ManageCacheSizeL();
    
    //!media in array should support KMPXMediaGeneralId otherwise we don't cache it
    if(toCache && aResults.IsSupported(KMPXMediaArrayContents))
        {
        const CMPXMediaArray* resultsArray(
                    aResults.Value<CMPXMediaArray>(KMPXMediaArrayContents));
        User::LeaveIfNull(const_cast<CMPXMediaArray*>(resultsArray));
        TInt arrCount(resultsArray->Count());
        for(TInt arrInd = 0; arrInd < arrCount; ++arrInd)
            {
            const CMPXMedia* curItem(resultsArray->AtL(arrInd));
            if(!curItem->IsSupported(KMPXMediaGeneralId))
                {
                MPX_DEBUG1("CMPXCollectionCache::AddL: Missing MediaGeneralId - caching skipped");
                toCache = EFalse;
                break;
                }
            }
        }
    
    if (toCache && aResults.IsSupported(KMPXMediaGeneralNonPermissibleActions ) )
        {
        // check for auto playlist, it should not be cached
        TMPXGeneralNonPermissibleActions attr(
                aResults.ValueTObjectL<TMPXGeneralNonPermissibleActions>( 
                KMPXMediaGeneralNonPermissibleActions ) );
        if ( attr & EMPXCache )
            {
            toCache = EFalse;
            }
        }                                        

    if(toCache)
        {
        // get the corresponding node, create if not available
        CMPXCollectionCacheNode* node = NodeFromPathL(aPath, ETrue);

        // update the node
        node->SetResultsL( aAttrs, aResults, aMediaFromOpenL );
        node->iMediaFromOpenL = node->iMediaFromOpenL || aMediaFromOpenL;

        // set the priority and the "last updated" timestamp
        node->iPriority = aPriority;
        node->iLevel = aPath.Levels();
        node->iTime.HomeTime();

        ret = node->iResults;
        }
#ifdef _DEBUG
    PrintTree( ETrue );
#endif
    return ret;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCache::GetL
// ----------------------------------------------------------------------------
//
CMPXMedia* CMPXCollectionCache::GetL(
    const CMPXCollectionPath& aPath,
    const TArray<TMPXAttribute>& aAttrs,
    TBool aMediaFromOpenL /* = EFalse */ )
    {
    MPX_FUNC("CMPXCollectionCache::GetL");
    MPX_DEBUG_PATH(aPath);

    CMPXMedia* results(NULL);

    // get the corresponding node, do not create if not available
    CMPXCollectionCacheNode* node = NodeFromPathL(aPath, EFalse);

    TInt selCount = aPath.Selection().Count();

    // Check if node exists and attributes match
    // If we are calling from openL() or mediaL() on multiple items, we check the array
    // attributes. If we are calling from MediaL() with a single item
    // we check the container attributes
    //
    if ( (aMediaFromOpenL || selCount )
          && node && node->AttributesExist(aAttrs))
        {
        // Check if it needs to be a media from openl
        // If not just return the results.  But if it needs to be a media from
        // OpenL, then need to check if the results are actually from an OpenL
        if ( (aMediaFromOpenL && node->iMediaFromOpenL) ||
             !aMediaFromOpenL )
            {
            results = node->iResults;
            }
        }
    else if( node && node->ContainerAttributesExist(aAttrs) )
        {
        // Here we end from a MediaL call, we need to check if there are items selected
        // in this case NodeFromPathL result may be ivalid as it using only focused item
        // so we will pass that call to plugin and ignore found node from cache in that case
        if(selCount == 0)
            {
            results = node->iResults;
            }
        }

    // note that NULL is returned for nodes without results
    // (created empty to link to other nodes)
    return results;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCache::RemoveL
// ----------------------------------------------------------------------------
//
void CMPXCollectionCache::RemoveL(
    const CMPXCollectionPath& aPath)
    {
    MPX_FUNC("CMPXCollectionCache::RemoveL(CMPXCollectionPath)");
    MPX_DEBUG_PATH(aPath);

    // get the corresponding node, do not create if not available
    CMPXCollectionCacheNode* node( NodeFromPathL( aPath, EFalse ));

    // if node exists
    if ( node )
        {
        // delete it and its children
        RemoveNode( node );
#ifdef _DEBUG
        PrintTree( ETrue );
#endif
        }
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCache::Reset
// ----------------------------------------------------------------------------
//
void CMPXCollectionCache::Reset()
    {
    MPX_FUNC("CMPXCollectionCache::ClearCache");
    iRoot->iChildren.ResetAndDestroy();
    iRoot->iAttrs.Reset();
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCache::HandleChangeL()
// ----------------------------------------------------------------------------
//
void CMPXCollectionCache::HandleChangeL( TUid aCollectionId, TMPXItemId aChangeItemId )
    {
    // First get the cache node associated with the collection
    // If the node exists, then we recursively get every node to handle the change
    //
    TMPXItemId root( aCollectionId.iUid );
    CMPXCollectionCacheNode* rootNode = iRoot->GetChild( root );
    
    // Array does not own any of the objects
    RPointerArray<CMPXCollectionCacheNode> affectedNodes; 
    CleanupClosePushL( affectedNodes );
    if( rootNode )
        {
        // Iteratively check every node
        //
        rootNode->HandleChangeL( aChangeItemId, 
                                 affectedNodes );
        }
        
    // Remove all affected nodes
    TInt c( affectedNodes.Count() ); 
    if( c )
        {
        for(TInt i=0; i<c; ++i )
            {
            // FYI:
            // Child nodes are always inserted before their parents
            // Otherwise this will ASSERT
            RemoveNode(affectedNodes[i]);
            }
        }
    
    CleanupStack::PopAndDestroy( &affectedNodes );
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCache::CMPXCollectionCache
// ----------------------------------------------------------------------------
//
CMPXCollectionCache::CMPXCollectionCache(
    TInt aMaximumSizeRatio ) :
    iMaximumSizeRatio( aMaximumSizeRatio )
    {
    MPX_FUNC("CMPXCollectionCache::CMPXCollectionCache");
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCache::ConstructL
// ----------------------------------------------------------------------------
//
void CMPXCollectionCache::ConstructL()
    {
    MPX_FUNC("CMPXCollectionCache::ConstructL");
    iRoot = CMPXCollectionCacheNode::NewL(KMPXRootNodeId);
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCache::NodeFromPathL
// ----------------------------------------------------------------------------
//
CMPXCollectionCacheNode* CMPXCollectionCache::NodeFromPathL(
    const CMPXCollectionPath& aPath,
    TBool aCreateNodes /* = EFalse */)
    {
    MPX_FUNC("CMPXCollectionCache::NodeFromPathL");

    CMPXCollectionCacheNode* crtNode(iRoot);

    TInt count(aPath.Levels());
    for (TInt level = 0; level < count; ++level)
        {
        TMPXItemId id = aPath.Id(level);
        CMPXCollectionCacheNode* childNode = crtNode->GetChild(id);
        if (!childNode)
            {
            if (aCreateNodes)
                {
                // create the child node and add it
                childNode = CMPXCollectionCacheNode::NewL(id);
                CleanupStack::PushL(childNode);
                crtNode->AddChildL(childNode);	// ownership transferred
                CleanupStack::Pop(childNode);
                }
            else
                {
                // exit and return NULL
                crtNode = NULL;
                break;
                }
            }

        // move to the next level
        crtNode = childNode;
        }

    return crtNode;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCache::RemoveNode
// ----------------------------------------------------------------------------
//
void CMPXCollectionCache::RemoveNode(
    CMPXCollectionCacheNode* aNode)
    {
    MPX_FUNC("CMPXCollectionCache::RemoveNode");
    MPX_ASSERT( aNode );


    // remove from parent node
    aNode->iParent->RemoveChild(*aNode);

    // delete the node and its children
    delete aNode;
    }

// ----------------------------------------------------------------------------
// CMPXCollectionCache::ManageCacheSizeL
// ----------------------------------------------------------------------------
//
TBool CMPXCollectionCache::ManageCacheSizeL()
    {
    MPX_FUNC("CMPXCollectionCache::ManageCacheSizeL");
    
    TBool addToCache(ETrue);
    TInt totalMemory(0);
    TInt usedMemory(0);
    CMPXMedia::HeapMemoryInfoL( totalMemory, usedMemory );
    TInt ratio( (usedMemory * 100) / totalMemory );
    MPX_DEBUG4("ManageCacheSizeL : ratio=%d, total=%d, used=%d",ratio,totalMemory,usedMemory);
    if(ratio > iMaximumSizeRatio)
        {
        // clear cache to free all memory we can
        Reset();
        addToCache = EFalse;
        }
        
    return addToCache;
    }


#ifdef _DEBUG

// ----------------------------------------------------------------------------
// CMPXCollectionCache::PrintTree
// ----------------------------------------------------------------------------
//
void CMPXCollectionCache::PrintTree( TBool aPrintAtts /*= EFalse */ )
    {
    MPX_FUNC("CMPXCollectionCache::PrintTree");
    iRoot->PrintNode( ETrue, aPrintAtts);
    }

#endif

// End of file
