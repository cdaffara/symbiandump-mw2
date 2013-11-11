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
* Description:  Implementation of array of variable size elements on global chunk
*
*/

#include <mpxlog.h>
#include "mpxdataarray.h"
#include "mpxheapmanager.h"
#include "mpxdata.h"


#define ITEM(x,y) RMPXHeapManager::Ptr<RMPXDataItem>(x,y)
#define OFFSET(x,y) RMPXHeapManager::Offset(x,y)

// -----------------------------------------------------------------------------
// RMPXDataItem::Reset
// -----------------------------------------------------------------------------
//
void RMPXDataItem::Reset(TUint aClientHandle)
    {
    MPX_ASSERT(aClientHandle);
    if (iBufLen)
        {
        // If the buffer contains a media/media array, then we need
        // to release its reference (could end up being recursive)
        //
        TAny* data=Buf(aClientHandle);
        MMPXData* d=MMPXData::Data(aClientHandle,data,iBufLen);
        if(d)
            {
            d->Release(aClientHandle);
            }
        // Now free the actual buffer
        RMPXHeapManager::HeapManager(aClientHandle);
        iUid=0;
        iBufLen=0;
        }
    }

// -----------------------------------------------------------------------------
// RMPXDataItem::Copy
// -----------------------------------------------------------------------------
//
void RMPXDataItem::Copy(TUint aClientHandle,TAny* aSrc,TInt aSize)
    {
    //The source could be zero-length descriptor: aSize is 0 and aSrc is NULL
    //ASSERT if aSrc is NULL while aSize is not zero, invalid descriptor
    MPX_ASSERT(((aSrc!=NULL)?ETrue:(aSize==0))&&aClientHandle);

    //
    Reset(aClientHandle);
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    //Set uid for this item
    iUid=m.IncrementCounter();
    if (aSize) // Could be zero sized, e.g. zero-length descriptor
        {
        (void)Mem::Copy((TAny*)((TUint8*)this+iBufOffset),aSrc,aSize);
        iBufLen=aSize;
        //
        // Need to increment ref count if adding a 'sleeping' media/media array object
        //
        TAny* data=Buf(aClientHandle);
        MMPXData* d=MMPXData::Data(aClientHandle,data,iBufLen);
        if(d)
            {
            d->AddRef(aClientHandle);
            }
        }
    }

// -----------------------------------------------------------------------------
// RMPXDataItem::Data
// -----------------------------------------------------------------------------
//
TAny* RMPXDataItem::Buf(TUint aClientHandle)
    {
    ASSERT(aClientHandle);
    return iBufLen?(TAny*)((TUint8*)this+iBufOffset):NULL;
    }
    
// -----------------------------------------------------------------------------
// RMPXDataItem::Insert
// -----------------------------------------------------------------------------
//
void RMPXDataItem::Insert(
    TUint aClientHandle,
    RMPXDataItem* aPrevious,
    RMPXDataItem* aNext)
    {
    MPX_ASSERT(aClientHandle);
    //
    iPreviousOffset=aPrevious?OFFSET(aClientHandle,aPrevious):0;
    iNextOffset=aNext?OFFSET(aClientHandle,aNext):0;
    TInt thisOffset=OFFSET(aClientHandle,this);
    if (aPrevious)
        {
        aPrevious->iNextOffset=thisOffset;
        }
    if (aNext)
        {
        aNext->iPreviousOffset=thisOffset;
        }
    }

// -----------------------------------------------------------------------------
// RMPXDataItem::Insert
// -----------------------------------------------------------------------------
//
void RMPXDataItem::Insert(
    TUint aClientHandle,
    TInt aPreviousOffset,
    TInt aNextOffset)
    {
    MPX_ASSERT(aClientHandle);
    iPreviousOffset=aPreviousOffset;
    iNextOffset=aNextOffset;
    TInt thisOffset=OFFSET(aClientHandle,this);
    if (aPreviousOffset)
        {
        RMPXDataItem* previous=ITEM(aClientHandle,aPreviousOffset);
        previous->iNextOffset=thisOffset;
        }
    if (aNextOffset)
        {
        RMPXDataItem* next=ITEM(aClientHandle,aNextOffset);
        next->iPreviousOffset=thisOffset;
        }
    }

