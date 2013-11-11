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

#include <s32mem.h>
#include <f32file.h>
#include <e32property.h>
#include <ctsydomainpskeys.h>
#include <mpxmediageneraldefs.h>
#include <mpxmedia.h>
#include <mpxattribute.h>
#include <mpxlog.h>
#include "mpxuser.h"

// ----------------------------------------------------------------------------
// Recreate a buffer
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::RecreateBufferL(TInt aSize, CBufBase*& aBuf)
    {
    delete aBuf;
    aBuf=NULL;
    CBufBase* buf=CBufFlat::NewL(aSize);
    CleanupStack::PushL(buf);
    buf->ResizeL(aSize);
    CleanupStack::Pop(buf);
    aBuf = buf;
    }


// ----------------------------------------------------------------------------
// Helper funciton to create descriptor array from buffer
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::CreateFromBufferL(
    const CBufBase &aBuf,
    CDesCArray*& aArray)
    {
    delete aArray;
    aArray = NULL;
    RBufReadStream rs(aBuf);
    CleanupClosePushL(rs);
    InternalizeL(aArray, rs);
    CleanupStack::PopAndDestroy(&rs);
    }

// ----------------------------------------------------------------------------
// Helper funciton to create a buffer
// ----------------------------------------------------------------------------
//
EXPORT_C CBufBase* MPXUser::CreateBufferLC(TInt aSize)
    {
    CBufBase* buf=CBufFlat::NewL(aSize);
    CleanupStack::PushL(buf);
    buf->ResizeL(aSize);
    return buf;
    }

// ----------------------------------------------------------------------------
// Create a buffer from an array
// aBuf replaced with the contents of the descriptor array and its sizes.
// The size of the buffer is the size of data in the buffer
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::CreateBufferL(const MDesCArray* aArray, CBufBase*& aBuf)
    {
    delete aBuf;
    aBuf = NULL;

    CBufBase* buffer=CreateBufferLC(KMPXBufGranularity);
    RBufWriteStream ws(*buffer);
    CleanupClosePushL(ws);
    ExternalizeL(aArray, ws);
    ws.CommitL();
    CleanupStack::PopAndDestroy(&ws);
    buffer->Compress();
    CleanupStack::Pop(buffer);
    aBuf = buffer;
    }

// ----------------------------------------------------------------------------
// Create buffer from message
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::CreateBufferL(
    const RMessage2& aMessage,
    TInt aMsgSlot,
    CBufBase*& aBuffer)
    {
    TInt s=aMessage.GetDesLengthL(aMsgSlot);
    if ( s <= 0 )
        {
        User::Leave( KErrArgument );
        }
    RecreateBufferL(s, aBuffer);
    TPtr8 p(aBuffer->Ptr(0));
    aMessage.Read(aMsgSlot,p);
    }

// ----------------------------------------------------------------------------
// Create buffer from a descriptor
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::CreateBufferL(const TDesC& aDes, CBufBase*& aBuffer)
    {
    TPtrC8 ptr8 = MPXUser::Ptr(aDes);
    RecreateBufferL(ptr8.Length(), aBuffer);
    aBuffer->Write(0, ptr8);
    aBuffer->Compress();
    }

// ----------------------------------------------------------------------------
// Creates a narrow heap descriptor from a unicode descriptor.
// No character conversion
// ----------------------------------------------------------------------------
//
EXPORT_C HBufC8* MPXUser::Alloc8L(const TDesC& aDes)
    {
    HBufC8* des8=HBufC8::NewL(aDes.Length());
    des8->Des().Copy(aDes);
    return des8;
    }

// ----------------------------------------------------------------------------
// Creates a zero-terminated narrow heap descriptor from a unicode descriptor.
// No character conversion
// ----------------------------------------------------------------------------
//
EXPORT_C HBufC8* MPXUser::Alloc8ZL(const TDesC& aDes)
    {
    HBufC8* des8=HBufC8::NewL(aDes.Length()+1);
    des8->Des().Copy(aDes);
    des8->Des().ZeroTerminate();
    return des8;
    }

