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
* Description:  Common utility functions
*
*/


#ifndef MPXCMN_H
#define MPXCMN_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <bamdesca.h>
#include <badesca.h> 
#include <s32mem.h>
#include <mpxuser.h>
#include <mpxsession.h>
#include <mpxcommonframeworkdefs.h>

/**
* Helper to copy an array.
*
* @since S60 3.2.3
* @param aSrc soure
* @param sDest destination
*/ 
template<typename T> 
inline void CopyArrayL(const TArray<T>& aSrc, RArray<T>& aDest);

/**
* Helper to copy a descriptor array.
*
* @since S60 3.2.3
* @param aSrc soure
* @param sDest destination
*/ 
inline void CopyArrayL(const MDesCArray& aSrc, CDesCArray& aDest);

/**
* Helper to transfer a buffer from server.
*
* @since S60 3.2.3
* @param aSess session object
* @param aCmd Cmd to read buffer from server
* @param aSize size of buffer
* @param aBuffer buffer to returned
*/ 
inline void TransferBufferFromServerL(const RMPXSession& aSess, 
                                      TInt aCmd, 
                                      TInt aSize,
                                      CBufBase*& aBuffer);

/**
* Helper to create an array from stream.
* Note: class T should public default C++ constructor.
* 
* @since S60 3.2.3
* @param aArray array object returned
* @param aStream stream object
*/ 
template<typename T> 
inline void InternalizeL(RPointerArray<T>& aArray, RReadStream& aStream);

/**
* Helper to create an array from stream.
* Note: CBased class T contains NewL().
* 
* @since S60 3.2.3
* @param aArray array object returned
* @param aStream stream object
*/ 
template<typename T> 
inline void InternalizeCObjectArrayL(RPointerArray<T>& aArray, 
                                     RReadStream& aStream);

/**
* Helper to create an array from stream.
*
* @since S60 3.2.3
* @param aArray array object returned
* @param aStream stream object
*/ 
template<typename T> 
inline void InternalizeL(RArray<T>& aArray, RReadStream& aStream);

/**
* Helper to create a descriptor array from stream.
*
* @since S60 3.2.3
* @param aArray descriptor array object returned
* @param aStream stream object
*/ 
inline void InternalizeL(CDesCArray& aArray, RReadStream& aStream);

/**
* Helper to externalize an array to stream.
*
* @since S60 3.2.3
* @param aArray array object 
* @param aStream stream object
*/ 
template<typename T> 
inline void ExternalizeL(const TArray<T*>& aArray, RWriteStream& aStream);

/**
* Helper to externalize an array to stream.
*
* @since S60 3.2.3
* @param aArray array object 
* @param aStream stream object
*/ 
template<typename T> 
inline void ExternalizeL(const TArray<T>& aArray, RWriteStream& aStream);

/**
* Helper to externalize a descriptor array to stream.
*
* @since S60 3.2.3
* @param aArray descriptor array object 
* @param aStream stream object
*/ 
inline void ExternalizeL(const MDesCArray& aArray, RWriteStream& aStream);

/**
* Helper to create an array from buf.
*
* @since S60 3.2.3
* @param aBuf buf object
* @param aArray array object returned
*/ 
template<typename T> 
inline void CreateFromBufferL(const CBufBase &aBuf, CArrayFix<T>*& aArray);

/**
* Helper to create a descriptor array from buf.
*
* @since S60 3.2.3
* @param aBuf buf object
* @param aArray descriptor array object returned
*/ 
inline void CreateFromBufferL(const CBufBase &aBuf, CDesCArray*& aArray);

/**
* Helper to create an array from buf.
*
* @since S60 3.2.3
* @param aBuf buf object
* @param aArray array object returned
*/ 
template<typename T> 
inline void CreateFromBufferL(const CBufBase &aBuf, RPointerArray<T>& aArray);

/**
* Helper to create an array from buf.
*
* @since S60 3.2.3
* @param aBuf buf object
* @param aArray array object returned
*/ 
template<typename T> 
inline void CreateFromBufferL(const CBufBase &aBuf, RArray<T>& aArray);

/**
* Helper to create an object from buf.
* Note: use C++ default constructor to create a new object.
*
* @since S60 3.2.3
* @param aBuf buf object
* @param aObject created
*/ 
template<typename T>
inline void CreateFromBufferL(const CBufBase &aBuf, T*& aObject);

