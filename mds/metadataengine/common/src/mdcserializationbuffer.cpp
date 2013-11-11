/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Serialize and deserialize data between client and server
*
*/

// INCLUDE FILES
#include <e32def.h>

#include "mdcserializationbuffer.h"
#include "mdcserializationtype.h"

#ifdef _DEBUG
#include <e32debug.h>
_LIT( KCMdCSerializationBuffer, "CMdCSerializationBuffer" );

enum TSerializedBufferType
	{
	ESerializedBufferTBool,
	ESerializedBufferTInt8,
	ESerializedBufferTUint8,
	ESerializedBufferTInt16,
	ESerializedBufferTUint16,
	ESerializedBufferTInt32,
	ESerializedBufferTUint32,
	ESerializedBufferTUint,
	ESerializedBufferTInt64,
	ESerializedBufferTReal32,
	ESerializedBufferTReal64,
	ESerializedBufferTTime,
	ESerializedBufferTText8,
	ESerializedBufferTText16,
	ESerializedBufferTMdCValueUnion,
	ESerializedBufferTText, // DON'T USE, special case ONLY for ReceiveDesciptorLength
	};
#endif

const TUint32 KRealRequiredSizeForTUint8  = CMdCSerializationBuffer::KRequiredSizeForTUint16 - CMdCSerializationBuffer::KRequiredSizeForTypeCheck;
const TUint32 KRealRequiredSizeForTInt16  = CMdCSerializationBuffer::KRequiredSizeForTInt16 - CMdCSerializationBuffer::KRequiredSizeForTypeCheck;
const TUint32 KRealRequiredSizeForTUint16 = CMdCSerializationBuffer::KRequiredSizeForTUint16 - CMdCSerializationBuffer::KRequiredSizeForTypeCheck;
const TUint32 KRealRequiredSizeForTInt32  = CMdCSerializationBuffer::KRequiredSizeForTInt32 - CMdCSerializationBuffer::KRequiredSizeForTypeCheck;
const TUint32 KRealRequiredSizeForTUint32 = CMdCSerializationBuffer::KRequiredSizeForTUint32 - CMdCSerializationBuffer::KRequiredSizeForTypeCheck;
const TUint32 KRealRequiredSizeForTInt64  = CMdCSerializationBuffer::KRequiredSizeForTInt64 - CMdCSerializationBuffer::KRequiredSizeForTypeCheck;
const TUint32 KRealRequiredSizeForTReal32 = CMdCSerializationBuffer::KRequiredSizeForTReal32 - CMdCSerializationBuffer::KRequiredSizeForTypeCheck;
const TUint32 KRealRequiredSizeForTReal64 = CMdCSerializationBuffer::KRequiredSizeForTReal64 - CMdCSerializationBuffer::KRequiredSizeForTypeCheck;
const TUint32 KRealRequiredSizeForTTime   = CMdCSerializationBuffer::KRequiredSizeForTTime - CMdCSerializationBuffer::KRequiredSizeForTypeCheck;

// artificial types
const TUint32 KRealRequiredSizeForEmptyText = CMdCSerializationBuffer::KRequiredSizeForEmptyText - CMdCSerializationBuffer::KRequiredSizeForTypeCheck;
const TUint32 KRealRequiredSizeForTMdCValueUnion = CMdCSerializationBuffer::KRequiredSizeForTMdCValueUnion - CMdCSerializationBuffer::KRequiredSizeForTypeCheck;


