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

#include "mpxmediadataarray.h"
#include "mpxheapmanager.h"
#include "mpxdata.h"

#define ITEM(x,y) RMPXHeapManager::Ptr<RMPXMediaDataItem>(x,y)

// -----------------------------------------------------------------------------
// RMPXMediaDataItemArray::Append
// -----------------------------------------------------------------------------
//
TInt RMPXMediaDataItemArray::Append(
    TUint aClientHandle,
    const TMPXAttribute& aAttr,
    TMPXAttributeType aAttrType,
    const TDesC8& aData)
    {
    MPX_ASSERT(aClientHandle);
    //
    TInt r=KErrNone;
    RMPXMediaDataItem* item=NewItem(aClientHandle,aAttr,aAttrType,aData);
    if (item)
        {
        RMPXDataItemArray::Append(aClientHandle,*item);
        }
    else
        {
        r=KErrNoMemory;
        }
    return r;
    }


// -----------------------------------------------------------------------------
// RMPXMediaDataItemArray::Set
// -----------------------------------------------------------------------------
//
TInt RMPXMediaDataItemArray::Set(
    TUint aClientHandle,
    const TMPXAttribute& aAttr,
    TMPXAttributeType aAttrType,
    const TDesC8& aData,
    TInt aIndex)
    {
    TInt r=KErrNone;
    RMPXMediaDataItem* item=NewItem(aClientHandle,aAttr,aAttrType,aData);
    if (item)
        {       
        RMPXDataItemArray::Set(aClientHandle,*item,aIndex);
        }
    else
        {
        r=KErrNoMemory;
        }
    return r;
    }

// -----------------------------------------------------------------------------
// RMPXMediaDataItemArray::RMPXMediaDataItemArray
// -----------------------------------------------------------------------------
//
TInt RMPXMediaDataItemArray::Insert(
    TUint aClientHandle,
    const TMPXAttribute& aAttr,
    TMPXAttributeType aAttrType,
    const TDesC8& aData,
    TInt aIndex)
    {
    TInt r=KErrNone;
    RMPXMediaDataItem* item=NewItem(aClientHandle,aAttr,aAttrType,aData);
    if (item)
        {       
        RMPXDataItemArray::Insert(aClientHandle,*item,aIndex);
        }
    else
        {
        r=KErrNoMemory;
        }
    return r;
    }

// -----------------------------------------------------------------------------
// RMPXMediaDataItemArray::Index
// -----------------------------------------------------------------------------
//
TInt RMPXMediaDataItemArray::Index(TUint aClientHandle,const TMPXAttribute& aAttr)
    {
    TInt index( KErrNotFound );
    RMPXMediaDataItem* item=NULL;
    
    if (iFirstItemOffset)
        {
        item=ITEM(aClientHandle,iFirstItemOffset);
        while(item)
            {
            index++;
            if (item->Attribute()==aAttr)
                {
                //
                // Found it, so break outer of the loop, but first store
                // the current position in case an index based query will
                // follow (e,g, for data, type etc.)
                //
                iIndex=index;
                iPos=RMPXHeapManager::Offset(aClientHandle,item);
                break;
                }
            TInt next=item->NextOffset();
            item=next?ITEM(aClientHandle,next):NULL;
            }
        }
    return item?index:KErrNotFound;
    }
 
 // -----------------------------------------------------------------------------
// RMPXMediaDataItemArray::NewItem
// -----------------------------------------------------------------------------
//   
 RMPXMediaDataItem* RMPXMediaDataItemArray::NewItem(
    TUint aClientHandle,
    const TMPXAttribute& aAttr,
    TMPXAttributeType aAttrType,
    const TDesC8& aData)
    {
    MPX_ASSERT(aClientHandle);
    //
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    TAny* ptr=m.Alloc(aClientHandle, sizeof(RMPXMediaDataItem) + aData.Size());
    RMPXMediaDataItem* item(NULL);
    if (ptr) 
        { 
        //Alloc media data item
        item = new(ptr)RMPXMediaDataItem();
        //Copy data to RMPXMediaDataItem
        item->Copy(aClientHandle,aData);
        item->SetAttribute(aAttr,aAttrType);
        }
    return item;
    }

// -----------------------------------------------------------------------------
// RMPXDataItemArray::Copy
// -----------------------------------------------------------------------------
//
TInt RMPXMediaDataItemArray::Copy(
    TUint aClientHandle,
    const RMPXMediaDataItemArray& aArray)
    {
    Reset(aClientHandle); // Clear existing data
    TInt r=KErrNone;
    TInt first=aArray.iFirstItemOffset;
    if (first)
        {
        RMPXMediaDataItem* item=ITEM(aClientHandle,first);
        while(item)
            {
            MMPXData* d=MMPXData::Data(aClientHandle,
                                       item->Buf(aClientHandle),
                                       item->Size());
            if (d)
                { 
                // It's a global object. e.g. media or media array, so we need to copy
                // that and place the handle into the buffer
                //
                MMPXData* newd=NULL;
                TRAP(r,newd=MMPXData::NewL(aClientHandle,*d)); // will end up recursive
                if (r==KErrNone)
                    {
                    // Create the data: a buffer of 8 bytes
                    //
                    const TInt KMediaStreamLen=sizeof(MMPXData::TMPXObjectType)+sizeof(TUint);
                    TBuf8<KMediaStreamLen> newbuf;
                    //
                    TUint* ptr=(TUint*)newbuf.Ptr(); // pointer to the new (empty) buffer
                    TUint* oldPtr=(TUint*)item->Buf(aClientHandle); // pointer to the old data
                    //
                    *ptr=*oldPtr; // the TMPXObjectType
                    *++ptr=newd->DataHandle(aClientHandle); //  the data: handle to this new data
                    //
                    newbuf.SetLength(KMediaStreamLen);
                    //
                    r=Append(aClientHandle,item->Attribute(),item->Type(),newbuf);
                    //
                    // Don't need to call newd->Release(aClientHandle) currently, since the Append()
                    // would have created a ref count of 1, and the MMPXData::NewL() doesn't.
                    //
                    }
                }
            else 
                {
                // Regular streamed object, so we can duplicate it by just copying the bytes
                // in an Append()
                //
                r=Append(aClientHandle,item->Attribute(),item->Type(),item->Data(aClientHandle));
                }
            item=item->NextOffset()&&r==KErrNone?ITEM(aClientHandle,item->NextOffset()):NULL;
            }
        }
    return r;
    }

// End of file

