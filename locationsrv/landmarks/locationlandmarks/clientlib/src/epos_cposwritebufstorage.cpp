/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Internal class, supports writing data elements to fixed-size buffer.
*
*/


#include "epos_cposwritebufstorage.h"

// ======== LOCAL FUNCTIONS ========

#ifdef _DEBUG
_LIT(  KPanic, "EPosLm-Pack"  );
void Panic(  TInt aReason  )
    {
    User::Panic(  KPanic, aReason  );
    }
#endif

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
//  C++ constructor
// ---------------------------------------------------------------------------
//
CPosWriteBufStorage::CPosWriteBufStorage()
    : CBase()
    {
    }

// ---------------------------------------------------------------------------
//  Destructor
// ---------------------------------------------------------------------------
//
CPosWriteBufStorage::~CPosWriteBufStorage()
    {
    delete iBuffer;
    }

// ---------------------------------------------------------------------------
//  Static constructor
// ---------------------------------------------------------------------------
//
CPosWriteBufStorage* CPosWriteBufStorage::NewLC( TInt aMaxLen )
    {
    CPosWriteBufStorage* self = new ( ELeave ) CPosWriteBufStorage;
    CleanupStack::PushL( self );
    self->iBuffer = HBufC8::NewL( aMaxLen );
    return self;
    }

// ---------------------------------------------------------------------------
//  Gives how many bytes passed descriptor needs for packing
// ---------------------------------------------------------------------------
//
TInt CPosWriteBufStorage::PackedSize( const TDesC& aText )
    {
    return 2 * sizeof( TInt ) + aText.Size(); // length, size, data
    }

// ---------------------------------------------------------------------------
//  Gives how many bytes passed descriptor needs for packing
// ---------------------------------------------------------------------------
//
TInt CPosWriteBufStorage::PackedSize( const TDesC8& aText )
    {
    return sizeof( TInt ) + aText.Size(); // size, data
    }

// ---------------------------------------------------------------------------
//  Packs aSize bytes from aSrc address into internal buffer
// ---------------------------------------------------------------------------
//
void CPosWriteBufStorage::Put( TUint8* aSrc, const TInt aSize )
    {
    __ASSERT_DEBUG( iBuffer, Panic( KErrGeneral ) );
    iBuffer->Des().Append( aSrc, aSize );
    }

// ---------------------------------------------------------------------------
//  Packs TInt into internal buffer
// ---------------------------------------------------------------------------
//
void CPosWriteBufStorage::Put( const TInt aData )
    {
    Put( (TUint8*) &aData, sizeof( TInt ) );
    }

// ---------------------------------------------------------------------------
//  Packs TUin32 into internal buffer
// ---------------------------------------------------------------------------
//
void CPosWriteBufStorage::Put( const TUint32 aData )
    {
    Put( (TUint8*) &aData, sizeof( TUint32 ) );
    }

// ---------------------------------------------------------------------------
//  Packs TUint16 into internal buffer
// ---------------------------------------------------------------------------
//
void CPosWriteBufStorage::Put( const TUint16 aData )
    {
    Put( (TUint8*) &aData, sizeof( TUint16 ) );
    }

// ---------------------------------------------------------------------------
//  Packs descriptor into internal buffer
// ---------------------------------------------------------------------------
//
void CPosWriteBufStorage::Put( const TDesC& aText )
    {
    Put( aText.Length() );
    Put( TPtrC8( (TUint8*) aText.Ptr(), aText.Size() ) );
    }

// ---------------------------------------------------------------------------
//  Packs descriptor into internal buffer
// ---------------------------------------------------------------------------
//
void CPosWriteBufStorage::Put( const TDesC8& aText )
    {
    Put( aText.Size() );
    Put( (TUint8*) aText.Ptr(), aText.Size() );
    }
    
// ---------------------------------------------------------------------------
//  Returns result of packing. Ownersip is transferred to client
// ---------------------------------------------------------------------------
//
HBufC8* CPosWriteBufStorage::FinalizeL()
    {
    HBufC8* buf = iBuffer;
    iBuffer = NULL;
    return buf;
    }
