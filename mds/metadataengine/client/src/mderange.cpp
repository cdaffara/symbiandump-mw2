/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Serialization for mderange template class
*
*/

#include "mderange.h"
#include "mdcserializationbuffer.h"


void TMdESerializeRange::SerializeL( CMdCSerializationBuffer& aBuffer, TMdERangeType aType,
		void* aMin, void* aMax, TInt aValueSize )
	{
#ifdef _DEBUG
	if ( aValueSize > sizeof(TMdCValueUnion) )
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("Incorrect type"), KErrMdEIncorrectType ) );
		User::Leave( KErrMdEIncorrectType );
		}
#endif

	aBuffer.InsertL( (TInt32)aType );

	TMdCValueUnion min;
	Mem::Copy( &min, aMin, aValueSize );
	TMdCValueUnion max;
	Mem::Copy( &max, aMax, aValueSize );

	aBuffer.InsertL( min );
	aBuffer.InsertL( max );
	}

TUint32 TMdESerializeRange::RequiredBufferSize( TInt 
#ifdef _DEBUG		
		aValueSize
#endif
		)
	{
#ifdef _DEBUG
	if ( aValueSize > sizeof(TMdCValueUnion) )
		{
		User::Panic( _L("Incorrect type"), KErrMdEIncorrectType );
		}
#endif
	
	// Required size for type (TInt32), iMin and iMax (fixed size)
	TUint32 bufferSize = CMdCSerializationBuffer::KRequiredSizeForTInt32
			+ 2 * CMdCSerializationBuffer::KRequiredSizeForTMdCValueUnion;

	return bufferSize;
	}