// ----------------------------------------------------------------------------
// Creates a heap descriptor from a narrow descriptor.
// No character conversion
// ----------------------------------------------------------------------------
//
EXPORT_C HBufC* MPXUser::AllocL(const TDesC8& aDes)
    {
    HBufC* item = HBufC::NewL(aDes.Length());
    item->Des().Copy(aDes);
    return item;
    }

// ----------------------------------------------------------------------------
// Creates a narrow ptr descriptor on a unicode descriptor
// ----------------------------------------------------------------------------
//
EXPORT_C TPtrC8 MPXUser::Ptr(const TDesC& aDes)
    {
#ifdef _UNICODE
    TInt len=aDes.Length()*2; // Unicode->Narrow
    return TPtrC8((TUint8*)(&aDes[0]),len);
#else
    return TPtrC8(aDes);
#endif
    }

// ----------------------------------------------------------------------------
// Creates a wide ptr descriptor on a narrow descriptor
// ----------------------------------------------------------------------------
//
EXPORT_C TPtrC MPXUser::Ptr(const TDesC8& aDes)
    {
#ifdef _UNICODE
    TInt len=aDes.Length()/2; // Narrrow->Unicode
    return TPtrC((TUint16*)(&aDes[0]),len);
#else
    return TPtrC(aDes);
#endif
    }

// ----------------------------------------------------------------------------
// Helper internalize a descriptor array from a stream
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::InternalizeL(
    CDesCArray*& aArray,
    RReadStream& aStream)
    {
    delete aArray;
    aArray = NULL;
    TInt n=aStream.ReadInt32L();
    if( n )
        {
        CDesCArray* array=new(ELeave)CDesCArrayFlat(n);
        CleanupStack::PushL(array);
        TInt len;
        for (TInt i=0;i<n;++i)
            {
            len=aStream.ReadInt32L();
            HBufC* item=HBufC::NewLC(len);
            TPtr bp = item->Des();
            aStream.ReadL(bp,len);
            array->AppendL(*item);
            CleanupStack::PopAndDestroy(item);
            }
        CleanupStack::Pop(array);
        aArray = array;
        }
    else
        {
        // Dummy
        aArray = new(ELeave)CDesCArrayFlat(1);
        }
    }

// ----------------------------------------------------------------------------
// Helper externalize a descriptor array to a stream
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::ExternalizeL(
    const MDesCArray* aArray,
    RWriteStream& aStream)
    {
    if (aArray)
        {
        TInt n = aArray->MdcaCount();
        aStream.WriteInt32L(n);
        for (TInt i=0;i<n;++i)
            {
            TPtrC item=aArray->MdcaPoint(i);
            aStream.WriteInt32L( item.Length() );
            aStream.WriteL(item, item.Length());
            }
        }
    else
        {
        // Nothing to externalize
        aStream.WriteInt32L(0);
        }
    }

// ----------------------------------------------------------------------------
// Group attributes belonging to one content into an item in the array
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::MergeAttributeL(
    const TArray<TMPXAttribute>& aSrc,
    RArray<TMPXAttribute>& aDest)
    {
    CleanupClosePushL(aDest);
    aDest.Reset();
    for (TInt i = 0; i < aSrc.Count(); i++)
        {
        const TMPXAttribute& s = aSrc[i];
        TInt index = aDest.Find(s, TMPXAttribute::MatchContentId);
        if ( KErrNotFound == index )
            {
            aDest.AppendL(s);
            }
        else
            {
            TMPXAttribute& d = aDest[index];
            d = TMPXAttribute(d.ContentId(), d.AttributeId() | s.AttributeId());
            }
        }
    CleanupStack::Pop();
    }

