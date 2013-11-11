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
* Description:  Implementation of media base class
*
*/


#include <mpxlog.h>
#include <mpxuser.h>
#include "mpxmediabase.h"


// -----------------------------------------------------------------------------
// C++ Constructor.
// -----------------------------------------------------------------------------
//
CMPXMediaBase::CMPXMediaBase()
    {
    // Open the global chunk and return the handle of the global chunk.
    // If called for the first time, creates the global chunk and heap manager.
    iClientHandle=MMPXData::ClientHandle();
    MMPXData::AddClientRef(iClientHandle);  // Ref for this thread
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMPXMediaBase::~CMPXMediaBase()
    {
    ResetLocal();
    iValues.Close();
    if (iData)
        {
        iData->Release(iClientHandle);
        MMPXData::ReleaseClient(iClientHandle); // This thread's reference count
        }
    }

// -----------------------------------------------------------------------------
// CMPXMediaBase::ConstructL
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::ConstructL()
    {
    MMPXData* d=MMPXData::NewL(iClientHandle);
    TInt dh=d->DataHandle(iClientHandle);
    ConstructL(dh,*d);
    }

// -----------------------------------------------------------------------------
// CMPXMediaBase::ConstructL
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::ConstructL(const CMPXMediaBase& aBase)
    {
    ConstructL(aBase.iDataHandle,*aBase.iData);
    }

// -----------------------------------------------------------------------------
// CMPXMediaBase::CopyConstructL
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::CopyConstructL(const CMPXMediaBase& aBase)
    {
    MMPXData* d=MMPXData::NewL(iClientHandle,*aBase.iData);
    TInt dh=d->DataHandle(iClientHandle);
    ConstructL(dh,*d);
    }

// -----------------------------------------------------------------------------
// CMPXMediaBase::ConstructL
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::ConstructL(TInt aDataHandle)
    {
    MMPXData* d=MMPXData::Data(iClientHandle,aDataHandle);
    ConstructL(aDataHandle,*d);
    }

// -----------------------------------------------------------------------------
// CMPXMediaBase::ConstructL
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::ConstructL(TInt aDataHandle,MMPXData& aData)
    {
    MPX_ASSERT(!iData&&!iDataHandle&&aDataHandle);
    if (aDataHandle==0)
        {
        User::Leave(KErrArgument);
        }
    iDataHandle=aDataHandle;
    iData=&aData;
    iData->AddRef(iClientHandle);
    iValues.ReserveL(Count());
    }

// -----------------------------------------------------------------------------
// CMPXMediaBase::Clear
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::Clear()
    {
    if (iData)
        {
        ResetLocal(); // remove local data only, keep heap data for other media objects
        iData->Release(iClientHandle); // MMPXData's reference count
        iData=NULL;
        iDataHandle=0;
        }
    }

// -----------------------------------------------------------------------------
// CMPXMediaBase::ResetLocal
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::ResetLocal()
    {
    THashMapIter<TInt, TValue> iter(iValues);
    const TValue* v = NULL;
    while ((v = iter.NextValue())!=NULL) 
        {
        DeletePtr(v->iValue, v->iType);
        iter.RemoveCurrent();
        }
    iValues.Close();
    } 

TInt CMPXMediaBase::SetLocal(const TValue& aValue)
    {
    DeleteLocalByUid(aValue.iUid);
    return iValues.Insert(aValue.iUid, aValue);
    }
    
// -----------------------------------------------------------------------------
// CMPXMediaBase::LocalIndex
// -----------------------------------------------------------------------------
//
const CMPXMediaBase::TValue* CMPXMediaBase::LocalValue(TInt aUid) const
    {
    return iValues.Find(aUid);
    }

// -----------------------------------------------------------------------------
// CMPXMediaBase::DeleteLocal
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::DeleteLocalByUid(const TInt aUid)
    {
    TValue* val = iValues.Find(aUid);
    if (val)
        {
        DeletePtr(val->iValue,val->iType);
        iValues.Remove(aUid);
        }
    }

// -----------------------------------------------------------------------------
// CMPXMediaBase::DeleteLocal
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::DeleteLocal(TInt aIndex)
    {
    MPX_ASSERT(KErrNotFound != aIndex);
    TInt uid = iData->Uid(iClientHandle,aIndex);
    DeleteLocalByUid(uid);
    }

// -----------------------------------------------------------------------------
// CMPXMediaBase::DeletePtr
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::DeletePtr(TAny* aValue,TMPXAttributeType aType)
    {
    switch(aType)
        {
        case EMPXTypeCObject:
            delete static_cast<CBase*>(aValue);
            break;
        default:
            // Both T objects and text are held in TPtrC8 which are
            // allocated on the heap.
            //
            User::Free(aValue);
            break;
        }
    }

// -----------------------------------------------------------------------------
// Externalize
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::DoExternalizeL(
    RWriteStream& aStream,
    MMPXData::TMPXObjectType aType) const
    {
    //
    // NB: Client must persist the object until after it has been
    // internalized, i.e. there must always be at least one object
    // in existance for Internalize to work!
    //
    MPX_ASSERT(iDataHandle);
    //
    aStream.WriteInt32L(aType);
    aStream.WriteUint32L(iDataHandle);
    }

// -----------------------------------------------------------------------------
// Internalize. ConstructL() will have been called prior to this so there
// will definitely be a iClientHandle, iDataHandle and iData, though there may or
// may not be any data added. Either way, the data should be reset.
// -----------------------------------------------------------------------------
//
void CMPXMediaBase::DoInternalizeL(
    RReadStream& aStream,
    MMPXData::TMPXObjectType /*aType*/)
    {
    MPX_ASSERT(iClientHandle);
    //
    Clear();  // Clear local data only from this object
    TInt type( aStream.ReadInt32L() ); // object type
    if ( type != MMPXData::EMedia &&
         type != MMPXData::EMediaArray)
        {
        MPX_DEBUG1("CMPXMediaBase::DoInternalizeL(): Invalid Type");
        User::Leave( KErrArgument );
        }
    //
    iDataHandle=aStream.ReadUint32L();
    iData=MMPXData::Data(iClientHandle,iDataHandle);
    iData->AddRef(iClientHandle);
    }

// ----------------------------------------------------------------------------
// CMPXMediaBase::LockHeapLC
// ----------------------------------------------------------------------------
//
void CMPXMediaBase::LockHeapLC() const
    {
    MMPXData::LockHeap( iClientHandle );
    CleanupStack::PushL(TCleanupItem(UnlockHeap, const_cast<CMPXMediaBase*>(this)));
    }

// ----------------------------------------------------------------------------
// CMPXMediaBase::UnlockHeap
//
// Executed when the cleanup stack item is destroyed as a result of
// CleanupStack::PopAndDestroy being called. This could either be called
// manually or as a result of a leave being generated and trapped.
//
// This is a static method.
// ----------------------------------------------------------------------------
//
void CMPXMediaBase::UnlockHeap(TAny* aMediaBase)
    {
    CMPXMediaBase* base = static_cast<CMPXMediaBase*>(aMediaBase);
    MMPXData::UnlockHeap(base->iClientHandle);
    }

// END OF FILE
