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
* Description:  Implementation of data
*
*/

#include <mpxlog.h>
#include "mpxheapmanager.h"
#include "mpxmedia.h"
#include "mpxmediaarray.h"
#include "mpxfixedarray.h"
#include "mpxmediadataarray.h"
#include "mpxdata.h"

/**
*  Class RMPXData. Implementation of MMPXData interface
*/
NONSHARABLE_CLASS( RMPXData ) : public MMPXData
    {
public:
    /**
     * C++ default constructor.
     */
    RMPXData();
public:
    /**
    * Copies contents of aData to this object, deleting any
    * existing data
    */
    TInt Copy(TUint aClientHandle,const RMPXData& aData);

private: // From MMPXData
    TUint DataHandle(TUint aClientHandle);
    void AddRef(TUint aClientHandle);
    void Release(TUint aClientHandle);
    void AddClientRef(TUint aClientHandle);
    TPtrC8 Value(TUint aClientHandle,TInt aIndex);
    const TMPXAttribute& Attribute(TUint aClientHandle,TInt aIndex);
    TMPXAttributeType AttributeType(TUint aClientHandle,TInt aIndex);
    TArray<TInt> SupportedIds(TUint aClientHandle);
    TInt SetSupportedIds(TUint aClientHandle,const TArray<TInt>& aSupportedIds);
    void Delete(TUint aClientHandle,TInt aIndex);
    void Set(TUint aClientHandle,const TMPXAttribute& aAttr,
             TMPXAttributeType aType,const TDesC8& aData,TInt aIndex);
    TInt Insert(TUint aClientHandle,const TMPXAttribute& aAttr,
                TMPXAttributeType aType,const TDesC8& aData,TInt aIndex);
    TInt Append(TUint aClientHandle,const TMPXAttribute& aAttr,
                TMPXAttributeType aType,const TDesC8& aData);
    void Reset(TUint aClientHandle);
    TInt Count(TUint aClientHandle);
    TInt Index(TUint aClientHandle,const TMPXAttribute& aAttr);
    TInt Index(TUint aClientHandle,TInt aUid);
    TInt Uid(TUint aClientHandle,TInt aIndex);

private:
    /**
    * Number of clients (for example CMPXMedia) sharing this object
    */
    TInt iRef;
    /**
    * Array of data items
    */
    RMPXMediaDataItemArray iDataArray;
    /**
    * The IDs supported by the source of the data
    */
    RMPXFixedArray<TInt> iSupportedIds;
    };


// -----------------------------------------------------------------------------
// MMPXData::NewL
// -----------------------------------------------------------------------------
//
MMPXData* MMPXData::NewL(TUint aClientHandle)
    {
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    m.Lock(aClientHandle);
    MMPXData* d=m.Alloc<RMPXData>(aClientHandle);
    m.Unlock(aClientHandle);
    User::LeaveIfNull(d);
    return d;
    }

// -----------------------------------------------------------------------------
// MMPXData::NewL
// -----------------------------------------------------------------------------
//
MMPXData* MMPXData::NewL(TUint aClientHandle,const MMPXData& aData)
    {
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    m.Lock(aClientHandle);
    RMPXData* d1=m.Alloc<RMPXData>(aClientHandle); // Create empty one
    TInt r=KErrNone;
    if (d1)
        { // Now copy data from old one
        const RMPXData& d2=static_cast<const RMPXData&>(aData);
        r=d1->Copy(aClientHandle,d2);
        }
    m.Unlock(aClientHandle);
    User::LeaveIfNull(d1);
    User::LeaveIfError(r);
    return d1;
    }

// -----------------------------------------------------------------------------
// MMPXData::Data
// -----------------------------------------------------------------------------
//
MMPXData* MMPXData::Data(TUint aClientHandle,TAny* aData,TInt aSize)
    {
    MMPXData* d=NULL;
    //
    // Check its the right size, and it's on a word boundary first:
    // if either is not true, then its not an MMPXData implementation
    // for sure
    //
    TBool aligned=((TUint)aData&3)==0;
    const TInt KDataSize=sizeof(TInt)+sizeof(TUint);

    if (aSize==KDataSize&&aligned)
        {
        TUint* ptr=(TUint*)aData;
        MMPXData::TMPXObjectType t=*(MMPXData::TMPXObjectType*)ptr;
        switch(t)
            {
            case MMPXData::EMedia:
            case MMPXData::EMediaArray:
                d=MMPXData::Data(aClientHandle,*++ptr);
                MPX_ASSERT(d);
                MPX_ASSERT_WORD_ALIGNED(d);
                break;
                //
            default:
                break;
            }
        }
    return d;
    }

// -----------------------------------------------------------------------------
// MMPXData::Data
// -----------------------------------------------------------------------------
//
MMPXData* MMPXData::Data(TUint aClientHandle,const TDesC8& aData)
    {
    return Data(aClientHandle,(TAny*)aData.Ptr(),aData.Size());
    }