EXPORT_C TUint32 CMdCSerializationBuffer::RequiredSize( const TDesC8& aDes )
	{
	TInt desLength = aDes.Length();

	if( KSerializedDesMaxLength < desLength )
		{
		desLength = KSerializedDesMaxLength;
		}

	// Required size for lenght of the descriptor, actual descriptor data and
	// possible needed 1 byte padding
	return KRequiredSizeForEmptyText
	       + (desLength % 2 ? desLength + sizeof( KPadding8 ) : desLength) * KRealRequiredSizeForTUint8;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::RequiredSize( const TDesC16& aDes )
	{
	TInt desLength = aDes.Length();

	if( KSerializedDesMaxLength < desLength )
		{
		desLength = KSerializedDesMaxLength;
		}

	// Required size for lenght of the descriptor and actual descriptor data
	return KRequiredSizeForEmptyText + desLength * KRealRequiredSizeForTUint16;
	}

EXPORT_C CMdCSerializationBuffer* CMdCSerializationBuffer::NewL( TInt32 aSize )
	{
    CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( aSize );
    CleanupStack::Pop( buffer );
    return buffer;
    }

EXPORT_C CMdCSerializationBuffer* CMdCSerializationBuffer::NewLC( TInt32 aSize )
	{
    CMdCSerializationBuffer* buffer = new( ELeave ) CMdCSerializationBuffer();
    CleanupStack::PushL( buffer );
    buffer->ConstructL( aSize );
    return buffer;
	}

EXPORT_C CMdCSerializationBuffer* CMdCSerializationBuffer::NewL( TUint8* aBuffer, TInt32 aSize )
	{
    CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( aBuffer, aSize );
    CleanupStack::Pop( buffer );
    return buffer;
    }

EXPORT_C CMdCSerializationBuffer* CMdCSerializationBuffer::NewLC( TUint8* aBuffer, TInt32 aSize )
	{
    CMdCSerializationBuffer* buffer = new( ELeave ) CMdCSerializationBuffer();
    CleanupStack::PushL( buffer );
    buffer->ConstructL( aBuffer, aSize );
    return buffer;
	}

EXPORT_C CMdCSerializationBuffer* CMdCSerializationBuffer::NewLC( const RMessage2 &aMsg, TInt aIndex )
	{
	const TInt newDesLength = aMsg.GetDesLength( aIndex );
	if ( newDesLength < 0 )
		{
		User::Leave( KErrBadDescriptor );
		}

    CMdCSerializationBuffer* buf = new (ELeave)CMdCSerializationBuffer(); 
    CleanupStack::PushL(buf);
    buf->ConstructL(newDesLength);
	
    TPtr8 bufferPtr(buf->Buffer());
	aMsg.ReadL( aIndex, bufferPtr );

	return buf;
	}

EXPORT_C CMdCSerializationBuffer* CMdCSerializationBuffer::NewL( const RMessage2 &aMsg, TInt aIndex )	
	{
	CMdCSerializationBuffer* buf = CMdCSerializationBuffer::NewLC(aMsg, aIndex);
	CleanupStack::Pop(buf);
	return buf;
	}

EXPORT_C CMdCSerializationBuffer* CMdCSerializationBuffer::NewLC( const CMdCSerializationBuffer& aBuffer )
	{
	const TInt newDesLength = aBuffer.Size();
	CMdCSerializationBuffer* buf = CMdCSerializationBuffer::NewLC( newDesLength );
	TPtr8 bufferPtr( buf->Buffer() );
	bufferPtr.Copy( aBuffer.Buffer() );
	return buf;
	}

EXPORT_C CMdCSerializationBuffer* CMdCSerializationBuffer::NewL( const CMdCSerializationBuffer& aBuffer )
	{
	CMdCSerializationBuffer* buf = CMdCSerializationBuffer::NewLC(aBuffer);
	CleanupStack::Pop(buf);
	return buf;
	}

CMdCSerializationBuffer::~CMdCSerializationBuffer()
	{
	delete iHeapBuffer;
	iHeapBuffer = NULL;
	}

EXPORT_C TPtr8 CMdCSerializationBuffer::Buffer() const
	{	
	return iBuffer;
	}

EXPORT_C TPtr8* CMdCSerializationBuffer::BufferPtr() const
	{
	return (TPtr8*)&iBuffer;
	}

EXPORT_C const TPtr8* CMdCSerializationBuffer::BufferConstPtr() const
	{
	return &iBuffer;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::Size() const
	{
	return (TUint32)iBuffer.Length();
	}

EXPORT_C void CMdCSerializationBuffer::PositionL( TUint32 aPosition )
	{
	__ASSERT_DEBUG( aPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrTooBig ) );
	__ASSERT_DEBUG( aPosition % 2 == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	
	if( aPosition <= Size() )
		{
		iPosition = aPosition;
		return;
		}
	else
		{
		User::Leave( KErrTooBig );
		}
	}

EXPORT_C TUint32 CMdCSerializationBuffer::Position() const
	{
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TBool aValue )
	{
	INSERTTYPE(ESerializedBufferTBool);
	
	TInt32 value = aValue;
	return InsertL( value );
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TInt8 aValue )
	{
	INSERTTYPE(ESerializedBufferTInt8);
	
	TInt16 value = aValue;
	return InsertL( value );
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TUint8 aValue )
	{
	INSERTTYPE(ESerializedBufferTUint8);
	
	TUint16 value = aValue;
	return InsertL( value );
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TInt16 aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTInt16);

	TInt16* ptr = (TInt16*)(iBuffer.Ptr() + iPosition);
	
	Mem::Copy(ptr, &aValue, sizeof(aValue));

	iPosition += KRealRequiredSizeForTInt16;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TUint16 aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTUint16);

	TUint16* ptr = (TUint16*)(iBuffer.Ptr() + iPosition);
	
	Mem::Copy(ptr, &aValue, sizeof(aValue));

	iPosition += KRealRequiredSizeForTUint16;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TInt32 aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTInt32);

	TInt32* ptr = (TInt32*)(iBuffer.Ptr() + iPosition);
	
	Mem::Copy(ptr, &aValue, sizeof(aValue));

	iPosition += KRealRequiredSizeForTInt32;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TUint32 aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTUint32);

	TUint32* ptr = (TUint32*)(iBuffer.Ptr() + iPosition);
	
	Mem::Copy(ptr, &aValue, sizeof(aValue));

	iPosition += KRealRequiredSizeForTUint32;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TInt64 aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTInt64);

	TInt64* ptr = (TInt64*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(ptr, &aValue, sizeof(aValue));

	iPosition += KRealRequiredSizeForTInt64;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TUint aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTUint);

	TUint* ptr = (TUint*)(iBuffer.Ptr() + iPosition);
	
	Mem::Copy(ptr, &aValue, sizeof(aValue));

	iPosition += sizeof( TUint );
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TReal32 aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTReal32);

	TReal32* ptr = (TReal32*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(ptr, &aValue, sizeof(aValue));

	iPosition += KRealRequiredSizeForTReal32;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TReal64 aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTReal64);

	TReal64* ptr = (TReal64*)(iBuffer.Ptr() + iPosition);
	
	Mem::Copy(ptr, &aValue, sizeof(aValue));

	iPosition += KRealRequiredSizeForTReal64;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TTime aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTTime);

	TInt64* ptr = (TInt64*)(iBuffer.Ptr() + iPosition);

	TInt64 value = aValue.Int64();
	
	Mem::Copy(ptr, &value, sizeof(value));

	iPosition += KRealRequiredSizeForTTime;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );

	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TDesC8& aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTText8);

	const TInt desLength = aValue.Length();
	
	if( KSerializedDesMaxLength < desLength )
		{
		User::Leave( KErrTooBig );
		}

	TSerializedDesLength* ptr = (TSerializedDesLength*)(iBuffer.Ptr() + iPosition);

	const TSerializedDesLength serDesLength = (TSerializedDesLength)desLength;
	Mem::Copy( ptr, &serDesLength, sizeof( serDesLength ) );

	iPosition += KRealRequiredSizeForEmptyText;

	if( desLength == 0 )
		{
		return iPosition;
		}

	TUint8* bufPtr = (TUint8*)(iBuffer.Ptr() + iPosition);
	
	TPtrC8 desPtrC8 ((TUint8*)aValue.Ptr(), aValue.Length());
	TUint8* desPtr = (TUint8*)desPtrC8.Ptr();

	Mem::Copy( bufPtr, desPtr, desLength * sizeof( TUint8 ) );

	iPosition += desLength * KRealRequiredSizeForTUint8;

	// If position is odd, add padding and goto next even position
	if( iPosition & 1 )
		{
		iPosition += sizeof( KPadding8 );
		}

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );

	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TDesC16& aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTText16);

	const TInt desLength = aValue.Length();
	
	if( KSerializedDesMaxLength < desLength )
		{
		User::Leave( KErrTooBig );
		}

	TSerializedDesLength* ptr = (TSerializedDesLength*)(iBuffer.Ptr() + iPosition);

	const TSerializedDesLength serDesLength = (TSerializedDesLength)desLength;
	Mem::Copy(ptr, &serDesLength, sizeof(serDesLength));

	iPosition += KRealRequiredSizeForEmptyText;

	if( desLength == 0 )
		{
		return iPosition;
		}

	TUint16* bufPtr = (TUint16*)(iBuffer.Ptr() + iPosition);

	TPtrC16 desPtrC16 ((TUint16*)aValue.Ptr(), aValue.Length());
	TUint16* desPtr = (TUint16*)desPtrC16.Ptr();

	Mem::Copy(bufPtr, desPtr, desLength*sizeof(TUint16));

	iPosition += desLength * KRealRequiredSizeForTUint16;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::InsertL( TMdCValueUnion& aValue )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	INSERTTYPE(ESerializedBufferTMdCValueUnion);

	TMdCValueUnion* ptr = (TMdCValueUnion*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(ptr, &aValue, sizeof(aValue));

	iPosition += KRealRequiredSizeForTMdCValueUnion;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;

	}
