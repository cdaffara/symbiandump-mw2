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
* Description:  Inline implementation of heap manager 
*
*/

#include <mpxuser.h>

// -----------------------------------------------------------------------------
// Client handle
// -----------------------------------------------------------------------------
//
inline TUint RMPXHeapManager::ClientHandle(TUint8* aBase,TInt aClientIndex)
    { 
    // We assume here that the chunk base address ends in 0x00 so we can put the client
    // index at this location, so we must make sure that is true, and that there are
    // no more clients than will fit, i.e. 0xff (255)
    // 
    MPX_ASSERT(aBase&&aClientIndex>=0&&aClientIndex<ENumClients);
    MPX_ASSERT(((TUint)aBase&0xff)==0);
    TUint h=(TUint)aBase|aClientIndex;
    MPX_ASSERT((h&0xff)==aClientIndex);
    MPX_ASSERT(reinterpret_cast<TUint8*>(h&0xffffff00)==aBase);
    return h; 
    }

// -----------------------------------------------------------------------------
// Client index
// -----------------------------------------------------------------------------
// 
inline TInt RMPXHeapManager::ClientIndex(TUint aClientHandle)
    { 
    MPX_ASSERT(aClientHandle);
    return aClientHandle&0xff; 
    }

// -----------------------------------------------------------------------------
// Chunk Base
// -----------------------------------------------------------------------------
// 
inline TUint8* RMPXHeapManager::ChunkBase(TUint aClientHandle)
    { 
    MPX_ASSERT(aClientHandle);
    return reinterpret_cast<TUint8*>(aClientHandle&0xffffff00); 
    }
    
// -----------------------------------------------------------------------------
// Absolute pointer to T (thread relative)
// -----------------------------------------------------------------------------
//
template<typename T>  
inline T* RMPXHeapManager::Ptr(TUint aClientHandle,TInt aOffset)
    { 
    T* ptr=reinterpret_cast<T*>(ChunkBase(aClientHandle)+aOffset);
    MPX_ASSERT(aOffset==((TUint8*)ptr-ChunkBase(aClientHandle))); 
    return ptr; 
    }

// -----------------------------------------------------------------------------
// Relative offset from absolute pointer
// -----------------------------------------------------------------------------
//
inline TInt RMPXHeapManager::Offset(TUint aClientHandle,TAny* aPtr)
    { 
    TInt o=((TUint8*)aPtr-ChunkBase(aClientHandle));
    MPX_ASSERT(aPtr==reinterpret_cast<TAny*>(ChunkBase(aClientHandle)+o));
    return o; 
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::HeapManager
// -----------------------------------------------------------------------------
//
inline RMPXHeapManager& RMPXHeapManager::HeapManager(TUint aClientHandle)
    { return *Ptr<RMPXHeapManager>(aClientHandle); }

// -----------------------------------------------------------------------------
// Frees an offset on the chunk
// -----------------------------------------------------------------------------
//
inline TInt RMPXHeapManager::Free(TUint aClientHandle,TInt aOffset)
    {
    return Free(aClientHandle,Ptr<TAny>(aClientHandle,aOffset));
    }

// -----------------------------------------------------------------------------
// Allocate an object on the chunk
// -----------------------------------------------------------------------------
//
template<typename T> 
inline T* RMPXHeapManager::Alloc(TUint aClientHandle)
    {
    T* obj=NULL;
    TAny* ptr=Alloc(aClientHandle,sizeof(T));
    if (ptr)
        {
        obj=new(ptr)T;
        }
    MPX_ASSERT_WORD_ALIGNED(obj);
    return obj;
    }

// -----------------------------------------------------------------------------
// Copy a buffer on the chunk
// -----------------------------------------------------------------------------
//
inline TInt RMPXHeapManager::Copy(TUint aClientHandle,TInt aSrcOffset,TInt aSize)
    {
    return Copy(aClientHandle,RMPXHeapManager::Ptr<TAny>(aClientHandle,aSrcOffset),aSize);
    }

// End of file
