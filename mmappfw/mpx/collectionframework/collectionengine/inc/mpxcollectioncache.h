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
* Description:  Generic collection cache
*
*/


#ifndef MPXCOLLECTIONCACHE_H
#define MPXCOLLECTIONCACHE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <mpxitemid.h>

// CLASS FORWARDS
class CMPXMedia;
class CMPXCollectionPath;
class TMPXAttribute;
class CMPXCollectionCacheNode;

/**
*  The collection cache stores information already retrieved by clients so it can be
*  returned immediately if requested again. The information is stored in a tree
*  structure, where each tree node corresponds to a path node. When information is
*  requested, the path is mapped against the tree and if a corresponding node exists the
*  results are returned from cache.
*
*  The cache can be limited in size, if the client defines a maximum size, lower priority
*  nodes are discarded to make room for new information. The criteria considered when
*  discarding nodes include: priority value (specified by the client), path level and
*  time of addition.
*
*  @lib mpxcollectionengine.lib (not exported)
*/
NONSHARABLE_CLASS(CMPXCollectionCache) :
    public CBase
    {
public:
    /**
    *  Two-phased constructor.
    *  @param aMaximumSizeRatio maximum cache size in percentage ratio
    *         If not specified the cache does not have a limit
    *  @return Constructed object
    */
    static CMPXCollectionCache* NewL(TInt aMaximumSizeRatio = -1);

    /**
    *  Destructor
    */
    virtual ~CMPXCollectionCache();

public:
    /**
    *  Cache entry priority values. Entries with lower priority will be discarded
    *  sooner than the ones with higher priority.
    */
    enum TCachePriority
        {
        EPriorityLow,
        EPriorityNormal,
        EPriorityHigh
        };

public:
    /**
    *  Adds a result set to the cache.
    *  @param aPath path defining the context of the results.
    *  @param aAttrs attributes requested by the client application and included in
    *         the result set.
    *  @param aResults result set corresponding to the path.  Not owned.
    *  @param aNotCacheableAttrs attributes that are not cacheable
    *  @param aMediaFromOpenL whether or not this media was from an OpenL
    *  @param aPriority result set priority
    *  @return CMPXMedia object in the cache or the original object if add failed
    */
    CMPXMedia* AddL(
        const CMPXCollectionPath& aPath, 
        const TArray<TMPXAttribute>& aAttrs,
        CMPXMedia& aResults, 
        TBool aMediaFromOpenL = EFalse,
        TCachePriority aPriority = EPriorityNormal );

    /**
    *  Returns the information corresponding to the specified path and attribute set
    *  if available in the cache.
    *  @param aPath path defining the information context.
    *  @param aAttrs attribute set requested by the client.
    *  @return Valid results or NULL if the path/attribute set specified cannot be
    *          found in the cache.
    */
    CMPXMedia* GetL(
        const CMPXCollectionPath& aPath,
        const TArray<TMPXAttribute>& aAttrs,
        TBool aMediaFromOpenL = EFalse );

    /**
    *  Removes the specified path and all its children from the cache.
    *  @param aPath path defining the context to be removed.
    */
    void RemoveL(const CMPXCollectionPath& aPath);

    /**
    * Resets cache and removes all nodes
    */
    void Reset();

    /**
    * Handle some change from the collection
    * @param aCollectionId collectoin that the change is comming from
    * @param aChangeItemId item ID of the changed event
    */
    void HandleChangeL( TUid aCollectionId, TMPXItemId aChangeItemId );
    
private:

    /**
    *  C++ constructor
    *  @param aMaximumSizeKb maximum cache size in percentage ratio.
    */
    CMPXCollectionCache(TInt aMaximumSizeRatio);

    /**
    *  2nd phase contructor
    */
    void ConstructL();

    /**
    *  Returns the cache node corresponding to the specified path.
    *  @param aPath path to be looked up in the cache.
    *  @param aCreateNodes if ETrue all the missing nodes for the specified
    *         path will be created if they do not already exist. If EFalse
    *         the method returns NULL if corresponding node is not found.
    *  @return valid cache node if found or created, NULL if not found.
    */
    CMPXCollectionCacheNode* NodeFromPathL(const CMPXCollectionPath& aPath,
        TBool aCreateNodes = EFalse);
        
    /**
    *  Removes a specified node and all its children from the cache.
    *  @param aNode node to be removed.  Ownership is transferred.
    */
    void RemoveNode(CMPXCollectionCacheNode* aNode);
    
    /**
    *  Checks the memory condition and clears cache to free some memory
    *  maximum percentage ratio specified in the constructor is used to detect
    *  low memory condition
    *  @return EFalse if low memory condition detected, ETrue otherwise 
    */
    TBool ManageCacheSizeL();

#ifdef _DEBUG
    /**
    *  Prints the cache tree.
    */
    void PrintTree( TBool aPrintAtts = EFalse );
#endif
    
private:
    /**
    *  Root node for the cache tree, created when the cache is created.
    */
    CMPXCollectionCacheNode* iRoot;

    /**
    *  Maximum size of the cache in percentage ratio, specified by the client.
    */
    TInt iMaximumSizeRatio;
    };

#endif // MPXCOLLECTIONCACHE_H