EXPORT_C TUint32 CMdCSerializationBuffer::InsertL(TMdCSerializationType& aValue,
		TUint32 aSize)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	void* ptr = (void*)(iBuffer.Ptr() + iPosition);

	TMdCSerializationType* value = &aValue;

	Mem::Copy(ptr, value, aSize);

	iPosition += aSize;

	// If position is odd, add padding and goto next even position
	if( iPosition & 1 )
		{
		iPosition += sizeof( KPadding8 );
		}

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );

	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TBool& aDestination)
	{
	CHECKTYPE(ESerializedBufferTBool);

	TInt32 value;
	ReceiveL(value);
	aDestination = value ? ETrue : EFalse;
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TInt8& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTInt8);

	TInt16 value;
	ReceiveL(value);
	aDestination = (TInt8)value;
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TUint8& aDestination)
	{
	CHECKTYPE(ESerializedBufferTUint8);

	TUint16 value;
	ReceiveL(value);
	aDestination = (TUint8)value;
	return iPosition;
	}
	
EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TInt16& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTInt16);

	TInt16* ptr = (TInt16*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(&aDestination, ptr, sizeof(aDestination));

	iPosition += KRealRequiredSizeForTInt16;

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TUint16& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTUint16);

	TUint16* ptr = (TUint16*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(&aDestination, ptr, sizeof(aDestination));

	iPosition += KRealRequiredSizeForTUint16;

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TInt32& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTInt32);

	TInt32* ptr = (TInt32*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(&aDestination, ptr, sizeof(aDestination));

	iPosition += KRealRequiredSizeForTInt32;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TUint32& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTUint32);

	TUint32* ptr = (TUint32*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(&aDestination, ptr, sizeof(aDestination));  
	
	aDestination = *ptr;

	iPosition += KRealRequiredSizeForTUint32;

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TInt64& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTInt64);

	TInt64* ptr = (TInt64*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(&aDestination, ptr, sizeof(aDestination));

	iPosition += KRealRequiredSizeForTInt64;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TUint& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTUint);

	TUint* ptr = (TUint*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(&aDestination, ptr, sizeof(aDestination));

	iPosition += sizeof( TUint );

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TReal32& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTReal32);

	TReal32* ptr = (TReal32*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(&aDestination, ptr, sizeof(aDestination));

	iPosition += KRealRequiredSizeForTReal32;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TReal64& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTReal64);

	TReal64* ptr = (TReal64*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(&aDestination, ptr, sizeof(aDestination));

	iPosition += KRealRequiredSizeForTReal64;

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TTime& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTTime);

	TInt64* ptr = (TInt64*)(iBuffer.Ptr() + iPosition);

	TInt64 value;
	Mem::Copy(&value, ptr, sizeof(value));

	aDestination = value;

	iPosition += KRealRequiredSizeForTInt64;

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TMdCValueUnion& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTMdCValueUnion);

	TMdCValueUnion* ptr = (TMdCValueUnion*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(&aDestination, ptr, sizeof(aDestination));

	iPosition += KRealRequiredSizeForTMdCValueUnion;

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TMdCSerializationType& aDestination,
		TUint32 aSize)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	void* ptr = (void*)(iBuffer.Ptr() + iPosition);

	TMdCSerializationType* value = &aDestination;

	Mem::Copy(value, ptr, aSize);

	iPosition += aSize;

	// If position is odd, add padding and goto next even position
	if( iPosition & 1 )
		{
		iPosition += sizeof( KPadding8 );
		}

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}