// -----------------------------------------------------------------------------
// MMPXData::Data
// -----------------------------------------------------------------------------
//
MMPXData* MMPXData::Data(TUint aClientHandle,TUint aDataHandle)
    {
    return RMPXHeapManager::Ptr<RMPXData>(aClientHandle,aDataHandle);
    }

// -----------------------------------------------------------------------------
// MMPXData::ClientHandle
// -----------------------------------------------------------------------------
//
TUint MMPXData::ClientHandle()
    {
    // Already protected by the mutex
    return RMPXHeapManager::ClientHandle();
    }

// -----------------------------------------------------------------------------
// MMPXData::AddClientRef
// -----------------------------------------------------------------------------
//
void MMPXData::AddClientRef(TUint aClientHandle)
    {
    RMPXHeapManager::HeapManager(aClientHandle).AddRef(aClientHandle);
    }


// -----------------------------------------------------------------------------
// MMPXData::ReleaseClient
// -----------------------------------------------------------------------------
//
void MMPXData::ReleaseClient(TUint aClientHandle)
    {
    RMPXHeapManager::HeapManager(aClientHandle).Release(aClientHandle);
    }

// -----------------------------------------------------------------------------
// MMPXData::HeapMemoryInfoL
// -----------------------------------------------------------------------------
//
void MMPXData::HeapMemoryInfoL( TInt& aTotal, TInt& aUsed )
    {
    return RMPXHeapManager::HeapMemoryInfoL( aTotal, aUsed );
    }

// -----------------------------------------------------------------------------
// RMPXData::LockHeap
// -----------------------------------------------------------------------------
//
void MMPXData::LockHeap(TUint aClientHandle)
    {
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    m.Lock(aClientHandle);
    }

// -----------------------------------------------------------------------------
// RMPXData::UnlockHeap
// -----------------------------------------------------------------------------
//
void MMPXData::UnlockHeap(TUint aClientHandle)
    {
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    m.Unlock(aClientHandle);
    }

// -----------------------------------------------------------------------------
// RMPXData::RMPXData
// -----------------------------------------------------------------------------
//
RMPXData::RMPXData()
    :iRef(0)
    {
    }

// -----------------------------------------------------------------------------
// RMPXData::DataHandle
// -----------------------------------------------------------------------------
//
TUint RMPXData::DataHandle(TUint aClientHandle)
    {
    return RMPXHeapManager::Offset(aClientHandle,this);
    }

// -----------------------------------------------------------------------------
// RMPXData::AddRef
// -----------------------------------------------------------------------------
//
void RMPXData::AddRef(TUint aClientHandle)
    {
#ifdef __ENABLE_MPX_GARBAGE_COLLECTOR
    TUint dataHandle=DataHandle(aClientHandle); //GC
    (void)RMPXHeapManager::HeapManager(aClientHandle).AddData(aClientHandle,dataHandle);//GC
#else
    (void) aClientHandle;
#endif // __ENABLE_MPX_GARBAGE_COLLECTOR
    ++iRef;
    }

// -----------------------------------------------------------------------------
// RMPXData::Release
// -----------------------------------------------------------------------------
//
void RMPXData::Release(TUint aClientHandle)
    {
    --iRef;
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
#ifdef __ENABLE_MPX_GARBAGE_COLLECTOR
    TUint dataHandle=DataHandle(aClientHandle); //GC
    // if the reference of the data is zero, remove all reference from all
    // clients.
    (void)m.RemoveData(aClientHandle,dataHandle,iRef==0); //GC
#endif // __ENABLE_MPX_GARBAGE_COLLECTOR
    if(iRef==0)
        {
        m.Lock(aClientHandle);
        iDataArray.Close(aClientHandle);
        iSupportedIds.Close(aClientHandle);
        m.Free(aClientHandle,this);
        m.Unlock(aClientHandle);
        }
    }

// -----------------------------------------------------------------------------
// RMPXData::Value
// -----------------------------------------------------------------------------
//
TPtrC8 RMPXData::Value(TUint aClientHandle,TInt aIndex)
    {
    return iDataArray.Data(aClientHandle,aIndex);
    }

// -----------------------------------------------------------------------------
// RMPXData::Attribute
// -----------------------------------------------------------------------------
//
const TMPXAttribute& RMPXData::Attribute(TUint aClientHandle,TInt aIndex)
    {
    return iDataArray.MediaDataItem(aClientHandle,aIndex)->Attribute();
    }

// -----------------------------------------------------------------------------
// RMPXData::AttributeType
// -----------------------------------------------------------------------------
//
TMPXAttributeType RMPXData::AttributeType(TUint aClientHandle,TInt aIndex)
    {
    return iDataArray.MediaDataItem(aClientHandle,aIndex)->Type();
    }

