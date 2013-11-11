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


// ----------------------------------------------------------------------------
// Copies elements from one array of objects of type T  to another
// ----------------------------------------------------------------------------
//
template<typename T>
inline void CopyArrayL(const TArray<T>& aSrc, RArray<T>& aDest)
    {
    aDest.Reset();
    for (TInt i=0; i < aSrc.Count(); ++i)
        {
        aDest.AppendL(aSrc[i]);
        }
    }

// ----------------------------------------------------------------------------
// Copies elements from one array of descriptors to another
// ----------------------------------------------------------------------------
//
inline void CopyArrayL(const MDesCArray& aSrc, CDesCArray& aDest)
    {
    MPXUser::CopyArrayL(aSrc, aDest);
    }

// ----------------------------------------------------------------------------
// Transfer a buffer from server
// ----------------------------------------------------------------------------
//
inline void TransferBufferFromServerL(const RMPXSession& aSess,
                                      TInt aCmd,
                                      TInt aSize,
                                      CBufBase*& aBuffer)
    {
    delete aBuffer;
    aBuffer = NULL;
    if (aSize==0)
        {
        User::Leave(KErrArgument);
        }

    CBufBase* buffer = MPXUser::CreateBufferLC(aSize);
    TPtr8 ptr(buffer->Ptr(0));
    aSess.SendReceiveL(aCmd,TIpcArgs(&ptr));
    CleanupStack::Pop(buffer);
    aBuffer = buffer;
    }

// ----------------------------------------------------------------------------
// Internalize a pointer array from stream
// ----------------------------------------------------------------------------
//
template<typename T>
inline void InternalizeL(RPointerArray<T>& aArray, RReadStream& aStream)
    {
    TInt n=aStream.ReadInt32L();
    for (TInt i=0;i<n;++i)
        {
        T* entry = new (ELeave) T;
        CleanupStack::PushL(entry);
        aStream >> *entry;
        aArray.AppendL(entry);
        CleanupStack::Pop(entry);
        }
    }

// ----------------------------------------------------------------------------
// Internalize a pointer array from stream
// ----------------------------------------------------------------------------
//
template<typename T>
inline void InternalizeCObjectArrayL(
    RPointerArray<T>& aArray,
    RReadStream& aStream)
    {
    TInt n=aStream.ReadInt32L();
    for (TInt i=0;i<n;++i)
        {
        T* entry = T::NewL();
        CleanupStack::PushL(entry);
        aStream >> *entry;
        aArray.AppendL(entry);
        CleanupStack::Pop(entry);
        }
    }

// ----------------------------------------------------------------------------
// Internalize an array from stream
// ----------------------------------------------------------------------------
//
template<typename T>
inline void InternalizeL(RArray<T>& aArray, RReadStream& aStream)
    {
    TInt n=aStream.ReadInt32L();
    for (TInt i=0;i<n;++i)
        {
        TPckgBuf<T> item;
        aStream.ReadL(item);
        aArray.AppendL(item());
        }
    }

// ----------------------------------------------------------------------------
// Internalize a descriptor array from stream
// ----------------------------------------------------------------------------
//
inline void InternalizeL(CDesCArray& aArray, RReadStream& aStream)
    {
    TInt n=aStream.ReadInt32L();
    for (TInt i=0;i<n;++i)
        {
        TInt length=aStream.ReadInt32L();
        HBufC* item = HBufC::NewLC(length);
        TPtr ptr = item->Des();
        aStream.ReadL(ptr, length);
        aArray.AppendL(*item);
        CleanupStack::PopAndDestroy(item);
        }
    }

// ----------------------------------------------------------------------------
// Exernalize a pointer array to stream
// ----------------------------------------------------------------------------
//
template<typename T>
inline void ExternalizeL(const TArray<T*>& aArray, RWriteStream& aStream)
    {
    TInt len = aArray.Count();
    aStream.WriteInt32L(len);
    for(TInt i = 0;i<len;i++)
        {
        T &entry = *aArray[i];
        aStream << entry;
        }
    }

// ----------------------------------------------------------------------------
// Externalize an array to stream
// ----------------------------------------------------------------------------
//
template<typename T>
inline void ExternalizeL(const TArray<T>& aArray, RWriteStream& aStream)
    {
    TInt n=aArray.Count();
    aStream.WriteInt32L(n);
    for (TInt i=0;i<n;++i)
        {
        TPckgBuf<T> item(aArray[i]);
        aStream.WriteL(item);
        }
    }

// ----------------------------------------------------------------------------
// Externalize a descriptor array to stream
// ----------------------------------------------------------------------------
//
inline void ExternalizeL(const MDesCArray& aArray, RWriteStream& aStream)
    {
    TInt n=aArray.MdcaCount();
    aStream.WriteInt32L(n);
    for (TInt i=0;i<n;++i)
        {
        TInt length=aArray.MdcaPoint(i).Length();
        aStream.WriteInt32L(length);
        aStream.WriteL(aArray.MdcaPoint(i));
        }
    }