EXPORT_C TSerializedDesLength CMdCSerializationBuffer::ReceiveDesciptorLength() const
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	TSerializedDesLength* ptr = (TSerializedDesLength*)(iBuffer.Ptr() + KRequiredSizeForTypeCheck + iPosition);

	TSerializedDesLength desLength;
	Mem::Copy(&desLength, ptr, sizeof(desLength));

	return desLength;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TDes8& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTText8);

	TSerializedDesLength* ptr = (TSerializedDesLength*)(iBuffer.Ptr() + iPosition);

	TSerializedDesLength desLength;
	Mem::Copy(&desLength, ptr, sizeof(desLength));

	iPosition += KRealRequiredSizeForEmptyText;

	TUint8* bufPtr = (TUint8*)(iBuffer.Ptr() + iPosition);

	aDestination.SetLength( desLength );

	TPtrC8 desPtrC8((TUint8*)aDestination.Ptr(), aDestination.Length());
	TUint8* desPtr = (TUint8*)desPtrC8.Ptr();

	Mem::Copy(desPtr, bufPtr, desLength*sizeof(TUint8));

	iPosition += desLength * KRealRequiredSizeForTUint8;

	// If position is odd, add padding and goto next even position
	if( iPosition & 1 )
		{
		iPosition += sizeof( KPadding8 );
		}

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C TUint32 CMdCSerializationBuffer::ReceiveL(TDes16& aDestination)
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTText16);

	TSerializedDesLength* ptr = (TSerializedDesLength*)(iBuffer.Ptr() + iPosition);

	TSerializedDesLength desLength;
	Mem::Copy(&desLength, ptr, sizeof(desLength));

	iPosition += KRealRequiredSizeForEmptyText;

	TUint16* bufPtr = (TUint16*)(iBuffer.Ptr() + iPosition);

	aDestination.SetLength( desLength );

	TPtrC16 desPtrC16((TUint16*)aDestination.Ptr(), aDestination.Length());
	TUint16* desPtr = (TUint16*)desPtrC16.Ptr();

	Mem::Copy(desPtr, bufPtr, desLength*sizeof(TUint16));

	iPosition += desLength * KRealRequiredSizeForTUint16;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return iPosition;
	}