/**
* Helper to create an object from buf.
* Note: class T must implement T::NewL() constructor.
*
* @since S60 3.2.3
* @param aBuf buf object
* @param aObject created
*/ 
template<typename T>
inline void NewFromBufferL(const CBufBase &aBuf, T*& aObject);


/**
* Helper to create an object from a message buffer.
*
* @since S60 3.2.3
* @param aMessage the message
* @param aMsgSlot message slot index
*/
template<typename T>
inline void CreateFromMessageL(const RMessage2& aMessage, 
                               TInt aMsgSlot,
                               T*& aObject);
                              
/**
* Helper to create an object from a message buffer.
* Note: class T must implement T::NewL() constructor.
*
* @since S60 3.2.3
* @param aMessage the message
* @param aMsgSlot message slot index
*/
template<typename T>
inline void NewFromMessageL(const RMessage2& aMessage, 
                            TInt aMsgSlot,
                            T*& aObject);

/**
* Copy an object into the buffer.
*
* @since S60 3.2.3
* @param aObj the object
* @param aBuffer buffer object
*/
template<typename T> 
inline void CreateBufferL(const T& aObj, CBufBase*& aBuffer);

/**
* Fill up the buffer from an array.
*
* @since S60 3.2.3
* @param aArray an array of object
* @param aBuffer buffer object
*/
template<typename T> 
inline void CreateBufferL(const TArray<T>& aArray, CBufBase*& aBuffer);

/**
* Fill up the buffer from an array.
*
* @since S60 3.2.3
* @param aArray an array of object pointer
* @param aBuffer buffer object
*/
template<typename T> 
inline void CreateBufferL(const TArray<T*>& aArray, CBufBase*& aBuffer);

/**
* Helper to transfer an array from server.
*
* @since S60 3.2.3
* @param aSess session object
* @param aCmd Cmd to read buffer from server
* @param aSize size of buffer
* @param aArray array to returned
*/ 
template<typename T> 
void ArrayFromServerL(const RMPXSession& aSess, 
                      TInt aCmd, 
                      TInt aSize,
                      CArrayFixBase*& aArray);

/**
* Helper to transfer an array from server.
*
* @since S60 3.2.3
* @param aSess session object
* @param aCmd Cmd to read buffer from server
* @param aSize size of buffer
* @param aArray array to returned
*/ 
template<typename T> 
void ArrayFromServerL(const RMPXSession& aSess, 
                      TInt aCmd, 
                      TInt aSize,
                      RPointerArray<T>& aArray);

/**
* Helper to transfer an array from server.
*
* @since S60 3.2.3
* @param aSess session object
* @param aCmd Cmd to read buffer from server
* @param aSize size of buffer
* @param aArray array to returned
*/ 
template<typename T> 
void ArrayFromServerL(const RMPXSession& aSess, 
                      TInt aCmd, 
                      TInt aSize,
                      RArray<T>& aArray);

/**
*  Packages an object of type T into a descriptor and writes back to the 
*  client space in message slot aMsgSlot of aMessage.
*
*  @since S60 3.2.3
*  @param aMessage message object
*  @param aMsgSlot message slot
*  @param aValue value
*/
template<typename T> void WriteL(const RMessage2& aMessage,
                                 TInt aMsgSlot, T aValue);
                      
/** 
 * An array of pointers to objects that auto cleanup.
 *
 * @lib mpxcommon.lib
 */
template<class T> 
class RMPXPointerArray : public RPointerArray<T>
    {
 public:
    /** 
    * Constructor.
    *
    * @since S60 3.2.3
    */
    inline RMPXPointerArray();

    /** 
    * Closes the array and frees all 
    * resources (including deleting objects in the array). 
    *
    * @since S60 3.2.3
    */
    inline void Close();

    /** 
    * Destructor.
    *
    * @since S60 3.2.3
    */
    inline ~RMPXPointerArray();
    };

/**
* Helper to delete an item from array.
*
* @since S60 3.2.3
* @param aItem an item in the array to be deleted
* @param aArray array object returned
*/ 
template<typename T> 
inline void DeleteL(const T* aItem, RPointerArray<T>& aArray);

#include "mpxcmn.inl"

#endif  // MPXCMN_H

// End of File
