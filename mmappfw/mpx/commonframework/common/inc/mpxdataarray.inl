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
* Description:  Implementation of array of variable size elements on global chunk
*
*/



// -----------------------------------------------------------------------------
// RMPXDataItemArray::Count
// -----------------------------------------------------------------------------
//
inline TInt RMPXDataItemArray::Count() const
    { 
    return iNumItems;
    }

// -----------------------------------------------------------------------------
// RMPXDataItemArray::Data
// -----------------------------------------------------------------------------
//
inline TPtrC8 RMPXDataItemArray::Data(TUint aClientHandle,TInt aIndex)
    {
    ASSERT(aClientHandle&&aIndex>=0&&aIndex<iNumItems);
    return DataItem(aClientHandle,aIndex)->Data(aClientHandle);
    }

// -----------------------------------------------------------------------------
// RMPXDataItem::RMPXDataItem
// -----------------------------------------------------------------------------
//
inline RMPXDataItem::RMPXDataItem()
    : iPreviousOffset(0),iNextOffset(0),iBufLen(0),iUid(0)
    {
    iBufOffset = sizeof (RMPXDataItem);
    }

// -----------------------------------------------------------------------------
// RMPXDataItem::Close
// -----------------------------------------------------------------------------
//
inline void RMPXDataItem::Close(TUint aClientHandle)
    {
    ASSERT(aClientHandle);
    Reset(aClientHandle);
    }
    
// -----------------------------------------------------------------------------
// RMPXDataItem::Copy
// -----------------------------------------------------------------------------
//
inline void RMPXDataItem::Copy(TUint aClientHandle,const TDesC8& aData)
    {
    ASSERT(aClientHandle);
    return Copy(aClientHandle,(TAny*)aData.Ptr(),aData.Size());
    }

// -----------------------------------------------------------------------------
// RMPXDataItem::Des
// -----------------------------------------------------------------------------
//
inline TPtrC8 RMPXDataItem::Data(TUint aClientHandle)
    {
    ASSERT(aClientHandle);
    return iBufLen?TPtrC8((TUint8*)Buf(aClientHandle),Size()):TPtrC8();
    }
    
// -----------------------------------------------------------------------------
// RMPXDataItem::SetValue
// -----------------------------------------------------------------------------
//
inline TInt RMPXDataItem::Size() const
    {
    return iBufLen;
    }

// -----------------------------------------------------------------------------
// RMPXDataItem::Uid
// -----------------------------------------------------------------------------
//
inline TInt RMPXDataItem::Uid() const
    {
    return iUid;
    }
    
// -----------------------------------------------------------------------------
// RMPXDataItem::NextOffset
// -----------------------------------------------------------------------------
//
inline TInt RMPXDataItem::NextOffset() const
    {
    return iNextOffset;
    }

// -----------------------------------------------------------------------------
// RMPXDataItem::PreviousOffset
// -----------------------------------------------------------------------------
//
inline TInt RMPXDataItem::PreviousOffset() const
    {
    return iPreviousOffset;
    }    

// -----------------------------------------------------------------------------
// RMPXDataObject::Data
// -----------------------------------------------------------------------------
// 
template <class T>
inline T* RMPXDataObject<T>::Object(TUint aClientHandle)
    {
    return (T*)RMPXDataItem::Buf(aClientHandle);
    }

// -----------------------------------------------------------------------------
// RMPXDataObject::Copy
// -----------------------------------------------------------------------------
// 

template <class T>
inline void RMPXDataObject<T>::CopyObject(TUint aClientHandle,const T& aSrc)
    {
    return RMPXDataItem::Copy(aClientHandle,(TAny*)&aSrc,sizeof(aSrc));
    }

// End of file