EXPORT_C HBufC8* CMdCSerializationBuffer::ReceiveDes8L()
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTText8);

	TSerializedDesLength* ptr = (TSerializedDesLength*)(iBuffer.Ptr() + iPosition);

	TSerializedDesLength desLength;
	Mem::Copy(&desLength, ptr, sizeof(desLength));

	iPosition += KRealRequiredSizeForEmptyText;
	
	if( desLength == 0 )
		{
		return NULL;
		}
	
	TUint8* bufPtr = (TUint8*)(iBuffer.Ptr() + iPosition);
	
	HBufC8* desBuf = HBufC8::NewL( desLength );
	desBuf->Des().SetLength( desLength );
	
	TPtrC8 desPtrC8((TUint8*)desBuf->Ptr(), desBuf->Length());
	TUint8* desPtr = (TUint8*)desPtrC8.Ptr();

	Mem::Copy(desPtr, bufPtr, desLength*sizeof(TUint8));

	iPosition += desLength * KRealRequiredSizeForTUint8;

	if( iPosition & 1 )
		{
		iPosition += sizeof( KPadding8 );
		}

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return desBuf;
	}

EXPORT_C HBufC16* CMdCSerializationBuffer::ReceiveDes16L()
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTText16);

	TSerializedDesLength* ptr = (TSerializedDesLength*)(iBuffer.Ptr() + iPosition);

	TSerializedDesLength desLength;
	Mem::Copy(&desLength, ptr, sizeof(desLength));

	iPosition += KRealRequiredSizeForEmptyText;
	
	if( desLength == 0 )
		{
		return NULL;
		}
	
	TUint16* bufPtr = (TUint16*)(iBuffer.Ptr() + iPosition);
	
	HBufC16* desBuf = HBufC16::NewL( desLength );
	desBuf->Des().SetLength( desLength );

	TPtrC16 desPtrC16((TUint16*)desBuf->Ptr(), desBuf->Length());
	TUint16* desPtr = (TUint16*)desPtrC16.Ptr();

	Mem::Copy(desPtr, bufPtr, desLength*sizeof(TUint16));

	iPosition += desLength * KRealRequiredSizeForTUint16;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	
	return desBuf;
	}

