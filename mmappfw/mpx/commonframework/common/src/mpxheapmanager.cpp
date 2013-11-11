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
* Description:  Implementation of heap manager 
*
*/

#include <hal.h>
#include <centralrepository.h>
#include <mpxprivatecrkeys.h>
#include <mpxdata.h>
#include <mpxlog.h>
#include "mpxheapmanager.h"

_LIT(KMPXChunkNameV2,"MPX_DATA");
_LIT(KMPXMutexNameV2,"MPX_MUTEX");

const TInt KMPXBytesPerMB = 0x100000;   // bytes of 1 MB, 1024*1024
#ifdef __WINS__
const TInt KMPXMaxHeapDefaultSize=8; // MB
#else
const TInt KMPXMaxHeapDefaultSize=15; // MB
#endif
const TInt KMPXMinHeapSize=0x10000;     // 64KB
const TInt KMPXChunkAdjustSize=0x40000; // 256KB
#ifdef __ENABLE_MPX_GARBAGE_COLLECTOR
const TInt KMPXGarbageCollectorClientThreshold=1000;//GC
#endif // __ENABLE_MPX_GARBAGE_COLLECTOR

#define MPX_NEXT_CELL(p) ((RMPXHeapCell*)(((TUint8*)p)+p->iLen))
#define MPX_NEXT_FREE_CELL(ch,p) (p->iNextOffset?Ptr<RMPXHeapCell>(ch, p->iNextOffset):NULL)

