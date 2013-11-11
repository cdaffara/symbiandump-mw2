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
* Description:  Manages global heap/chunk
*
*/



#ifndef RMPXHEAPMANAGER_H
#define RMPXHEAPMANAGER_H

#include "mpxdataarray.h"


class RMPXHeapCell
    {
    public:
        RMPXHeapCell(TInt aNextOffset, TInt aLen):
            iNextOffset(aNextOffset),
            iLen(aLen)
            {
            }
        RMPXHeapCell()
            {
            }
    public:
        /**
        * Offset of next cell from the base of the chunk to the next heap
        * cell, or 0 if none
        */
        TInt iNextOffset;
        /**
        * Length of the cell, which includes the length of header
        */
        TInt iLen;
        /**
        * Is this cell's contents paged to file storage
        */
        // TBool iPaged;
        };

/**
*  Class RMPXHeapManager
*/
class RMPXHeapManager
    {
public:
    /**
    * The heap manager
    *
    * @param aClientHandle, client handle to the global chunk
    */
    static inline RMPXHeapManager& HeapManager(TUint aClientHandle);

    /**
    * Gets the total size and the currently used size of the global heap
    *
    * @param aTotal Total size of memory in bytes (out parameter)
    * @param aUsed Used size of memory in bytes (out parameter)
    * @leave Leave with KErrNotFound if failed to open global chunk
    */
    static void HeapMemoryInfoL( TInt& aTotal, TInt& aUsed );


public:
    /**
    * Handle which represents the current client (thread).
    * This should be stored by the client and provided in all methods;
    * by so doing, the Heap Manager can optimally cope with the chunk mapped
    * to different addresses, thread relative handles for the chunk and mutex,
    * etc. Handle should never be zero: zero indicates that it has failed.
    *
    * @return client handle to the global chunk
    */
    static TUint ClientHandle();

    /**
    * Returns absolute address from an offset from the base of the chunk
    *
    * @param aClientHandle, client handle to the global chunk
    * @param aOffset, offset to the glboal chunk
    * @return pointer to the global address
    */
    template<typename T>
    static inline T* Ptr(TUint aClientHandle,TInt aOffset=0);

    /**
    * Returns offset from the base of the chunk from absolute pointer
    *
    * @param aClientHandle, client handle to the global chunk
    * @param aPtr pointer to the global address
    * @return offset to the glboal chunk
    */
    static inline TInt Offset(TUint aClientHandle,TAny* aPtr);

public:
    /**
    * Add another/new reference to a client (thread)
    *
    * @param aClientHandle, client handle to the global chunk
    */
    void AddRef(TUint aClientHandle);

    /**
    * Remove a reference to a client (thread)
    * @param aClientHandle, client handle to the global chunk
    */
    void Release(TUint aClientHandle);

    /**
    * Allocate an object of type T on the chunk. An absolute pointer to the
    * object created is returned, though the object must contain no absolute
    * pointers within itself
    *
    * @param aClientHandle, client handle to the global chunk
    * @return pointer to the memory
    */
    template<typename T>
    inline T* Alloc(TUint aClientHandle);

    /**
    * Allocate a buffer of size aSize and return an absolute pointer to the
    * start of the buffer.
    *
    * @param aClientHandle, client handle to the global chunk
    * @param aSize, size of the memory
    * @return pointer to the memory
    */
    TAny* Alloc(TUint aClientHandle,TInt aSize);

    /**
    * Allocate a buffer of size aSize, copy aSrcOffset to the buffer,
    * and return an offset to the start of the buffer.
    *
    * @param aClientHandle, client handle to the global chunk
    * @param aSrcOffset, source data offset
    * @param aSize, size of the data buffer
    * @return system error code
    */
    inline TInt Copy(TUint aClientHandle,TInt aSrcOffset,TInt aSize);

    /**
    * Allocate a buffer of size aSize, copy aSrc to the buffer,
    * and return an offset to the start of the buffer.
    *
    * @param aClientHandle, client handle to the global chunk
    * @param aSrc, source
    * @param aSize, size of the data buffer
    * @return system error code
    */
    TInt Copy(TUint aClientHandle,TAny* aSrc,TInt aSize);

    /**
    * Delete an object/buffer on the chunk. Can return error
    *
    * @param aClientHandle, client handle to the global chunk
    * @param aSrcPtr, memory address to be freed
    * @return system error code
    */
    TInt Free(TUint aClientHandle,TAny* aPtr);

    /**
    * Delete an object/buffer on the chunk. Can return error
    *
    * @param aClientHandle, client handle to the global chunk
    * @param aOffset, offset to global memory to be freed
    * @return system error code
    */
    inline TInt Free(TUint aClientHandle,TInt aOffset);

    /**
    * Locks heap
    *
    * @param aClientHandle, client handle to the global chunk
    */
    void Lock(TUint aClientHandle);

    /**
    * Unocks heap
    *
    * @param aClientHandle, client handle to the global chunk
    */
    void Unlock(TUint aClientHandle);

    /**
     * Increment consecutive number
     *
     * @return current number
     */
    TInt IncrementCounter();

#ifdef __ENABLE_MPX_GARBAGE_COLLECTOR
    /**
    * Run the Garbage Collector
    */
    void RunGc(TUint aClientHandle); //GC

    /**
    * Associate data with this client
    */
    TInt AddData(TUint aClientHandle,TUint aDataHandle); //GC

    /**
    * Unassociate data with this client
    */
    TInt RemoveData(TUint aClientHandle,TUint aDataHandle,TBool aRemoveAll); //GC
#endif // __ENABLE_MPX_GARBAGE_COLLECTOR

private:
    /**
    * Data associated with a client (thread)
    */
    class TClientContext
        {
    public:
        TClientContext();
        TClientContext(TUint8* aBase,TInt aChunkHandle,TInt aMutexHandle);
    public:
        TThreadId iTid;
        RChunk iChunk; // Thread relative
        RMutex iMutex; // Thread relative
        TInt iCount; // Number of clients in this thread (media objects)
        TUint8* iBase; // Base of the chunk for this process
#ifdef __ENABLE_MPX_GARBAGE_COLLECTOR
        RMPXDataItemArray iData; // Dynamic array of TUint //GC
#endif // __ENABLE_MPX_GARBAGE_COLLECTOR
        };
private:
    RMPXHeapManager(const RChunk& aChunk);

private:
    /**
    * Index which represents the current client (thread).
    * Refers to iClients array.
    */
    TInt ClientIndex(const TClientContext& aContext);

    /**
    * Client index from the handle. Refers to iClients array.
    */
    static inline TInt ClientIndex(TUint aClientHandle);

    /**
    * Returns index of first entry for that TID. There will
    * only ever be one, unless the search is for KNullThreadId
    */
    TInt Find(const TThreadId& aTid);

    /**
    * Creates client handle
    */
    static inline TUint ClientHandle(TUint8* aChunkBase,TInt aClientIndex);

    /**
    * Base address of the chunk for this client
    */
    static inline TUint8* ChunkBase(TUint aClientHandle);

    /**
    * Finds (or creates) first Heap Cell which can fit aSize bytes
    */
    RMPXHeapCell* HeapCell(TUint aClientHandle,TInt aSize);

    /**
    * Finds (or creates) first Heap Cell which can fit aSize bytes
    */
    RMPXHeapCell* DoHeapCell(TUint aClientHandle,TInt aSize,RMPXHeapCell*& aLastFree);

    /**
    * Try to grow the global chunk
    */
    TInt TryToGrowHeap(TUint aClientHandle, TInt aSize, RMPXHeapCell* aLastFree);
    
    /**
    * Number of clients (threads)
    */
    TInt ClientCount() const;
    
    /**
    * Check if it is the last cell
    */
    TBool IsLastCell(TUint aClientHandle, RMPXHeapCell* aCell);

private:

    enum {ENumClients=0x100};
    enum {EHeapCellHeaderSize = sizeof(RMPXHeapCell)};
    enum {EMinCellSize = sizeof(RMPXHeapCell) + 4 };
    
private:
    /**
    * Client data (per thread)
    */
    TFixedArray<TClientContext,ENumClients> iClients;
    /**
    * Offset from the base of the chunk to the end of the chunk, i.e.
    * the size of the chunk
    */
    TInt iEndOffset;
    /**
     * Head to free cell list
     */
    RMPXHeapCell iFree;
    /**
     * Counter
     */
    TInt iCounter;

    /**
     * Total memory in bytes used in Global Chunk
     */
    TInt iUsedMemory;

    /**
     * Maximum size of  memory in bytes used in Global Chunk
     */
    TInt iMaxMemory;

#ifdef __ENABLE_MPX_GARBAGE_COLLECTOR
    TInt iDeadContextIndex;
#endif // __ENABLE_MPX_GARBAGE_COLLECTOR
    };

#include "mpxheapmanager.inl"

#endif // RMPXHEAPMANAGER_H