// ----------------------------------------------------------------------------
// Helper function to create an array from buf.
// ----------------------------------------------------------------------------
//
template<typename T>
inline void CreateFromBufferL(const CBufBase &aBuf, CArrayFix<T>*& aArray)
    {
    delete aArray;
    aArray = NULL;
    RBufReadStream rs(aBuf);
    CleanupClosePushL(rs);
    CArrayFix<T>* array=new(ELeave)CArrayFixFlat<T>(1); //magic number
    CleanupStack::PushL(array);
    rs >> *array;
    CleanupStack::Pop(array);
    aArray = array;
    CleanupStack::PopAndDestroy(&rs);
    }

// ----------------------------------------------------------------------------
// Helper function to create a descriptor array from buf.
// ----------------------------------------------------------------------------
//
inline void CreateFromBufferL(const CBufBase &aBuf, CDesCArray*& aArray)
    {
    delete aArray;
    aArray = NULL;
    RBufReadStream rs(aBuf);
    CleanupClosePushL(rs);
    CDesCArray* array=new(ELeave)CDesCArrayFlat(1); //magic number
    CleanupStack::PushL(array);
    InternalizeL(*array, rs);
    CleanupStack::Pop(array);
    aArray = array;
    CleanupStack::PopAndDestroy(&rs);
    }

// ----------------------------------------------------------------------------
// Helper function to create an array from buf.
// ----------------------------------------------------------------------------
//
template<typename T>
inline void CreateFromBufferL(const CBufBase &aBuf, RPointerArray<T>& aArray)
    {
    aArray.ResetAndDestroy();
    RBufReadStream rs(aBuf);
    CleanupClosePushL(rs);
    InternalizeL(aArray, rs);
    CleanupStack::PopAndDestroy(&rs);
    }

// ----------------------------------------------------------------------------
// Helper function to create an array from buf.
// ----------------------------------------------------------------------------
//
template<typename T>
inline void CreateFromBufferL(const CBufBase &aBuf, RArray<T>& aArray)
    {
    aArray.Reset();
    RBufReadStream rs(aBuf);
    CleanupClosePushL(rs);
    InternalizeL(aArray, rs);
    CleanupStack::PopAndDestroy(&rs);
    }

// ----------------------------------------------------------------------------
// Retrieves an object of from buffer
// ----------------------------------------------------------------------------
//
template<typename T>
inline void CreateFromBufferL(const CBufBase &aBuf, T*& aObject)
    {
    delete aObject;
    aObject = NULL;
    RBufReadStream rs(aBuf);
    CleanupClosePushL(rs);
    aObject=new(ELeave)T;
    CleanupStack::PushL(aObject);
    rs>>*aObject;
    CleanupStack::Pop(aObject);
    CleanupStack::PopAndDestroy(&rs);
    }

// ----------------------------------------------------------------------------
// Retrieves an object of from buffer
// Note: class T must implement T::NewL(RReadStream& aStream) constructor
// ----------------------------------------------------------------------------
//
template<typename T>
inline void NewFromBufferL(const CBufBase &aBuf, T*& aObject)
    {
    delete aObject;
    aObject = NULL;
    RBufReadStream rs(aBuf);
    CleanupClosePushL(rs);
    aObject=T::NewL();
    CleanupStack::PushL(aObject);
    rs>>*aObject;
    CleanupStack::Pop(aObject);
    CleanupStack::PopAndDestroy(&rs);
    }

// ----------------------------------------------------------------------------
// Retrieves an object of from message buffer
// ----------------------------------------------------------------------------
//
template<typename T>
inline void CreateFromMessageL(const RMessage2& aMessage,
                              TInt aMsgSlot,
                              T*& aObject)
    {
    CBufBase* buffer(NULL);
    MPXUser::CreateBufferL(aMessage, aMsgSlot, buffer);
    CleanupStack::PushL(buffer);
    CreateFromBufferL<T>(*buffer, aObject);
    CleanupStack::PopAndDestroy(buffer);
    }

// ----------------------------------------------------------------------------
// Retrieves an object of from message buffer
// ----------------------------------------------------------------------------
//
template<typename T>
inline void NewFromMessageL(const RMessage2& aMessage,
                            TInt aMsgSlot,
                            T*& aObject)
    {
    CBufBase* buffer(NULL);
    MPXUser::CreateBufferL(aMessage, aMsgSlot, buffer);
    CleanupStack::PushL(buffer);
    NewFromBufferL<T>(*buffer, aObject);
    CleanupStack::PopAndDestroy(buffer);
    }