// ----------------------------------------------------------------------------
// Determines the owning process id of the thread
// ----------------------------------------------------------------------------
//
EXPORT_C TProcessId MPXUser::ProcessIdL(TThreadId aId)
    {
    RThread t;
    User::LeaveIfError(t.Open(aId));
    CleanupClosePushL(t);
    RProcess p;
    User::LeaveIfError(t.Process(p));
    TProcessId pid=p.Id();
    p.Close();
    CleanupStack::PopAndDestroy(&t);
    return pid;
    }

// ----------------------------------------------------------------------------
// Add array items in aSrc into sorted array aDest if the item is not in
// the aDest yet
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::MergeArray(const MDesCArray& aSrc, CDesCArray& aDest)
    {
    for (TInt i=aSrc.MdcaCount(); --i>=0 ;)
        {
        ////ignore leave when insert the same item
        TRAP_IGNORE(aDest.InsertIsqL(aSrc.MdcaPoint(i)));
        }
    }

// ----------------------------------------------------------------------------
// Copies elements from one array of descriptors to another
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::CopyArrayL(const MDesCArray& aSrc, CDesCArray& aDest)
    {
    aDest.Reset();
    for (TInt i=0; i < aSrc.MdcaCount(); ++i)
        {
        aDest.AppendL(aSrc.MdcaPoint(i));
        }
    }