EXPORT_C TPtrC8 CMdCSerializationBuffer::ReceivePtr8L()
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTText8);

	TSerializedDesLength* ptr = (TSerializedDesLength*)(iBuffer.Ptr() + iPosition);

	TSerializedDesLength desLength;
	Mem::Copy(&desLength, ptr, sizeof(desLength));

	iPosition += KRealRequiredSizeForEmptyText;

	TPtrC8 bufPtr8((const TUint8*)(iBuffer.Ptr() + iPosition), (TInt)desLength);

	iPosition += desLength * KRealRequiredSizeForTUint8;

	if( iPosition & 1 )
		{
		iPosition += sizeof( KPadding8 );
		}

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );

	return bufPtr8;
	}

EXPORT_C TPtrC16 CMdCSerializationBuffer::ReceivePtr16L()
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	CHECKTYPE(ESerializedBufferTText16);

	TSerializedDesLength* ptr = (TSerializedDesLength*)(iBuffer.Ptr() + iPosition);

	TSerializedDesLength desLength;
	Mem::Copy(&desLength, ptr, sizeof(desLength));

	iPosition += KRealRequiredSizeForEmptyText;

	TPtrC16 bufPtr16((const TUint16*)(iBuffer.Ptr() + iPosition), (TInt)desLength);

	iPosition += desLength * KRealRequiredSizeForTUint16;

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );

	return bufPtr16;
	}

CMdCSerializationBuffer::CMdCSerializationBuffer() 
	: iHeapBuffer( NULL ), iBuffer( NULL, 0 ), iPosition( 0 )
	{
	}

void CMdCSerializationBuffer::ConstructL( TInt32 aSize )
	{
   	if(aSize < 0)
		{
		User::Leave( KErrBadDescriptor );
		}	
	
    iHeapBuffer = HBufC8::NewL( aSize );
	iHeapBuffer->Des().SetLength( aSize );

    iBuffer.Set( iHeapBuffer->Des() );

#ifdef _DEBUG
    // for testing initilize to 0x99
    TUint8* ptr = (TUint8*)(iBuffer.Ptr());
    for( TInt32 i = aSize - 1; i >=0; i-- )
    	{
    	ptr[i] = 0x99;
    	}
#endif
	}

