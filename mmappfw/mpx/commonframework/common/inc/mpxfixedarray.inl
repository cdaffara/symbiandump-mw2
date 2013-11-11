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
* Description:  Implementation of array of fixed size elements on global chunk
*
*/


// -----------------------------------------------------------------------------
// RMPXFixedArrayBase::Close
// -----------------------------------------------------------------------------
//
inline void RMPXFixedArrayBase::Close(TUint aClientHandle)
    {
    Reset(aClientHandle);
    }

// -----------------------------------------------------------------------------
// RMPXFixedArrayBase::Buf
// -----------------------------------------------------------------------------
//
inline TAny* RMPXFixedArrayBase::Buf(TUint aClientHandle) const
    {
    return RMPXHeapManager::Ptr<TAny>(aClientHandle,iDataOffset);
    }

// -----------------------------------------------------------------------------
// RMPXFixedArrayBase::Count
// -----------------------------------------------------------------------------
//
inline TInt RMPXFixedArrayBase::Count() const
    {
    return iCount;
    }

// -----------------------------------------------------------------------------
// RMPXFixedArray::RMPXFixedArray
// -----------------------------------------------------------------------------
//
template <class T>
inline RMPXFixedArray<T>::RMPXFixedArray(const RMPXFixedArray<T>& aArray)
    : RMPXFixedArrayBase(aArray,sizeof(T))
    {      
    }

// -----------------------------------------------------------------------------
// RMPXFixedArray::RMPXFixedArray
// -----------------------------------------------------------------------------
//
template <class T>
inline RMPXFixedArray<T>::RMPXFixedArray()
    : RMPXFixedArrayBase(sizeof(T))
    {    
    }

// -----------------------------------------------------------------------------
// RMPXFixedArray::Copy
// -----------------------------------------------------------------------------
//
template <class T>
inline TInt RMPXFixedArray<T>::Copy(TUint aClientHandle,const TArray<T>& aArray)
    {   
    TInt n=aArray.Count();
    T* ptr=(T*)RMPXFixedArrayBase::Alloc(aClientHandle,n);
    if (ptr)
        {        
        for (TInt i=0;i<n;++i)
            {
            *ptr++=aArray[i];
            }
        }
    MPX_ASSERT(ptr);
    return ptr?KErrNone:KErrNoMemory; 
    }

// -----------------------------------------------------------------------------
// RMPXFixedArray::Array
// -----------------------------------------------------------------------------
//
template <class T>
inline TArray<T> RMPXFixedArray<T>::Array(TUint aClientHandle) const
    {
    TInt thisOffset=RMPXHeapManager::Offset(aClientHandle,(TAny*)this);
    return TArray<T>(CountFunctionR,AtFunctionR,reinterpret_cast<const CBase*>(thisOffset));
    }

// -----------------------------------------------------------------------------
// RMPXFixedArray::CountFunctionR
// -----------------------------------------------------------------------------
//
template <class T>
inline TInt RMPXFixedArray<T>::CountFunctionR(const CBase* aThis)
    {
    // Non-optimal, obtaining client handle every time count is accessed, however
    // there's no choice since the data could be shared by multiple threads and the
    // function signature does not allow the handle to be passed through
    //
    TInt offset=reinterpret_cast<TInt>(aThis);
    TInt ch=RMPXHeapManager::ClientHandle();    
    const RMPXFixedArrayBase* arr=RMPXHeapManager::Ptr<RMPXFixedArrayBase>(ch,offset);
    return arr->Count();
    }

// -----------------------------------------------------------------------------
// RMPXFixedArray::AtFunctionR
// -----------------------------------------------------------------------------
//
template <class T>
inline const TAny* RMPXFixedArray<T>::AtFunctionR(const CBase* aThis,TInt aIndex)
    { 
    // Non-optimal, obtaining client handle every time data is accessed, however
    // there's no choice since the data could be shared by multiple threads and the
    // function signature does not allow the handle to be passed through
    //
    TInt offset=reinterpret_cast<TInt>(aThis);
    TInt ch=RMPXHeapManager::ClientHandle(); 
    const RMPXFixedArrayBase* arr=RMPXHeapManager::Ptr<RMPXFixedArrayBase>(ch,offset);   
    T* ptr=(T*)arr->Buf(ch);
    return &ptr[aIndex];
    }

// End of file