// ----------------------------------------------------------------------------
// Copy an object into the buffer
// ----------------------------------------------------------------------------
//
template<typename T>
inline void CreateBufferL(const T& aObj, CBufBase*& aBuffer)
    {
    delete aBuffer;
    aBuffer = NULL;
    CBufBase* buffer = CBufFlat::NewL(KMPXBufGranularity);
    CleanupStack::PushL(buffer);
    RBufWriteStream ws(*buffer);
    CleanupClosePushL(ws);
    ws<<aObj;
    ws.CommitL();
    CleanupStack::PopAndDestroy(&ws);
    buffer->Compress();
    CleanupStack::Pop(buffer);
    aBuffer = buffer;
    }

// ----------------------------------------------------------------------------
// Create the buffer from an array
// ----------------------------------------------------------------------------
//
template<typename T>
inline void CreateBufferL(const TArray<T>& aArray, CBufBase*& aBuffer)
    {
    delete aBuffer;
    aBuffer = NULL;
    TInt size=aArray.Count()*sizeof(TPckgBuf<T>)+sizeof(TInt);
    CBufBase* buffer = MPXUser::CreateBufferLC(size);
    RBufWriteStream ws(*buffer);
    CleanupClosePushL(ws);
    ExternalizeL(aArray, ws);
    ws.CommitL();
    CleanupStack::PopAndDestroy(&ws);
    buffer->Compress();
    CleanupStack::Pop(buffer);
    aBuffer = buffer;
    }

// ----------------------------------------------------------------------------
// Create the buffer from an array of object
// ----------------------------------------------------------------------------
//
template<typename T>
inline void CreateBufferL(const TArray<T*>& aArray, CBufBase*& aBuffer)
    {
    delete aBuffer;
    aBuffer = NULL;
    CBufBase* buffer = CBufFlat::NewL(KMPXBufGranularity);
    CleanupStack::PushL(buffer);
    RBufWriteStream ws(*buffer);
    CleanupClosePushL(ws);
    ExternalizeL(aArray, ws);
    ws.CommitL();
    CleanupStack::PopAndDestroy(&ws);
    buffer->Compress();
    CleanupStack::Pop(buffer);
    aBuffer = buffer;
    }

// ----------------------------------------------------------------------------
// Copy an array from the server
// ----------------------------------------------------------------------------
//
template<typename T>
void ArrayFromServerL(const RMPXSession& aSess,
                      TInt aCmd,
                      TInt aSize,
                      CArrayFixBase*& aArray)
    {
    delete aArray;
    aArray = NULL;

    CBufBase* buffer(NULL);
    TransferBufferFromServerL(aSess, aCmd, aSize, buffer);
    if (!buffer)
        {
        CleanupStack::PushL(buffer);
        CArrayFix<T>* array = NULL;
        CreateFromBufferL<T>(*buffer, array);
        aArray = array;
        CleanupStack::PopAndDestroy(buffer);
        }
    }

// ----------------------------------------------------------------------------
// Copy an array from the server
// ----------------------------------------------------------------------------
//
template<typename T>
void ArrayFromServerL(const RMPXSession& aSess,
                      TInt aCmd,
                      TInt aSize,
                      RPointerArray<T>& aArray)
    {
    CBufBase* buffer(NULL);
    TransferBufferFromServerL(aSess, aCmd, aSize, buffer);
    CleanupStack::PushL(buffer);
    CreateFromBufferL<T>(*buffer, aArray);
    CleanupStack::PopAndDestroy(buffer);
    }

// ----------------------------------------------------------------------------
// Copy an array from the server
// ----------------------------------------------------------------------------
//
template<typename T>
void ArrayFromServerL(const RMPXSession& aSess,
                      TInt aCmd,
                      TInt aSize,
                      RArray<T>& aArray)
    {
    CBufBase* buffer(NULL);
    TransferBufferFromServerL(aSess, aCmd, aSize, buffer);
    CleanupStack::PushL(buffer);
    CreateFromBufferL<T>(*buffer, aArray);
    CleanupStack::PopAndDestroy(buffer);
    }

// ----------------------------------------------------------------------------
//  Packages an object of type T into a descriptor and writes back to the
//  client space in message slot aMsgSlot of aMessage.
// ----------------------------------------------------------------------------
//
template<typename T> void WriteL(const RMessage2& aMessage,
                                 TInt aMsgSlot,T aValue)
    {
    TPckgC<T> value(aValue);
    aMessage.WriteL(aMsgSlot,value);
    }

// ----------------------------------------------------------------------------
// Default constructor
// ----------------------------------------------------------------------------
//
template <class T>
inline RMPXPointerArray<T>::RMPXPointerArray()
        {
        }

// ----------------------------------------------------------------------------
// Closes the array and frees all resources;
// ----------------------------------------------------------------------------
//
template <class T>
inline void RMPXPointerArray<T>::Close()
        {
        this->ResetAndDestroy();
        RPointerArray<T>::Close();
        }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
template <class T>
inline RMPXPointerArray<T>::~RMPXPointerArray()
    {
    Close();
    }

template<typename T>
inline void DeleteL(const T* aItem, RPointerArray<T>& aArray)
    {
    TInt index = aArray.FindL(aItem);
    aArray.Remove(index);
    delete aItem;
    }
// End of file