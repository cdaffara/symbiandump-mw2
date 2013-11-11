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
* Description:  Array of variable size elements on global chunk
*
*/



#ifndef RMPXMEDIADATAARRAY_H
#define RMPXMEDIADATAARRAY_H

#include "mpxdataarray.h"
#include <mpxattribute.h>

class RMPXMediaDataItem;


/**
*  Class RMPXMediaDataItemArray. A dynamic array of RMPXMediaDataItems, where 
*  each item can have variable length data
*/
class RMPXMediaDataItemArray : public RMPXDataItemArray
    {
public:   
        
    /**
    * Sets the data for this object
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aArray, source array
    * @return KErrNone if succeeded, otherwise system error code
    */
    TInt Copy(TUint aClientHandle,const RMPXMediaDataItemArray& aArray);   
    
    /**
    * Data item at aIndex
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aIndex, index to the data item
    * @return data item object
    */
    inline RMPXMediaDataItem* MediaDataItem(TUint aClientHandle,TInt aIndex);
    
    /**
    * Adds an item to the array
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aAttr, media attribute
    * @param aAttrType, object type
    * @param aData, raw data
    * @return KErrNone if succeeded. Otheriwse system error code
    */
    TInt Append(TUint aClientHandle,
                const TMPXAttribute& aAttr,
                TMPXAttributeType aAttrType,
                const TDesC8& aData);
    
    /**
    * Sets an item in the array
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aAttr, media attribute
    * @param aAttrType, object type
    * @param aData, raw data
    * @param aIndex, index of data item to be set.
    * @return KErrNone if succeeded. Otheriwse system error code
    */
    TInt Set(TUint aClientHandle,
            const TMPXAttribute& aAttr,
            TMPXAttributeType aAttrType,
            const TDesC8& aData,
            TInt aIndex);
    
    /**
    * Inserts an item to the array
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aAttr, media attribute
    * @param aAttrType, object type
    * @param aData, raw data
    * @param aIndex, index of data item.
    * @return KErrNone if succeeded. Otheriwse system error code
    */
    TInt Insert(TUint aClientHandle,
                const TMPXAttribute& aAttr,
                TMPXAttributeType aAttrType,
                const TDesC8& aData,
                TInt aIndex); 
    
    /**
    * Index of an attribute
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aAttr, media attribute
    * @return index of the media attribute
    */
    TInt Index(TUint aClientHandle,const TMPXAttribute& aAttr);
    
private:
    
    /**
    * Create a new item, or NULL if OOM
    * 
    * @param aClientHandle, client handle to the global chunk
    * @param aAttr, media attribute
    * @param aAttrType, object type
    * @param aData, raw data
    * @return pointer to new data item
    */
    RMPXMediaDataItem* NewItem(TUint aClientHandle,
                               const TMPXAttribute& aAttr,
                               TMPXAttributeType aAttrType,
                               const TDesC8& aData);
    };

/**
*  Class RMPXMediaDataItem. Encapsulates a media item of data on the heap.
*  E.g. a single attribute and value of a media object
*/
class RMPXMediaDataItem : public RMPXDataItem
    {
public:
    /**
    * Default C++ constructor
    */
    inline RMPXMediaDataItem();

    /**
    * Sets the attribute and its type
    * 
    * @param aAttr, media attribute
    * @param aAttrType, object type
    */
    inline void SetAttribute(const TMPXAttribute& aAttr,TMPXAttributeType aType);
    
    /**
    * The attribute type
    * 
    * @return object type of the item
    */
    inline TMPXAttributeType Type() const;
    
    /**
    * The attribute
    * 
    * @return media attribute of the item
    */
    inline const TMPXAttribute& Attribute() const;
    
private:
    /**
    * Attribute stored by this object
    */
    TMPXAttribute iAttr;
    /**
    * Data type of attribute. Can be EMPXTypeUnknown,EMPXTypeTInt,
    * EMPXTypeText,EMPXTypeTObject,EMPXTypeCObject
    */
    TMPXAttributeType iAttrType; 
    };

#include "mpxmediadataarray.inl"

#endif // RMPXMEDIADATAARRAY_H
