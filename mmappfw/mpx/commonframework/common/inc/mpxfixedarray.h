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
* Description:  Array of fixed size elements on global chunk
*
*/



#ifndef RMPXFIXEDARRAY_H
#define RMPXFIXEDARRAY_H

#include "mpxheapmanager.h"

/**
* Base class for fixed arrays on global heap; not for direct use 
*/
class RMPXFixedArrayBase
    {
public:
    /**
    * Returns pointer to start of Array (buffer holding array items)
    * 
    * @param aClientHandle, client handle to the global chunk
    * @return raw data of the array
    */
    inline TAny* Buf(TUint aClientHandle) const;

    /**
    * Number of elements in array
    * 
    * @return number of elements in array
    */
    inline TInt Count() const;
    
    /**
    * Frees resources; object can go out of scope
    * 
    * @param aClientHandle, client handle to the global chunk 
    */

    inline void Close(TUint aClientHandle);
    
    /**
    * Delete's content; array can be reused
    * 
    * @param aClientHandle, client handle to the global chunk
    */
    void Reset(TUint aClientHandle); // Frees contents

    /**
    * Copies the contents of aArray into this array
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aArray, source array
    * @return KErrNone if succeeded, otherwise system error code
    */
    TInt Copy(TUint aClientHandle,const RMPXFixedArrayBase& aArray);

    /**
    * Allocates buffer for an array of aCount items, each of size
    * aElementSize; returns pointer to start of buffer
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aCount, number of the element in the array
    * @return address of memory allocated
    */
    TAny* Alloc(TUint aClientHandle,TInt aCount);  

protected: 
    RMPXFixedArrayBase();

    /**
    * Constructor.
    * @param aElementSize size of the array element
    */
    RMPXFixedArrayBase(TInt aElementSize);

    /**
    * Constructor: copies content from aArray
    * 
    * @param aArray source array
    */
    RMPXFixedArrayBase(const RMPXFixedArrayBase& aArray);
    
private:
    TInt iCount;
    TInt iDataOffset;
    TInt iElementSize;
    };

/** 
* Array of fixed number of objects of type T
*/
template <class T>
class RMPXFixedArray : public RMPXFixedArrayBase
    {
public:
    /**
     * C++ default constructor.
     */
    inline RMPXFixedArray();

    /**
     * Copy constructor: copies all data from aArray into
     * this object
     */
    inline RMPXFixedArray(const RMPXFixedArray& aArray);
public:
    /**
    * Copies the contents of aArray into this array
    */
    inline TInt Copy(TUint aClientHandle,const TArray<T>& aArray);

    /**
    * Returns array
    */   
    inline TArray<T> Array(TUint aClientHandle) const;
    
private:
    /**
    * Count function for TArray
    */
    inline static TInt CountFunctionR(const CBase* aThis);

    /**
    * At function for TArray
    */
    inline static const TAny* AtFunctionR(const CBase* aThis,TInt aIndex);  	      
    };

#include "mpxfixedarray.inl"

#endif // RMPXFIXEDARRAY_H