// -----------------------------------------------------------------------------
// RMPXDataItem::Set
// -----------------------------------------------------------------------------
//
void RMPXDataItem::Set(TUint aClientHandle,RMPXDataItem* aOldItem)
    {
    MPX_ASSERT(aOldItem&&aClientHandle);
    //
    iPreviousOffset=aOldItem->iPreviousOffset;
    iNextOffset=aOldItem->iNextOffset;
    TInt thisOffset=OFFSET(aClientHandle,this);
    if (iPreviousOffset)
        {
        RMPXDataItem* previous=ITEM(aClientHandle,iPreviousOffset);
        previous->iNextOffset=thisOffset;
        }
    if (iNextOffset)
        {
        RMPXDataItem* next=ITEM(aClientHandle,iNextOffset);
        next->iPreviousOffset=thisOffset;
        }
    
    }

// -----------------------------------------------------------------------------
// RMPXDataItem::Set
// -----------------------------------------------------------------------------
//
void RMPXDataItem::Set(TUint aClientHandle,TInt aOldItemOffset)
    {
    RMPXDataItem* oldItem=ITEM(aClientHandle,aOldItemOffset);
    Set(aClientHandle,oldItem);
    }

// -----------------------------------------------------------------------------
// RMPXDataItem::Remove
// -----------------------------------------------------------------------------
//
void RMPXDataItem::Remove(TUint aClientHandle)
    {
    MPX_ASSERT(aClientHandle);
    if (iPreviousOffset)
        {
        RMPXDataItem* previous=ITEM(aClientHandle,iPreviousOffset);
        previous->iNextOffset=iNextOffset;
        }
    if (iNextOffset)
        {
        RMPXDataItem* next=ITEM(aClientHandle,iNextOffset);
        next->iPreviousOffset=iPreviousOffset;
        }
    iPreviousOffset=0;
    iNextOffset=0;
    }


// -----------------------------------------------------------------------------
// RMPXDataItemArray::RMPXDataItemArray
// -----------------------------------------------------------------------------
//
RMPXDataItemArray::RMPXDataItemArray()
    : iFirstItemOffset(0),iLastItemOffset(0),iNumItems(0),iPos(0),iIndex(0)
    {
    }

// -----------------------------------------------------------------------------
// RMPXDataItemArray::Close
// -----------------------------------------------------------------------------
//    
void RMPXDataItemArray::Close(TUint aClientHandle)
    { 
    MPX_ASSERT(aClientHandle); 
    Reset(aClientHandle);
    }

// -----------------------------------------------------------------------------
// RMPXDataItemArray::DataItem
// -----------------------------------------------------------------------------
//
RMPXDataItem* RMPXDataItemArray::DataItem(TUint aClientHandle,TInt aIndex)
    {
    MPX_ASSERT(aClientHandle&&aIndex>=0&&aIndex<iNumItems);
    //
    RMPXDataItem* dataItem=NULL;
    while(!dataItem)
        {
        //
        // Start looking from the current position: this is an optimisation
        // since the client is likely to either iterate from this position,
        // or make another query from this item. iPos and iIndex MUST 
        // correspond
        //
        RMPXDataItem* item=ITEM(aClientHandle,iPos);
        if (iIndex==aIndex)
            {
            dataItem=item;
            }
        else if (iIndex>aIndex)
            {
            iIndex--;
            iPos=item->PreviousOffset();
            }
        else // if (iIndex<aIndex)
            {
            iIndex++;
            iPos=item->NextOffset();
            }
        }
    MPX_ASSERT(dataItem && (TInt)dataItem > (TInt)&RMPXHeapManager::HeapManager(aClientHandle));
    return dataItem;
    }

// -----------------------------------------------------------------------------
// RMPXDataItemArray::Delete
// -----------------------------------------------------------------------------
//
void RMPXDataItemArray::Delete(TUint aClientHandle,RMPXDataItem* aItem)
    {
    MPX_ASSERT(aClientHandle&&aItem);
    aItem->Close(aClientHandle);  // Release objects in the buffer
    TInt r=RMPXHeapManager::HeapManager(aClientHandle).Free(aClientHandle,aItem);
    MPX_ASSERT(r==KErrNone);
    (void)r;
    }

// -----------------------------------------------------------------------------
// RMPXDataItemArray::Append
// -----------------------------------------------------------------------------
//
void RMPXDataItemArray::Append(TUint aClientHandle,RMPXDataItem& aItem)
    {
    MPX_ASSERT(aClientHandle);
    //
    aItem.Insert(aClientHandle,iLastItemOffset,0);
    iLastItemOffset=OFFSET(aClientHandle,&aItem);
    if (iNumItems++==0)
        {
        iFirstItemOffset=iLastItemOffset;
        }
    iPos=iFirstItemOffset;
    iIndex=0;
    }