// -----------------------------------------------------------------------------
// RMPXHeapManager::TClientContext::TClientContext
// -----------------------------------------------------------------------------
//
RMPXHeapManager::TClientContext::TClientContext()
    : iTid(KNullThreadId),iCount(0),iBase(0)
    {
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::TClientContext::TClientContext
// -----------------------------------------------------------------------------
//
RMPXHeapManager::TClientContext::TClientContext(
    TUint8* aBase,
    TInt aChunkHandle,
    TInt aMutexHandle) :
    iTid(RThread().Id()),
    iCount(0),
    iBase(aBase)
    {
    iChunk.SetHandle(aChunkHandle);
    iMutex.SetHandle(aMutexHandle);
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::RMPXHeapManager
// -----------------------------------------------------------------------------
//
RMPXHeapManager::RMPXHeapManager(const RChunk& aChunk)
    : iEndOffset(aChunk.Size()),iCounter(0)
#ifdef __ENABLE_MPX_GARBAGE_COLLECTOR
    ,iDeadContextIndex(KErrNotFound)
#endif // __ENABLE_MPX_GARBAGE_COLLECTOR
    {
    TInt hmSize = sizeof(RMPXHeapManager); 
    MPX_ASSERT_WORD_ALIGNED(aChunk.Base()+hmSize);
    //
    // Create first free heap cell
    //
    new(aChunk.Base()+hmSize)RMPXHeapCell(0,iEndOffset-hmSize);
    // set up free cell list header
    iFree.iNextOffset = hmSize;
    iFree.iLen = 0;
    iUsedMemory = hmSize;
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::HeapMemoryInfoL
// -----------------------------------------------------------------------------
//
void RMPXHeapManager::HeapMemoryInfoL(TInt& aTotal, TInt& aUsed)
    {
    TUint h = RMPXHeapManager::ClientHandle();
    if (!h)
        {
        User::Leave(KErrNotFound);
        }
    RMPXHeapManager& hm = RMPXHeapManager::HeapManager(h);
    aUsed = hm.iUsedMemory;
    HAL::Get(HALData::EMemoryRAMFree, aTotal);
    aTotal +=aUsed;
    if (aTotal > hm.iMaxMemory)
        {
        aTotal = hm.iMaxMemory;
        }
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::ClientHandle
// -----------------------------------------------------------------------------
//
TUint RMPXHeapManager::ClientHandle()
    {
    // First, try to retrieve the handle from TLS. Although slow, this seems to be
    // faster than openeing the chunk, though probably requires further testing. It
    // will be in TLS if another client from this thread has previously called this
    // method
    //
    TUint h=(TUint)Dll::Tls();
    RMPXHeapManager* m=NULL;
    if (h==0)
        {
        // Must be first client in this thread, so try opening the chunk, but
        // first, create a "critical section" on this code to protect against multiple
        // threads creating the chunk/heap at the same time
        //
        RMutex mx;
        TInt n=2;
        TInt r=KErrNotFound;
        while(r!=KErrNone && n--)
            {
            r=mx.OpenGlobal(KMPXMutexNameV2,EOwnerThread);
            if (r==KErrNotFound)
                {
                r=mx.CreateGlobal(KMPXMutexNameV2,EOwnerThread);
                }
            };
        if (r==KErrNone)
            {
            mx.Wait();
            //
            TUint8* base=NULL;
            RChunk c;
            r=c.OpenGlobal(KMPXChunkNameV2,EFalse,EOwnerThread);
            if (r==KErrNotFound)
                {
                // Chunk not there, so this must be the first client in the system and so
                // the chunk must be created
                //
                // Get maximum size of memory from CenRep key
                CRepository* rep(NULL);
                // Set to default size
                TInt maxMemory = KMPXMaxHeapDefaultSize;
                TRAP_IGNORE(rep =  CRepository::NewL( KCRUidMPXSettings ));
                if (rep)
                    {
                    rep->Get( KMPXMaxGlobalHeapSize, maxMemory );
                    delete rep;
                    }
                // size in bytes
                maxMemory *= KMPXBytesPerMB;
                r=c.CreateGlobal(KMPXChunkNameV2,0,maxMemory,EOwnerThread);
                if (r==KErrNone && c.Adjust(KMPXMinHeapSize)==KErrNone)
                    {
                    base=c.Base();
                    //
                    // Add the Heap Manager and the bottom of the chunk. Thereafter will come the
                    // data
                    //
                    MPX_ASSERT(sizeof(RMPXHeapManager)<KMPXMinHeapSize);
                    m=new(base)RMPXHeapManager(c); // Add on chunk
                    m->iMaxMemory = maxMemory;
                    }
                }
            if (r==KErrNone)
                {
                if (!base)
                    {
                    base=c.Base();
                    }
                m=reinterpret_cast<RMPXHeapManager*>(base);
                TClientContext cc(base,c.Handle(),mx.Handle());
                TInt index=m->ClientIndex(cc);
                h=ClientHandle(base,index);
                Dll::SetTls((TAny*)h);
                mx.Signal();
                }
            else
                {
                c.Close();
                mx.Signal();
                mx.Close();
                m=NULL;
                }
            }
        }
    MPX_ASSERT(h);
    return h;
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::Find
// -----------------------------------------------------------------------------
//
TInt RMPXHeapManager::Find(const TThreadId& aTid)
    {
    TInt r=KErrNotFound;
    for (TInt i=0;i<ENumClients;++i)
        {
        if (iClients[i].iTid==aTid)
            {
            r=i;
            break;
            }
        }
    return r;
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::ClientIndex
// -----------------------------------------------------------------------------
//
TInt RMPXHeapManager::ClientIndex(const TClientContext& aContext)
    {
    TInt i=Find(aContext.iTid);
    if (i==KErrNotFound)
        {
        i=Find(KNullThreadId); // First unused slot
        MPX_ASSERT(i>=0); // Run out of space (num threads > ENumClients)
        iClients[i]=aContext;
        }
    return i;
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::AddRef
// -----------------------------------------------------------------------------
//
void RMPXHeapManager::AddRef(TUint aClientHandle)
    {
    MPX_ASSERT(aClientHandle);
    TClientContext& c=iClients[ClientIndex(aClientHandle)];
    ++c.iCount;
#ifdef __ENABLE_MPX_GARBAGE_COLLECTOR
    if (iCounter%KMPXGarbageCollectorClientThreshold==0 && iCounter)
        {//GC
        RunGc(aClientHandle);
        }
#endif // __ENABLE_MPX_GARBAGE_COLLECTOR
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::Release()
// -----------------------------------------------------------------------------
//
void RMPXHeapManager::Release(TUint aClientHandle)
    {
    MPX_ASSERT(aClientHandle);
    TClientContext& c=iClients[ClientIndex(aClientHandle)];
    if (--c.iCount==0)
        {
#ifdef __ENABLE_MPX_GARBAGE_COLLECTOR
        c.iData.Close(aClientHandle); //GC
#endif // __ENABLE_MPX_GARBAGE_COLLECTOR
        c.iTid=KNullThreadId; // Mark as free
        c.iMutex.Close();
        c.iChunk.Close();
        Dll::SetTls(NULL);
        }
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::HeapCell
// -----------------------------------------------------------------------------
//
RMPXHeapCell* RMPXHeapManager::HeapCell(
    TUint aClientHandle,
    TInt aSize)
    {
    aSize = Align4(aSize + EHeapCellHeaderSize);
    RMPXHeapCell* lastFree(NULL);
    RMPXHeapCell* cell = DoHeapCell(aClientHandle, aSize, lastFree);
    if (!cell)
        { // try to grow chunk
        TInt r = TryToGrowHeap(aClientHandle, aSize, lastFree);
        if (KErrNone == r)
            {
            cell = DoHeapCell(aClientHandle, aSize, lastFree);
            }
        }
    if (cell)
        {
        iUsedMemory += (cell->iLen);
        }
    return cell;
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::HeapCell
// -----------------------------------------------------------------------------
//
RMPXHeapCell* RMPXHeapManager::DoHeapCell(
    TUint aClientHandle,
    TInt aSize, 
    RMPXHeapCell*& aLastFree)
    {
    //
    // Start with the first cell and walk the heap until a free cell is found which
    // is big enough
    //
    RMPXHeapCell* prevCell = &iFree;
    RMPXHeapCell* cell = MPX_NEXT_FREE_CELL(aClientHandle, prevCell); 
    TInt tempOffset(0);
    for (; cell; prevCell=cell, cell=MPX_NEXT_FREE_CELL(aClientHandle, cell))
        { // scan the free list
        //cell is valid and free
        if (cell->iLen >= aSize)
            { // size is big enough
            if (cell->iLen-aSize < EMinCellSize) // leftover must larger enough to split
                { // it isn't, so take it all
                aSize = cell->iLen;
                tempOffset = cell->iNextOffset;
                MPX_ASSERT(tempOffset <= iEndOffset - EMinCellSize);
                }
            else
                { // Take amount and create a new free cell
                tempOffset = Offset(aClientHandle, cell) + aSize;
                MPX_ASSERT(tempOffset <= iEndOffset - EMinCellSize);
                // create a new cell
                new(Ptr<TAny>(aClientHandle,tempOffset)) 
                           RMPXHeapCell(cell->iNextOffset, cell->iLen - aSize);
                }
            prevCell->iNextOffset = tempOffset;
            cell->iLen = aSize;
            return cell;
            }
        }
    aLastFree = prevCell;
    return NULL;
    }

TInt RMPXHeapManager::TryToGrowHeap(
    TUint aClientHandle, 
    TInt aSize, 
    RMPXHeapCell* aLastFree)
    {
    TBool atEnd = IsLastCell(aClientHandle, aLastFree);
    TInt grow=Max((TInt)KMPXChunkAdjustSize, aSize);
    // Grow the chunk
    TClientContext& cc=iClients[ClientIndex(aClientHandle)];
    TInt r(KErrNoMemory);
    if (iEndOffset+grow <= iMaxMemory)
        {
        r=cc.iChunk.Adjust(iEndOffset+grow);
        if (KErrNone == r)
            {
            if (atEnd)
                {
                aLastFree->iLen += grow;
                }
            else
                { // create a new free cell
                new (Ptr<RMPXHeapCell>(aClientHandle, iEndOffset))RMPXHeapCell(0, grow);
                aLastFree->iNextOffset = iEndOffset;
                }
            iEndOffset=cc.iChunk.Size();
            }
        else
            {
            MPX_DEBUG2("RMPXHeapManager::HeapCell chunk failed to grow %d", r);
            }
        }
    return r;
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::ClientCount
// -----------------------------------------------------------------------------
//
TInt RMPXHeapManager::ClientCount() const
    {
    TInt c=0;
    for (TInt i=0;i<ENumClients;++i)
        {
        if (iClients[i].iTid.Id()!=KNullThreadId)
            {
            ++c;
            }
        }
    return c;
    }

// -----------------------------------------------------------------------------
// Allocate a buffer on the chunk
// -----------------------------------------------------------------------------
//
TAny* RMPXHeapManager::Alloc(TUint aClientHandle,TInt aSize)
    {
    TAny* ptr=NULL;
    TInt size=Align4(aSize);
    RMPXHeapCell* cell=HeapCell(aClientHandle,size);
    if (cell)
        {
        TInt offset = RMPXHeapManager::Offset(aClientHandle, cell) + EHeapCellHeaderSize;
        ptr=Ptr<TAny>(aClientHandle,offset);
        MPX_ASSERT(ptr);
        MPX_ASSERT_WORD_ALIGNED(ptr);
        }
    return ptr;
    }

// -----------------------------------------------------------------------------
// Copy a buffer on the chunk
// -----------------------------------------------------------------------------
//
TInt RMPXHeapManager::Copy(TUint aClientHandle,TAny* aSrc,TInt aSize)
    {
    TInt r=KErrNoMemory;
    TAny* ptr=Alloc(aClientHandle,aSize);
    if (ptr)
        {
        (void)Mem::Copy(ptr,aSrc,aSize);
        r=RMPXHeapManager::Offset(aClientHandle,ptr);
        }
    return r;
    }

// -----------------------------------------------------------------------------
// Frees a pointer on the chunk
// -----------------------------------------------------------------------------
//
TInt RMPXHeapManager::Free(TUint aClientHandle,TAny* aPtr)
    {
    TInt cellOffset=RMPXHeapManager::Offset(aClientHandle, aPtr)-sizeof(RMPXHeapCell);
    RMPXHeapCell* cell = Ptr<RMPXHeapCell>(aClientHandle,cellOffset);
    RMPXHeapCell* prevCell = &iFree;
    RMPXHeapCell* tempCell = MPX_NEXT_FREE_CELL(aClientHandle, prevCell);
    RMPXHeapCell* nextCell(NULL);
    TInt size = cell->iLen;
    // find the position in the list for the new free cell
    for (; tempCell && tempCell<cell; 
         prevCell=tempCell, tempCell= MPX_NEXT_FREE_CELL(aClientHandle, tempCell))
        {
        }
    if (tempCell)
        { // Is there a following free cell?
        nextCell = MPX_NEXT_CELL(cell);
        if (nextCell == tempCell)
            { //Yes, merge two cells
            cell->iLen += tempCell->iLen;
            cell->iNextOffset = tempCell->iNextOffset;
            }
        else
            {
            cell->iNextOffset = RMPXHeapManager::Offset(aClientHandle, tempCell);
            }
        }
    else
        {
        cell->iNextOffset = 0; // No following free cell
        }

    nextCell = MPX_NEXT_CELL(prevCell);
    if (nextCell==cell) // Is it adjacent
        {
        prevCell->iLen += cell->iLen;
        prevCell->iNextOffset = cell->iNextOffset;
        cell = prevCell;
        }
    else
        {
        prevCell->iNextOffset = RMPXHeapManager::Offset(aClientHandle, cell);
        }
    
    iUsedMemory -= size;
    MPX_ASSERT(iUsedMemory > 0);
    // Shrink chunk
    // How big is the chunk now
    //
    // TInt gap=last->iBufOffset+last->iBufLen-iEndOffset;
    //if (gap>2*KMPXChunkAdjustSize)
    //   {
        // We assume that a spare capacity of KMPXChunkAdjustSize bytes
        // is all we need, so if we have more we can shrink the chunk
        //
    //    TClientContext& cc=iClients[ClientIndex(aClientHandle)];
    //    cc.iChunk.Adjust(iEndOffset-KMPXChunkAdjustSize);
    //    iEndOffset=cc.iChunk.Size();
    //    }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::Lock
// -----------------------------------------------------------------------------
//
void RMPXHeapManager::Lock(TUint aClientHandle)
    {
    MPX_ASSERT(aClientHandle);
    TClientContext& cc=iClients[ClientIndex(aClientHandle)];
    cc.iMutex.Wait();
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::Unlock
// -----------------------------------------------------------------------------
//
void RMPXHeapManager::Unlock(TUint aClientHandle)
    {
    MPX_ASSERT(aClientHandle);
    TClientContext& cc=iClients[ClientIndex(aClientHandle)];
    cc.iMutex.Signal();
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::Counter
// -----------------------------------------------------------------------------
//
TInt RMPXHeapManager::IncrementCounter()
    {
    MPX_DEBUG3("RMPXHeapManager::Counter 0x%08x, count %d", this, iCounter);
    return ++iCounter;
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::IsLastCell
// -----------------------------------------------------------------------------
//
TBool RMPXHeapManager::IsLastCell(
    TUint aClientHandle, 
    RMPXHeapCell* aCell)
    {
    return (RMPXHeapManager::Offset(aClientHandle, aCell) + aCell->iLen) == iEndOffset;
    }

#ifdef __ENABLE_MPX_GARBAGE_COLLECTOR
// -----------------------------------------------------------------------------
// Cleans up any objects associated with threads that have died
// -----------------------------------------------------------------------------
//
void RMPXHeapManager::RunGc(TUint aClientHandle)
    {//GC
    MPX_FUNC("RMPXHeapManager::RunGc");
    RThread t;
    for (TInt i=0;i<ENumClients;++i)
        {
        if (i!=ClientIndex(aClientHandle)) // No point in looking at current thread!
            {
            TClientContext& c=iClients[i];
            TThreadId tid=c.iTid;
            if (tid.Id()!=KNullThreadId)
                {
                TInt r=t.Open(tid,EOwnerThread);
                if (r==KErrNone) // Thread still exists, so that's OK
                    {
                    t.Close();
                    }
                else
                    {
                    // Cannot open the thread, but we still have an entry for it. This (may)
                    // mean that there data stranded on the heap which we must remove
                    //
                    // Remove data associated with this thread
                    //
                    TInt j = c.iData.Count()-1;
                    while (j>=0)
                        {
                        RMPXDataObject<TUint>* d=
                            (RMPXDataObject<TUint>*)c.iData.DataItem(aClientHandle,j);
                        TUint* dataHandle=d->Object(aClientHandle);
                        MMPXData* data=MMPXData::Data(aClientHandle, *dataHandle);
                        iDeadContextIndex=i;
                        data->Release(aClientHandle);
                        iDeadContextIndex=KErrNotFound;
                        j = c.iData.Count()-1;
                        }

                    // Now remove context, now that we've cleaned up its data
                    //
                    c.iTid=KNullThreadId; // Mark as free
                    c.iData.Close(aClientHandle);
                    }
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::AddData
// -----------------------------------------------------------------------------
//
TInt RMPXHeapManager::AddData(TUint aClientHandle,TUint aDataHandle)
    { //GC
    TAny* ptr =Alloc(aClientHandle, sizeof(RMPXDataObject<TUint>));
    TInt r(KErrNone);
    if (ptr)
        {
        RMPXDataObject<TUint>* obj=new(ptr)RMPXDataObject<TUint>();
        obj->CopyObject(aClientHandle,aDataHandle);
        TClientContext& c=iClients[ClientIndex(aClientHandle)];
        c.iData.Append(aClientHandle,*obj);
        }
    else
        {
        r = KErrNoMemory;
        }
    return r;
    }

// -----------------------------------------------------------------------------
// RMPXHeapManager::RemoveData
// -----------------------------------------------------------------------------
//
TInt RMPXHeapManager::RemoveData(
    TUint aClientHandle,
    TUint aDataHandle,
    TBool aRemoveAll)
    {//GC
    RMPXDataObject<TUint> obj;
    obj.CopyObject(aClientHandle,aDataHandle);
    TInt clientIndex = ClientIndex(aClientHandle);
    TInt ret(KErrNotFound);

    if (aRemoveAll && iDeadContextIndex==KErrNotFound)
        { // remove all and GC is not running
        for (TInt i=0;i<ENumClients;++i)
            {
            TClientContext& c=iClients[i];
            TThreadId tid=c.iTid;
            if (tid.Id()!=KNullThreadId)
                { // Remove datahandle
                ret = c.iData.Find(aClientHandle, obj);
                if (KErrNotFound != ret)
                    {
                    c.iData.Remove(aClientHandle, ret);
                    ret = KErrNone;
                    }
                }
            }
        }
    else
        {
        // Remove data from current context
        TClientContext& cc = (iDeadContextIndex==KErrNotFound ?
                             iClients[clientIndex] : iClients[iDeadContextIndex]);
        ret = cc.iData.Find(aClientHandle,obj);
        if (KErrNotFound != ret)
            {
            cc.iData.Remove(aClientHandle,ret);
            ret=KErrNone;
            }
        }
    obj.Close(aClientHandle);
    return ret;
    }
#endif // __ENABLE_MPX_GARBAGE_COLLECTOR

// End of file
