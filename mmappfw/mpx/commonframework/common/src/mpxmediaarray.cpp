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
* Description:  Implementation of array of media objects
*
*/


#include "mpxmediaarray.h"
#include "mpxmedia.h"
#include "mpxcmn.h"

// CONSTANTS
//
// Media attribute for the media object added to array
//
const TMPXAttributeData KMPXMediaArrayItem= {0x101FFC12, 0X01};

// -----------------------------------------------------------------------------
// CMPXMediaArray::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMediaArray* CMPXMediaArray::NewL()
    {
    CMPXMediaArray* a=new(ELeave)CMPXMediaArray();
    CleanupStack::PushL(a);
    a->ConstructL();
    CleanupStack::Pop(a);
    return a;
    }

// -----------------------------------------------------------------------------
// CMPXMediaArray::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMediaArray* CMPXMediaArray::NewL(const CMPXMediaArray& aArray)
    {
    CMPXMediaArray* a=new(ELeave)CMPXMediaArray();
    CleanupStack::PushL(a);
    a->ConstructL(aArray);
    CleanupStack::Pop(a);
    return a;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMediaArray::~CMPXMediaArray()
    {
    }

// -----------------------------------------------------------------------------
// Externalize
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMediaArray::ExternalizeL(RWriteStream& aStream) const
    {
    DoExternalizeL(aStream,MMPXData::EMediaArray);
    }

// -----------------------------------------------------------------------------
// Internalize. ConstructL() will have been called prior to this so there
// will definitely be a iClientHandle, iDataHandle and iData, though there may or
// may not be any data added. Either way, the data should be reset.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMediaArray::InternalizeL(RReadStream& aStream)
    {
    DoInternalizeL(aStream,MMPXData::EMediaArray);
    }

// -----------------------------------------------------------------------------
// Total number of objects contained in array
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXMediaArray::Count() const
    {
    return CMPXMediaBase::Count();
    }

// -----------------------------------------------------------------------------
// Array assessor
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMedia* CMPXMediaArray::operator[](TInt aIndex) const
    {
    return Value(aIndex);
    }

// -----------------------------------------------------------------------------
// Get a pointer to the media object at the specified position within the array
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMedia* CMPXMediaArray::AtL(TInt aIndex) const
    {
    return ValueL(aIndex);
    }

// -----------------------------------------------------------------------------
// Appends media object to array
// Takes the ownship of aMedia
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMediaArray::AppendL(const CMPXMedia* aMedia)
    {
    AppendL(*aMedia);
    delete const_cast<CMPXMedia*>(aMedia);
    }

// -----------------------------------------------------------------------------
// Appends media object to array
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMediaArray::AppendL(const CMPXMedia& aMedia)
    {
    User::LeaveIfError(AddToArray(aMedia,CMPXMediaBase::Count()));
    }

// -----------------------------------------------------------------------------
// Resets the media array
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMediaArray::Reset()
    {
    MPX_ASSERT(iClientHandle&&iData);
    //
    ResetLocal(); // Local data
    iData->Reset(iClientHandle); // Clears heap data for this object
    }

// -----------------------------------------------------------------------------
// Removes the specified media from the array
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMediaArray::Remove(TInt aIndex)
    {
    MPX_ASSERT(iData&&iClientHandle&&aIndex>=0&&aIndex<Count());
    DeleteLocal(aIndex);
    iData->Delete(iClientHandle,aIndex); // And data for this object
    }

// -----------------------------------------------------------------------------
// Inserts the specified media into the array at the specified position
// Takes the ownership of aMedia
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXMediaArray::Insert(const CMPXMedia *aMedia, TInt aPos)
    {
    TInt r=AddToArray(*aMedia,aPos);
    if (r==KErrNone)
        {
        delete const_cast<CMPXMedia*>(aMedia);
        }
    return r;
    }

// -----------------------------------------------------------------------------
// Inserts the specified media into the array at the specified position
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXMediaArray::Insert(const CMPXMedia& aMedia, TInt aPos)
    {
    return AddToArray(aMedia,aPos);
    }

// -----------------------------------------------------------------------------
// Sets the specified media into the array at the specified position
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMediaArray::Set(const CMPXMedia& aMedia, TInt aPos)
    {
    MMPXData::LockHeap( iClientHandle );
    DeleteLocal(aPos);
    (void)AddToArray(aMedia,aPos,ETrue);
    MMPXData::UnlockHeap( iClientHandle );
    }

// -----------------------------------------------------------------------------
// Inserts the specified media into the array at the specified position and
// leave if an error is encountered
// Takes ownership of the aMedia
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMediaArray::InsertL(const CMPXMedia *aMedia, TInt aPos)
    {
    User::LeaveIfError(Insert(aMedia,aPos));
    }

// -----------------------------------------------------------------------------
// Inserts the specified media into the array at the specified position and
// leave if an error is encountered
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMediaArray::InsertL(const CMPXMedia& aMedia, TInt aPos)
    {
    User::LeaveIfError(AddToArray(aMedia,aPos));
    }


// -----------------------------------------------------------------------------
// The value for a specific index
// -----------------------------------------------------------------------------
//
CMPXMedia* CMPXMediaArray::Value(TInt aIndex) const
    {
    CMPXMedia* m=NULL;
    TRAP_IGNORE(m=ValueL(aIndex));
    return m;
    }

// -----------------------------------------------------------------------------
// The value for a specific index
// -----------------------------------------------------------------------------
//
CMPXMedia* CMPXMediaArray::ValueL(TInt aIndex) const
    {
    MPX_ASSERT(iData&&iClientHandle&&aIndex>=0&&aIndex<Count());
    //
    CMPXMedia* obj=NULL;
    //
    // Check local values to see if there and current
    //
    LockHeapLC();
    TInt uid=iData->Uid(iClientHandle,aIndex);
    const TValue* v=LocalValue(uid);
    if (!v)
        {
        TPtrC8 data=iData->Value(iClientHandle,aIndex);
        TUint* ptr=(TUint*)data.Ptr();
        //
        // Check that it's really a media object
        //
        MPX_ASSERT(*(MMPXData::TMPXObjectType*)ptr==MMPXData::EMedia);
        MPX_ASSERT(sizeof(TUint)+sizeof(TInt)==data.Size());
        //
        ++ptr; // Actual data offset
        obj=CMPXMedia::NewL(*ptr); // new media object from the data
        CleanupStack::PushL(obj);
        CMPXMediaArray& theArray=*const_cast<CMPXMediaArray*>(this);
        User::LeaveIfError(theArray.SetLocal(TValue(obj,uid,EMPXTypeCObject)));
        CleanupStack::Pop(obj);
        }
    else
        {
        obj=static_cast<CMPXMedia*>(v->iValue);
        }
    CleanupStack::PopAndDestroy(); // unlock the heap
    return obj;
    }

// -----------------------------------------------------------------------------
// Inserts the specified media into the array at the specified position
// -----------------------------------------------------------------------------
//
TInt CMPXMediaArray::AddToArray(const CMPXMedia& aMedia,TInt aPos,TBool aReplace)
    {
    MPX_ASSERT(iData&&iDataHandle&&iClientHandle);
    const TInt KMediaStreamLen=sizeof(MMPXData::TMPXObjectType)+sizeof(TUint);
    TBuf8<KMediaStreamLen> buf;
    TUint* p=(TUint*)buf.Ptr();
    *p=MMPXData::EMedia;
    *++p=aMedia.Data();
    buf.SetLength(KMediaStreamLen);
    TInt r(KErrNone);
    if (aReplace)
        {
        iData->Set(iClientHandle,KMPXMediaArrayItem,EMPXTypeCObject,buf,aPos);
        }
    else if (aPos==CMPXMediaBase::Count())
        {
        r=iData->Append(iClientHandle,KMPXMediaArrayItem,EMPXTypeCObject,buf);
        }
    else
        {
        r=iData->Insert(iClientHandle,KMPXMediaArrayItem,EMPXTypeCObject,buf,aPos);
        }
    return r;
    }

// END OF FILE
