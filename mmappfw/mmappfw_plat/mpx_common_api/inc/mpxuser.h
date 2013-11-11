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
* Description:  mpx user helper classes
*
*/


#ifndef MPXUSER_H
#define MPXUSER_H

#include <e32base.h>
#include <e32cmn.h>
#include <s32strm.h>
#include <badesca.h>
#include <w32std.h>
#include <mpxitemid.h>

// MACRO
#if !defined(MPX_ASSERT)
_LIT(KMPX, "MPX");
_LIT(_KMPXAssertErrorInfo, "**MPX ASSERT**: error %d file %s line %d");
#define MPX_ASSERT_S(a) _S(a)
#define MPX_ASSERT_ALWAYS(c) {\
    if (!(c)) {RDebug::Print(_KMPXAssertErrorInfo, 0, MPX_ASSERT_S(__FILE__), __LINE__);\
    User::Invariant();}\
    }
#define MPX_ASSERT_EX_ALWAYS(c,e) {\
    if (!(c)) {RDebug::Print(_KMPXAssertErrorInfo, e, MPX_ASSERT_S(__FILE__), __LINE__);\
    User::Panic(KMPX, (e));}\
    }
#ifdef _DEBUG
#define MPX_ASSERT(c) MPX_ASSERT_ALWAYS(c)
#define MPX_ASSERT_EX(c,e) MPX_ASSERT_EX_ALWAYS(c,e)
#else
#define MPX_ASSERT(c)
#define MPX_ASSERT_EX(c,e)
#endif // _DEBUG
#define MPX_ASSERT_WORD_ALIGNED(x) MPX_ASSERT((((TUint)x)&3)==0)
#endif // MPX_ASSERT

// CONSTANTS
const TInt KMPXBufGranularity = 0x100; // 256 bytes

// Generic type for voice/video/VOIP calls
const TInt KMPXCallTypeGenericVoice( -1 );

class CMPXMedia;
class TMPXAttribute;