// -----------------------------------------------------------------------------
// RMPXDataItemArray::Set
// -----------------------------------------------------------------------------
//
void RMPXDataItemArray::Set(TUint aClientHandle,RMPXDataItem& aItem,TInt aIndex)
    {
    MPX_ASSERT(aClientHandle&&aIndex>=0&&aIndex<iNumItems);
    //
    RMPXDataItem* oldItem=DataItem(aClientHandle,aIndex);
    aItem.Set(aClientHandle,oldItem);
    TInt oldItemOffset=OFFSET(aClientHandle,oldItem); 
    TInt itemOffset=OFFSET(aClientHandle,&aItem); 
    if (oldItemOffset==iLastItemOffset) 
        {
        iLastItemOffset=itemOffset;
        }
     if (oldItemOffset==iFirstItemOffset) 
        {
        iFirstItemOffset=itemOffset;
        } 
    Delete(aClientHandle,oldItem);
    iPos=iFirstItemOffset;
    iIndex=0;
    }

// -----------------------------------------------------------------------------
// RMPXDataItemArray::Insert
// -----------------------------------------------------------------------------
//
void RMPXDataItemArray::Insert(TUint aClientHandle,RMPXDataItem& aItem,TInt aIndex)
    {
    MPX_ASSERT(aClientHandle&&aIndex>=0&&aIndex<iNumItems);
    //
    RMPXDataItem* itemBefore=aIndex==0?NULL:DataItem(aClientHandle,aIndex-1);
    RMPXDataItem* itemAfter=aIndex<iNumItems?DataItem(aClientHandle,aIndex):NULL;
    aItem.Insert(aClientHandle,itemBefore,itemAfter);
    TInt itemOffset=OFFSET(aClientHandle,&aItem);
    if (!itemAfter) // this is the last one
        {
        iLastItemOffset=itemOffset;
        }
    if (!itemBefore||iNumItems==0)  // this is the first or the only one
        {
        iFirstItemOffset=itemOffset;
        } 
    iPos=iFirstItemOffset;
    iIndex=0;
    ++iNumItems;
    }

// -----------------------------------------------------------------------------
// RMPXDataItemArray::Remove
// -----------------------------------------------------------------------------
//
void RMPXDataItemArray::Remove(TUint aClientHandle,TInt aIndex)
    {
    MPX_ASSERT(aClientHandle&&aIndex>=0&&aIndex<iNumItems);
    //
    RMPXDataItem* item=DataItem(aClientHandle,aIndex);
    TInt itemOffset=OFFSET(aClientHandle,item);
    if (iFirstItemOffset==itemOffset)
        {
        iFirstItemOffset=item->NextOffset();
        }
    if (iLastItemOffset==itemOffset)
        {
        iLastItemOffset=item->PreviousOffset();
        }   
    item->Remove(aClientHandle);
    Delete(aClientHandle,item);
    iPos=iFirstItemOffset; 
    iIndex=0; 
    --iNumItems;
    }

// -----------------------------------------------------------------------------
// RMPXDataItemArray::Reset
// -----------------------------------------------------------------------------
//
void RMPXDataItemArray::Reset(TUint aClientHandle)
    {
    if (iFirstItemOffset)
        {
        RMPXDataItem* item=ITEM(aClientHandle,iFirstItemOffset);
        while(item)
            {
            Delete(aClientHandle,item);
            item=item->NextOffset()?ITEM(aClientHandle,item->NextOffset()):NULL;
            }
        }
    // Reset members
    iFirstItemOffset=0;
    iLastItemOffset=0;
    iNumItems=0;
    iPos=0;
    iIndex=0; 
    }

// -----------------------------------------------------------------------------
// RMPXDataItemArray::Find
// -----------------------------------------------------------------------------
//
TInt RMPXDataItemArray::Find(TUint aClientHandle,const RMPXDataItem& aItem)
    {
    TInt r=KErrNotFound;
    if (iFirstItemOffset)
        {
        TInt index=KErrNotFound;
        RMPXDataItem* item=ITEM(aClientHandle,iFirstItemOffset);
        while(item)
            {  
            index++;
            TUint8* d1 = reinterpret_cast<TUint8*>(item->Buf(aClientHandle));
            TUint8* d2 = 
             reinterpret_cast<TUint8*>(const_cast<RMPXDataItem*>(&aItem)->
                                                            Buf(aClientHandle));
            if (Mem::Compare(d1, item->Size(), d2, aItem.Size()) == 0)
                {
                r=index;
                break;
                }
            item=item->NextOffset()?ITEM(aClientHandle,item->NextOffset()):NULL;
            }
        }  
    return r;
    }
// End of file