// ----------------------------------------------------------------------------
// Compare two uids
// ----------------------------------------------------------------------------
//
EXPORT_C TInt MPXUser::CompareUids(const TUid& aId1, const TUid& aId2)
    {
    TInt ret(0);
    if (aId1.iUid > aId2.iUid)
        {
        ret=1;
        }
    else if (aId1.iUid < aId2.iUid)
        {
        ret=-1;
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// Compare two sorted arrays of uids
// ----------------------------------------------------------------------------
//
EXPORT_C TInt MPXUser::CompareOrderedUidArrays(
    const TArray<TUid>& aArray1,
    const TArray<TUid>& aArray2)
    {
    TInt ret(-1);
    TInt count1 = aArray1.Count();
    TInt count2 = aArray2.Count();
    if (count1 >= count2)
        {
        TInt s1(0); // start point of array 1
        TBool found(ETrue);
        for (TInt i=0; i<count2 && found; ++i)
            {
            found = EFalse;
            for (TInt j=s1; j<count1; ++j)
                {
                if (aArray1[j].iUid == aArray2[i].iUid)
                    {
                    s1=j+1; // compare from next item
                    found = ETrue;
                    break;
                    }
                }
            }
        if (found)
            {
            if (count1==count2)
                {
                ret=0;
                }
            else
                {
                ret=1;
                }
            }
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// Internalize an array of item ids from stream
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::InternalizeL(RArray<TMPXItemId>& aArray, RReadStream& aStream)
    {
    CleanupClosePushL(aArray);
    TInt n=aStream.ReadInt32L();
    for (TInt i=0;i<n;++i)
        {
        TUint32 id1(aStream.ReadUint32L());
        TUint32 id2(aStream.ReadUint32L());
        aArray.AppendL(TMPXItemId(id1,id2));
        }
    CleanupStack::Pop();
    }

// ----------------------------------------------------------------------------
// Externalize an array of itemids to stream
// ----------------------------------------------------------------------------
//
EXPORT_C void MPXUser::ExternalizeL(const TArray<TMPXItemId>& aArray, RWriteStream& aStream)
    {
    TInt n=aArray.Count();
    aStream.WriteInt32L(n);
    for (TInt i=0;i<n;++i)
        {
        aStream.WriteUint32L(aArray[i].iId1);
        aStream.WriteUint32L(aArray[i].iId2);
        }
    }

// ----------------------------------------------------------------------------
// Helper to get the window group ID associated with a process
// ----------------------------------------------------------------------------
//
EXPORT_C TInt MPXUser::FindWindowGroupIdL(
    TProcessId aProcId,
    RWsSession& aWsSession )
    {
    TInt wgId( KErrNotFound );
    RArray<RWsSession::TWindowGroupChainInfo> windowArray;
    CleanupClosePushL( windowArray );
    aWsSession.WindowGroupList( &windowArray );
    TInt wgCount( windowArray.Count() );

    for ( TInt i = 0; i < wgCount && wgId == KErrNotFound; i++ )
        {
        TThreadId threadId;
        User::LeaveIfError(
            aWsSession.GetWindowGroupClientThreadId(
                windowArray[i].iId, threadId ) );
        RThread thread;
        CleanupClosePushL( thread );
        User::LeaveIfError( thread.Open( threadId ));
        RProcess proc;
        CleanupClosePushL( proc );
        User::LeaveIfError( thread.Process( proc ));
        if ( proc.Id() == aProcId )
            {
            wgId = windowArray[i].iId;
            }
        CleanupStack::PopAndDestroy( &proc );
        CleanupStack::PopAndDestroy( &thread );
        }
    CleanupStack::PopAndDestroy( &windowArray );
    return wgId;
    }

// -----------------------------------------------------------------------------
// Check if there is the given type call ongoing.
// -----------------------------------------------------------------------------
//
EXPORT_C TBool MPXUser::IsCallOngoing( TInt aCallType )
    {
    MPX_FUNC( "MPXUser::IsCallOngoing" );
    // In case call info not fully received (err != KErrNone),
    // assume no call exists.
    TBool ret( EFalse );

    // Get call status and type.
    TInt callState( EPSCTsyCallStateUninitialized );

    TInt err( RProperty::Get(
        KPSUidCtsyCallInformation,
        KCTsyCallState,
        callState ) );

    MPX_DEBUG3("MPXUser::IsCallOngoing() call state: %d , error (%d)", callState, err );

    if ( !err &&
         ( callState != EPSCTsyCallStateUninitialized &&
           callState != EPSCTsyCallStateNone ) )
        {
        TInt callType( EPSCTsyCallTypeUninitialized );

        // There is call ongoing, check it's type.
        err = RProperty::Get(
            KPSUidCtsyCallInformation,
            KCTsyCallType,
            callType );

        MPX_DEBUG3("MPXUser::IsCallOngoing() call type:  %d, error (%d)", callType,  err );

        if ( !err )
            {
            if ( aCallType == KMPXCallTypeGenericVoice )
                {
                // Return true for CSVoice/Video/VOIP calls
                ret = ( callType == EPSCTsyCallTypeCSVoice ||
                        callType == EPSCTsyCallTypeH324Multimedia ||
                        callType == EPSCTsyCallTypeVoIP );
                MPX_DEBUG2("MPXUser::IsCallOngoing KMPlayerCallTypeGenericVoice return (%d) ", ret );
                }
            else
                {
                // Check the requested type call.
                ret = ( callType == aCallType );
                MPX_DEBUG2("MPXUser::IsCallOngoing return (%d) ", ret );
                }
            }
        }
    MPX_DEBUG2("MPXUser::IsCallOngoing return (%d) ", ret );
    return ret;
    }

// -----------------------------------------------------------------------------
// Compelete file name with file path of the dll
// -----------------------------------------------------------------------------
//
EXPORT_C TInt MPXUser::CompleteWithDllPath(const TDesC& aDllName, TDes& aFileName)
    {
    MPX_DEBUG2("==>MPXUser::CompleteWithDllPath aFileName %S", &aFileName);
    TInt error(KErrNotSupported);
    MPX_DEBUG2("MPX Dll file path: %S", &aDllName);
    TPtrC driveAndPath = TParsePtrC(aDllName).DriveAndPath();
    TParse parse;
    error = parse.Set(aFileName, &driveAndPath, NULL);
    if (!error)
        {
        aFileName.Zero();
        aFileName.Append(parse.FullName());
        }
    MPX_DEBUG2("<==MPXUser::CompleteWithDllPath aFileName %S", &aFileName);
    return error;
    }

// End of file