// -----------------------------------------------------------------------------
// RMPXData::SupportedIds
// -----------------------------------------------------------------------------
//
TArray<TInt> RMPXData::SupportedIds(TUint aClientHandle)
    {
    return iSupportedIds.Array(aClientHandle);
    }

// -----------------------------------------------------------------------------
// RMPXData::SetSupportedIds
// -----------------------------------------------------------------------------
//
TInt RMPXData::SetSupportedIds(TUint aClientHandle,const TArray<TInt>& aSupportedIds)
    {
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    m.Lock(aClientHandle);
    TInt r=iSupportedIds.Copy(aClientHandle,aSupportedIds);
    m.Unlock(aClientHandle);
    return r;
    }

// -----------------------------------------------------------------------------
// RMPXData::Delete
// -----------------------------------------------------------------------------
//
void RMPXData::Delete(TUint aClientHandle,TInt aIndex)
    {
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    m.Lock(aClientHandle);
    iDataArray.Remove(aClientHandle,aIndex);
    m.Unlock(aClientHandle);
    }

// -----------------------------------------------------------------------------
// RMPXData::Set
// -----------------------------------------------------------------------------
//
void RMPXData::Set(
    TUint aClientHandle,
    const TMPXAttribute& aAttr,
    TMPXAttributeType aType,
    const TDesC8& aData,
    TInt aIndex)
    {
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    m.Lock(aClientHandle);
    iDataArray.Set(aClientHandle,aAttr,aType,aData,aIndex);
    m.Unlock(aClientHandle);
    }

// -----------------------------------------------------------------------------
// RMPXData::Insert
// -----------------------------------------------------------------------------
//
TInt RMPXData::Insert(
    TUint aClientHandle,
    const TMPXAttribute& aAttr,
    TMPXAttributeType aType,
    const TDesC8& aData,
    TInt aIndex)
    {
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    m.Lock(aClientHandle);
    TInt r=iDataArray.Insert(aClientHandle,aAttr,aType,aData,aIndex);
    m.Unlock(aClientHandle);
    return r;
    }

// -----------------------------------------------------------------------------
// RMPXData::Append
// -----------------------------------------------------------------------------
//
TInt RMPXData::Append(
    TUint aClientHandle,
    const TMPXAttribute& aAttr,
    TMPXAttributeType aType,
    const TDesC8& aData)
    {
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    m.Lock(aClientHandle);
    TInt r=iDataArray.Append(aClientHandle,aAttr,aType,aData);
    m.Unlock(aClientHandle);
    return r;
    }

// -----------------------------------------------------------------------------
// RMPXData::Reset
// -----------------------------------------------------------------------------
//
void RMPXData::Reset(TUint aClientHandle)
    {
    RMPXHeapManager& m=RMPXHeapManager::HeapManager(aClientHandle);
    m.Lock(aClientHandle);
    iDataArray.Reset(aClientHandle);
    m.Unlock(aClientHandle);
    }

// -----------------------------------------------------------------------------
// RMPXData::Count
// -----------------------------------------------------------------------------
//
TInt RMPXData::Count(TUint /*aClientHandle*/)
    {
    return iDataArray.Count();
    }

// -----------------------------------------------------------------------------
// RMPXData::Index
// -----------------------------------------------------------------------------
//
TInt RMPXData::Index(TUint aClientHandle,const TMPXAttribute& aAttr)
    {
    return iDataArray.Index(aClientHandle,aAttr);
    }

// -----------------------------------------------------------------------------
// RMPXData::Index
// -----------------------------------------------------------------------------
//
TInt RMPXData::Index(TUint aClientHandle,TInt aUid)
    {
    TInt ret(KErrNotFound);
    for (TInt i=iDataArray.Count();--i>=0;)
        {
        RMPXMediaDataItem* item=iDataArray.MediaDataItem(aClientHandle,i);
        if (item->Uid()==aUid)
            {
            ret = i;
            break;
            }
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// RMPXData::Uid
// -----------------------------------------------------------------------------
//
TInt RMPXData::Uid(TUint aClientHandle,TInt aIndex)
    {
    RMPXMediaDataItem* item=iDataArray.MediaDataItem(aClientHandle,aIndex);
    return item->Uid();
    }

// -----------------------------------------------------------------------------
// RMPXData::Copy
// -----------------------------------------------------------------------------
//
TInt RMPXData::Copy(TUint aClientHandle,const RMPXData& aData)
    {
    // Now copy data from old one. The data members are iDataArray
    // and iSupportedIds that need duplicating, and any values that they
    // now hold are deleted; iRef remains the same
    //
    TInt r(KErrNone);
    if (aData.iSupportedIds.Count())
        {
        r=iSupportedIds.RMPXFixedArrayBase::Copy(aClientHandle,aData.iSupportedIds);
        }
    return r==KErrNone?iDataArray.Copy(aClientHandle,aData.iDataArray):r;
    }

// End of file