/**
* Set of static functions for MPX clients.
*
* @lib MPXCommon.lib
*/
NONSHARABLE_CLASS(MPXUser)
    {
public: // static helper functions

    /**
    * Helper to recreate a buffer.
    *
    * @since S60 3.2.3
    * @param aSize the size of buffer
    * @param aBuf buf object
    */
    IMPORT_C static void RecreateBufferL(TInt aSize, CBufBase*& aBuf);

    /**
    * Helper to create an descriptor array from buf.
    *
    * @since S60 3.2.3
    * @param aBuf buf object
    * @param aArray object
    */
    IMPORT_C  static void CreateFromBufferL(const CBufBase &aBuf,
                                            CDesCArray*& aArray);
    /**
    * Helper to create a buffer.
    *
    * @since S60 3.2.3
    * @param aSize the size of buffer
    * @return aBuf buf object
    */
    IMPORT_C static CBufBase* CreateBufferLC(TInt aSize);

    /**
    * Helper to create a buffer from descriptor array.
    *
    * @since S60 3.2.3
    * @param aArray array object
    * @param aBuf buf object
    * @return the size of buffer
    */
    IMPORT_C  static void CreateBufferL(const MDesCArray* aArray,
                                        CBufBase*& aBuf);

    /**
    * Fill up the buffer from a message.
    *
    * @since S60 3.2.3
    * @param aMessage the message
    * @param aMsgSlot message slot index
    * @param aBuffer buffer object
    * @leave KErrArgument Invalid Message
    */
    IMPORT_C static void CreateBufferL(const RMessage2& aMessage,
                                       TInt aMsgSlot,
                                       CBufBase*& aBuffer);

    /**
    * Fill up the buffer from a desriptormessage.
    *
    * @since S60 3.2.3
    * @param aDes descriptor
    * @param aBuffer buffer object
    */
    IMPORT_C static void CreateBufferL(const TDesC& aDes,
                                       CBufBase*& aBuffer);

    /**
    * Creates a narrow heap descriptor from a unicode descriptor.
    * No character conversion.
    *
    * @since S60 3.2.3
    * @param aDes source data
    * @return new heap buffer object
    */
    IMPORT_C static HBufC8* Alloc8L(const TDesC& aDes);

    /**
    * Creates a zero-terminated narrow heap descriptor from a unicode descriptor.
    * No character conversion.
    *
    * @since S60 3.2.3
    * @param aDes source data
    * @return new heap buffer object
    */
    IMPORT_C static HBufC8* Alloc8ZL(const TDesC& aDes);

    /**
    * Creates a heap descriptor from a narrow descriptor.
    * No character conversion.
    *
    * @since S60 3.2.3
    * @param aDes source data
    * @return new heap buffer object
    */
    IMPORT_C static HBufC* AllocL(const TDesC8& aDes);

    /**
    * Creates a narrow ptr descriptor on a unicode descriptor.
    *
    * @since S60 3.2.3
    * @param aDes source data
    * @return ptr descriptor
    */
    IMPORT_C static TPtrC8 Ptr(const TDesC& aDes);

    /**
    * Creates a unicode ptr descriptor on a narrow descriptor.
    *
    * @since S60 3.2.3
    * @param aDes source data
    * @return ptr descriptor
    */
    IMPORT_C static TPtrC Ptr(const TDesC8& aDes);

    /**
    * Internalize a descriptor from a stream.
    *
    * @since S60 3.2.3
    * @param aArray descriptor array
    * @param aStream stream object
    */
    IMPORT_C static void InternalizeL(CDesCArray*& aArray,
                                      RReadStream& aStream);

    /**
    * Externalize a descriptor to a stream.
    *
    * @since S60 3.2.3
    * @param aArray descriptor array
    * @param aStream stream object
    */
    IMPORT_C static void ExternalizeL(const MDesCArray* aArray,
                                      RWriteStream& aStream);

    /**
    * Group attributes belonging to one content into an item in the array.
    *
    * @since S60 3.2.3
    * @param aSrc attribute array source
    * @param aDest array with attributes merged
    */
    IMPORT_C static void MergeAttributeL(const TArray<TMPXAttribute>& aSrc,
                                         RArray<TMPXAttribute>& aDest);

    /**
    *  Return Process Id.
    *
    *  @since S60 3.2.3
    *  @param aId thread id of the client
    *  @return  id of the process which owns the thread
    */
    IMPORT_C static TProcessId ProcessIdL(TThreadId aId);

    /**
    *  Add array items in aSrc into sorted array aDest if the item is not in
    *  the aDest yet. 
    *
    *  @since S60 3.2.3
    *  @param aSrc souce array
    *  @param aDest destination array
    */
    IMPORT_C static void MergeArray(const MDesCArray& aSrc, CDesCArray& aDest);

    /**
     *  Copies elements from one array of descriptors to another.
     *
     *  @since S60 3.2.3
     *  @param aSrc souce array
     *  @param aDest destination array
     */
    IMPORT_C static void CopyArrayL(const MDesCArray& aSrc, CDesCArray& aDest);

    /**
     *  Compare two uids.
     *
     *  @since S60 3.2.3
     *  @param aId1 the first uid
     *  @param aId2 the second uid
     *  @return zero if the two uids are equal.
     *        a negative value(-1), if the first uid is less than the second.
     *        a positive value(1),  if the first uid is greater than the second.
     */
    IMPORT_C static TInt CompareUids(const TUid& aId1, const TUid& aId2);

    /**
     *  Compare two sorted arrays of uids.
     *
     *  @since S60 3.2.3
     *  @param aArray1 the first array of uids
     *  @param aArray2 the second array of uids
     *  @return zero if the content of two arrays are equal.
     *        a positive value(1),  if the ids count in the first array is
     *        more than the second's and the first array contains all of uids
     *        in the second array.
     *        otherwise, -1 returned.
     */
    IMPORT_C static TInt CompareOrderedUidArrays(const TArray<TUid>& aArray1,
                                                 const TArray<TUid>& aArray2);

    /**
    * Helper to create an array from stream.
    *
    * @since S60 3.2.3
    * @param aArray array object returned
    * @param aStream stream object
    */
    IMPORT_C static void InternalizeL(RArray<TMPXItemId>& aArray, RReadStream& aStream);

    /**
    * Helper to externalize an array to stream.
    *
    * @since S60 3.2.3
    * @param aArray array object
    * @param aStream stream object
    */
    IMPORT_C static void ExternalizeL(const TArray<TMPXItemId>& aArray, RWriteStream& aStream);

    /**
    * Helper to get the window group ID associated with a process.
    *
    * @since S60 3.2.3
    * @param aProcId process ID
    * @return The Window Group ID associated with this process.  Or KErrNotFound if there is none.
    */
    IMPORT_C static TInt FindWindowGroupIdL( TProcessId aProcId, RWsSession& aWsSession );

    /**
    * Check if there is the given type call ongoing.
    *
    * @since S60 3.2.3
    * @param aCallType Call type defined in TPSTelephonyCallState
    * @return ETrue if there is ongoing calls.  EFalse otherwise.
    */
    IMPORT_C static TBool IsCallOngoing( TInt aCallType );

    /**
     * Complete file name with file path of dll.
     *
     * @since S60 3.2.3
     * @param aFileName file name will be completed with dll's path
     * @return error code if an error occured. In case of an error aFileName will
     *         not be changed
     */
    IMPORT_C static TInt CompleteWithDllPath(const TDesC& aDllName, TDes& aFileName);

    /**
     * Helper method to complete file name with file path of dll.
     *
     * @since S60 3.2.3
     * @param aFileName file name will be completed with dll's path
     * @return error code if an error occured. In case of an error aFileName will
     *         not be changed
     */
    inline static TInt CompleteWithDllPath(TDes& aFileName)
        {
        TFileName dllName;
        Dll::FileName(dllName);
        return CompleteWithDllPath(dllName, aFileName);
        }
    };
#endif   // MPXUSER_H

// End of File
