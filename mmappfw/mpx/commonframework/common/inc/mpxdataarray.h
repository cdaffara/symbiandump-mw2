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
* Description:  Array of variable size elements on global chunk
*
*/



#ifndef RMPXDATAARRAY_H
#define RMPXDATAARRAY_H

#include <e32base.h>

class RMPXDataItem;

/**
*  Class RMPXDataItemArray. A dynamic array of RMPXDataItems, where each item
*  can have variable length data
*/
class RMPXDataItemArray
    {
public:
    /**
     * C++ default constructor.
     */
    RMPXDataItemArray();
    
    /**
    * Frees resources; object can go out of scope
    * 
    * @param aClientHandle, client handle to the global chunk
    */
    void Close(TUint aClientHandle);
    
public:
    /**
    * Number of items in array
    * 
    * @return number of contained media objects
    */
    inline TInt Count() const;

    /**
    * Data at aIndex
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aIndex, index of data item
    * @return raw data of the item
    */
    inline TPtrC8 Data(TUint aClientHandle,TInt aIndex);

    /**
    * Data item at aIndex
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aIndex, index of data item
    * @return data item object
    */
    RMPXDataItem* DataItem(TUint aClientHandle,TInt aIndex);

    /**
    * Adds an item to the array
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aItem, data item object
    */
    void Append(TUint aClientHandle,RMPXDataItem& aItem);

    /**
    * Replaces an item in the array
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aItem data item object
    * @param aIndex, index of data item
    */
    void Set(TUint aClientHandle,RMPXDataItem& aItem,TInt aIndex);

    /**
    * Inserts an item to the array
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aItem data item object
    * @param aIndex, index of data item
    */
    void Insert(TUint aClientHandle,RMPXDataItem& aItem,TInt aIndex);

    /**
    * Removes an item from the array
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aIndex, index of data item
    */
    void Remove(TUint aClientHandle,TInt aIndex);

    /**
    * Removes all items from the array
    * 
    * @param aClientHandle, client handle to the global chunk
    */
    void Reset(TUint aClientHandle);
    
    /**
     * Find an item from the array
     * 
     * @param aClientHandle, client handle to the global chunk
     * @param aItem, data item object
     */
    TInt Find(TUint aClientHandle,const RMPXDataItem& aItem);
    
protected: 
    /**
    * Deletes an item from the array
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aItem, data item object
    */
    void Delete(TUint aClientHandle,RMPXDataItem* aItem);

protected:
    /**
    * Offset from the base of the chunk to the first item
    */
    TInt iFirstItemOffset;
    /**
    * Offset from the base of the chunk to the last item
    */
    TInt iLastItemOffset;
    /**
    * Number of items stored in this array
    */
    TInt iNumItems;
    /**
    * Current position within the list, offset from the base of the chunk
    */
    TInt iPos; 
    /**
    * Current index within the list (i.e. first item=0, last=iNumItems-1)
    */
    TInt iIndex; 
    };

/**
*  Class RMPXDataItem. Encapsulates an item of data on the heap.
*/
class RMPXDataItem
    {
public:
    /**
    * Default C++ constructor
    */
    inline RMPXDataItem();
    
    /**
    * Frees resources; object can go out of scope
    * 
    * @param aClientHandle, client handle to the global chunk
    */
    inline void Close(TUint aClientHandle);
    
    /**
    * Frees resources; object can be reused
    * 
    * @param aClientHandle, client handle to the global chunk
    */
    void Reset(TUint aClientHandle);
    
public:
    /**
    * Sets the data for this object
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aSrc, source data item
    * @param aSize, size of data item 
    */
    void Copy(TUint aClientHandle,TAny* aSrc,TInt aSize);
    
    /**
    * Sets the data for this object
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aData, source data item
    */
    inline void Copy(TUint aClientHandle,const TDesC8& aData);
    
    /**
    * Pointer to the data for this object
    * 
    * @param aClientHandle, client handle to the global chunk
    * @return data item buffer
    */
    TAny* Buf(TUint aClientHandle);
    
    /**
    * Value of the data for this object as a descriptor
    * 
    * @param aClientHandle, client handle to the global chunk
    * @return data item buffer
    */
    inline TPtrC8 Data(TUint aClientHandle);
    
    /**
    * Size of the data for this object
    * 
    * @return the size of data item
    */
    inline TInt Size() const;
    
    /**
    * UID of this object
    * 
    * @return uid of the data item
    */
    inline TInt Uid() const;
    
    /**
    * Next item in list, or 0 if none
    * 
    * @return next item offset
    */
    inline TInt NextOffset() const;
    
    /**
    * Previous item in list, or 0 if none
    * 
    * @return previous item offset
    */
    inline TInt PreviousOffset() const;

public:
    /**
    * Inserts this object in a doubly linked list. Either (but not
    * both) can be NULL in which case this item is appended to the
    * start or end of the list
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aPrevious, pointer to previous data item
    * @param aNext, pointer to next data item
    */
    void Insert(TUint aClientHandle,RMPXDataItem* aPrevious,RMPXDataItem* aNext);
    
    /**
    * Inserts this object in a doubly linked list. Either (but not
    * both) can be 0 in which case this item is appended to the
    * start or end of the list
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aPreviousOffset, previous data item offset
    * @param aNextOffset, next data item offset
    */
    void Insert(TUint aClientHandle,TInt aPreviousOffset,TInt aNextOffset);
    
    /**
    * Sets this object in a doubly linked list.
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aOldItem, pointer to old data item
    */
    void Set(TUint aClientHandle,RMPXDataItem* aOldItem);
    
    /**
    * Sets this object in a doubly linked list.
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aOldItemOffset, old data item offset
    */
    void Set(TUint aClientHandle,TInt aOldItemOffset);
  
    /**
    * Removes this object from a doubly linked list
    * 
    * @param aClientHandle, client handle to the global chunk
    */
    void Remove(TUint aClientHandle);
  
protected:
    /**
    * Offset from the base of the chunk to the previous data
    * item, or 0 if none
    */  
    TInt iPreviousOffset;
    /**
    * Offset from the base of the chunk to the next data
    * item, or 0 if none
    */
    TInt iNextOffset;
    /**
    * Offset within this object
    */
    TInt iBufOffset;
    /**
    * Length of the data buffer
    */    
    TInt iBufLen; 
    /**
    * Unique ID of data
    */
    TInt iUid;
    };

/**
* Class RMPXDataObject. Data item which holds an object of type T
*/
template <class T>
class RMPXDataObject : public RMPXDataItem
    {
public:
    /**
     * Get the data object
     * 
     * @param aClientHandle, client handle to the global chunk
     * @return the data object
     */
    inline T* Object(TUint aClientHandle);

    /**
     * Copy data object
     *  
     * @param aClientHandle, client handle to the global chunk
     * @param aSrc, source data object
     */
    inline void CopyObject(TUint aClientHandle,const T& aSrc);
private:
    T iData;
    };


#include "mpxdataarray.inl"

#endif // RMPXDATAARRAY_H