void CMdCSerializationBuffer::ConstructL( TUint8* aBuffer, TInt32 aSize )
	{
   	if(aSize < 0)
		{
		User::Leave( KErrBadDescriptor );
		}	
	
    iBuffer.Set( aBuffer, aSize, aSize );
	}

#ifdef _DEBUG
void CMdCSerializationBuffer::InsertTypeL( TUint16 aType )
	{
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	TUint16* ptr = (TUint16*)(iBuffer.Ptr() + iPosition);
	
	Mem::Copy(ptr, &aType, sizeof(aType));

	iPosition += KRealRequiredSizeForTUint16;
	
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );
	}

void CMdCSerializationBuffer::CheckTypeL( TUint16 aType )
	{
	_LIT( KErrDescription, "Incorrect types!" );
	TUint16 type;
	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );

	TUint16* ptr = (TUint16*)(iBuffer.Ptr() + iPosition);

	Mem::Copy(&type, ptr, sizeof(type));

	iPosition += KRealRequiredSizeForTUint16;

	__ASSERT_DEBUG( (iPosition & 1) == 0, User::Panic( KCMdCSerializationBuffer, KErrCorrupt ) );
	__ASSERT_DEBUG( iPosition <= Size(), User::Panic( KCMdCSerializationBuffer, KErrOverflow ) );

	
	if ( aType == ESerializedBufferTText && (type == ESerializedBufferTText8 || type == ESerializedBufferTText16) )
		{
		aType = type;
		}

#ifdef _DEBUG
	if (type != aType)
		{
		RDebug::Print( _L("Incorrect serialization buffer types, actual: %S, wanted: %S, position: %u"),
				&GetPropertyName(type), &GetPropertyName(aType), iPosition );
		}
#endif
	__ASSERT_DEBUG( type == aType, User::Panic( KErrDescription, KErrArgument ) );
	}

const TDesC& CMdCSerializationBuffer::GetPropertyName( TUint16 aType )
	{
	_LIT( KSerializeTBool, "TBool" );
	_LIT( KSerializeTInt8, "TInt8" );
	_LIT( KSerializeTUint8, "TUint8" );
	_LIT( KSerializeTInt16, "TInt16" );
	_LIT( KSerializeTUint16, "Uint16" );
	_LIT( KSerializeTInt32, "TInt32" );
	_LIT( KSerializeTUint32, "TUint32" );
	_LIT( KSerializeTUint, "TUint" );
	_LIT( KSerializeTInt64, "TInt64" );
	_LIT( KSerializeTReal32, "TReal32" );
	_LIT( KSerializeTReal64, "TReal64" );
	_LIT( KSerializeTTime, "TTime" );
	_LIT( KSerializeTText8, "TText8" );
	_LIT( KSerializeTText16, "TText16" );
	_LIT( KSerializeTMdCValueUnion, "TMdCValueUnion" );

	switch (aType)
		{
		case ESerializedBufferTBool:
			return KSerializeTBool;
		case ESerializedBufferTInt8:
			return KSerializeTInt8;
		case ESerializedBufferTUint8:
			return KSerializeTUint8;
		case ESerializedBufferTInt16:
			return KSerializeTInt16;
		case ESerializedBufferTUint16:
			return KSerializeTUint16;
		case ESerializedBufferTInt32:
			return KSerializeTInt32;
		case ESerializedBufferTUint32:
			return KSerializeTUint32;
		case ESerializedBufferTUint:
			return KSerializeTUint;
		case ESerializedBufferTInt64:
			return KSerializeTInt64;
		case ESerializedBufferTReal32:
			return KSerializeTReal32;
		case ESerializedBufferTReal64:
			return KSerializeTReal64;
		case ESerializedBufferTTime:
			return KSerializeTTime;
		case ESerializedBufferTText8:
			return KSerializeTText8;
		case ESerializedBufferTText16:
			return KSerializeTText16;
		case ESerializedBufferTMdCValueUnion:
			return KSerializeTMdCValueUnion;
		default:
			User::Panic( _L("Unknown serialization type!"), KErrNotFound );
		}
	return KSerializeTText16;
	}
#endif
