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
* Description:  Manages data and access to the global chunk
*
*/



#ifndef MMPXDATA_H
#define MMPXDATA_H

// INCLUDES
#include <mpxattribute.h>

/**
 *  Base class for managing data in global heap.
 *
 *  @lib mpxcommon.lib
 */
class MMPXData
    {
public:
    /**
    * Defines 'markers' that identify objects from their streamed buffers, e.g.
    * if the first 4 bytes of an externalized object constitute the value EMedia,
    * then we assume the it is an externalized media object.
    */
    enum TMPXObjectType
        {
        EMedia=0x12343219,
        EMediaArray=0x987678ab,
        EPath=0x567982fe // Note: This can be removed when NO clients call
                         // CMPXMedia::Value API for CMPXCollectionPath.
        };
public:
    /**
    * Create a new implementation of the MMPXData interface. This is thread-
    * relative.
    *
    * @since S60 3.2.3
    * @param aClientHandle client handle to the global chunk
    * @return pointer to the implementation object of MMXData
    */
    static MMPXData* NewL(TUint aClientHandle);

    /**
    * Create a new implementation from an existing MMPXData implementation.
    * This is an duplicate copy of the original.
    * This is thread-relative.
    *
    * @since S60 3.2.3
    * @param aClientHandle client handle to the global chunk
    * @param aData existing MMPXData implementation.
    * @return pointer to the implementation object of MMXData
    */
    static MMPXData* NewL(TUint aClientHandle,const MMPXData& aData);

    /**
    * Obtain the interface of an existing implementation from the handle.
    * This is thread-relative.
    *
    * @since S60 3.2.3
    * @param aClientHandle client handle to the global chunk
    * @param aDataHandle data handle to the MMPXData implementation.
    * @return pointer to the implementation object of MMXData
    */
    static MMPXData* Data(TUint aClientHandle,TUint aDataHandle);

    /**
    * Obtain the interface from a buffer, or NULL if the buffer does
    * not contain an implementation of the interface.
    *
    * @since S60 3.2.3
    * @param aClientHandle client handle to the global chunk
    * @param aData buffer which contains the object of MMPXData implementation
    * @return pointer to the implementation object of MMXData
    */
    static MMPXData* Data(TUint aClientHandle,const TDesC8& aData);

    /**
    * Obtains the interface from a buffer, or NULL if the buffer does
    * not contain an implementation of the interface.
    *
    * @since S60 3.2.3
    * @param aClientHandle client handle to the global chunk
    * @param aData buffer which contains the object of MMPXData implementation
    * @param aSize size of the data buffer
    * @return pointer to the implementation object of MMXData
    */
    static MMPXData* Data(TUint aClientHandle,TAny* aData,TInt aSize);

    /**
    * Handle which represents the current client (thread).
    * This should be stored by the client and provided in all methods;
    * zero indicates that it has failed.
    *
    * @since S60 3.2.3
    * @return Client handle of global chunk
    */
    static TUint ClientHandle();

    /**
    * To increment the reference count for this client (thread);
    * should be called for every media/media array object that
    * is not contained within another.
    *
    * @since S60 3.2.3
    * @param aClientHandle client handle to the global chunk
    */
    static void AddClientRef(TUint aClientHandle);

    /**
    * To decrement the reference count on this this client (thread).
    *
    * @since S60 3.2.3
    * @param aClientHandle client handle to the global chunk
    */
    static void ReleaseClient(TUint aClientHandle);

    /**
    * Gets the total size and the currently used size of the global heap.
    *
    * @since S60 3.2.3
    * @param aTotal Total size of memory (out parameter)
    * @param aUsed Used size of memory (out parameter)
    */
    static void HeapMemoryInfoL( TInt& aTotal, TInt& aUsed );


    /**
     * Lock the heap.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     */
    static void LockHeap(TUint aClientHandle);

    /**
     * Unlock the heap.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     */
    static void UnlockHeap(TUint aClientHandle);

public:
    /**
    * Handle to the object implementing this interface; can
    * be used to reference the object.
    *
    * @since S60 3.2.3
    * @param aClientHandle client handle to the global chunk
    * @return data handle
    */
    virtual TUint DataHandle(TUint aClientHandle)=0;

    /**
    * To increment the reference count on this data.
    *
    * @since S60 3.2.3
    * @param aClientHandle client handle to the global chunk
    */
    virtual void AddRef(TUint aClientHandle)=0;

    /**
    * To decrement the reference count on this data; when zero,
    * the data is deleted.
    *
    * @since S60 3.2.3
    * @param aClientHandle client handle to the global chunk
    */
    virtual void Release(TUint aClientHandle)=0;

    /**
    * Gets the data value by index.
    *
    * @since S60 3.2.3
    * @param aClientHandle client handle to the global chunk
    * @param aIndex index of the data item
    * @return raw data
    */
    virtual TPtrC8 Value(TUint aClientHandle,TInt aIndex)=0;

    /**
     * Gets attribute by index.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @param aIndex index of the data item
     * @return attribute of the data
     */
    virtual const TMPXAttribute& Attribute(TUint aClientHandle,TInt aIndex)=0;

    /**
     * Gets object type by index.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @param aIndex index of the data item
     * @return object type of the data
     */
    virtual TMPXAttributeType AttributeType(TUint aClientHandle,TInt aIndex)=0;

    /**
     * Gets supported content ids.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @return array of supported content ids
     */
    virtual TArray<TInt> SupportedIds(TUint aClientHandle)=0;

    /**
     * Sets supported content ids.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @param aSupportedIds array of supported content ids
     * @return KErrNone if succeeded. Otheriwse system error code
     */
    virtual TInt SetSupportedIds(TUint aClientHandle,
                                 const TArray<TInt>& aSupportedIds)=0;

    /**
     * Delete a data item.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @param aIndex index of the data item
     */
    virtual void Delete(TUint aClientHandle,TInt aIndex)=0;

    /**
     * Set a data item.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @param aAttr media attribute
     * @param aType object type
     * @param aData raw data
     * @param aIndex index of the data item
     */
    virtual void Set(TUint aClientHandle,
                     const TMPXAttribute& aAttr,
                     TMPXAttributeType aType,
                     const TDesC8& aData,
                     TInt aIndex)=0;

    /**
     * Insert a data item.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @param aAttr media attribute
     * @param aType object type
     * @param aData raw data
     * @param aIndex index of the data item
     * @return KErrNone if succeeded. Otheriwse system error code
     */
    virtual TInt Insert(TUint aClientHandle,
                        const TMPXAttribute& aAttr,
                        TMPXAttributeType aType,
                        const TDesC8& aData,
                        TInt aIndex)=0;

    /**
     * Append a data item.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @param aAttr media attribute
     * @param aType object type
     * @param aData raw data
     * @return KErrNone if succeeded. Otheriwse system error code
     */
    virtual TInt Append(TUint aClientHandle,
                        const TMPXAttribute& aAttr,
                        TMPXAttributeType aType,
                        const TDesC8& aData)=0;

    /**
     * Remove all data items.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     */
    virtual void Reset(TUint aClientHandle)=0;

    /**
     * Returns number of data items.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @return number of data items.
     */
    virtual TInt Count(TUint aClientHandle)=0;

    /**
     * Returns index of the data item with the media attribute.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @param aAttr media attribute
     * @return index of the data items with the attribute
     */
    virtual TInt Index(TUint aClientHandle,const TMPXAttribute& aAttr)=0;

    /**
     * Returns index of the data item with aUid.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @param aUid uid of the data item
     * @return index of the data items with the attribute
     */
    virtual TInt Index(TUint aClientHandle,TInt aUid)=0;

    /**
     * Returns uid by index.
     *
     * @since S60 3.2.3
     * @param aClientHandle client handle to the global chunk
     * @param aIndex index of the data item
     * @return uid of the data item
     */
    virtual TInt Uid(TUint aClientHandle,TInt aIndex)=0;
    };

#endif // MMPXDATA_H
