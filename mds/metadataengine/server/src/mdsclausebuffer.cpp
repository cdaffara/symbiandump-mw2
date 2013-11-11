/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A unicode des buffer with automatic memory handling*
*/

#include "mdsclausebuffer.h"

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CMdsClauseBuffer* CMdsClauseBuffer::NewL( TInt aInitialSize )
    {
    CMdsClauseBuffer* that = CMdsClauseBuffer::NewLC( aInitialSize );
    CleanupStack::Pop( that );
    return that;
    }

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CMdsClauseBuffer* CMdsClauseBuffer::NewLC( TInt aInitialSize )
    {
    CMdsClauseBuffer* that = new( ELeave ) CMdsClauseBuffer();
    CleanupStack::PushL( that );
    that->ConstructL( aInitialSize );
    return that;
    }

// ---------------------------------------------------------------------------
// CMdCAutoBuf
// ---------------------------------------------------------------------------
//
CMdsClauseBuffer::CMdsClauseBuffer()
    {
    }

// ---------------------------------------------------------------------------
// ~CMdCAutoBuf
// ---------------------------------------------------------------------------
//
CMdsClauseBuffer::~CMdsClauseBuffer()
    {
    delete iBuffer;
    iBuffer = NULL;
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CMdsClauseBuffer::ConstructL( TInt aInitialSize )
    {
    if ( aInitialSize < 1 || aInitialSize > 100000 )
    	{
    	aInitialSize = 1;
    	}
    iCurrentBufSize = aInitialSize;

    iBuffer = HBufC::NewL( iCurrentBufSize );
    }

// ---------------------------------------------------------------------------
// AppendL
// ---------------------------------------------------------------------------
//
void CMdsClauseBuffer::AppendL( const TDesC& aDes )
    {
    AppendL( aDes, 0 );
    }

void CMdsClauseBuffer::AppendL( const TDesC& aDes, const TInt aAdditional )
	{
	ReserveSpaceL( iBuffer->Size() + aDes.Length() + 1 + aAdditional );

	iBuffer->Des().Append( aDes );
	}


// ---------------------------------------------------------------------------
// ReserveSpaceL
// ---------------------------------------------------------------------------
//
TBool CMdsClauseBuffer::ReserveSpaceL( TInt aRequired )
    {
    // check if the current buffer is enough
    if ( iCurrentBufSize >= aRequired  )
        {
        return EFalse;
        }

    // realloc and copy
    iBuffer = iBuffer->ReAllocL( aRequired );
    
	iCurrentBufSize = aRequired;

    return ETrue;
    }

// ---------------------------------------------------------------------------
// BufferL
// ---------------------------------------------------------------------------
//
TPtr CMdsClauseBuffer::BufferL() const
	{
	if( !iBuffer )
		{
		User::Leave( KErrNotFound );
		}

	return iBuffer->Des();
	}

// ---------------------------------------------------------------------------
// ConstBufferL
// ---------------------------------------------------------------------------
//
const TDesC& CMdsClauseBuffer::ConstBufferL() const
	{
	if( !iBuffer )
		{
		User::Leave( KErrNotFound );
		}

	return *iBuffer;
	}
